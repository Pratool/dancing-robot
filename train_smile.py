from load_smiles import *
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from pickle import dump
import numpy
import pickle


from sklearn.externals import joblib



def train_smiles():
    data = load_smiles()
    model = LogisticRegression()
    #Could also do SVC()
    model.fit(data.data, data.target)
    
    joblib.dump(model, 'my_model.pkl', compress=9)
    
    return model
 
     
	#print model#.score(data.data, data.target)
     
     
     #plt.matshow(numpy.reshape(model.raw_coef_[0][1:],(24,24)).transpose(),cmap='gray')
#
#	plt.show()
     

if __name__ == '__main__':
	model = train_smiles()
