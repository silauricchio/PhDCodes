#!/bin/python  

import os, re, pickle
import uproot
import pandas as pd
pd.options.mode.chained_assignment = None  # default='warn'
import numpy as np
import sys
np.set_printoptions(threshold=sys.maxsize)
import sklearn.utils
import matplotlib.pyplot as plt
from colorama import init, Fore, Back, Style
init(autoreset=True)
from sklearn import preprocessing
from sklearn.preprocessing import StandardScaler, LabelEncoder
import joblib

import tensorflow as tf
from keras.models import Sequential,Model,load_model
from keras.layers.core import Dense, Activation
from keras.layers import BatchNormalization, Dropout, concatenate
from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.optimizers import Adam
from keras import backend as K
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix

Save_newTree = True
Print_Norm_factors = False #True if you want to print normalization factors for bootstrap sistematics

mc_campaign = 'mc16a'
#mc_campaign = 'mc16d'                                                                                                                                                                                 
#mc_campaign = 'mc16e'                                                                                                                                                                                 
  
file_name = 'data_0'

model_name = 'fatjet_HwithNTracks_andTrackJets'

dataset_path = '/eos/user/s/sauricch/ReaderOutput/SuperNtuples_v1.9_skimmed/'+ mc_campaign +'/'
path_pickles = '/eos/user/s/sauricch/MachineLearning/YXH/pickle_files/SuperNtuples_v1p9_WP60/' 
path_model= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_145_175_v1p6_schemeA_WP60/fatjet_HwithNTracks_andTrackJets/'
path_pdf = path_model + 'FULLSTAT/Bootstrap_pdfs/'

try:
    os.mkdir(path_pdf)
    print("Directory ", path_pdf,  " Created ")
except FileExistsError:
    print("Directory ", path_pdf, " already exists")

if file_name == 'data':
    files = ["data.root"]


if model_name == 'fatjet_HwithNTracks_andTrackJets':
     VariablesToModel = ['pTH', 'etaH', 'phiH', 'EH', 'ntrkVVJJ_H', "trackjet1_pt", "trackjet1_eta", "trackjet1_phi", "trackjet1_m", "trackjet2_pt", "trackjet2_eta", "trackjet2_phi", "trackjet2_m"]
     Variables_for_sel = ['pTH', 'etaH', 'phiH', 'EH', 'ntrkVVJJ_H', "trackjet1_pt", "trackjet1_eta", "trackjet1_phi", "trackjet1_m", "trackjet2_pt", "trackjet2_eta", "trackjet2_phi", "trackjet2_m", "mH", "HCand_XbbScore", "d2TracksV", "SmallJet_X_deltaY", "SmallJet_X_pT_imb", "isX_smalljets", "mVHres"]

print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16a/' + file_name + '_fulltree.pkl')
DF_list_16a = pd.read_pickle(path_pickles + 'mc16a/' + file_name + '_fulltree.pkl')
DF_list_16a = DF_list_16a[VariablesToModel]
print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16d/' + file_name + '_fulltree.pkl')
DF_list_16d = pd.read_pickle(path_pickles + 'mc16d/' + file_name + '_fulltree.pkl')
DF_list_16d = DF_list_16d[VariablesToModel]
print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16e/' + file_name + '_fulltree.pkl')
DF_list_16e = pd.read_pickle(path_pickles + 'mc16e/' + file_name + '_fulltree.pkl')
DF_list_16e = DF_list_16e[VariablesToModel]

DF_list = []
DF_list += [DF_list_16a]
DF_list += [DF_list_16d]
DF_list += [DF_list_16e]
DF_list = pd.concat(DF_list)

del DF_list_16a    
del DF_list_16d
del DF_list_16e

X = DF_list
del DF_list

for v in VariablesToModel:
    if 'trackjet' in v: 
        twotrackjets = True
    
