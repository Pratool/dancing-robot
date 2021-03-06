# -*- coding: utf-8 -*-
"""
Code from CompRobo
Adela Wee and Michelle Sit"""

#This code runs the smile detector
#detects faces, then analyzes region within the detected face to determine if there is a smile

import scipy
import numpy
import cv2
from train_smile import train_smiles
import sys
import pdb
from datetime import datetime
from sklearn.externals import joblib
import serial
import time

#send messages to the arduino
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=0)

#Create xml classifiers
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
mouthCascade = cv2.CascadeClassifier('Mouth.xml')

numberOfFaces = 0
smileExists = 0


def detectFaces():
    
    #reads in the images from the camera
    video_capture = cv2.VideoCapture(0)

    while True:
        # Capture frame-by-frame
        ret, frame = video_capture.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        faces = face_cascade.detectMultiScale(
            gray,
            scaleFactor=1.1,
            minNeighbors=10,
            minSize=(30, 30)
        )
        numberOfFaces = len(faces)
        
        #No faces are detected
        if len(faces) == 0:
            ser.write('0')
            print "There are no faces."
            
        else:
            #Faces detected
            # Draw a rectangle around the faces
            for (x,y,w,h) in faces:
                print "I see %s people" %len(faces)
                #center of shape, rectangle dimensions, color, line thickness
                #cv2.rectangle (img, vertex1, vertex across from 1, color, something)
                faceRect = cv2.rectangle(frame,(x,y),(x+w,y+h),(255,0,0),0)
                roi_gray = gray[6*(y/5):y+h,x+20:x+w-20]

                if len(roi_gray) == 0:
                    pass
                else:
                    #calculates how similar a detected face is to the generalized face created from train_smiles and load_smiles
                    resized_roi = cv2.resize(roi_gray, (24, 24)).T/255.0
                    # scipy.misc.imsave('outfile.jpg', resized_roi)
                    roi_vec = resized_roi.reshape((resized_roi.shape[0]*resized_roi.shape[1],1))
                    smile_prob = -model.predict_log_proba(roi_vec.T)[0][0]
                    print "Smile probability: %s" %smile_prob
    
                    if (smile_prob < 0.75):
                        smileExists = 0
                        print "NO SMILE DETECTED"
                        ser.write('1')
    
                    elif (smile_prob >= 0.75):
                        smileExists = 1
                        print "SMILE DETECTED"
                        ser.write('2')
                        
                        #draws rectangle around mouth region if smile detected
                        for (x, y, w, h) in faces:
                            roi_gray = gray[y:y+h, x:x+w]
                            roi_color = frame[y:y+h, x:x+w]
                            
                            mouth = mouthCascade.detectMultiScale(
                                roi_gray,
                                scaleFactor=1.1,
                                minNeighbors=5,
                                minSize=(30, 30),
                                flags=cv2.cv.CV_HAAR_SCALE_IMAGE)
                            
                            yValues = []
                            
                            for (ex,ey,ew,eh) in mouth:
                                yValues.append(ey)
                                
                            for (ex,ey,ew,eh) in mouth:
                                if ey == max(yValues):
                                    mvals = (ex, ey, ew, eh)
                                    cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)

            cv2.imshow("ROI", roi_gray)
            cv2.imshow('ROI_resized', resized_roi)

        cv2.imshow('Video', frame)
    
        if cv2.waitKey(1) & 0xFF == ord('q'):
            ser.close()
            break

    # When everything is done, release the capture
    ser.close()
    video_capture.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    model = joblib.load('my_model.pkl')
    detectFaces()
