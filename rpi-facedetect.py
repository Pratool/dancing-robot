'''Test code for R-Pi camera'''

import numpy as np
import picamera
import time
import io

#For Arduino communication
import serial

#For facial detection
import cv2
from train_smile import train_smiles
from sklearn.externals import joblib
import time

#Create XML classifiers
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
#eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
#mouthCascade = cv2.CascadeClassifier('Mouth.xml')

model = joblib.load("my_model.pkl")

ser = serial.Serial('/dev/ttyACM0', 9600)
print 'Serial port warming up'
time.sleep(5)

numberOfFaces = 0

#while True:	# make sure ser.close() get called, I used a finite loop instead
for i in range(15):
    stream = io.BytesIO()
    with picamera.PiCamera() as camera:
        #camera.resolution = (1024, 768)
        camera.resolution = (320, 240)
        camera.capture(stream, format='jpeg', resize=(320, 240))
        time.sleep(2)    # camera warm-up time

    # Construct a numpy array from the stream
    data = np.fromstring(stream.getvalue(), dtype=np.uint8)

    # "Decode" the image from the array, preserving color
    frame = cv2.imdecode(data, 1)    # OpenCV returns an array with data in BGR order
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = face_cascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=10,
        minSize=(30, 30)
    )

    numberOfFaces = len(faces)

    if numberOfFaces == 0:
        print "There aren't any faces."
	ser.write('0')
    else:
        print "There is at least one face."

        for (x, y, w, h) in faces:
#            cv2.rectangle(frame, (x,y), (x+w,y+h), (0,255,0), 2) # just for visualization purposes

            # need to readjust the crop to reflect proportional things
            roi_gray = gray[6*(y/5):y+h, x+20:x+w-20]

            # resize roi_gray to (24, 24)
            if len(roi_gray) == 0:
		pass
            else:
                resized_roi = cv2.resize(roi_gray, (24, 24)).T/255.0
                roi_vec = resized_roi.reshape((resized_roi.shape[0]*resized_roi.shape[1], 1))
                smile_prob = -model.predict_log_proba(roi_vec.T)[0][0]
                print "Smile probability: %s" %smile_prob
                if smile_prob > 0.1:
                    ser.write('2')
                else:
                    ser.write('1')

#    cv2.imshow('Video', frame)

#    if cv2.waitKey(0):  # quitting doesn't work
#        cv2.destroyAllWindows()
#        break

#    if cv2.waitKey(1) & 0xFF == ord('q'):    # quitting doesn't work
#        cv2.destroyAllWindows()
#        break

ser.close()

#cv2.waitKey(0)
#cv2.destroyAllWindows()