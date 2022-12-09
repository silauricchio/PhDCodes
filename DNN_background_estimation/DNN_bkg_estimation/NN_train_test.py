#!/bin/python                                                                                                                                                                                         

#decide here if you want either to fit or to evaluate an existent model or both
fit = True
doRewPlots = False
doFeaturesRanking = False

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
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix

from collections import Counter

import sys
import NN_performances_lib as lib

Save_json_model = False

model_name = 'fatjet_HwithNTracks_andTrackJets'

#change with your path
path_pickles = '/eos/user/s/sauricch/MachineLearning/YXH/pickle_files/Ntuples_v1p5_WP60/'
path_model= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_165_200_v1p5b_WP60/' + model_name +  '/'
path_output = '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/output/' + model_name + '_FULLSTAT_Ntuples_v1p5_trainAllHSB_WP60/'

try:
    os.mkdir(path_output)
    print("Directory ", path_output,  " Created ")
except FileExistsError:
    print("Directory ", path_output,  " already exists")


#open pickles files

X_train_16a = pd.read_pickle(path_model + 'X_Train_mc16a.pkl')
y_train_16a = pd.read_pickle(path_model + 'y_Train_mc16a.pkl')
X_test_16a = pd.read_pickle(path_model + 'X_Test_mc16a.pkl')
y_test_16a = pd.read_pickle(path_model + 'y_Test_mc16a.pkl')

X_train_16d = pd.read_pickle(path_model + 'X_Train_mc16d.pkl')
y_train_16d = pd.read_pickle(path_model + 'y_Train_mc16d.pkl')
X_test_16d = pd.read_pickle(path_model + 'X_Test_mc16d.pkl')
y_test_16d = pd.read_pickle(path_model + 'y_Test_mc16d.pkl')

X_train_16e = pd.read_pickle(path_model + 'X_Train_mc16e.pkl')
y_train_16e = pd.read_pickle(path_model + 'y_Train_mc16e.pkl')
X_test_16e = pd.read_pickle(path_model + 'X_Test_mc16e.pkl')
y_test_16e = pd.read_pickle(path_model + 'y_Test_mc16e.pkl')

X_train = []
y_train = []
X_test = []
y_test = []

X_train += [X_train_16a]
X_train += [X_train_16d]
X_train += [X_train_16e]
X_train = pd.concat(X_train)

y_train += [y_train_16a]
y_train += [y_train_16d]
y_train += [y_train_16e]
y_train = pd.concat(y_train)

X_test += [X_test_16a]
X_test += [X_test_16d]
X_test += [X_test_16e]
X_test = pd.concat(X_test)

y_test += [y_test_16a]
y_test += [y_test_16d]
y_test += [y_test_16e]
y_test = pd.concat(y_test)


VariablesToModel = list(X_train)
print(VariablesToModel)


X_train = X_train.values
X_test = X_test.values
y_train = y_train.values
y_test = y_test.values


path_model += 'FULLSTAT/'

try:
    os.mkdir(path_model)
    print("Directory ", path_model,  " Created ")
except FileExistsError:
    print("Directory ", path_model,  " already exists")


#separate training and validation sets
X_train_noval, X_val, y_train_noval, y_val = train_test_split(X_train, y_train, test_size=0.2)

#normalization factors for training
cls_ytrain_count = Counter(y_train_noval)
cls_yval_count = Counter(y_val)
wCateg0_train = 1/cls_ytrain_count[0]   
wCateg1_train = 1/cls_ytrain_count[1]

#standardize datasets to the mean and the std of training set (the one without validation)

scaler = preprocessing.StandardScaler().fit(X_train)

print("       ")
print("scaler info")
print("means")
print(scaler.mean_)
print("scales")
print(scaler.scale_)
print("       ")


scaler_filename = "training_scaler"
joblib.dump(scaler, path_model + scaler_filename + '.bin', compress=True)

X_train = scaler.transform(X_train)
X_train_noval = scaler.transform(X_train_noval)
X_val = scaler.transform(X_val)
X_test = scaler.transform(X_test)


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


if fit :
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
        
    FitHistory = model.fit(X_train_noval, y_train_noval, epochs=1600, batch_size=len(y_train_noval), validation_data=(X_val, y_val, sw_val), callbacks=callbacks, verbose=1, sample_weight=sw_noval)

    lib.plotTrainPerformance(path_output, FitHistory, logY=True)

    print(Fore.RED + 'Saving model in ' + path_model)
    model.save(path_model+ 'model.h5')


