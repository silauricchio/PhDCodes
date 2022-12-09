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

from colour import Color

from keras.models import Sequential,Model,load_model
from keras.layers.core import Dense, Activation
from keras.layers import BatchNormalization, Dropout, concatenate
from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.optimizers import Adam
from keras import backend as K
import tensorflow as tf
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix


import sys
sys.path.append('/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/lib/')
import NN_performances_lib as lib

#decide the dataset where to test the network                                                                                                                                       
run_HSB0_train = True
run_LSB0_val2 = False
run_HSB0_val = False
run_LSB0_val = True
run_CR0_only = False

run_HSB0_train_merged = False
run_LSB0_val2_merged = False
run_HSB0_val_merged = False
run_LSB0_val_merged = False
run_CR0_merged = False

run_HSB0_train_resolved = False
run_LSB0_val2_resolved = False
run_HSB0_val_resolved = False
run_LSB0_val_resolved = False
run_CR0_resolved = False

WP = '60'
#WP = '70'                                                                                                                                                                                                
#WP = '80' 

file_name = 'data'                                                                                                                                                                                       

model_names = ['train_inHSB_mH_145_175_v1p6_schemeA_WP' + WP + '/fatjet_HwithNTracks_andTrackJets', 'train_inLSB_mH_65_75_v1p6_schemeA_WP' + WP + '/'+ '/fatjet_HwithNTracks_andTrackJets']

model_labels = ['train in HSB', 'train in LSB']               


path_pickles = '/eos/user/s/sauricch/MachineLearning/YXH/pickle_files/Ntuples_v1p6_schemeA_WP' + WP + '/'
path_models = '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/'
path_output = '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/output/ModelsComparisons/train_in_HSB_or_LSB_schemeA_WP' + WP + '/'


try:
    os.mkdir(path_output)
    print("Directory ", path_output,  " Created ")
except FileExistsError:
    print("Directory ", path_output,  " already exists")

if run_HSB0_train: 
    path_plots_HSB0_train = path_output + 'HSB0_train/'
    try:
        os.mkdir(path_plots_HSB0_train)
        print("Directory ", path_plots_HSB0_train, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_train, " already exists")
        
if run_LSB0_val2:
    path_plots_LSB0_val2 = path_output + 'LSB0_val2/'
    try:
        os.mkdir(path_plots_LSB0_val2)
        print("Directory ", path_plots_LSB0_val2, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val2, " already exists")

if run_HSB0_val:
    path_plots_HSB0_val = path_output + 'HSB0_val/'
    try:
        os.mkdir(path_plots_HSB0_val)
        print("Directory ", path_plots_HSB0_val, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_val, " already exists")

if run_LSB0_val:
    path_plots_LSB0_val = path_output + 'LSB0_val/'
    try:
        os.mkdir(path_plots_LSB0_val)
        print("Directory ", path_plots_LSB0_val, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val, " already exists")

if run_CR0_only:
    path_plots_CR0_only = path_output + 'CR0_only/'
    try:
        os.mkdir(path_plots_CR0_only)
        print("Directory ", path_plots_CR0_only, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_CR0_only, " already exists")

if run_HSB0_train_merged: 
    path_plots_HSB0_train_merged = path_output + 'HSB0_train_merged/'
    try:
        os.mkdir(path_plots_HSB0_train_merged)
        print("Directory ", path_plots_HSB0_train_merged, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_train_merged, " already exists")
        
if run_LSB0_val2_merged:
    path_plots_LSB0_val2_merged = path_output + 'LSB0_val2_merged/'
    try:
        os.mkdir(path_plots_LSB0_val2_merged)
        print("Directory ", path_plots_LSB0_val2_merged, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val2_merged, " already exists")

if run_HSB0_val_merged:
    path_plots_HSB0_val_merged = path_output + 'HSB0_val_merged/'
    try:
        os.mkdir(path_plots_HSB0_val_merged)
        print("Directory ", path_plots_HSB0_val_merged, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_val_merged, " already exists")

if run_LSB0_val_merged:
    path_plots_LSB0_val_merged = path_output + 'LSB0_val_merged/'
    try:
        os.mkdir(path_plots_LSB0_val_merged)
        print("Directory ", path_plots_LSB0_val_merged, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val_merged, " already exists")

if run_CR0_merged:
    path_plots_CR0_merged = path_output + 'CR0_merged/'
    try:
        os.mkdir(path_plots_CR0_merged)
        print("Directory ", path_plots_CR0_merged, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_CR0_merged, " already exists")


if run_HSB0_train_resolved: 
    path_plots_HSB0_train_resolved = path_output + 'HSB0_train_resolved/'
    try:
        os.mkdir(path_plots_HSB0_train_resolved)
        print("Directory ", path_plots_HSB0_train_resolved, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_train_resolved, " already exists")
        
if run_LSB0_val2_resolved:
    path_plots_LSB0_val2_resolved = path_output + 'LSB0_val2_resolved/'
    try:
        os.mkdir(path_plots_LSB0_val2_resolved)
        print("Directory ", path_plots_LSB0_val2_resolved, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val2_resolved, " already exists")

if run_HSB0_val_resolved:
    path_plots_HSB0_val_resolved = path_output + 'HSB0_val_resolved/'
    try:
        os.mkdir(path_plots_HSB0_val_resolved)
        print("Directory ", path_plots_HSB0_val_resolved, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_HSB0_val_resolved, " already exists")

if run_LSB0_val_resolved:
    path_plots_LSB0_val_resolved = path_output + 'LSB0_val_resolved/'
    try:
        os.mkdir(path_plots_LSB0_val_resolved)
        print("Directory ", path_plots_LSB0_val_resolved, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_LSB0_val_resolved, " already exists")

if run_CR0_resolved:
    path_plots_CR0_resolved = path_output + 'CR0_resolved/'
    try:
        os.mkdir(path_plots_CR0_resolved)
        print("Directory ", path_plots_CR0_resolved, " Created ")
    except FileExistsError:
        print("Directory ", path_plots_CR0_resolved, " already exists")



#open the dataframes with all variables                                                                                                                                                                    
path_pickles_16a = path_pickles + 'mc16a/' 
path_pickles_16d = path_pickles + 'mc16d/'
path_pickles_16e = path_pickles + 'mc16e/'

