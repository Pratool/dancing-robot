#For Arduino communication
import serial

#For facial detection
import cv2
from train_smile import train_smiles
from sklearn.externals import joblib

#Create XML classifiers
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
#eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')
#mouthCascade = cv2.CascadeClassifier('Mouth.xml')

model = joblib.load("my_model.pkl")

def detect_faces(data, ser):
    """
        INPUTS
        data is a numpy array containing the image
        ser is the serial port being written to
    """
    global model

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