if not fit:
    #opening the model
    print(Fore.RED + 'Opening model in ' + path_model)
    model = load_model(path_model + 'model.h5', compile=False)



if Save_json_model: # save the model in json format for the reader 
    # serialize model to json                                                                                                                                                                             
    json_model = model.to_json()
    
    #save the model architecture to JSON file                                                                                                                                                               
    with open(path_model + 'model.json', 'w') as json_file:
        json_file.write(json_model)

    #saving the weights of the model                                                                                                                                                                        
    model.save_weights(path_model + 'model_weights.h5')

  
#read network on training and test sets
X_train_categ0 = X_train[y_train==0]
X_train_categ1 = X_train[y_train==1]

X_test_categ0 = X_test[y_test==0]
X_test_categ1 = X_test[y_test==1]


print ('Running model prediction on Xtrain')
ypred_train= model.predict(X_train, verbose = True, batch_size=2048)

print ('Running model prediction on Xtest')
ypred_test= model.predict(X_test, verbose = True, batch_size=2048)

print ('Running model prediction on Xtrain categ HSB0')
ypred_train_categ0 = model.predict(X_train_categ0, verbose = True, batch_size=2048)

print ('Running model prediction on Xtest categ HSB0')
ypred_test_categ0 = model.predict(X_test_categ0, verbose = True, batch_size=2048)

print ('Running model prediction on Xtrain categ HSB1')
ypred_train_categ1 = model.predict(X_train_categ1, verbose = True, batch_size=2048)

print ('Running model prediction on Xtest categ HSB1')
ypred_test_categ1 = model.predict(X_test_categ1, verbose = True, batch_size=2048)


fpr_w, tpr_w, thresholds_w = roc_curve(y_test, ypred_test)
roc_auc_w = auc(fpr_w, tpr_w)
print ('{:<35} {:<25.3f}'.format(Fore.GREEN+'ROC AUC - Test',roc_auc_w))

fpr_train_w, tpr_train_w, thresholds_train_w = roc_curve(y_train, ypred_train)
roc_auc_train_w = auc(fpr_train_w, tpr_train_w)
print ('{:<35} {:<25.3f}'.format(Fore.GREEN+'ROC AUC - Train',roc_auc_train_w))

lib.printROCCurve(path_output, fpr_w, tpr_w, fpr_train_w, tpr_train_w, roc_auc_w, roc_auc_train_w)


min = 0
max = 3
nbins = 60
bins=np.linspace(min, max, nbins)
bin_size = (max-min)/nbins
plt.hist(np.exp(ypred_train), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'train'], density=True)
plt.hist(np.exp(ypred_test), bins=bins, histtype='step', lw=2, label=[r'test'], density=True)
plt.xlabel('exp(NN score)')
plt.ylabel('Entries/'+str(bin_size))
plt.yscale('log')
plt.legend(loc="upper right")
plt.savefig(path_output + "TrainTestScore.png")
plt.clf()


min = 0
max = 3
nbins = 60
bins=np.linspace(min, max, nbins)
bin_size = (max-min)/nbins
plt.hist(np.exp(ypred_train_categ0), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'train HSB0'], density=True)
plt.hist(np.exp(ypred_test_categ0), bins=bins, histtype='step', lw=2, label=[r'test HSB0'], density=True)
plt.xlabel('exp(NN score)')
plt.ylabel('Entries/'+str(bin_size))
plt.yscale('log')
plt.legend(loc="upper right")
plt.savefig(path_output + "TrainTestScore_onHSB0.png")
plt.clf()

min = 0
max = 3
nbins = 60
bins=np.linspace(min, max, nbins)
bin_size = (max-min)/nbins
plt.hist(np.exp(ypred_train_categ1), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'train HSB1'], density=True)
plt.hist(np.exp(ypred_test_categ1), bins=bins, histtype='step', lw=2, label=[r'test HSB1'], density=True)
plt.xlabel('exp(NN score)')
plt.ylabel('Entries/'+str(bin_size))
plt.yscale('log')
plt.legend(loc="upper right")
plt.savefig(path_output + "TrainTestScore_onHSB1.png")
plt.clf()