if (run_HSB0_train | run_HSB0_val | run_HSB0_train_merged | run_HSB0_val_merged | run_HSB0_train_resolved | run_HSB0_val_resolved): 
    print(Fore.BLUE+'Opening the DataFrame HSB0 for all mc campaignes')
    DF_HSB0_16a = pd.read_pickle(path_pickles_16a + file_name + '_HSB0_Full.pkl')
    print(Fore.BLUE+'Opening the DataFrame HSB1 for all mc campaignes')
    DF_HSB1_16a = pd.read_pickle(path_pickles_16a + file_name + '_HSB1_Full.pkl')

    DF_HSB0_16d = pd.read_pickle(path_pickles_16d + file_name + '_HSB0_Full.pkl')
    DF_HSB1_16d = pd.read_pickle(path_pickles_16d + file_name + '_HSB1_Full.pkl')
    
    DF_HSB0_16e = pd.read_pickle(path_pickles_16e + file_name + '_HSB0_Full.pkl')
    DF_HSB1_16e = pd.read_pickle(path_pickles_16e + file_name + '_HSB1_Full.pkl')
    
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

    DF_HSB1_full['trackjet_pt_balance'] = (DF_HSB1_full['trackjet1_pt'] - DF_HSB1_full['trackjet2_pt'])/(DF_HSB1_full['trackjet1_pt'] + DF_HSB1_full['trackjet2_pt'])
    DF_HSB0_full['trackjet_pt_balance'] = (DF_HSB0_full['trackjet1_pt'] - DF_HSB0_full['trackjet2_pt'])/(DF_HSB0_full['trackjet1_pt'] + DF_HSB0_full['trackjet2_pt'])

    DF_HSB1_full["deta_trackjets"] = np.absolute(DF_HSB1_full["trackjet1_eta"] - DF_HSB1_full["trackjet2_eta"])
    DF_HSB0_full["deta_trackjets"] = np.absolute(DF_HSB0_full["trackjet1_eta"] - DF_HSB0_full["trackjet2_eta"])
    DF_HSB1_full["deta_fatjets"] = np.abs(DF_HSB1_full["etaH"] - DF_HSB1_full["etaV"])
    DF_HSB0_full["deta_fatjets"] = np.abs(DF_HSB0_full["etaH"] - DF_HSB0_full["etaV"])
    DF_HSB1_full["deltaR_trackjets"] = np.sqrt(np.power(DF_HSB1_full["trackjet1_eta"] - DF_HSB1_full["trackjet2_eta"], 2) + np.power(DF_HSB1_full["trackjet1_phi"] - DF_HSB1_full["trackjet2_phi"],2))
    DF_HSB0_full["deltaR_trackjets"] = np.sqrt(np.power(DF_HSB0_full["trackjet1_eta"] - DF_HSB0_full["trackjet2_eta"], 2) + np.power(DF_HSB0_full["trackjet1_phi"] - DF_HSB0_full["trackjet2_phi"],2))
    DF_HSB1_full["deltaR_fatjets"] = np.sqrt(np.power(DF_HSB1_full["etaH"] - DF_HSB1_full["etaV"], 2) + np.power(DF_HSB1_full["phiH"] - DF_HSB1_full["phiV"],2))
    DF_HSB0_full["deltaR_fatjets"] = np.sqrt(np.power(DF_HSB0_full["etaH"] - DF_HSB0_full["etaV"], 2) + np.power(DF_HSB0_full["phiH"] - DF_HSB0_full["phiV"],2))

    theta_DF_HSB0 = 2* np.arctan(1/(np.exp(DF_HSB0_full['trackjet1_eta'])))
    DF_HSB0_full['trackjet1_e'] = np.sqrt((DF_HSB0_full['trackjet1_m']**2) + ((DF_HSB0_full['trackjet1_pt']/np.cos(theta_DF_HSB0))**2))

    theta_DF_HSB1 = 2* np.arctan(1/(np.exp(DF_HSB1_full['trackjet1_eta'])))
    DF_HSB1_full['trackjet1_e'] = np.sqrt((DF_HSB1_full['trackjet1_m']**2) + ((DF_HSB1_full['trackjet1_pt']/np.cos(theta_DF_HSB1))**2))

    theta_DF_HSB0 = 2* np.arctan(1/(np.exp(DF_HSB0_full['trackjet2_eta'])))
    DF_HSB0_full['trackjet2_e'] = np.sqrt((DF_HSB0_full['trackjet2_m']**2) + ((DF_HSB0_full['trackjet2_pt']/np.cos(theta_DF_HSB0))**2))

    theta_DF_HSB1 = 2* np.arctan(1/(np.exp(DF_HSB1_full['trackjet2_eta'])))
    DF_HSB1_full['trackjet2_e'] = np.sqrt((DF_HSB1_full['trackjet2_m']**2) + ((DF_HSB1_full['trackjet2_pt']/np.cos(theta_DF_HSB1))**2))



    if run_HSB0_train:
        HSB0_train_cut = ((DF_HSB0_full['mH']>145) & (DF_HSB0_full['mH']<175))
        DF_HSB0_train = DF_HSB0_full[HSB0_train_cut]
        
        HSB1_train_cut = ((DF_HSB1_full['mH']>145) & (DF_HSB1_full['mH']<175))
        DF_HSB1_train = DF_HSB1_full[HSB1_train_cut]

    if run_HSB0_val:
        HSB0_val_cut = ((DF_HSB0_full['mH']>165) & (DF_HSB0_full['mH']<200))
        DF_HSB0_val = DF_HSB0_full[HSB0_val_cut]
        
        HSB1_val_cut = ((DF_HSB1_full['mH']>165) & (DF_HSB1_full['mH']<200))
        DF_HSB1_val = DF_HSB1_full[HSB1_val_cut]

    if run_HSB0_train_merged:
        HSB0_train_merged_cut = ((DF_HSB0_full['mH']>145) & (DF_HSB0_full['mH']<175) & (DF_HSB0_full['d2TracksV']<1.2))
        DF_HSB0_train_merged = DF_HSB0_full[HSB0_train_merged_cut]

        HSB1_train_merged_cut = ((DF_HSB1_full['mH']>145) & (DF_HSB1_full['mH']<175) & (DF_HSB1_full['d2TracksV']<1.2))
        DF_HSB1_train_merged = DF_HSB1_full[HSB1_train_merged_cut]
        
    if run_HSB0_val_merged:
        HSB0_val_merged_cut = ((DF_HSB0_full['mH']>165) & (DF_HSB0_full['mH']<200) & (DF_HSB0_full['d2TracksV']<1.2))
        DF_HSB0_val_merged = DF_HSB0_full[HSB0_val_merged_cut]
        
        HSB1_val_merged_cut = ((DF_HSB1_full['mH']>165) & (DF_HSB1_full['mH']<200) & (DF_HSB1_full['d2TracksV']<1.2))
        DF_HSB1_val_merged = DF_HSB1_full[HSB1_val_merged_cut]

    if run_HSB0_train_resolved:
        HSB0_train_resolved_cut = ((DF_HSB0_full['isX_smalljets']==1) & (DF_HSB0_full['mVHres']>1300) & (DF_HSB0_full['mH']>145) & (DF_HSB0_full['mH']<175) & (DF_HSB0_full['d2TracksV']>1.2) & (abs(DF_HSB0_full['deltaY_smalljets'])<2.5) & (DF_HSB0_full['smalljets_ptbalance']<0.8))                                                                                  
        DF_HSB0_train_resolved = DF_HSB0_full[HSB0_train_resolved_cut]
  
        HSB1_train_resolved_cut = ((DF_HSB1_full['isX_smalljets']==1) & (DF_HSB1_full['mVHres']>1300) & (DF_HSB1_full['mH']>145) & (DF_HSB1_full['mH']<175) & (DF_HSB1_full['d2TracksV']>1.2) & (abs(DF_HSB1_full['deltaY_smalljets'])<2.5) & (DF_HSB1_full['smalljets_ptbalance']<0.8))                                                                                 
        DF_HSB1_train_resolved = DF_HSB1_full[HSB1_train_resolved_cut]

    if run_HSB0_val_resolved:
        HSB0_val_resolved_cut = ((DF_HSB0_full['isX_smalljets']==1) & (DF_HSB0_full['mVHres']>1300) & (DF_HSB0_full['mH']>165) & (DF_HSB0_full['mH']<200) & (DF_HSB0_full['d2TracksV']>1.2) & (abs(DF_HSB0_full['deltaY_smalljets'])<2.5) & (DF_HSB0_full['smalljets_ptbalance']<0.8))
        DF_HSB0_val_resolved = DF_HSB0_full[HSB0_train_resolved_cut]

        HSB1_val_resolved_cut = ((DF_HSB1_full['isX_smalljets']==1) & (DF_HSB1_full['mVHres']>1300) & (DF_HSB1_full['mH']>165) & (DF_HSB1_full['mH']<200) & (DF_HSB1_full['d2TracksV']>1.2) & (abs(DF_HSB1_full['deltaY_smalljets'])<2.5) & (DF_HSB1_full['smalljets_ptbalance']<0.8))
        DF_HSB1_val_resolved = DF_HSB1_full[HSB1_train_resolved_cut]



if (run_LSB0_val2 | run_LSB0_val | run_LSB0_val2_merged | run_LSB0_val_merged | run_LSB0_val2_resolved | run_LSB0_val_resolved): 
    print(Fore.BLUE+'Opening the DataFrame LSB0 for all mc campaignes')
    DF_LSB0_16a = pd.read_pickle(path_pickles_16a + file_name + '_LSB0_Full.pkl')
    print(Fore.BLUE+'Opening the DataFrame LSB1 for all mc campaignes')
    DF_LSB1_16a = pd.read_pickle(path_pickles_16a + file_name + '_LSB1_Full.pkl')

    DF_LSB0_16d = pd.read_pickle(path_pickles_16d + file_name + '_LSB0_Full.pkl')
    DF_LSB1_16d = pd.read_pickle(path_pickles_16d + file_name + '_LSB1_Full.pkl')

    DF_LSB0_16e = pd.read_pickle(path_pickles_16e + file_name + '_LSB0_Full.pkl')
    DF_LSB1_16e = pd.read_pickle(path_pickles_16e + file_name + '_LSB1_Full.pkl')

    DF_LSB0_full = []
    DF_LSB1_full = []

    DF_LSB0_full += [DF_LSB0_16a]
    DF_LSB0_full += [DF_LSB0_16d]
    DF_LSB0_full += [DF_LSB0_16e]
    DF_LSB0_full = pd.concat(DF_LSB0_full)

    DF_LSB1_full += [DF_LSB1_16a]
    DF_LSB1_full += [DF_LSB1_16d]
    DF_LSB1_full += [DF_LSB1_16e]
    DF_LSB1_full = pd.concat(DF_LSB1_full)

    DF_LSB1_full['trackjet_pt_balance'] = (DF_LSB1_full['trackjet1_pt'] - DF_LSB1_full['trackjet2_pt'])/(DF_LSB1_full['trackjet1_pt'] + DF_LSB1_full['trackjet2_pt'])
    DF_LSB0_full['trackjet_pt_balance'] = (DF_LSB0_full['trackjet1_pt'] - DF_LSB0_full['trackjet2_pt'])/(DF_LSB0_full['trackjet1_pt'] + DF_LSB0_full['trackjet2_pt'])

    DF_LSB0_full["deta_trackjets"] = np.absolute(DF_LSB0_full["trackjet1_eta"] - DF_LSB0_full["trackjet2_eta"])
    DF_LSB1_full["deta_trackjets"] = np.absolute(DF_LSB1_full["trackjet1_eta"] - DF_LSB1_full["trackjet2_eta"])
    DF_LSB0_full["deta_fatjets"] = np.abs(DF_LSB0_full["etaH"] - DF_LSB0_full["etaV"])
    DF_LSB1_full["deta_fatjets"] = np.abs(DF_LSB1_full["etaH"] - DF_LSB1_full["etaV"])
    DF_LSB0_full["deltaR_trackjets"] = np.sqrt(np.power(DF_LSB0_full["trackjet1_eta"] - DF_LSB0_full["trackjet2_eta"], 2) + np.power(DF_LSB0_full["trackjet1_phi"] - DF_LSB0_full["trackjet2_phi"],2))
    DF_LSB1_full["deltaR_trackjets"] = np.sqrt(np.power(DF_LSB1_full["trackjet1_eta"] - DF_LSB1_full["trackjet2_eta"], 2) + np.power(DF_LSB1_full["trackjet1_phi"] - DF_LSB1_full["trackjet2_phi"],2))
    DF_LSB0_full["deltaR_fatjets"] = np.sqrt(np.power(DF_LSB0_full["etaH"] - DF_LSB0_full["etaV"], 2) + np.power(DF_LSB0_full["phiH"] - DF_LSB0_full["phiV"],2))
    DF_LSB1_full["deltaR_fatjets"] = np.sqrt(np.power(DF_LSB1_full["etaH"] - DF_LSB1_full["etaV"], 2) + np.power(DF_LSB1_full["phiH"] - DF_LSB1_full["phiV"],2))

    theta_DF_LSB0 = 2* np.arctan(1/(np.exp(DF_LSB0_full['trackjet1_eta'])))
    DF_LSB0_full['trackjet1_e'] = np.sqrt((DF_LSB0_full['trackjet1_m']**2) + ((DF_LSB0_full['trackjet1_pt']/np.cos(theta_DF_LSB0))**2))

    theta_DF_LSB1 = 2* np.arctan(1/(np.exp(DF_LSB1_full['trackjet1_eta'])))
    DF_LSB1_full['trackjet1_e'] = np.sqrt((DF_LSB1_full['trackjet1_m']**2) + ((DF_LSB1_full['trackjet1_pt']/np.cos(theta_DF_LSB1))**2))

    theta_DF_LSB0 = 2* np.arctan(1/(np.exp(DF_LSB0_full['trackjet2_eta'])))
    DF_LSB0_full['trackjet2_e'] = np.sqrt((DF_LSB0_full['trackjet2_m']**2) + ((DF_LSB0_full['trackjet2_pt']/np.cos(theta_DF_LSB0))**2))

    theta_DF_LSB1 = 2* np.arctan(1/(np.exp(DF_LSB1_full['trackjet2_eta'])))
    DF_LSB1_full['trackjet2_e'] = np.sqrt((DF_LSB1_full['trackjet2_m']**2) + ((DF_LSB1_full['trackjet2_pt']/np.cos(theta_DF_LSB1))**2))



    if run_LSB0_val2:
        LSB0_val2_cut = ((DF_LSB0_full['mH']>50) & (DF_LSB0_full['mH']<65))
        DF_LSB0_val2 = DF_LSB0_full[LSB0_val2_cut]

        LSB1_train_cut = ((DF_LSB1_full['mH']>50) & (DF_LSB1_full['mH']<65))
        DF_LSB1_train = DF_LSB1_full[LSB1_train_cut]

    if run_LSB0_val:
        LSB0_val_cut = ((DF_LSB0_full['mH']>65) & (DF_LSB0_full['mH']<75))
        DF_LSB0_val = DF_LSB0_full[LSB0_val_cut]

        LSB1_val_cut = ((DF_LSB1_full['mH']>65) & (DF_LSB1_full['mH']<75))
        DF_LSB1_val = DF_LSB1_full[LSB1_val_cut]

    if run_LSB0_val2_merged:
        LSB0_val2_merged_cut = ((DF_LSB0_full['mH']>50) & (DF_LSB0_full['mH']<65) & (DF_LSB0_full['d2TracksV']<1.2))
        DF_LSB0_val2_merged = DF_LSB0_full[LSB0_val2_merged_cut]

        LSB1_val2_merged_cut = ((DF_LSB1_full['mH']>50) & (DF_LSB1_full['mH']<65) & (DF_LSB1_full['d2TracksV']<1.2))
        DF_LSB1_val2_merged = DF_LSB1_full[LSB1_train_merged_cut]

    if run_LSB0_val_merged:
        LSB0_val_merged_cut = ((DF_LSB0_full['mH']>65) & (DF_LSB0_full['mH']<75) & (DF_LSB0_full['d2TracksV']<1.2))
        DF_LSB0_val_merged = DF_LSB0_full[LSB0_val_merged_cut]

        LSB1_val_merged_cut = ((DF_LSB1_full['mH']>65) & (DF_LSB1_full['mH']<75) & (DF_LSB1_full['d2TracksV']<1.2))
        DF_LSB1_val_merged = DF_LSB1_full[LSB1_val_merged_cut]

    if run_LSB0_val2_resolved:
        LSB0_val2_resolved_cut = ((DF_LSB0_full['isX_smalljets']==1) & (DF_LSB0_full['mVHres']>1300) & (DF_LSB0_full['mH']>50) & (DF_LSB0_full['mH']<65) & (DF_LSB0_full['d2TracksV']>1.2) & (abs(DF_LSB0_full['deltaY_smalljets'])<2.5) & (DF_LSB0_full['smalljets_ptbalance']<0.8))
        DF_LSB0_val2_resolved = DF_LSB0_full[LSB0_val2_resolved_cut]

        LSB1_val2_resolved_cut = ((DF_LSB1_full['isX_smalljets']==1) & (DF_LSB1_full['mVHres']>1300) & (DF_LSB1_full['mH']>50) & (DF_LSB1_full['mH']<65) & (DF_LSB1_full['d2TracksV']>1.2) & (abs(DF_LSB1_full['deltaY_smalljets'])<2.5) & (DF_LSB1_full['smalljets_ptbalance']<0.8))
        DF_LSB1_val2_resolved = DF_LSB1_full[LSB1_train_resolved_cut]

    if run_LSB0_val_resolved:
        LSB0_val_resolved_cut = ((DF_LSB0_full['isX_smalljets']==1) & (DF_LSB0_full['mVHres']>1300) & (DF_LSB0_full['mH']>65) & (DF_LSB0_full['mH']<75) & (DF_LSB0_full['d2TracksV']>1.2) & (abs(DF_LSB0_full['deltaY_smalljets'])<2.5) & (DF_LSB0_full['smalljets_ptbalance']<0.8))
        DF_LSB0_val_resolved = DF_LSB0_full[LSB0_val_resolved_cut]
        
        LSB1_val_resolved_cut = ((DF_LSB1_full['isX_smalljets']==1) & (DF_LSB1_full['mVHres']>1300) & (DF_LSB1_full['mH']>65) & (DF_LSB1_full['mH']<75) & (DF_LSB1_full['d2TracksV']>1.2) & (abs(DF_LSB1_full['deltaY_smalljets'])<2.5) & (DF_LSB1_full['smalljets_ptbalance']<0.8))
        DF_LSB1_val_resolved = DF_LSB1_full[LSB1_val_resolved_cut]


