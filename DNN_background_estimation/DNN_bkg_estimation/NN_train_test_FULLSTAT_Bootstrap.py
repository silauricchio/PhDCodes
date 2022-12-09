#!/bin/python                                                                                                                                                                                         

import os, re, pickle
import uproot
import pandas as pd
import numpy as np
from colorama import init, Fore, Back, Style
init(autoreset=True)
import sklearn.utils
import matplotlib.pyplot as plt
from sklearn import preprocessing
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.model_selection import train_test_split
import joblib

from keras.models import model_from_json
from keras.models import Sequential,Model,load_model
from keras.layers.core import Dense, Activation
from keras.layers import Input, Dense
from keras.layers import BatchNormalization, Dropout, concatenate
from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.optimizers import Adam
from keras import backend as K
import tensorflow as tf
from tensorflow.python.keras.utils import tf_utils
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix

from collections import Counter

import sys


model_name = 'fatjet_HwithNTracks_andTrackJets'


path_model = '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_145_175_v1p6_schemeA_WP60/' + model_name +  '/' 


#open pickles files

X_train_16a = pd.read_pickle(path_model + 'X_Train_mc16a.pkl')
y_train_16a = pd.read_pickle(path_model + 'y_Train_mc16a.pkl')

X_train_16d = pd.read_pickle(path_model + 'X_Train_mc16d.pkl')
y_train_16d = pd.read_pickle(path_model + 'y_Train_mc16d.pkl')

X_train_16e = pd.read_pickle(path_model + 'X_Train_mc16e.pkl')
y_train_16e = pd.read_pickle(path_model + 'y_Train_mc16e.pkl')

X_train = []
y_train = []
 
X_train += [X_train_16a]
X_train += [X_train_16d]
X_train += [X_train_16e]
X_train = pd.concat(X_train)

y_train += [y_train_16a]
y_train += [y_train_16d]
y_train += [y_train_16e]
y_train = pd.concat(y_train)

VariablesToModel = list(X_train)
if 'weight' in VariablesToModel:
    VariablesToModel.remove('weight')
    X_train = X_train[VariablesToModel]
#print(VariablesToModel)

X_train = X_train.values
y_train = y_train.values

Pweights = np.random.poisson(1, len(y_train)) #poissonian weights for boostrap resampling with replacement

#separate training and validation sets                                                                                                                                                                     
X_train_noval, X_val, y_train_noval, y_val, sw_noval, sw_val = train_test_split(X_train, y_train, Pweights, test_size=0.2, random_state=1)

path_model += 'FULLSTAT/'

try:
    os.mkdir(path_model)
    print("Directory ", path_model,  " Created ")
except FileExistsError:
    print("Directory ", path_model,  " already exists")


#standardize datasets to the mean and the std of training set

scaler = preprocessing.StandardScaler().fit(X_train_noval)

scaler_filename = "training_scaler"
X_train_noval = scaler.transform(X_train_noval)
X_val = scaler.transform(X_val)

joblib.dump(scaler, path_model + scaler_filename + '.bin', compress=True)

path_model += '/Bootstrap/' 
try:                                                                                                                                                                            
    os.mkdir(path_model)                                                                                                                                                          
    print("Directory ", path_model,  " Created ")                                                                                                                                          
except FileExistsError:                                                                                                                                                                  
    print("Directory ", path_model,  " already exists")      

path_model += 'm_' + sys.argv[1] + '/'  #here we should modify the output name each time it is launched
try: 
    os.mkdir(path_model)  
    print("Directory ", path_model,  " Created ") 
except FileExistsError: 
    print("Directory ", path_model,  " already exists") 


#create the model


class Loss(tf.keras.losses.Loss):

    def __call__(self, y_true, y_pred, sample_weight=None):
        weights = tf.ones_like(y_true, tf.dtypes.float32)
        if(sample_weight!=None):
            weights = tf.convert_to_tensor(sample_weight, tf.dtypes.float32)

        ones_elements=tf.math.multiply( tf.cast(tf.equal(y_true, tf.ones_like(y_true)), tf.dtypes.float32), weights)
        evt_1 = tf.cast(tf.math.reduce_sum(ones_elements), tf.dtypes.float32)
        zeros_elements = tf.math.multiply(tf.cast(tf.equal(y_true, tf.zeros_like(y_true)), tf.dtypes.float32), weights)
        evt_0 =  tf.cast(tf.math.reduce_sum(zeros_elements), tf.dtypes.float32)

        f_loss = ((1.0 - tf.cast(y_true,  tf.dtypes.float32)) * (1.0/evt_0)  * tf.math.pow((K.exp(y_pred/10.)),5) +  tf.cast(y_true, tf.dtypes.float32) * (1.0/evt_1) * tf.math.pow(K.exp(y_pred/10.)+1*10**(-5),-5))
        f_loss = tf.math.multiply(f_loss, weights)

        return K.sum(f_loss)


print (Fore.RED+"--------------------------------")                                                                                                                                                   
print (Back.RED+"      BUILDING DNN MODEL      ")                                                                                                                                                     
print (Fore.RED+"--------------------------------")       

N_input = len(VariablesToModel)
width = 20 # number of neurons per layer                                                                                                                                              
dropout = 0.1
depth = 3# number of hidden layers                                                                                                                                                                    
lr = 0.01 #learning rate


visible = Input(shape=(N_input,))
hidden = Dense(width, activation='relu', name='dense_layer_0')(visible)
hidden = Dropout(dropout, name = 'dropout_0')(hidden)

for i in range(0, depth): 
    hidden = Dense(width, activation='relu', name='dense_layer_'+str(i+1))(hidden)
    hidden = Dropout(dropout, name = 'dropout_'+str(i+1))(hidden)
    
hidden = Dense(1, name='dense_layer_out')(hidden)
model = Model(inputs=visible, outputs=hidden)

opt = tf.keras.optimizers.Adam(learning_rate=lr)
model.compile(loss=Loss(), optimizer='Adam')


callbacks = [
    EarlyStopping(verbose=True, patience=100, monitor='val_loss'),  
    ModelCheckpoint(path_model + '_model.h5', monitor='val_loss', verbose=True, save_best_only=True),
    tf.keras.callbacks.TerminateOnNaN(),
]
 
FitHistory = model.fit(X_train_noval, y_train_noval, epochs=1600, batch_size=len(y_train_noval), validation_data=(X_val, y_val, sw_val), callbacks=callbacks, verbose=1, sample_weight=sw_noval)   


if (((np.isnan(FitHistory.history['loss']).any()) == False) & ((np.isnan(FitHistory.history['val_loss']).any()) == False)):
    print(Fore.RED + 'Saving model in ' + path_model)
    model.save(path_model+ 'model.h5')
else:
    os.rmdir(path_model)
  