if (twotrackjets==True): #insert default values for events without trackjets variables (with mean values in training region)
    
    sel_notrackjet1 = X['trackjet1_pt']<0
    sel_notrackjet2 = X['trackjet2_pt']<0

    X['trackjet1_pt'] = X['trackjet1_pt'].replace([-999], 344) 
    X['trackjet1_eta'] = X['trackjet1_eta'].replace([-999], 0.)
    X['trackjet1_phi'] = X['trackjet1_phi'].replace([-999], 0.)
    X['trackjet1_m'] = X['trackjet1_m'].replace([-999], 9.)

    X['trackjet2_pt'] = X['trackjet2_pt'].replace([-999], 93.)
    X['trackjet2_eta'] = X['trackjet2_eta'].replace([-999], 0)
    X['trackjet2_phi'] = X['trackjet2_phi'].replace([-999], 0)
    X['trackjet2_m'] = X['trackjet2_m'].replace([-999], 9.)
    

#load the models
modelpath = path_model + 'FULLSTAT/Bootstrap/'
#modelpath = path_model + 'FULLSTAT/old/'                                                                                                                                                                  

model_names = list()

print('We are in ' + str(modelpath))
print('subdirs here: ')
for subdirs in os.listdir(modelpath):
    print(subdirs)
    model_names.append(subdirs)


X = X.values
scaler = joblib.load(path_model + 'FULLSTAT/training_scaler.bin')
X = scaler.transform(X)

ypredictions = {}
outl = int(0)
for model_name in model_names:

    modelpath = path_model + 'FULLSTAT/Bootstrap/'                                                                                                                                                       
    #modelpath = path_model + 'FULLSTAT/old/'

    modelpath += model_name + '/'

    print('')
    print('')
    print('-----------------------------')

    print('Reading model -->', model_name)

    #opening models                                                                                                                                                                                        
    model = load_model(modelpath + 'model.h5', compile=False)    


    print ('Running model prediction')

    ypred = model.predict(X, verbose = True, batch_size=2048)
    #print(ypred[:20])

    #remove models with outlayers                                                                                                                                                                       
    q_low = np.quantile(a=np.exp(ypred)[:,0], q=0.01)
    q_high = np.quantile(a=np.exp(ypred)[:,0], q=0.99)
    median = np.median(a=np.exp(ypred)[:,0])
    outlayers = np.any(np.exp(ypred)>10*q_high)
    if(outlayers):
        print('model ' + model_name + ' with outlayers in untagged mH[50, 200]GeV region!!!')
        print('median :' + str(median))
        print('quantile at 0.05: ' + str(q_low))
        print('quantile at 0.95: ' + str(q_high))
        outl = outl + 1
    else:
        ypredictions[model_name] = ypred


good_models = len(model_names) - outl
print('')
print('There are ' + str(good_models) + ' good models')
print('')


#for memory problems, delete this DF and create another one with more variables
del X

print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16a/' + file_name + '_fulltree.pkl')
DF_list_16a = pd.read_pickle(path_pickles + 'mc16a/' + file_name + '_fulltree.pkl')
DF_list_16a = DF_list_16a[Variables_for_sel]
print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16d/' + file_name + '_fulltree.pkl')
DF_list_16d = pd.read_pickle(path_pickles + 'mc16d/' + file_name + '_fulltree.pkl')
DF_list_16d = DF_list_16d[Variables_for_sel]
print(Fore.BLUE+'Opening DataFrame ' + path_pickles + 'mc16e/' + file_name + '_fulltree.pkl')
DF_list_16e = pd.read_pickle(path_pickles + 'mc16e/' + file_name + '_fulltree.pkl')
DF_list_16e = DF_list_16e[Variables_for_sel]

DF_list = []
DF_list += [DF_list_16a]
DF_list += [DF_list_16d]
DF_list += [DF_list_16e]
DF_list = pd.concat(DF_list)

size_16a = len(DF_list_16a['pTH'])
size_16d = len(DF_list_16d['pTH'])
size_16e = len(DF_list_16e['pTH'])


#del DF_list_16a
#del DF_list_16d
#del DF_list_16e