if (run_CR0_only | run_CR0_merged | run_CR0_resolved): 
    print(Fore.BLUE+'Opening the DataFrame CR0 for all mc campaignes')
    DF_CR0_16a = pd.read_pickle(path_pickles_16a + file_name + '_CR0_Full.pkl')
    DF_CR0_16d = pd.read_pickle(path_pickles_16d + file_name + '_CR0_Full.pkl')
    DF_CR0_16e = pd.read_pickle(path_pickles_16e + file_name + '_CR0_Full.pkl')

    DF_CR0_full = []

    DF_CR0_full += [DF_CR0_16a]
    DF_CR0_full += [DF_CR0_16d]
    DF_CR0_full += [DF_CR0_16e]
    DF_CR0_full = pd.concat(DF_CR0_full)
    
    #print(list(DF_CR0_full))

    DF_CR0_full['trackjet_pt_balance'] = (DF_CR0_full['trackjet1_pt'] - DF_CR0_full['trackjet2_pt'])/(DF_CR0_full['trackjet1_pt'] + DF_CR0_full['trackjet2_pt'])

    DF_CR0_full["deta_trackjets"] = np.absolute(DF_CR0_full["trackjet1_eta"] - DF_CR0_full["trackjet2_eta"])
    DF_CR0_full["deta_fatjets"] = np.abs(DF_CR0_full["etaH"] - DF_CR0_full["etaV"])
    DF_CR0_full["deltaR_trackjets"] = np.sqrt(np.power(DF_CR0_full["trackjet1_eta"] - DF_CR0_full["trackjet2_eta"], 2) + np.power(DF_CR0_full["trackjet1_phi"] - DF_CR0_full["trackjet2_phi"],2))
    DF_CR0_full["deltaR_fatjets"] = np.sqrt(np.power(DF_CR0_full["etaH"] - DF_CR0_full["etaV"], 2) + np.power(DF_CR0_full["phiH"] - DF_CR0_full["phiV"],2))

    theta_DF_CR0 = 2* np.arctan(1/(np.exp(DF_CR0_full['trackjet1_eta'])))
    DF_CR0_full['trackjet1_e'] = np.sqrt((DF_CR0_full['trackjet1_m']**2) + ((DF_CR0_full['trackjet1_pt']/np.cos(theta_DF_CR0))**2))

    theta_DF_CR0 = 2* np.arctan(1/(np.exp(DF_CR0_full['trackjet2_eta'])))
    DF_CR0_full['trackjet2_e'] = np.sqrt((DF_CR0_full['trackjet2_m']**2) + ((DF_CR0_full['trackjet2_pt']/np.cos(theta_DF_CR0))**2))

    if run_CR0_only:
        DF_CR0 = DF_CR0_full

    if run_CR0_merged:
        CR0_merged_cut = (DF_CR0_full['d2TracksV']<1.2)
        DF_CR0_merged = DF_CR0_full[CR0_merged_cut]

    if run_CR0_resolved:
        CR0_resolved_cut = ((DF_CR0_full['isX_smalljets']==1) & (DF_CR0_full['mVHres']>1300) & (DF_CR0_full['d2TracksV']>1.2) & (abs(DF_CR0_full['deltaY_smalljets'])<2.5) & (DF_CR0_full['smalljets_ptbalance']<0.8))
        DF_CR0_resolved = DF_CR0_full[CR0_resolved_cut]


twotrackjets = False
VariablesToModel = {} #create a dictionary for variablestomodel for all models

for model_name in model_names:
    modelpath = path_models + model_name + '/'
    #modelpath = path_models #canc

    X_train_16a = pd.read_pickle(modelpath +  'X_Train_mc16a.pkl')
    VariablesToModel[model_name] = list(X_train_16a)

    for v in VariablesToModel[model_name]:
        if 'trackjet' in v: twotrackjets = True