min = 0
max = 3
nbins = 60
bins=np.linspace(min, max, nbins)
bin_size = (max-min)/nbins
plt.hist(np.exp(ypred_train_categ0), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'train HSB0'], density=True)
plt.hist(np.exp(ypred_test_categ0), bins=bins, histtype='step', lw=2, label=[r'test HSB0'], density=True)
plt.hist(np.exp(ypred_train_categ1), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'train HSB1'], density=True)
plt.hist(np.exp(ypred_test_categ1), bins=bins, histtype='step', lw=2, label=[r'test HSB1'], density=True)
plt.xlabel('exp(NN score)')
plt.ylabel('Entries/'+str(bin_size))
plt.yscale('log')
plt.legend(loc="upper right")
plt.savefig(path_output + "TrainTestScore_onHSB0HSB1.png")
plt.clf()




#open the dataframes with all variables
path_pickles_16a = path_pickles + 'mc16a/'
print(Fore.BLUE+'Opening the DataFrame HSB0 for all mc campaignes')
DF_HSB0_16a = pd.read_pickle(path_pickles_16a + 'data_HSB0_Full.pkl')
print(Fore.BLUE+'Opening the DataFrame HSB1 for all mc campaignes')
DF_HSB1_16a = pd.read_pickle(path_pickles_16a + 'data_HSB1_Full.pkl')

path_pickles_16d = path_pickles +'mc16d/'
DF_HSB0_16d = pd.read_pickle(path_pickles_16d + 'data_HSB0_Full.pkl')
DF_HSB1_16d = pd.read_pickle(path_pickles_16d + 'data_HSB1_Full.pkl')
                
path_pickles_16e = path_pickles +'mc16e/'
DF_HSB0_16e = pd.read_pickle(path_pickles_16e + 'data_HSB0_Full.pkl')
DF_HSB1_16e = pd.read_pickle(path_pickles_16e + 'data_HSB1_Full.pkl')

DF_HSB0_full = []
DF_HSB1_full = []

DF_HSB0_full += [DF_HSB0_16a]
DF_HSB0_full += [DF_HSB0_16d]
DF_HSB0_full += [DF_HSB0_16e]
DF_HSB0_full = pd.concat(DF_HSB0_full)

DF_HSB1_full += [DF_HSB1_16a]
DF_HSB1_full += [DF_HSB1_16d]
DF_HSB1_full += [DF_HSB1_16e]
DF_HSB1_full = pd.concat(DF_HSB1_full)


               
twotrackjets = False
for v in VariablesToModel :
    if 'trackjet' in v: twotrackjets = True
    
if twotrackjets==True:
    print(Fore.BLUE+str('Select only event with at least 2 track jets in full dataset'))
    sel_trackjets_HSB0 = ( (DF_HSB0_full["trackjet1_pt"]>=0) & (DF_HSB0_full["trackjet2_pt"]>=0) )
    DF_HSB0 = DF_HSB0_full[sel_trackjets_HSB0]
    
    sel_trackjets_HSB1 = ( (DF_HSB1_full["trackjet1_pt"]>=0) & (DF_HSB1_full["trackjet2_pt"]>=0) )
    DF_HSB1 = DF_HSB1_full[sel_trackjets_HSB1]
    
else:
    DF_HSB0 = DF_HSB0_full
    DF_HSB1 = DF_HSB1_full

    
DF_HSB1["deta_trackjets"] = np.absolute(DF_HSB1["trackjet1_eta"] - DF_HSB1["trackjet2_eta"])
DF_HSB0["deta_trackjets"] = np.absolute(DF_HSB0["trackjet1_eta"] - DF_HSB0["trackjet2_eta"])

DF_HSB1["deta_fatjets"] = np.abs(DF_HSB1["etaH"] - DF_HSB1["etaV"])
DF_HSB0["deta_fatjets"] = np.abs(DF_HSB0["etaH"] - DF_HSB0["etaV"])

DF_HSB1["deltaR_trackjets"] = np.sqrt(np.power(DF_HSB1["trackjet1_eta"] - DF_HSB1["trackjet2_eta"], 2) + np.power(DF_HSB1["trackjet1_phi"] - DF_HSB1["trackjet2_phi"],2))
DF_HSB1["deltaR_fatjets"] = np.sqrt(np.power(DF_HSB1["etaH"] - DF_HSB1["etaV"], 2) + np.power(DF_HSB1["phiH"] - DF_HSB1["phiV"],2))