if Print_Norm_factors:
    
    HSB0_train_sel = ((DF_list['mH']>145) & (DF_list['mH']<175) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46))
    HSB0_val_sel = ((DF_list['mH']>165) & (DF_list['mH']<200) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46))
    LSB0_val_sel = ((DF_list['mH']>65) & (DF_list['mH']<75) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46))
    LSB0_val2_sel = ((DF_list['mH']>50) & (DF_list['mH']<65) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46))
    CR0_sel = ((DF_list['mH']>75) & (DF_list['mH']<145) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46))
    
    HSB0_train_merged_sel = ((DF_list['mH']>145) & (DF_list['mH']<175) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['d2TracksV']<1.2))
    HSB0_val_merged_sel = ((DF_list['mH']>165) & (DF_list['mH']<200) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['d2TracksV']<1.2))
    LSB0_val_merged_sel = ((DF_list['mH']>65) & (DF_list['mH']<75) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['d2TracksV']<1.2))
    LSB0_val2_merged_sel = ((DF_list['mH']>50) & (DF_list['mH']<65) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['d2TracksV']<1.2))
    CR0_merged_sel = ((DF_list['mH']>75) & (DF_list['mH']<145) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['d2TracksV']<1.2))
    
    HSB0_train_resolved_sel = ((DF_list['mH']>145) & (DF_list['mH']<175) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['isX_smalljets']==1) & (DF_list['mVHres']>1300) & (DF_list['d2TracksV']>1.2) & (abs(DF_list['SmallJet_X_deltaY'])<2.5) & (DF_list['SmallJet_X_pT_imb']<0.8))
    HSB0_val_resolved_sel = ((DF_list['mH']>165) & (DF_list['mH']<200) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) &  (DF_list['isX_smalljets']==1) & (DF_list['mVHres']>1300) & (DF_list['d2TracksV']>1.2) & (abs(DF_list['SmallJet_X_deltaY'])<2.5) & (DF_list['SmallJet_X_pT_imb']<0.8))
    LSB0_val_resolved_sel = ((DF_list['mH']>65) & (DF_list['mH']<75) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['isX_smalljets']==1) & (DF_list['mVHres']>1300) & (DF_list['d2TracksV']>1.2) & (abs(DF_list['SmallJet_X_deltaY'])<2.5) & (DF_list['SmallJet_X_pT_imb']<0.8))
    LSB0_val2_resolved_sel = ((DF_list['mH']>50) & (DF_list['mH']<65) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['isX_smalljets']==1) & (DF_list['mVHres']>1300) & (DF_list['d2TracksV']>1.2) & (abs(DF_list['SmallJet_X_deltaY'])<2.5) & (DF_list['SmallJet_X_pT_imb']<0.8))
    CR0_resolved_sel = ((DF_list['mH']>75) & (DF_list['mH']<145) & (DF_list['HCand_XbbScore']>(-2)) & (DF_list['HCand_XbbScore']<2.46) & (DF_list['isX_smalljets']==1) & (DF_list['mVHres']>1300) & (DF_list['d2TracksV']>1.2) & (abs(DF_list['SmallJet_X_deltaY'])<2.5) & (DF_list['SmallJet_X_pT_imb']<0.8))
    
del DF_list
    
Weights_list = []
Weights_norm_list_HSB0_train = []
Weights_norm_list_HSB0_val = []
Weights_norm_list_LSB0_val = []
Weights_norm_list_LSB0_val2 = []
Weights_norm_list_CR0 = []

Weights_norm_list_HSB0_train_merged = []
Weights_norm_list_HSB0_val_merged = []
Weights_norm_list_LSB0_val_merged = []
Weights_norm_list_LSB0_val2_merged = []
Weights_norm_list_CR0_merged = []

Weights_norm_list_HSB0_train_resolved = []
Weights_norm_list_HSB0_val_resolved = []
Weights_norm_list_LSB0_val_resolved = []
Weights_norm_list_LSB0_val2_resolved = []
Weights_norm_list_CR0_resolved = []