if twotrackjets==True:
    print(Fore.BLUE+str('Select only event with at least 2 track jets in full dataset'))


    if run_HSB0_train:
        sel_trackjets_HSB0_train = ( (DF_HSB0_train["trackjet1_pt"]>=0) & (DF_HSB0_train["trackjet2_pt"]>=0) )
        DF_HSB0_train = DF_HSB0_train[sel_trackjets_HSB0_train]
        sel_trackjets_HSB1_train = ( (DF_HSB1_train["trackjet1_pt"]>=0) & (DF_HSB1_train["trackjet2_pt"]>=0) )
        DF_HSB1_train = DF_HSB1_train[sel_trackjets_HSB1_train]
        
    if run_LSB0_val2:
        sel_trackjets_LSB0_val2 = ( (DF_LSB0_val2["trackjet1_pt"]>=0) & (DF_LSB0_val2["trackjet2_pt"]>=0) )
        DF_LSB0_val2 = DF_LSB0_val2[sel_trackjets_LSB0_val2]
        sel_trackjets_LSB1_train = ( (DF_LSB1_train["trackjet1_pt"]>=0) & (DF_LSB1_train["trackjet2_pt"]>=0) )
        DF_LSB1_train = DF_LSB1_train[sel_trackjets_LSB1_train]

    if run_HSB0_val:
        sel_trackjets_HSB0_val = ( (DF_HSB0_val["trackjet1_pt"]>=0) & (DF_HSB0_val["trackjet2_pt"]>=0) )
        DF_HSB0_val = DF_HSB0_val[sel_trackjets_HSB0_val]
        sel_trackjets_HSB1_val = ( (DF_HSB1_val["trackjet1_pt"]>=0) & (DF_HSB1_val["trackjet2_pt"]>=0) )
        DF_HSB1_val = DF_HSB1_val[sel_trackjets_HSB1_val]
        
    if run_LSB0_val:
        sel_trackjets_LSB0_val = ( (DF_LSB0_val["trackjet1_pt"]>=0) & (DF_LSB0_val["trackjet2_pt"]>=0) )
        DF_LSB0_val = DF_LSB0_val[sel_trackjets_LSB0_val]
        sel_trackjets_LSB1_val = ( (DF_LSB1_val["trackjet1_pt"]>=0) & (DF_LSB1_val["trackjet2_pt"]>=0) )
        DF_LSB1_val = DF_LSB1_val[sel_trackjets_LSB1_val]

    if run_CR0_only:
        sel_trackjets_CR0 = ( (DF_CR0["trackjet1_pt"]>=0) & (DF_CR0["trackjet2_pt"]>=0) )
        DF_CR0 = DF_CR0[sel_trackjets_CR0]
    
    if run_HSB0_train_merged:
        sel_trackjets_HSB0_train_merged = ( (DF_HSB0_train_merged["trackjet1_pt"]>=0) & (DF_HSB0_train_merged["trackjet2_pt"]>=0) )
        DF_HSB0_train_merged = DF_HSB0_train_merged[sel_trackjets_HSB0_train_merged]
        sel_trackjets_HSB1_train_merged = ( (DF_HSB1_train_merged["trackjet1_pt"]>=0) & (DF_HSB1_train_merged["trackjet2_pt"]>=0) )
        DF_HSB1_train_merged = DF_HSB1_train_merged[sel_trackjets_HSB1_train_merged]

    if run_LSB0_val2_merged:
        sel_trackjets_LSB0_val2_merged = ( (DF_LSB0_val2_merged["trackjet1_pt"]>=0) & (DF_LSB0_val2_merged["trackjet2_pt"]>=0) )
        DF_LSB0_val2_merged = DF_LSB0_val2_merged[sel_trackjets_LSB0_val2_merged]
        sel_trackjets_LSB1_train_merged = ( (DF_LSB1_train_merged["trackjet1_pt"]>=0) & (DF_LSB1_train_merged["trackjet2_pt"]>=0) )
        DF_LSB1_train_merged = DF_LSB1_train_merged[sel_trackjets_LSB1_train_merged]

    if run_HSB0_val_merged:
        sel_trackjets_HSB0_val_merged = ( (DF_HSB0_val_merged["trackjet1_pt"]>=0) & (DF_HSB0_val_merged["trackjet2_pt"]>=0) )
        DF_HSB0_val_merged = DF_HSB0_val_merged[sel_trackjets_HSB0_val_merged]
        sel_trackjets_HSB1_val_merged = ( (DF_HSB1_val_merged["trackjet1_pt"]>=0) & (DF_HSB1_val_merged["trackjet2_pt"]>=0) )
        DF_HSB1_val_merged = DF_HSB1_val_merged[sel_trackjets_HSB1_val_merged]

    if run_LSB0_val_merged:
        sel_trackjets_LSB0_val_merged = ( (DF_LSB0_val_merged["trackjet1_pt"]>=0) & (DF_LSB0_val_merged["trackjet2_pt"]>=0) )
        DF_LSB0_val_merged = DF_LSB0_val_merged[sel_trackjets_LSB0_val_merged]
        sel_trackjets_LSB1_val_merged = ( (DF_LSB1_val_merged["trackjet1_pt"]>=0) & (DF_LSB1_val_merged["trackjet2_pt"]>=0) )
        DF_LSB1_val_merged = DF_LSB1_val_merged[sel_trackjets_LSB1_val_merged]

    if run_CR0_merged:
        sel_trackjets_CR0_merged = ( (DF_CR0_merged["trackjet1_pt"]>=0) & (DF_CR0_merged["trackjet2_pt"]>=0) )
        DF_CR0_merged = DF_CR0_merged[sel_trackjets_CR0_merged]

    if run_HSB0_train_resolved:
        sel_trackjets_HSB0_train_resolved = ( (DF_HSB0_train_resolved["trackjet1_pt"]>=0) & (DF_HSB0_train_resolved["trackjet2_pt"]>=0) )
        DF_HSB0_train_resolved = DF_HSB0_train_resolved[sel_trackjets_HSB0_train_resolved]
        sel_trackjets_HSB1_train_resolved = ( (DF_HSB1_train_resolved["trackjet1_pt"]>=0) & (DF_HSB1_train_resolved["trackjet2_pt"]>=0) )
        DF_HSB1_train_resolved = DF_HSB1_train_resolved[sel_trackjets_HSB1_train_resolved]

    if run_LSB0_val2_resolved:
        sel_trackjets_LSB0_val2_resolved = ( (DF_LSB0_val2_resolved["trackjet1_pt"]>=0) & (DF_LSB0_val2_resolved["trackjet2_pt"]>=0) )
        DF_LSB0_val2_resolved = DF_LSB0_val2_resolved[sel_trackjets_LSB0_val2_resolved]
        sel_trackjets_LSB1_train_resolved = ( (DF_LSB1_train_resolved["trackjet1_pt"]>=0) & (DF_LSB1_train_resolved["trackjet2_pt"]>=0) )
        DF_LSB1_train_resolved = DF_LSB1_train_resolved[sel_trackjets_LSB1_train_resolved]

    if run_HSB0_val_resolved:
        sel_trackjets_HSB0_val_resolved = ( (DF_HSB0_val_resolved["trackjet1_pt"]>=0) & (DF_HSB0_val_resolved["trackjet2_pt"]>=0) )
        DF_HSB0_val_resolved = DF_HSB0_val_resolved[sel_trackjets_HSB0_val_resolved]
        sel_trackjets_HSB1_val_resolved = ( (DF_HSB1_val_resolved["trackjet1_pt"]>=0) & (DF_HSB1_val_resolved["trackjet2_pt"]>=0) )
        DF_HSB1_val_resolved = DF_HSB1_val_resolved[sel_trackjets_HSB1_val_resolved]

    if run_LSB0_val_resolved:
        sel_trackjets_LSB0_val_resolved = ( (DF_LSB0_val_resolved["trackjet1_pt"]>=0) & (DF_LSB0_val_resolved["trackjet2_pt"]>=0) )
        DF_LSB0_val_resolved = DF_LSB0_val_resolved[sel_trackjets_LSB0_val_resolved]
        sel_trackjets_LSB1_val_resolved = ( (DF_LSB1_val_resolved["trackjet1_pt"]>=0) & (DF_LSB1_val_resolved["trackjet2_pt"]>=0) )
        DF_LSB1_val_resolved = DF_LSB1_val_resolved[sel_trackjets_LSB1_val_resolved]

    if run_CR0_resolved:
        sel_trackjets_CR0_resolved = ( (DF_CR0_resolved["trackjet1_pt"]>=0) & (DF_CR0_resolved["trackjet2_pt"]>=0) )
        DF_CR0_resolved = DF_CR0_resolved[sel_trackjets_CR0_resolved]

        
goodsel_HSB0_train = {}
goodsel_LSB0_val2 = {}
goodsel_HSB0_val = {}
goodsel_LSB0_val = {}
goodsel_CR0 = {}
goodsel_HSB0_train_merged = {}
goodsel_LSB0_val2_merged = {}
goodsel_HSB0_val_merged = {}
goodsel_LSB0_val_merged = {}
goodsel_CR0_merged = {}
goodsel_HSB0_train_resolved = {}
goodsel_LSB0_val2_resolved = {}
goodsel_HSB0_val_resolved = {}
goodsel_LSB0_val_resolved = {}
goodsel_CR0_resolved = {}


ypred_HSB0_train = {}
ypred_LSB0_val2 = {}
ypred_HSB0_val = {}
ypred_LSB0_val = {}
ypred_CR0 = {}
ypred_HSB0_train_merged = {}
ypred_LSB0_val2_merged = {}
ypred_HSB0_val_merged = {}
ypred_LSB0_val_merged = {}
ypred_CR0_merged = {}
ypred_HSB0_train_resolved = {}
ypred_LSB0_val2_resolved = {}
ypred_HSB0_val_resolved = {}
ypred_LSB0_val_resolved = {}
ypred_CR0_resolved = {}


