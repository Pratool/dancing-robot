#loads smile database and takes a logistical regression of it to create a generalized image of a smile
#used in the smile detector

import sklearn
from sklearn.datasets import *
from scipy.io import loadmat

def load_smiles():
	smiles = loadmat('smile_dataset.mat')
	return_val = sklearn.datasets.base.Bunch()
	return_val.data = smiles['X']
	return_val.target = smiles['expressions']
	# print "hi"
	return return_val