for model_name in model_names:
    if model_name in list(ypredictions):
        Weights_list.append(np.exp(ypredictions[model_name]))
        if Print_Norm_factors:
            Weights_norm_list_HSB0_train.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_train_sel][ypredictions[model_name][HSB0_train_sel]!=0]))))
            Weights_norm_list_HSB0_val.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_val_sel][ypredictions[model_name][HSB0_val_sel]!=0]))))
            Weights_norm_list_LSB0_val.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val_sel][ypredictions[model_name][LSB0_val_sel]!=0]))))
            Weights_norm_list_LSB0_val2.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val2_sel][ypredictions[model_name][LSB0_val2_sel]!=0]))))
            Weights_norm_list_CR0.append(float(1/sum(np.exp(ypredictions[model_name][CR0_sel][ypredictions[model_name][CR0_sel]!=0]))))
            
            Weights_norm_list_HSB0_train_merged.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_train_merged_sel][ypredictions[model_name][HSB0_train_merged_sel]!=0]))))
            Weights_norm_list_HSB0_val_merged.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_val_merged_sel][ypredictions[model_name][HSB0_val_merged_sel]!=0]))))
            Weights_norm_list_LSB0_val_merged.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val_merged_sel][ypredictions[model_name][LSB0_val_merged_sel]!=0]))))
            Weights_norm_list_LSB0_val2_merged.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val2_merged_sel][ypredictions[model_name][LSB0_val2_merged_sel]!=0]))))
            Weights_norm_list_CR0_merged.append(float(1/sum(np.exp(ypredictions[model_name][CR0_merged_sel][ypredictions[model_name][CR0_merged_sel]!=0]))))
            
            Weights_norm_list_HSB0_train_resolved.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_train_resolved_sel][ypredictions[model_name][HSB0_train_resolved_sel]!=0]))))
            Weights_norm_list_HSB0_val_resolved.append(float(1/sum(np.exp(ypredictions[model_name][HSB0_val_resolved_sel][ypredictions[model_name][HSB0_val_resolved_sel]!=0]))))
            Weights_norm_list_LSB0_val_resolved.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val_resolved_sel][ypredictions[model_name][LSB0_val_resolved_sel]!=0]))))
            Weights_norm_list_LSB0_val2_resolved.append(float(1/sum(np.exp(ypredictions[model_name][LSB0_val2_resolved_sel][ypredictions[model_name][LSB0_val2_resolved_sel]!=0]))))
            Weights_norm_list_CR0_resolved.append(float(1/sum(np.exp(ypredictions[model_name][CR0_resolved_sel][ypredictions[model_name][CR0_resolved_sel]!=0]))))