DF_HSB0["deltaR_trackjets"] = np.sqrt(np.power(DF_HSB0["trackjet1_eta"] - DF_HSB0["trackjet2_eta"], 2) + np.power(DF_HSB0["trackjet1_phi"] - DF_HSB0["trackjet2_phi"],2))
DF_HSB0["deltaR_fatjets"] = np.sqrt(np.power(DF_HSB0["etaH"] - DF_HSB0["etaV"], 2) + np.power(DF_HSB0["phiH"] - DF_HSB0["phiV"],2))

DF_HSB0["dphi_trackjets"] = DF_HSB0["trackjet1_phi"]-DF_HSB0["trackjet2_phi"]
DF_HSB0["dphi_trackjets"][DF_HSB0["dphi_trackjets"]<0] += 2*np.pi

DF_HSB1["dphi_trackjets"] = DF_HSB1["trackjet1_phi"]-DF_HSB1["trackjet2_phi"]
DF_HSB1["dphi_trackjets"][DF_HSB1["dphi_trackjets"]<0] += 2*np.pi

DF_HSB0["dphi_fatjets"] = DF_HSB0["phiH"]-DF_HSB0["phiV"]
DF_HSB0["dphi_fatjets"][DF_HSB0["dphi_fatjets"]<0] += 2*np.pi

DF_HSB1["dphi_fatjets"] = DF_HSB1["phiH"]-DF_HSB1["phiV"]
DF_HSB1["dphi_fatjets"][DF_HSB1["dphi_fatjets"]<0] += 2*np.pi

theta_DF_HSB0 = 2* np.arctan(1/(np.exp(DF_HSB0['trackjet1_eta'])))
DF_HSB0['trackjet1_e'] = np.sqrt((DF_HSB0['trackjet1_m']**2) + ((DF_HSB0['trackjet1_pt']/np.cos(theta_DF_HSB0))**2))

theta_DF_HSB1 = 2* np.arctan(1/(np.exp(DF_HSB1['trackjet1_eta'])))
DF_HSB1['trackjet1_e'] = np.sqrt((DF_HSB1['trackjet1_m']**2) + ((DF_HSB1['trackjet1_pt']/np.cos(theta_DF_HSB1))**2))

theta_DF_HSB0 = 2* np.arctan(1/(np.exp(DF_HSB0['trackjet2_eta'])))
DF_HSB0['trackjet2_e'] = np.sqrt((DF_HSB0['trackjet2_m']**2) + ((DF_HSB0['trackjet2_pt']/np.cos(theta_DF_HSB0))**2))

theta_DF_HSB1 = 2* np.arctan(1/(np.exp(DF_HSB1['trackjet2_eta'])))
DF_HSB1['trackjet2_e'] = np.sqrt((DF_HSB1['trackjet2_m']**2) + ((DF_HSB1['trackjet2_pt']/np.cos(theta_DF_HSB1))**2))

X_HSB0 = DF_HSB0[VariablesToModel]


#this is for to be sure that there are no problems with input variables

goodsel_HSB0 = pd.Series(True, index= X_HSB0.index)
for v in VariablesToModel:
    goodsel_HSB0 &= (np.abs(X_HSB0[v])!= np.inf)

print("HSB0 events before infinities removing: ", X_HSB0.shape[0])
X_HSB0 = X_HSB0[goodsel_HSB0]    
print("HSB0 events after infinities removing: ", X_HSB0.shape[0])

X_HSB0 = X_HSB0.values

X_HSB0 = scaler.transform(X_HSB0)

print ('Running model prediction on full HSB0')
ypred_HSB0 = model.predict(X_HSB0, verbose = True, batch_size=2048)