for model_name in model_names:
    
    modelpath = path_models + model_name + '/'
    #modelpath = path_models + 'FULLSTAT/' #canc   

    print('')
    print('')
    print('-----------------------------')
    
    print('Reading model -->', modelpath)
    if 'weight' in VariablesToModel[model_name]:
        VariablesToModel[model_name].remove('weight')
    print(VariablesToModel[model_name])

    #opening models                                                                                                                                                                          
    modelpath += 'FULLSTAT/'
    scaler = joblib.load(modelpath + 'training_scaler.bin')

    #modelpath = path_models + 'FULLSTAT/10Attempts/' + model_name + '/'   #canc

    model = load_model(modelpath + 'model.h5', compile=False)

    if run_HSB0_train:

        X_HSB0_train_scaler = DF_HSB0_train[VariablesToModel[model_name]]
        
        goodsel_HSB0_train[model_name] = pd.Series(True, index= X_HSB0_train_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_train[model_name] &= (np.abs(X_HSB0_train_scaler[v])!= np.inf)
            
        print('')

        print("HSB0 train events before infinities removing: ", X_HSB0_train_scaler.shape[0])
        X_HSB0_train_scaler = X_HSB0_train_scaler[goodsel_HSB0_train[model_name]]
        print("HSB0 train events after infinities removing: ", X_HSB0_train_scaler.shape[0])

        X_HSB0_train_scaler = X_HSB0_train_scaler.values
        X_HSB0_train_scaler = scaler.transform(X_HSB0_train_scaler)

    if run_LSB0_val2:
        
        X_LSB0_val2_scaler = DF_LSB0_val2[VariablesToModel[model_name]]
        
        goodsel_LSB0_val2[model_name] = pd.Series(True, index= X_LSB0_val2_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val2[model_name] &= (np.abs(X_LSB0_val2_scaler[v])!= np.inf)
        
        print('')

        print("LSB0 train events before infinities removing: ", X_LSB0_val2_scaler.shape[0])
        X_LSB0_val2_scaler = X_LSB0_val2_scaler[goodsel_LSB0_val2[model_name]]
        print("LSB0 train events after infinities removing: ", X_LSB0_val2_scaler.shape[0])
        
        X_LSB0_val2_scaler = X_LSB0_val2_scaler.values
        X_LSB0_val2_scaler = scaler.transform(X_LSB0_val2_scaler)

    if run_HSB0_val:

        X_HSB0_val_scaler = DF_HSB0_val[VariablesToModel[model_name]]
        
        goodsel_HSB0_val[model_name] = pd.Series(True, index= X_HSB0_val_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_val[model_name] &= (np.abs(X_HSB0_val_scaler[v])!= np.inf)
            
        print('')

        print("HSB0 val events before infinities removing: ", X_HSB0_val_scaler.shape[0])
        X_HSB0_val_scaler = X_HSB0_val_scaler[goodsel_HSB0_val[model_name]]
        print("HSB0 val events after infinities removing: ", X_HSB0_val_scaler.shape[0])

        X_HSB0_val_scaler = X_HSB0_val_scaler.values
        X_HSB0_val_scaler = scaler.transform(X_HSB0_val_scaler)

    if run_LSB0_val:
        
        X_LSB0_val_scaler = DF_LSB0_val[VariablesToModel[model_name]]
        
        goodsel_LSB0_val[model_name] = pd.Series(True, index= X_LSB0_val_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val[model_name] &= (np.abs(X_LSB0_val_scaler[v])!= np.inf)
        
        print('')

        print("LSB0 val events before infinities removing: ", X_LSB0_val_scaler.shape[0])
        X_LSB0_val_scaler = X_LSB0_val_scaler[goodsel_LSB0_val[model_name]]
        print("LSB0 val events after infinities removing: ", X_LSB0_val_scaler.shape[0])
        
        X_LSB0_val_scaler = X_LSB0_val_scaler.values
        X_LSB0_val_scaler = scaler.transform(X_LSB0_val_scaler)


    if run_CR0_only:

        X_CR0_scaler = DF_CR0[VariablesToModel[model_name]]

        goodsel_CR0[model_name] = pd.Series(True, index= X_CR0_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_CR0[model_name] &= (np.abs(X_CR0_scaler[v])!= np.inf)

        print('')

        print("CR0 events before infinities removing: ", X_CR0_scaler.shape[0])
        X_CR0_scaler = X_CR0_scaler[goodsel_CR0[model_name]]
        print("CR0 events after infinities removing: ", X_CR0_scaler.shape[0])

        X_CR0_scaler = X_CR0_scaler.values
        X_CR0_scaler = scaler.transform(X_CR0_scaler)


    if run_HSB0_train_merged:

        X_HSB0_train_scaler_merged = DF_HSB0_train_merged[VariablesToModel[model_name]]
        
        goodsel_HSB0_train_merged[model_name] = pd.Series(True, index= X_HSB0_train_scaler_merged.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_train_merged[model_name] &= (np.abs(X_HSB0_train_scaler_merged[v])!= np.inf)
            
        print('')

        print("HSB0 train merged events before infinities removing: ", X_HSB0_train_scaler_merged.shape[0])
        X_HSB0_train_scaler_merged = X_HSB0_train_scaler_merged[goodsel_HSB0_train_merged[model_name]]
        print("HSB0 train merged events after infinities removing: ", X_HSB0_train_scaler_merged.shape[0])

        X_HSB0_train_scaler_merged = X_HSB0_train_scaler_merged.values
        X_HSB0_train_scaler_merged = scaler.transform(X_HSB0_train_scaler_merged)


    if run_LSB0_val2_merged:

        X_LSB0_val2_scaler_merged = DF_LSB0_val2_merged[VariablesToModel[model_name]]
        
        goodsel_LSB0_val2_merged[model_name] = pd.Series(True, index= X_LSB0_val2_scaler_merged.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val2_merged[model_name] &= (np.abs(X_LSB0_val2_scaler_merged[v])!= np.inf)

        print('')

        print("LSB0 train merged events before infinities removing: ", X_LSB0_val2_scaler_merged.shape[0])
        X_LSB0_val2_scaler_merged = X_LSB0_val2_scaler_merged[goodsel_LSB0_val2_merged[model_name]]
        print("LSB0 train merged events after infinities removing: ", X_LSB0_val2_scaler_merged.shape[0])

        X_LSB0_val2_scaler_merged = X_LSB0_val2_scaler_merged.values
        X_LSB0_val2_scaler_merged = scaler.transform(X_LSB0_val2_scaler_merged)

    if run_HSB0_val_merged:

        X_HSB0_val_scaler_merged = DF_HSB0_val_merged[VariablesToModel[model_name]]
        
        goodsel_HSB0_val_merged[model_name] = pd.Series(True, index= X_HSB0_val_scaler_merged.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_val_merged[model_name] &= (np.abs(X_HSB0_val_scaler_merged[v])!= np.inf)

        print('')

        print("HSB0 val merged events before infinities removing: ", X_HSB0_val_scaler_merged.shape[0])
        X_HSB0_val_scaler_merged = X_HSB0_val_scaler_merged[goodsel_HSB0_val_merged[model_name]]
        print("HSB0 val merged events after infinities removing: ", X_HSB0_val_scaler_merged.shape[0])

        X_HSB0_val_scaler_merged = X_HSB0_val_scaler_merged.values
        X_HSB0_val_scaler_merged = scaler.transform(X_HSB0_val_scaler_merged)

    if run_LSB0_val_merged:

        X_LSB0_val_scaler_merged = DF_LSB0_val_merged[VariablesToModel[model_name]]

        goodsel_LSB0_val_merged[model_name] = pd.Series(True, index= X_LSB0_val_scaler_merged.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val_merged[model_name] &= (np.abs(X_LSB0_val_scaler_merged[v])!= np.inf)

        print('')

        print("LSB0 val merged events before infinities removing: ", X_LSB0_val_scaler_merged.shape[0])
        X_LSB0_val_scaler_merged = X_LSB0_val_scaler_merged[goodsel_LSB0_val_merged[model_name]]
        print("LSB0 val merged events after infinities removing: ", X_LSB0_val_scaler_merged.shape[0])

        X_LSB0_val_scaler_merged = X_LSB0_val_scaler_merged.values
        X_LSB0_val_scaler_merged = scaler.transform(X_LSB0_val_scaler_merged)


    if run_CR0_merged:

        X_CR0_merged_scaler = DF_CR0_merged[VariablesToModel[model_name]]

        goodsel_CR0_merged[model_name] = pd.Series(True, index= X_CR0_merged_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_CR0_merged[model_name] &= (np.abs(X_CR0_merged_scaler[v])!= np.inf)

        print('')

        print("CR0 merged events before infinities removing: ", X_CR0_merged_scaler.shape[0])
        X_CR0_merged_scaler = X_CR0_merged_scaler[goodsel_CR0_merged[model_name]]
        print("CR0 merged events after infinities removing: ", X_CR0_merged_scaler.shape[0])

        X_CR0_merged_scaler = X_CR0_merged_scaler.values
        X_CR0_merged_scaler = scaler.transform(X_CR0_merged_scaler)


    if run_HSB0_train_resolved:

        X_HSB0_train_scaler_resolved = DF_HSB0_train_resolved[VariablesToModel[model_name]]
        
        goodsel_HSB0_train_resolved[model_name] = pd.Series(True, index= X_HSB0_train_scaler_resolved.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_train_resolved[model_name] &= (np.abs(X_HSB0_train_scaler_resolved[v])!= np.inf)
            
        print('')

        print("HSB0 train resolved events before infinities removing: ", X_HSB0_train_scaler_resolved.shape[0])
        X_HSB0_train_scaler_resolved = X_HSB0_train_scaler_resolved[goodsel_HSB0_train_resolved[model_name]]
        print("HSB0 train resolved events after infinities removing: ", X_HSB0_train_scaler_resolved.shape[0])

        X_HSB0_train_scaler_resolved = X_HSB0_train_scaler_resolved.values
        X_HSB0_train_scaler_resolved = scaler.transform(X_HSB0_train_scaler_resolved)


    if run_LSB0_val2_resolved:

        X_LSB0_val2_scaler_resolved = DF_LSB0_val2_resolved[VariablesToModel[model_name]]
        
        goodsel_LSB0_val2_resolved[model_name] = pd.Series(True, index= X_LSB0_val2_scaler_resolved.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val2_resolved[model_name] &= (np.abs(X_LSB0_val2_scaler_resolved[v])!= np.inf)

        print('')

        print("LSB0 train resolved events before infinities removing: ", X_LSB0_val2_scaler_resolved.shape[0])
        X_LSB0_val2_scaler_resolved = X_LSB0_val2_scaler_resolved[goodsel_LSB0_val2_resolved[model_name]]
        print("LSB0 train resolved events after infinities removing: ", X_LSB0_val2_scaler_resolved.shape[0])

        X_LSB0_val2_scaler_resolved = X_LSB0_val2_scaler_resolved.values
        X_LSB0_val2_scaler_resolved = scaler.transform(X_LSB0_val2_scaler_resolved)

    if run_HSB0_val_resolved:

        X_HSB0_val_scaler_resolved = DF_HSB0_val_resolved[VariablesToModel[model_name]]
        
        goodsel_HSB0_val_resolved[model_name] = pd.Series(True, index= X_HSB0_val_scaler_resolved.index)
        for v in VariablesToModel[model_name]:
            goodsel_HSB0_val_resolved[model_name] &= (np.abs(X_HSB0_val_scaler_resolved[v])!= np.inf)

        print('')

        print("HSB0 val resolved events before infinities removing: ", X_HSB0_val_scaler_resolved.shape[0])
        X_HSB0_val_scaler_resolved = X_HSB0_val_scaler_resolved[goodsel_HSB0_val_resolved[model_name]]
        print("HSB0 val resolved events after infinities removing: ", X_HSB0_val_scaler_resolved.shape[0])

        X_HSB0_val_scaler_resolved = X_HSB0_val_scaler_resolved.values
        X_HSB0_val_scaler_resolved = scaler.transform(X_HSB0_val_scaler_resolved)

    if run_LSB0_val_resolved:

        X_LSB0_val_scaler_resolved = DF_LSB0_val_resolved[VariablesToModel[model_name]]

        goodsel_LSB0_val_resolved[model_name] = pd.Series(True, index= X_LSB0_val_scaler_resolved.index)
        for v in VariablesToModel[model_name]:
            goodsel_LSB0_val_resolved[model_name] &= (np.abs(X_LSB0_val_scaler_resolved[v])!= np.inf)

        print('')

        print("LSB0 val resolved events before infinities removing: ", X_LSB0_val_scaler_resolved.shape[0])
        X_LSB0_val_scaler_resolved = X_LSB0_val_scaler_resolved[goodsel_LSB0_val_resolved[model_name]]
        print("LSB0 val resolved events after infinities removing: ", X_LSB0_val_scaler_resolved.shape[0])

        X_LSB0_val_scaler_resolved = X_LSB0_val_scaler_resolved.values
        X_LSB0_val_scaler_resolved = scaler.transform(X_LSB0_val_scaler_resolved)


    if run_CR0_resolved:

        X_CR0_resolved_scaler = DF_CR0_resolved[VariablesToModel[model_name]]

        goodsel_CR0_resolved[model_name] = pd.Series(True, index= X_CR0_resolved_scaler.index)
        for v in VariablesToModel[model_name]:
            goodsel_CR0_resolved[model_name] &= (np.abs(X_CR0_resolved_scaler[v])!= np.inf)

        print('')

        print("CR0 resolved events before infinities removing: ", X_CR0_resolved_scaler.shape[0])
        X_CR0_resolved_scaler = X_CR0_resolved_scaler[goodsel_CR0_resolved[model_name]]
        print("CR0 resolved events after infinities removing: ", X_CR0_resolved_scaler.shape[0])

        X_CR0_resolved_scaler = X_CR0_resolved_scaler.values
        X_CR0_resolved_scaler = scaler.transform(X_CR0_resolved_scaler)



    print('')
    print('-----------------------------')

    if run_HSB0_train:
        print ('Running model prediction on HSB0 train')
        ypred_HSB0_train[model_name] = model.predict(X_HSB0_train_scaler, verbose = True, batch_size=2048)

    if run_LSB0_val2:
        print ('Running model prediction on LSB0 train')
        ypred_LSB0_val2[model_name] = model.predict(X_LSB0_val2_scaler, verbose = True, batch_size=2048)
 
    if run_HSB0_val:
        print ('Running model prediction on HSB0 val')
        ypred_HSB0_val[model_name] = model.predict(X_HSB0_val_scaler, verbose = True, batch_size=2048)

    if run_LSB0_val:
        print ('Running model prediction on LSB0 val')
        ypred_LSB0_val[model_name] = model.predict(X_LSB0_val_scaler, verbose = True, batch_size=2048)
        
    if run_CR0_only:
        print ('Running model prediction on CR0')
        ypred_CR0[model_name] = model.predict(X_CR0_scaler, verbose = True, batch_size=2048)
        
    if run_HSB0_train_merged:
        print ('Running model prediction on HSB0 train (d2X<1.0)')
        ypred_HSB0_train_merged[model_name] = model.predict(X_HSB0_train_scaler_merged, verbose = True, batch_size=2048)

    if run_LSB0_val2_merged:
        print ('Running model prediction on LSB0 train (d2X<1.0)')
        ypred_LSB0_val2_merged[model_name] = model.predict(X_LSB0_val2_scaler_merged, verbose = True, batch_size=2048)

    if run_HSB0_val_merged:
        print ('Running model prediction on HSB0 val (d2X<1.0)')
        ypred_HSB0_val_merged[model_name] = model.predict(X_HSB0_val_scaler_merged, verbose = True, batch_size=2048)

    if run_LSB0_val_merged:
        print ('Running model prediction on LSB0 val (d2X<1.0)')
        ypred_LSB0_val_merged[model_name] = model.predict(X_LSB0_val_scaler_merged, verbose = True, batch_size=2048)

    if run_CR0_merged:
        print ('Running model prediction on CR0 merged')
        ypred_CR0_merged[model_name] = model.predict(X_CR0_merged_scaler, verbose = True, batch_size=2048)

    if run_HSB0_train_resolved:
        print ('Running model prediction on HSB0 train (d2X<1.0)')
        ypred_HSB0_train_resolved[model_name] = model.predict(X_HSB0_train_scaler_resolved, verbose = True, batch_size=2048)

    if run_LSB0_val2_resolved:
        print ('Running model prediction on LSB0 train (d2X<1.0)')
        ypred_LSB0_val2_resolved[model_name] = model.predict(X_LSB0_val2_scaler_resolved, verbose = True, batch_size=2048)

    if run_HSB0_val_resolved:
        print ('Running model prediction on HSB0 val (d2X<1.0)')
        ypred_HSB0_val_resolved[model_name] = model.predict(X_HSB0_val_scaler_resolved, verbose = True, batch_size=2048)

    if run_LSB0_val_resolved:
        print ('Running model prediction on LSB0 val (d2X<1.0)')
        ypred_LSB0_val_resolved[model_name] = model.predict(X_LSB0_val_scaler_resolved, verbose = True, batch_size=2048)

    if run_CR0_resolved:
        print ('Running model prediction on CR0 resolved')
        ypred_CR0_resolved[model_name] = model.predict(X_CR0_resolved_scaler, verbose = True, batch_size=2048)


#compare variables after reweighting                                                                                                                                                                      
Variables = ["trackjet1_eta","trackjet2_eta", "trackjet1_pt", "trackjet2_pt", "trackjet1_phi", "trackjet2_phi", "trackjet1_m", "trackjet2_m", "trackjet1_e", "trackjet2_e", "deta_trackjets", "pTH", "pTV", "etaH", "etaV", "phiH", "phiV", "mV", "mVH", "d2TracksV", "deltaR_trackjets", "deltaR_fatjets", "EH", "EV", "ntrkVVJJ_H", "ntrkVVJJ_V", "deta_fatjets",  "trackjet_pt_balance", "nTrackJetsH", "mVHres", "mVres", "mH"]   
#"SmallJet1_X_pT", "SmallJet2_X_pT", "SmallJet1_X_eta", "SmallJet2_X_eta", "SmallJet1_X_E", "SmallJet2_X_E", "SmallJet1_X_M", "SmallJet1_X_M", "mVHres", "mVres","mH"]
#"mVHres", "mVres", "mH", "dphi_fatjets", "dphi_trackjets", "HCand_nBTags", "trackjet1_btag_score", "trackjet2_btag_score",]
#Variables = ["ntrkVVJJ_H"]
#Variables = ["trackjet2_pt"]
#binning = [[0, 1,2,3,4,5,12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 100] ]

Higgs_mass = []


binning = [
    [-2.5, -1.8, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2.5], #trackjet1 eta                                                                         
    [-2.5, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 2.5], #trackjet2 eta                                                                         
    [0, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 450, 500, 550, 600, 700, 800], #trackjet1 pT 
    [0, 40, 60, 80, 100, 120, 150, 200, 250, 300, 400],  #trackjet2 pT                                                                                                                                   
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #trackjet1 phi                       
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #trackjet2 phi                       
    [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 28, 40], #trackjet1 m                                                                                                                    
    [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 24, 40], #trackjet2 m                                                                                                   
    [0, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 900, 1000, 1200, 1400, 1600, 1800, 2000, 3000, 4000], #trackjet1 e                                         
    [0, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1400,1700, 3000], #trackjet2 e   
    [0.03, 0.06, 0.09, 0.12, 0.15, 0.18, 0.21, 0.24, 0.27, 0.30, 0.33, 0.36, 0.39, 0.42, 0.45, 0.48, 0.51, 0.54, 0.57, 0.6, 0.70, 0.80, 0.90, 1.5], #deta trackjets                                       
    [200, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1800], #pTH                                                                                                                   
    [200, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1100, 1200, 1300, 1800], #pTV                                                                                       
    [-2, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 2], #etaH                                                                                                 
    [-2, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 2], #etaV                                                                                                 
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #phiH                                
    [-3.5, -3.25, -3, -2.75, -2.5, -2.25, -2, -1.75, -1.5, -1.25, -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5], #phiV                                
    [50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510, 540, 570, 800], #mV
    [1282, 1368, 1460, 1551, 1648, 1747, 1849, 1952, 2061, 2172, 2289, 2409, 2533, 2664, 2801, 2945, 3097, 3256, 3424, 3608, 3805, 4021, 4267, 4557, 4912, 5409, 6400],  #mVH
    [0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5], #d2TracksV                                                                          
    [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4],#deltaR_trackjets                                                                                                             
    [1.4, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5, 5.2, 5.4, 5.6, 6], #deltaR_fatjets                                                                                      
    [0, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 4000], #EH                                                                                      
    [0, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 3000,  4000], #EV                                                                         
    [0, 12, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 100], #ntracks H                                      
    [0, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 100], #ntracks V                                      
    [0, 0.6, 0.80, 0.90, 1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8, 4, 4.2], #deta fatjets                                                                                      
    #[-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12], #trackjet1_btagscore                      
    #[-5, -4.5, -4, -3.5, -3, -2.5, -2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10, 10.5, 11, 11.5, 12], #trackjet2_btagscore               
    [0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 1], #trackjet pt balance                                
    #[0, 1, 2, 3, 4], #nBtag jets H                                                                                                                                                                       
    [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15], #ntrackjetsH                                                                                                                                  
    #[0, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 450, 500, 700, 800, 1000], #SmallJet1_X_pT                                                                             
    #[0, 40, 60, 80, 100, 120, 150, 200, 250, 300, 400],  #SmallJet2_X_pT                                                                                                                                  
    #[-2.5, -1.8, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2.5], #SmallJet1_X_1_eta                                                                     
    #[-2.5, -1.8, -1.6, -1.4, -1.2, -1, -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8, 2.5], #SmallJet2_X_eta                                                                       
    #[0, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500,1600, 1700, 1800, 2000, 3000, 4000], #SmallJet1_X_1_E                                    
    #[0, 50, 100, 150, 200, 300, 400, 500, 600, 800, 1000, 1200, 1300, 1400, 1500, 1700, 2000, 3000], #SmallJet1_X_2_E                                                                                     
    #[0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 40], #SmallJet1_X_1_m                                                                                                                     
    #[0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 40], #SmallJet1_X_2_m       
    [1282, 1368, 1460, 1551, 1648, 1747, 1849, 1952, 2061, 2172, 2289, 2409, 2533, 2664, 2801, 2945, 3097, 3256, 3424, 3608, 3805, 4021, 4267, 4557, 4912, 5409, 6400], #mVHres 
    [50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510, 540, 570, 800], #mVres

    #[0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 5.75, 6, 6.25, 6.5], #deltaphi fatjets                                             
    #[0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 5.75, 6, 6.25, 6.5], #deltaphi trackjets    

]

if run_HSB0_train:

    Higgs_mass = [145, 147, 149, 151, 153, 155, 165, 167, 169, 171, 173, 175, 177] 
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_train_goodsels_OR = pd.Series(True, index= DF_HSB0_train.index)
    for model_name in model_names:
        HSB0_train_goodsels_OR &= goodsel_HSB0_train[model_name]
    
    DF_HSB0_train = DF_HSB0_train[HSB0_train_goodsels_OR]
    DFs_HSB0_train = []
    DFs_HSB0_train += [DF_HSB1_train] 
    DFs_HSB0_train += [DF_HSB0_train]

    for n in range(0, len(model_names)):
        DFs_HSB0_train += [DF_HSB0_train]

    Labels = [None]*len(DFs_HSB0_train)
    Labels[0] = 'HSB1 train'
    Labels[1] = 'HSB0 train'

    for n in range(2, len(DFs_HSB0_train)):
        Labels[n] = 'rewHSB0 train ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_train)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_train)
    no_weights_HSB1_train = pd.Series(1, index= DF_HSB1_train.index)
    no_weights_HSB0_train = pd.Series(1, index= DF_HSB0_train.index)
    Weights_list[0] = no_weights_HSB1_train
    Weights_list[1] = no_weights_HSB0_train
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_train[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_train, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_train, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_train[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 train weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + '/Exp_Score_onHSB0_train_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1

if run_LSB0_val2:

    Higgs_mass = [50, 52, 54, 56, 58, 60, 62, 64, 66]  
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)

    LSB0_val2_goodsel_OR = pd.Series(True, index= DF_LSB0_val2.index) 
    for model_name in model_names:
        LSB0_val2_goodsel_OR &= goodsel_LSB0_val2[model_name]  
    DF_LSB0_val2 = DF_LSB0_val2[LSB0_val2_goodsel_OR]

    DFs_LSB0_val2 = []
    DFs_LSB0_val2 += [DF_LSB1_train]
    DFs_LSB0_val2 += [DF_LSB0_val2]

    for n in range(0, len(model_names)):
        DFs_LSB0_val2 += [DF_LSB0_val2]

    Labels = [None]*len(DFs_LSB0_val2)
    Labels[0] = 'LSB1 val2'
    Labels[1] = 'LSB0 val2'

    for n in range(2, len(DFs_LSB0_val2)):
         Labels[n] = 'rewLSB0 val2 ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val2)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DF_LSB0_val2)
    no_weights_LSB1_train = pd.Series(1, index= DF_LSB1_train.index)
    no_weights_LSB0_val2 = pd.Series(1, index= DF_LSB0_val2.index)  
    Weights_list[0] = no_weights_LSB1_train
    Weights_list[1] = no_weights_LSB0_val2

    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val2[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val2, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val2, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)   
 

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val2[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val2 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val2_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1

if run_HSB0_val:

    Higgs_mass = [165, 169, 172, 175, 177, 179, 181, 183, 185, 187,189, 191, 193, 195, 197, 199, 201]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_val_goodsels_OR = pd.Series(True, index= DF_HSB0_val.index)
    for model_name in model_names:
        HSB0_val_goodsels_OR &= goodsel_HSB0_val[model_name]
    
    DF_HSB0_val = DF_HSB0_val[HSB0_val_goodsels_OR]
    DFs_HSB0_val = []
    DFs_HSB0_val += [DF_HSB1_val] 
    DFs_HSB0_val += [DF_HSB0_val]

    for n in range(0, len(model_names)):
        DFs_HSB0_val += [DF_HSB0_val]

    Labels = [None]*len(DFs_HSB0_val)
    Labels[0] = 'HSB1 val'
    Labels[1] = 'HSB0 val'

    for n in range(2, len(DFs_HSB0_val)):
        Labels[n] = 'rewHSB0 val ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_val)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_val)
    no_weights_HSB1_val = pd.Series(1, index= DF_HSB1_val.index)
    no_weights_HSB0_val = pd.Series(1, index= DF_HSB0_val.index)
    Weights_list[0] = no_weights_HSB1_val
    Weights_list[1] = no_weights_HSB0_val
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_val[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_val, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_val, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_val[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 val weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onHSB0_val_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1


if run_LSB0_val:

    Higgs_mass = [65, 67, 69, 71, 73, 76]  
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)

    LSB0_val_goodsel_OR = pd.Series(True, index= DF_LSB0_val.index) 
    for model_name in model_names:
        LSB0_val_goodsel_OR &= goodsel_LSB0_val[model_name]  

    DF_LSB0_val = DF_LSB0_val[LSB0_val_goodsel_OR]
    DFs_LSB0_val = []
    DFs_LSB0_val += [DF_LSB1_val]
    DFs_LSB0_val += [DF_LSB0_val]

    for n in range(0, len(model_names)):
        DFs_LSB0_val += [DF_LSB0_val]

    Labels = [None]*len(DFs_LSB0_val)
    Labels[0] = 'LSB1 val'
    Labels[1] = 'LSB0 val'

    for n in range(2, len(DFs_LSB0_val)):
         Labels[n] = 'rewLSB0 val ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DF_LSB0_val)
    no_weights_LSB1_val = pd.Series(1, index= DF_LSB1_val.index)
    no_weights_LSB0_val = pd.Series(1, index= DF_LSB0_val.index)  
    Weights_list[0] = no_weights_LSB1_val
    Weights_list[1] = no_weights_LSB0_val

    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)   

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1


if run_CR0_only:

    Higgs_mass = [75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145]  
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)

    CR0_goodsel_OR = pd.Series(True, index= DF_CR0.index) 
    for model_name in model_names:
        CR0_goodsel_OR &= goodsel_CR0[model_name]  

    DF_CR0 = DF_CR0[CR0_goodsel_OR]
    DFs_CR0 = []

    for n in range(0, len(model_names)):
        DFs_CR0 += [DF_CR0]

    Labels = [None]*len(DFs_CR0)

    for n in range(0, len(DFs_CR0)):
         Labels[n] = 'rewCR0 val ' + model_labels[n]

    Colors = [None]*len(DFs_CR0)
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(0)
    for model_name in model_names:
        Colors[0] = (Colors_models[n].hex)
        n = n + 1

    Weights_list = [None]*len(DF_CR0)

    n = int(0)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_CR0[model_name])
        n = n + 1

    n = int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_CR0[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'CR0 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onCR0_' + model_labels[n] + '.png')
        plt.clf()
        n = n + 1


    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_CR0, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_CR0_only, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)   
     


if run_HSB0_train_merged:

    Higgs_mass = [145, 147, 149, 151, 153, 155, 165, 167, 169, 171, 173, 175, 177, 179, 181]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_train_goodsels_OR_merged = pd.Series(True, index= DF_HSB0_train_merged.index)
    for model_name in model_names:
        HSB0_train_goodsels_OR_merged &= goodsel_HSB0_train_merged[model_name]
    
    DF_HSB0_train_merged = DF_HSB0_train_merged[HSB0_train_goodsels_OR_merged]
    DFs_HSB0_train_merged = []
    DFs_HSB0_train_merged += [DF_HSB1_train_merged] 
    DFs_HSB0_train_merged += [DF_HSB0_train_merged]

    for n in range(0, len(model_names)):
        DFs_HSB0_train_merged += [DF_HSB0_train_merged]

    Labels = [None]*len(DFs_HSB0_train_merged)
    Labels[0] = 'HSB1 train (d2X<1.0)'
    Labels[1] = 'HSB0 train (d2X<1.0)'

    for n in range(2, len(DFs_HSB0_train_merged)):
        Labels[n] = 'rewHSB0 (d2X<1.0) train ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_train_merged)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_train_merged)
    no_weights_HSB1_train_merged = pd.Series(1, index= DF_HSB1_train_merged.index)
    no_weights_HSB0_train_merged = pd.Series(1, index= DF_HSB0_train_merged.index)
    Weights_list[0] = no_weights_HSB1_train_merged
    Weights_list[1] = no_weights_HSB0_train_merged
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_train_merged[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_train_merged, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_train_merged, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_train_merged[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 train d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onHSB0_train_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1

 
if run_LSB0_val2_merged:

    Higgs_mass = [50, 52, 54, 56, 58, 60, 62, 64, 66]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    LSB0_val2_goodsels_OR_merged = pd.Series(True, index= DF_LSB0_val2_merged.index)
    for model_name in model_names:
        LSB0_val2_goodsels_OR_merged &= goodsel_LSB0_val2_merged[model_name]
    
    DF_LSB0_val2_merged = DF_LSB0_val2_merged[LSB0_val2_goodsels_OR_merged]
    DFs_LSB0_val2_merged = []
    DFs_LSB0_val2_merged += [DF_LSB1_train_merged] 
    DFs_LSB0_val2_merged += [DF_LSB0_val2_merged]

    for n in range(0, len(model_names)):
        DFs_LSB0_val2_merged += [DF_LSB0_val2_merged]

    Labels = [None]*len(DFs_LSB0_val2_merged)
    Labels[0] = 'LSB1 train (d2X<1.0)'
    Labels[1] = 'LSB0 train (d2X<1.0)'

    for n in range(2, len(DFs_LSB0_val2_merged)):
        Labels[n] = 'rewLSB0 (d2X<1.0) train ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val2_merged)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_LSB0_val2_merged)
    no_weights_LSB1_train_merged = pd.Series(1, index= DF_LSB1_train_merged.index)
    no_weights_LSB0_val2_merged = pd.Series(1, index= DF_LSB0_val2_merged.index)
    Weights_list[0] = no_weights_LSB1_train_merged
    Weights_list[1] = no_weights_LSB0_val2_merged
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val2_merged[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val2_merged, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val2_merged, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val2_merged[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val2 d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val2_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1


if run_HSB0_val_merged:

    Higgs_mass = [181, 183, 185, 187,189, 191, 193, 195, 197, 199, 201]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_val_goodsels_OR_merged = pd.Series(True, index= DF_HSB0_val_merged.index)
    for model_name in model_names:
        HSB0_val_goodsels_OR_merged &= goodsel_HSB0_val_merged[model_name]
    
    DF_HSB0_val_merged = DF_HSB0_val_merged[HSB0_val_goodsels_OR_merged]
    DFs_HSB0_val_merged = []
    DFs_HSB0_val_merged += [DF_HSB1_val_merged] 
    DFs_HSB0_val_merged += [DF_HSB0_val_merged]

    for n in range(0, len(model_names)):
        DFs_HSB0_val_merged += [DF_HSB0_val_merged]

    Labels = [None]*len(DFs_HSB0_val_merged)
    Labels[0] = 'HSB1 val (d2X<1.0)'
    Labels[1] = 'HSB0 val (d2X<1.0)'

    for n in range(2, len(DFs_HSB0_val_merged)):
        Labels[n] = 'rewHSB0 (d2X<1.0) val ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_val_merged)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_val_merged)
    no_weights_HSB1_val_merged = pd.Series(1, index= DF_HSB1_val_merged.index)
    no_weights_HSB0_val_merged = pd.Series(1, index= DF_HSB0_val_merged.index)
    Weights_list[0] = no_weights_HSB1_val_merged
    Weights_list[1] = no_weights_HSB0_val_merged
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_val_merged[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_val_merged, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_val_merged, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_val_merged[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 val d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onHSB0_val_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1
    

if run_LSB0_val_merged:

    Higgs_mass = [65, 67, 69, 71, 73, 75]

    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    LSB0_val_goodsels_OR_merged = pd.Series(True, index= DF_LSB0_val_merged.index)
    for model_name in model_names:
        LSB0_val_goodsels_OR_merged &= goodsel_LSB0_val_merged[model_name]
    
    DF_LSB0_val_merged = DF_LSB0_val_merged[LSB0_val_goodsels_OR_merged]
    DFs_LSB0_val_merged = []
    DFs_LSB0_val_merged += [DF_LSB1_val_merged] 
    DFs_LSB0_val_merged += [DF_LSB0_val_merged]

    for n in range(0, len(model_names)):
        DFs_LSB0_val_merged += [DF_LSB0_val_merged]

    Labels = [None]*len(DFs_LSB0_val_merged)
    Labels[0] = 'LSB1 val (d2X<1.0)'
    Labels[1] = 'LSB0 val (d2X<1.0)'

    for n in range(2, len(DFs_LSB0_val_merged)):
        Labels[n] = 'rewLSB0 (d2X<1.0) val ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val_merged)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_LSB0_val_merged)
    no_weights_LSB1_val_merged = pd.Series(1, index= DF_LSB1_val_merged.index)
    no_weights_LSB0_val_merged = pd.Series(1, index= DF_LSB0_val_merged.index)
    Weights_list[0] = no_weights_LSB1_val_merged
    Weights_list[1] = no_weights_LSB0_val_merged
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val_merged[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val_merged, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val_merged, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val_merged[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1
        

if run_CR0_merged:

    Higgs_mass = [75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145]  
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)

    CR0_merged_goodsel_OR = pd.Series(True, index= DF_CR0_merged.index) 
    for model_name in model_names:
        CR0_merged_goodsel_OR &= goodsel_CR0_merged[model_name]  

    DF_CR0_merged = DF_CR0_merged[CR0_merged_goodsel_OR]
    DFs_CR0_merged = []

    for n in range(0, len(model_names)):
        DFs_CR0_merged += [DF_CR0_merged]

    Labels = [None]*len(DFs_CR0_merged)

    for n in range(0, len(DFs_CR0_merged)):
         Labels[n] = 'rewCR0 merged ' + model_labels[n]

    Colors = [None]*len(DFs_CR0_merged)
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(0)
    for model_name in model_names:
        Colors[0] = (Colors_models[n].hex)
        n = n + 1

    Weights_list = [None]*len(DF_CR0_merged)

    n = int(0)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_CR0_merged[model_name])
        n = n + 1

    n = int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_CR0_merged[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'CR0 merged weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onCR0_merged_' + model_labels[n] + '.png')
        plt.clf()
        n = n + 1


    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_CR0_merged, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_CR0_merged, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)   
     
if run_HSB0_train_resolved:

    Higgs_mass = [145, 147, 149, 151, 153, 155, 165, 167, 169, 171, 173, 175, 177, 179, 181]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_train_goodsels_OR_resolved = pd.Series(True, index= DF_HSB0_train_resolved.index)
    for model_name in model_names:
        HSB0_train_goodsels_OR_resolved &= goodsel_HSB0_train_resolved[model_name]
    
    DF_HSB0_train_resolved = DF_HSB0_train_resolved[HSB0_train_goodsels_OR_resolved]
    DFs_HSB0_train_resolved = []
    DFs_HSB0_train_resolved += [DF_HSB1_train_resolved] 
    DFs_HSB0_train_resolved += [DF_HSB0_train_resolved]

    for n in range(0, len(model_names)):
        DFs_HSB0_train_resolved += [DF_HSB0_train_resolved]

    Labels = [None]*len(DFs_HSB0_train_resolved)
    Labels[0] = 'HSB1 train (d2X<1.0)'
    Labels[1] = 'HSB0 train (d2X<1.0)'

    for n in range(2, len(DFs_HSB0_train_resolved)):
        Labels[n] = 'rewHSB0 (d2X<1.0) train ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_train_resolved)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_train_resolved)
    no_weights_HSB1_train_resolved = pd.Series(1, index= DF_HSB1_train_resolved.index)
    no_weights_HSB0_train_resolved = pd.Series(1, index= DF_HSB0_train_resolved.index)
    Weights_list[0] = no_weights_HSB1_train_resolved
    Weights_list[1] = no_weights_HSB0_train_resolved
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_train_resolved[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_train_resolved, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_train_resolved, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)

    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_train_resolved[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 train d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onHSB0_train_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1

 
if run_LSB0_val2_resolved:

    Higgs_mass = [50, 52, 54, 56, 58, 60, 62, 64, 66]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    LSB0_val2_goodsels_OR_resolved = pd.Series(True, index= DF_LSB0_val2_resolved.index)
    for model_name in model_names:
        LSB0_val2_goodsels_OR_resolved &= goodsel_LSB0_val2_resolved[model_name]
    
    DF_LSB0_val2_resolved = DF_LSB0_val2_resolved[LSB0_val2_goodsels_OR_resolved]
    DFs_LSB0_val2_resolved = []
    DFs_LSB0_val2_resolved += [DF_LSB1_train_resolved] 
    DFs_LSB0_val2_resolved += [DF_LSB0_val2_resolved]

    for n in range(0, len(model_names)):
        DFs_LSB0_val2_resolved += [DF_LSB0_val2_resolved]

    Labels = [None]*len(DFs_LSB0_val2_resolved)
    Labels[0] = 'LSB1 train (d2X<1.0)'
    Labels[1] = 'LSB0 train (d2X<1.0)'

    for n in range(2, len(DFs_LSB0_val2_resolved)):
        Labels[n] = 'rewLSB0 (d2X<1.0) train ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val2_resolved)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_LSB0_val2_resolved)
    no_weights_LSB1_train_resolved = pd.Series(1, index= DF_LSB1_train_resolved.index)
    no_weights_LSB0_val2_resolved = pd.Series(1, index= DF_LSB0_val2_resolved.index)
    Weights_list[0] = no_weights_LSB1_train_resolved
    Weights_list[1] = no_weights_LSB0_val2_resolved
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val2_resolved[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val2_resolved, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val2_resolved, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val2_resolved[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val2 d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val2_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1


if run_HSB0_val_resolved:

    Higgs_mass = [181, 183, 185, 187,189, 191, 193, 195, 197, 199, 201]
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    HSB0_val_goodsels_OR_resolved = pd.Series(True, index= DF_HSB0_val_resolved.index)
    for model_name in model_names:
        HSB0_val_goodsels_OR_resolved &= goodsel_HSB0_val_resolved[model_name]
    
    DF_HSB0_val_resolved = DF_HSB0_val_resolved[HSB0_val_goodsels_OR_resolved]
    DFs_HSB0_val_resolved = []
    DFs_HSB0_val_resolved += [DF_HSB1_val_resolved] 
    DFs_HSB0_val_resolved += [DF_HSB0_val_resolved]

    for n in range(0, len(model_names)):
        DFs_HSB0_val_resolved += [DF_HSB0_val_resolved]

    Labels = [None]*len(DFs_HSB0_val_resolved)
    Labels[0] = 'HSB1 val (d2X<1.0)'
    Labels[1] = 'HSB0 val (d2X<1.0)'

    for n in range(2, len(DFs_HSB0_val_resolved)):
        Labels[n] = 'rewHSB0 (d2X<1.0) val ' + model_labels[n-2]

    Colors = [None]*len(DFs_HSB0_val_resolved)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_HSB0_val_resolved)
    no_weights_HSB1_val_resolved = pd.Series(1, index= DF_HSB1_val_resolved.index)
    no_weights_HSB0_val_resolved = pd.Series(1, index= DF_HSB0_val_resolved.index)
    Weights_list[0] = no_weights_HSB1_val_resolved
    Weights_list[1] = no_weights_HSB0_val_resolved
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_HSB0_val_resolved[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_HSB0_val_resolved, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_HSB0_val_resolved, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_HSB0_val_resolved[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'HSB0 val d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onHSB0_val_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1
    

if run_LSB0_val_resolved:

    Higgs_mass = [65, 67, 69, 71, 73, 75]

    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)
    
    LSB0_val_goodsels_OR_resolved = pd.Series(True, index= DF_LSB0_val_resolved.index)
    for model_name in model_names:
        LSB0_val_goodsels_OR_resolved &= goodsel_LSB0_val_resolved[model_name]
    
    DF_LSB0_val_resolved = DF_LSB0_val_resolved[LSB0_val_goodsels_OR_resolved]
    DFs_LSB0_val_resolved = []
    DFs_LSB0_val_resolved += [DF_LSB1_val_resolved] 
    DFs_LSB0_val_resolved += [DF_LSB0_val_resolved]

    for n in range(0, len(model_names)):
        DFs_LSB0_val_resolved += [DF_LSB0_val_resolved]

    Labels = [None]*len(DFs_LSB0_val_resolved)
    Labels[0] = 'LSB1 val (d2X<1.0)'
    Labels[1] = 'LSB0 val (d2X<1.0)'

    for n in range(2, len(DFs_LSB0_val_resolved)):
        Labels[n] = 'rewLSB0 (d2X<1.0) val ' + model_labels[n-2]

    Colors = [None]*len(DFs_LSB0_val_resolved)
    Colors[0] = 'black'
    Colors[1] = 'orange'
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(2)
    for model_name in model_names:
        Colors[n] = (Colors_models[n-2].hex)
        n = n + 1

    Weights_list = [None]*len(DFs_LSB0_val_resolved)
    no_weights_LSB1_val_resolved = pd.Series(1, index= DF_LSB1_val_resolved.index)
    no_weights_LSB0_val_resolved = pd.Series(1, index= DF_LSB0_val_resolved.index)
    Weights_list[0] = no_weights_LSB1_val_resolved
    Weights_list[1] = no_weights_LSB0_val_resolved
    
    n = int(2)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_LSB0_val_resolved[model_name])
        n = n + 1

    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_LSB0_val_resolved, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_LSB0_val_resolved, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)
 
    n=int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_LSB0_val_resolved[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'LSB0 val d2<1 weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onLSB0_val_d2less1_' + model_labels[n] + '.png')
        plt.clf()
        n=n+1
        

if run_CR0_resolved:

    Higgs_mass = [75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145]  
    
    Binning = binning.copy()
    Binning.append(Higgs_mass)
    Binning = tuple(Binning)

    CR0_resolved_goodsel_OR = pd.Series(True, index= DF_CR0_resolved.index) 
    for model_name in model_names:
        CR0_resolved_goodsel_OR &= goodsel_CR0_resolved[model_name]  

    DF_CR0_resolved = DF_CR0_resolved[CR0_resolved_goodsel_OR]
    DFs_CR0_resolved = []

    for n in range(0, len(model_names)):
        DFs_CR0_resolved += [DF_CR0_resolved]

    Labels = [None]*len(DFs_CR0_resolved)

    for n in range(0, len(DFs_CR0_resolved)):
         Labels[n] = 'rewCR0 resolved ' + model_labels[n]

    Colors = [None]*len(DFs_CR0_resolved)
    blue = Color("blue")
    Colors_models = list(blue.range_to(Color("green"),len(model_names)))
    n = int(0)
    for model_name in model_names:
        Colors[0] = (Colors_models[n].hex)
        n = n + 1

    Weights_list = [None]*len(DF_CR0_resolved)

    n = int(0)
    for model_name in model_names:
        Weights_list[n] = np.exp(ypred_CR0_resolved[model_name])
        n = n + 1

    n = int(0)
    for model_name in model_names:
        min = 0
        max = 6
        nbins = 60
        bins=np.linspace(min, max, nbins)
        bin_size = (max-min)/nbins
        plt.hist(np.exp(ypred_CR0_resolved[model_name]), bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'CR0 resolved weights ' + model_name], density=True)
        plt.xlabel('exp(NN score)')
        plt.ylabel('Entries/'+str(bin_size))
        plt.yscale('log')
        plt.legend(loc="upper right")
        plt.savefig(path_output + 'Exp_Score_onCR0_resolved_' + model_labels[n] + '.png')
        plt.clf()
        n = n + 1


    lib.comparison_multiplePlots_ratio(DFs_vector=DFs_CR0_resolved, labels_vector=Labels, colors_vector=Colors, variable_list=Variables, var_bins=Binning, path_plots=path_plots_CR0_resolved, sample_name ='data', name_histo = 'comparisons', Weights_vector=Weights_list, normalization=True)   
     