if len(list(Weights_list))>2:
    average = np.median(a=Weights_list, axis=0) #for each event calculate the median and IQR of the N weights (N bootstraps)                                                                   
    Q1 = np.quantile(a=Weights_list, q=0.25, axis=0)
    Q2 = np.quantile(a=Weights_list, q=0.75, axis=0)
    IQR = Q2 - Q1

    weights_up = average + (IQR/2)
    weights_down = average - (IQR/2)
       
    if Print_Norm_factors:
        
        
        average_norm = np.median(a=Weights_norm_list_HSB0_train, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_train, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_train, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_train')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        
        average_norm = np.median(a=Weights_norm_list_HSB0_val, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_val, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_val, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_val')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        average_norm = np.median(a=Weights_norm_list_LSB0_val, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))

        
        average_norm = np.median(a=Weights_norm_list_LSB0_val2, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val2, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val2, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val2')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        average_norm = np.median(a=Weights_norm_list_CR0, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_CR0, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_CR0, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('CR0')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        


        average_norm = np.median(a=Weights_norm_list_HSB0_train_merged, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_train_merged, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_train_merged, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_train_merged')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        average_norm = np.median(a=Weights_norm_list_HSB0_val_merged, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_val_merged, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_val_merged, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_val_merged')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        
        average_norm = np.median(a=Weights_norm_list_LSB0_val_merged, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val_merged, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val_merged, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val_merged')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        


        average_norm = np.median(a=Weights_norm_list_LSB0_val2_merged, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val2_merged, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val2_merged, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val2_merged')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))

        
        average_norm = np.median(a=Weights_norm_list_CR0_merged, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_CR0_merged, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_CR0_merged, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('CR0_merged')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        
        
        average_norm = np.median(a=Weights_norm_list_HSB0_train_resolved, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_train_resolved, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_train_resolved, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_train_resolved')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))

        
        average_norm = np.median(a=Weights_norm_list_HSB0_val_resolved, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_HSB0_val_resolved, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_HSB0_val_resolved, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('HSB0_val_resolved')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        
        
        average_norm = np.median(a=Weights_norm_list_LSB0_val_resolved, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val_resolved, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val_resolved, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val_resolved')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        

        average_norm = np.median(a=Weights_norm_list_LSB0_val2_resolved, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_LSB0_val2_resolved, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_LSB0_val2_resolved, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('LSB0_val2_resolved')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
        
        '''
        min = 0                                                                                                                                                                                
        max = 0.6*pow(10,-5)                                                                                                                                                                               
        nbins = 20                                                                                                                                                                                        
        bins=np.linspace(min, max, nbins)                                                                                                                                                                  
        plt.hist(Weights_norm_list_LSB0_val2_resolved, bins=bins, histtype='stepfilled', lw=2, alpha=0.5)                                                                                                 
        plt.autoscale(enable=True)
        plt.xlabel('Normalization factors')                                                                                                                                                               
        plt.ylabel('Entries')                                                                                                                                                
        plt.axvline(x=average_norm, color='red', alpha=0.2)
        plt.axvline(x=up_norm, color='blue', alpha=0.2)
        plt.axvline(x=down_norm, color='green', alpha=0.2)
        #plt.yscale('log')                                                                                                                                                                         
        plt.savefig(path_pdf + "Normalization_LSB0_val2_resolved.pdf")                                                                                                                                  
        plt.clf()  
        '''

        
        average_norm = np.median(a=Weights_norm_list_CR0_resolved, axis=0)
        Q1_norm = np.quantile(a=Weights_norm_list_CR0_resolved, q=0.25, axis=0)
        Q2_norm = np.quantile(a=Weights_norm_list_CR0_resolved, q=0.75, axis=0)
        IQR_norm = Q2_norm - Q1_norm
        up_norm = average_norm + (IQR_norm/2)
        down_norm = average_norm - (IQR_norm/2)
        
        print('-----------------------------------')
        print('       Normalization factors      ')
        print('   ')
        print('CR0_resolved')
        print('average: ' + str(average_norm))
        print('up norm : ' + str(up_norm))
        print('down norm : ' + str(down_norm))
                
        '''
        min = 0                                                                                                                                                                               
        max = 0.2*pow(10,-5)                                                                                                                                                                               
        nbins = 20                                                                                                                                                                                        
        bins=np.linspace(min, max, nbins)                                                                                                                                                                  
        plt.hist(Weights_norm_list_CR0_resolved, bins=bins, histtype='stepfilled', lw=2, alpha=0.5)                                                                                                        
        plt.xlabel('Normalization factors')                                                                                                                                                               
        plt.ylabel('Entries')
        plt.autoscale(enable=True)                                                                                                                                                              
        #plt.yscale('log')                                                                                                                                                                         
        plt.axvline(x=average_norm, color='red', alpha=0.2)
        plt.axvline(x=up_norm, color='blue', alpha=0.2)
        plt.axvline(x=down_norm, color='green', alpha=0.2)
        plt.savefig(path_pdf + "Normalization_CR0_resolved.pdf")                                                                                                                     
        plt.clf()  
        '''


    '''
    min = 0                                                                                                                                                                                               
    max = 10                                                                                                                                                                                              
    nbins = 60                                                                                                                                                                                            
    bins=np.linspace(min, max, nbins)                                                                                                                                                                     
    bin_size = (max-min)/nbins                                                                                                                                                                            
    plt.hist(average, bins=bins, histtype='stepfilled', lw=2, alpha=0.5)                                                                                                          
    plt.xlabel('Nominal weights')                                                                                                                                                                         
    plt.ylabel('Entries')                                                                                                                                                      
    plt.yscale('log')                                                                                                                                                                                     
    plt.autoscale(enable=True)
    plt.savefig(path_pdf + "AverageScores.pdf")                                                                                                                                      
    plt.clf()  
    
    min = 0
    max = 10
    nbins = 60
    bins=np.linspace(min, max, nbins)
    bin_size = (max-min)/nbins
    plt.hist(weights_up, bins=bins, histtype='stepfilled', lw=2, alpha=0.5)
    plt.xlabel('Up weights')
    plt.ylabel('Entries')
    plt.yscale('log')
    plt.autoscale(enable=True)
    plt.savefig(path_pdf + "UpScore.pdf")
    plt.clf()

    min = 0
    max = 10
    nbins = 60
    bins=np.linspace(min, max, nbins)
    bin_size = (max-min)/nbins
    plt.hist(weights_down, bins=bins, histtype='stepfilled', lw=2, alpha=0.5)
    plt.xlabel('Down weights')
    plt.ylabel('Entries')
    plt.yscale('log')
    plt.autoscale(enable=True)
    plt.savefig(path_pdf + "DownScore.pdf")
    plt.clf()
    '''

    if Save_newTree:
        
        
        print('-----------------')
        print('     CHECK      ')
        print('pTH first event 16a  (from 16a): ' + str(DF_list_16a['pTH'][0]))
        print('median first event 16a : ' + str(average[0]))
        print('weights up first event 16a : ' + str(weights_up[0]))
        print('weights down first event 16a : ' + str(weights_down[0]))
        print(' ')
        
        print('-----------------')
        print('     CHECK      ')
        print('pTH last event 16a  (from 16a): ' + str(DF_list_16a['pTH'][size_16a-1]))
        print('median last event 16a : ' + str(average[size_16a-1]))
        print('weights up last event 16a : ' + str(weights_up[size_16a-1]))
        print('weights down last event 16a : ' + str(weights_down[size_16a-1]))
        print(' ')
        
        print('-----------------')
        print('     CHECK      ')
        print('pTH first event 16d  (from 16d): ' + str(DF_list_16d['pTH'][0]))
        print('median first event 16d : ' + str(average[size_16a]))
        print('weights up first event 16d : ' + str(weights_up[size_16a]))
        print('weights down first event 16d : ' + str(weights_down[size_16a]))
        print(' ')
        
        print('-----------------')
        print('     CHECK      ')
        print('pTH last event 16d  (from 16d): ' + str(DF_list_16d['pTH'][size_16d-1]))
        print('median last event 16d : ' + str(average[size_16a+size_16d-1]))
        print('weights up last event 16d : ' + str(weights_up[size_16a+size_16d-1]))
        print('weights down last event 16d : ' + str(weights_down[size_16a+size_16d-1]))
        print(' ')
        
        
        print('-----------------')
        print('     CHECK      ')
        print('pTH first event 16e  (from 16e): ' + str(DF_list_16e['pTH'][0]))
        print('median first event 16e : ' + str(average[size_16a+size_16d]))
        print('weights up first event 16e : ' + str(weights_up[size_16a+size_16d]))
        print('weights down first event 16e : ' + str(weights_down[size_16a+size_16d]))
        print(' ')
    
        print('-----------------')
        print('     CHECK      ')
        print('pTH last event 16e (from 16e): ' + str(DF_list_16e['pTH'][size_16e-1]))
        print('median last event 16e : ' + str(average[size_16a+size_16d+size_16e-1]))
        print('weights up last event 16e : ' + str(weights_up[size_16a+size_16d+size_16e-1]))
        print('weights down last event 16e : ' + str(weights_down[size_16a+size_16d+size_16e-1]))
        print(' ')
        

if Save_newTree:
    print('')
    print('-----------------------------')
    print(Fore.RED+'Saving a new tree with the average NN scores, Bootstrap errors and the normalization factors as different branches')
    
    file = dataset_path + file_name + ".root"
    if os.path.isfile(file):
        old_file = uproot.open(file)
        print("opening file: " + str(file))
        print(old_file.keys())
        tree = old_file["Nominal"]

        size = len(tree.array("mVH"))
        if mc_campaign=='mc16a':
            if size!=size_16a:
                print("dataframe 16a has not the same dimension of the tree ")
                print('size DF: ' + str(size_16a))
                print('size tree: ' + str(size))
                exit
                
        if mc_campaign=='mc16d':
            if size!=size_16d:
                print("dataframe 16d has not the same dimension of the tree ")
                print('size DF: ' + str(size_16d))
                print('size tree: ' + str(size))
                exit

        if mc_campaign=='mc16e':
            if size!=size_16e:
                print("dataframe 16e has not the same dimension of the tree ")
                print('size DF: ' + str(size_16e))
                print('size tree: ' + str(size))
                exit

        NNRewScore = np.full(size,float(-999))
        NNRewScoreUp = np.full(size,float(-999))
        NNRewScoreDown = np.full(size,float(-999))

        
        if mc_campaign=='mc16a':
            NNRewScore = average[:size_16a,0]
            NNRewScoreUp = weights_up[:size_16a,0]
            NNRewScoreDown = weights_down[:size_16a,0]

        if mc_campaign=='mc16d':
            NNRewScore = average[size_16a:(size_16a+size_16d),0]
            NNRewScoreUp = weights_up[size_16a:(size_16a+size_16d),0]
            NNRewScoreDown = weights_down[size_16a:(size_16a+size_16d),0]

        if mc_campaign=='mc16e':
            NNRewScore = average[(size_16a+size_16d):,0]
            NNRewScoreUp = weights_up[(size_16a+size_16d):,0]
            NNRewScoreDown = weights_down[(size_16a+size_16d):,0]


        
        DF_withScore = []
        if mc_campaign=='mc16a':
            DF_withScore = DF_list_16a[VariablesToModel]

        if mc_campaign=='mc16d':
            DF_withScore = DF_list_16d[VariablesToModel] 

        if mc_campaign=='mc16e':
            DF_withScore = DF_list_16e[VariablesToModel]


        DF_withScore["NNScore_nominal"] = NNRewScore
        DF_withScore["NNScore_up"] = NNRewScoreUp
        DF_withScore["NNScore_down"] = NNRewScoreDown
            

        print(DF_withScore.head(n=10))
        
        
        new_file_name = dataset_path + file_name + "_NNscores_Bootstrap.root"
        new_file = uproot.recreate(new_file_name)
        
        print("creating a new file: " + str(new_file_name))
        
        branches = tree.arrays( namedecode='utf-8')
        br_names = list(branches.keys())
        print(br_names)

        branch_dict = {}
        

        for b in br_names :
            branch_dict[b] = type(branches[b][0])
            if branch_dict[b]==np.uint64:
                branch_dict[b] = np.int64
            if branch_dict[b]==np.uint32:
                branch_dict[b] = np.int32
            #print(branch_dict[b], len(tree.array(b)))

        branch_dict['NNScore_nominal'] = np.float64
        branch_dict['NNScore_up'] = np.float64 
        branch_dict['NNScore_down'] = np.float64    

        values = [tree.array(b) for b in br_names]
        zipbObj = zip(br_names, values)

        values_dict = dict(zipbObj)
        
        values_dict['NNScore_nominal'] = NNRewScore
        values_dict['NNScore_up'] = NNRewScoreUp
        values_dict['NNScore_down'] = NNRewScoreDown
    
        new_file['Nominal'] = uproot.newtree(branch_dict)
        new_file['Nominal'].extend(values_dict)

        print(new_file.keys())
    
    else:
        print("Path " + dataset_path + " not opened")