min = 0
max = 3
nbins = 60
bins=np.linspace(min, max, nbins)
bin_size = (max-min)/nbins
ns, bins, patches= plt.hist(np.exp(ypred_HSB0), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0'])
plt.xlabel('exp(NN score)')
plt.ylabel('Entries')
plt.yscale('log')
plt.legend(loc="upper right")
plt.savefig(path_output + "Score_onHSB0.png")
plt.clf()


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 


#create the directory for plots if it doesn't exist                                                                                                                                                      

path_plots = path_output + 'plots/'

try:
    os.mkdir(path_plots)
    print("Directory ", path_plots, " Created ")
except FileExistsError:
    print("Directory ", path_plots , " already exists")

path_plots = path_plots  + 'HSB/'
try:
    os.mkdir(path_plots)
    print("Directory ", path_plots, " Created ")
except FileExistsError:
    print("Directory ", path_plots , " already exists")

#compare variables after reweighting
Variables = ["trackjet1_eta","trackjet2_eta", "trackjet1_pt", "trackjet2_pt", "trackjet1_phi", "trackjet2_phi", "trackjet1_m", "trackjet2_m", "deta_trackjets", "pTH", "pTV", "etaH", "etaV", "phiH", "phiV", "mH", "mV", "mVH", "d2V", "deltaR_trackjets", "deltaR_fatjets", "EH", "EV", "ntrkVVJJ_H", "ntrkVVJJ_V", "deta_fatjets", "trackjet1_btag_score", "trackjet2_btag_score", "HCand_nBTags", "nTrackJetsH", "dphi_fatjets", "dphi_trackjets"]

min_list = [-2.5,-2.5, 0, 0, -3.5, -3.5, 0, 0]
max_list = [2.5, 2.5, 2000, 500, 3.5, 3.5, 100, 50]
binning_list = [30, 30, 50, 25, 20, 20, 50 , 25]

#lib.comparisonPlots_ratio(DF_type1=DF_HSB1, DF_type2=DF_HSB0, DF_type3=DF_HSB0 , label1 = 'HSB1', label2 = 'HSB0 reweighted', label3 = 'HSB0', variable_list=Variables, a_list=min_list, b_list=max_list, nbin_list=binning_list, path_plots=path_plots, sample_name = 'data', name_histo = 'after_reweighting', Weights2 = np.exp(ypred_HSB0), normalization=True)

binning = (
    [-2.5, -1.8, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2.5], #trackjet1 eta
    [-2.5, -1.8, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2.5], #trackjet2 eta 
    [0, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 450, 500,  700, 800, 1000, 2000], #trackjet1 pT 
    [0, 40, 60, 80, 100, 120, 150, 200, 250, 300, 1000],  #trackjet2 pT 
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #trackjet1 phi 
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #trackjet2 phi
    [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 40, 100], #trackjet1 m  
    [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 50], #trackjet2 m
    [0.03, 0.06, 0.09, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.30, 0.33, 0.36, 0.39, 0.42, 0.45, 0.48, 0.51, 0.54, 0.57, 0.6, 0.70, 0.80, 0.90, 1.5], #deta trackjets
    [200, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1800], #pTH
    [200, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1100, 1200, 1300, 1800], #pTV
    [-2, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 2], #etaH
    [-2, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 2], #etaV
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #phiH
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #phiV
    [145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200], #mH   
    [0, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 360, 410, 600], #mV
    [1000, 1500, 1600, 1700, 1800, 1900, 2000, 2200, 2400, 2600, 3000, 3500, 5000], #mVH   
    [0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2], #d2V   
    [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4],#deltaR_trackjets
    [1.4, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 6], #deltaR_fatjets 
    [0, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 4000], #EH
    [0, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 3000,  4000], #EV
    [0, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 100], #ntracks H
    [0, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 100], #ntracks V
    [0, 0.6, 0.80, 0.90, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2], #deta fatjets   
    [-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12], #trackjet1_btagscore
    [-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12], #trackjet2_btagscore
    [0, 1, 2, 3, 4], #nBtag jets H
    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], #ntrackjetsH
    [0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 5.75, 6, 6.25, 6.5], #deltaphi fatjets
    [0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 5.75, 6, 6.25, 6.5], #deltaphi trackjets
)

if doRewPlots:
    DF_HSB0 = DF_HSB0[goodsel_HSB0]
    lib.comparisonPlots_ratio(DF_type1=DF_HSB1, DF_type2=DF_HSB0, DF_type3=DF_HSB0 , label1 = 'HSB1', label2 = 'HSB0 reweighted', label3 = 'HSB0', variable_list=Variables,  var_bins = binning,  path_plots=path_plots, sample_name = 'data', name_histo = 'after_reweighting', Weights2 = np.exp(ypred_HSB0), normalization=True)

if doFeaturesRanking:
    lib.DoFeaturesRanking(path_model, X_train, y_train, VariablesToModel)
