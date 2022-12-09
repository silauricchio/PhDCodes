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
import joblib

from keras.models import Sequential,Model,load_model
from keras.layers.core import Dense, Activation
from keras.layers import BatchNormalization, Dropout, concatenate
from keras.callbacks import ModelCheckpoint, EarlyStopping
from keras.optimizers import Adam
from keras import backend as K
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix

create_pickles_fromTree = True
create_train_and_test = False

#create training dataset for nominal OR systematics 
run_nominal = False
run_systematics = True

mc_campaign = 'mc16a'
#mc_campaign = 'mc16d'
#mc_campaign = 'mc16e'

WP = '60'
#WP = '70'
#WP = '80'


file_name = 'data'


model_name = 'fatjet_HwithNTracks_andTrackJets'

dataset_path ='/eos/user/s/sauricch/ReaderOutput/ntuples_v1.8_skimmed/' + mc_campaign     
path_pickles = '/eos/user/s/sauricch/MachineLearning/YXH/pickle_files/Ntuples_v1p8_WP' +  WP + '/' +  mc_campaign + '/'
if run_nominal:
    path_model= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_145_175_v1p6_schemeA_WP' + WP + '/' + model_name +  "/" #change to your path
else if run_systematics:
    path_model= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_165_200_v1p5b_WP' + WP + '/' + model_name +  "/"  #change to your path  

if file_name == 'data':
    files = ["data.root"]
if model_name == 'fatjet_HwithNTracks_andTrackJets':
     VariablesToModel = ['pTH', 'etaH', 'phiH', 'EH', 'ntrkVVJJ_H', "trackjet1_pt", "trackjet1_eta", "trackjet1_phi", "trackjet1_m", "trackjet2_pt", "trackjet2_eta", "trackjet2_phi", "trackjet2_m"]


#create the output directories if they don't exist                                                                                                                                                         

try:
    os.mkdir(path_pickles)
    print("Directory ", path_pickles,  " Created ")
except FileExistsError:
    print("Directory ", path_pickles,  " already exists")

try:
    os.mkdir(path_model)
    print("Directory ", path_model,  " Created ")
except FileExistsError:
    print("Directory ", path_model,  " already exists")


if create_pickles_fromTree:
    
    DF_HSB0_list = []
    DF_HSB1_list = []
   
    DF_LSB0_list = []
    DF_LSB1_list = []
    
    DF_SR_list = []
    DF_CR0_list = []

    DF_HSB0_Train_list = []
    DF_HSB0_Test_list = []
    DF_HSB1_Train_list = []
    DF_HSB1_Test_list = []
    
    
    DF_LSB0_Train_list = []
    DF_LSB0_Test_list = []
    DF_LSB1_Train_list = []
    DF_LSB1_Test_list = []
    
    #DF_SR_Train_list = []
    #DF_SR_Test_list = []
    #DF_CR0_Train_list = []
    #DF_CR0_Test_list = []
    
    for f in files:                                                                                                                                                                                     
        print(f)                                                                                                                                                                                              
        file = dataset_path + "/" + f                                                                                                                                                                   
        print(file)           
    
        if os.path.isfile(file):                                                                                                                                                                        
            theFile = uproot.open(file)                                                                                                                                              
            tree = theFile["Nominal"]                                                                                                                                                                       
            DF = tree.pandas.df()
        
            #defining the datasets
            
            #DF = DF[((DF["Jet1_XbbScore_V3"]>(-2)) | (DF["Jet2_XbbScore_V3"]>(-2)))]

            #DF["HCand_XbbScore_V3"] = np.where(DF["HCand_isJet1"]==True, DF["Jet1_XbbScore_V3"], DF["Jet2_XbbScore_V3"])
 
            if WP == '60':
                
                print(Fore.RED+str("Data Frame 0 Selection: HSB0 region"))
                HSB0_cuts =( (DF["HCand_XbbScore"]>(-2)) & (DF["HCand_XbbScore"]<2.46) & (DF["mH"]>145) & (DF["mH"]<200) )
                DF_HSB0 = DF[HSB0_cuts]
                print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_HSB0.shape[0])))

                print(Fore.RED+str("Data Frame 1 Selection: HSB1 region"))
                HSB1_cuts = ( (DF["HCand_XbbScore"]>2.46) & (DF["mH"]>145) & (DF["mH"]<200) )
                DF_HSB1 = DF[HSB1_cuts]
                print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_HSB1.shape[0])))
                
                print(Fore.RED+str("Data Frame for Selection: LSB0 region"))
                LSB0_cuts = ( (DF["HCand_XbbScore"]>(-2)) & (DF["HCand_XbbScore"]<2.46) & (DF["mH"]>50) & (DF["mH"]<75) )
                DF_LSB0 = DF[LSB0_cuts]
                print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_LSB0.shape[0])))

                print(Fore.RED+str("Data Frame for Selection: LSB1 region"))
                LSB1_cuts = ( (DF["HCand_XbbScore"]>2.46) & (DF["mH"]>50) & (DF["mH"]<75))
                DF_LSB1 = DF[LSB1_cuts]
                print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_LSB1.shape[0])))
                
                print(Fore.RED+str("Data Frame for Selection: CR0 region"))
                CR0_cuts = ( (DF["HCand_XbbScore"]>(-2)) & (DF["HCand_XbbScore"]<2.46) & (DF["mH"]>75) & (DF["mH"]<145))
                DF_CR0 = DF[CR0_cuts]
                print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_CR0.shape[0])))
                
                #print(Fore.RED+str("Data Frame for Selection: SR region"))
                #SR_cuts = ( (DF["HCand_XbbScore"]>2.46) & (DF["mH"]>75) & (DF["mH"]<145) )
                #DF_SR = DF[SR_cuts]
                #print('{:<20} {:<15}'.format('with N events: ', Fore.BLUE+str(DF_SR.shape[0])))
                
                
            
            DF_HSB0["Category"] = 0
            DF_HSB1["Category"] = 1
            
            DF_LSB0["Category"] = 0
            DF_LSB1["Category"] = 1
            
            DF_CR0["Category"] = 0
            #DF_SR["Category"] = 1

            
            #save DataFrames
            file_name_simple = f
            file_name_simple = file_name_simple.replace(".root","")
            
            DF_HSB0['sample_name'] = file_name_simple
            DF_HSB1['sample_name'] = file_name_simple
            DF_LSB0['sample_name'] = file_name_simple
            DF_LSB1['sample_name'] = file_name_simple
            
            DF_CR0['sample_name'] = file_name_simple
            #DF_SR['sample_name'] = file_name_simple

            print(list(DF_HSB0))
            
            DF_HSB0_list += [DF_HSB0]
            DF_HSB1_list += [DF_HSB1]
            DF_LSB0_list += [DF_LSB0]
            DF_LSB1_list += [DF_LSB1]
            
            DF_CR0_list += [DF_CR0]            
            #DF_SR_list += [DF_SR]
            
            # Split in Train/Test samples                                                                                                                                                                                
            
            print ('{:>20} {:<15} {:<15}'.format('Number of raw events in HSB0: ',Fore.GREEN+str(DF_HSB0.shape[0])," Splitting to..."))
            Ntrain_DF0 = int(round(DF_HSB0.shape[0] * 0.7))    #70% train-30%test                                                                                                                                    
            DF_HSB0_Train = DF_HSB0[:Ntrain_DF0]
            DF_HSB0_Test = DF_HSB0[Ntrain_DF0:]
            print ('{:>20} {:<15}'.format('DF0 Train events: ',Fore.GREEN+str(DF_HSB0_Train.shape[0])))
            print ('{:>20} {:<15}'.format('DF0 Test events: ',Fore.GREEN+str(DF_HSB0_Test.shape[0])+'\n' ))
            
            print ('{:>20} {:<15} {:<15}'.format('Number of raw events in HSB1: ',Fore.GREEN+str(DF_HSB1.shape[0])," Splitting to..."))
            Ntrain_DF1 = int(round(DF_HSB1.shape[0] * 0.7))    #70% train-30%test            
            
            DF_HSB1_Train = DF_HSB1[:Ntrain_DF1]
            DF_HSB1_Test = DF_HSB1[ Ntrain_DF1:]
            print ('{:>20} {:<15}'.format('DF1 Train events: ',Fore.GREEN+str(DF_HSB1_Train.shape[0])))
            print ('{:>20} {:<15}'.format('DF1 Test events: ',Fore.GREEN+str(DF_HSB1_Test.shape[0])+'\n' ))
            
            DF_HSB0_Train_list += [DF_HSB0_Train]
            DF_HSB0_Test_list += [DF_HSB0_Test]
            DF_HSB1_Train_list += [DF_HSB1_Train]
            DF_HSB1_Test_list += [DF_HSB1_Test]
            
            
            print ('{:>20} {:<15} {:<15}'.format('Number of raw events in LSB0: ',Fore.GREEN+str(DF_LSB0.shape[0])," Splitting to..."))
            Ntrain_DF0 = int(round(DF_LSB0.shape[0] * 0.7))    #70% train-30%test                                                                                                                          

            DF_LSB0_Train = DF_LSB0[:Ntrain_DF0]
            DF_LSB0_Test = DF_LSB0[Ntrain_DF0:]
            print ('{:>20} {:<15}'.format('LSB0 Train events: ',Fore.GREEN+str(DF_LSB0_Train.shape[0])))
            print ('{:>20} {:<15}'.format('LSB0 Test events: ',Fore.GREEN+str(DF_LSB0_Test.shape[0])+'\n' ))

        
            print ('{:>20} {:<15} {:<15}'.format('Number of raw events in LSB1: ',Fore.GREEN+str(DF_LSB1.shape[0])," Splitting to..."))
            Ntrain_DF1 = int(round(DF_LSB1.shape[0] * 0.7))    #70% train-30%test  

            DF_LSB1_Train = DF_LSB1[:Ntrain_DF1]
            DF_LSB1_Test = DF_LSB1[Ntrain_DF1:]
            print ('{:>20} {:<15}'.format('LSB1 Train events: ',Fore.GREEN+str(DF_LSB1_Train.shape[0])))
            print ('{:>20} {:<15}'.format('LSB1 Test events: ',Fore.GREEN+str(DF_LSB1_Test.shape[0])+'\n' ))

            DF_LSB0_Train_list += [DF_LSB0_Train]
            DF_LSB0_Test_list += [DF_LSB0_Test]
            DF_LSB1_Train_list += [DF_LSB1_Train]
            DF_LSB1_Test_list += [DF_LSB1_Test]
            
            
    #save DataFrames train and test
    
    DF_HSB0_list = pd.concat(DF_HSB0_list)
    DF_HSB1_list = pd.concat(DF_HSB1_list)

    
    DF_LSB0_list = pd.concat(DF_LSB0_list)
    DF_LSB1_list = pd.concat(DF_LSB1_list)
    
    DF_CR0_list = pd.concat(DF_CR0_list)    
    #DF_SR_list = pd.concat(DF_SR_list)
    
    DF_HSB0_Train_list = pd.concat(DF_HSB0_Train_list)
    DF_HSB0_Test_list = pd.concat(DF_HSB0_Test_list)
    DF_HSB1_Train_list = pd.concat(DF_HSB1_Train_list)
    DF_HSB1_Test_list = pd.concat(DF_HSB1_Test_list)
    
    
    DF_LSB0_Train_list = pd.concat(DF_LSB0_Train_list)
    DF_LSB0_Test_list = pd.concat(DF_LSB0_Test_list)
    DF_LSB1_Train_list = pd.concat(DF_LSB1_Train_list)
    DF_LSB1_Test_list = pd.concat(DF_LSB1_Test_list)
    

    DF_HSB0_list.to_pickle(path_pickles + file_name + "_HSB0_Full.pkl")
    DF_HSB1_list.to_pickle(path_pickles + file_name + "_HSB1_Full.pkl")
    
    DF_LSB0_list.to_pickle(path_pickles + file_name + "_LSB0_Full.pkl")
    DF_LSB1_list.to_pickle(path_pickles + file_name + "_LSB1_Full.pkl")
    
    DF_CR0_list.to_pickle(path_pickles + file_name + "_CR0_Full.pkl")    
    #DF_SR_list.to_pickle(path_pickles + file_name + "_SR_Full.pkl")
    
    DF_HSB0_Train_list.to_pickle(path_pickles + file_name + "_HSB0_Train.pkl")
    DF_HSB0_Test_list.to_pickle(path_pickles + file_name + "_HSB0_Test.pkl")
    DF_HSB1_Train_list.to_pickle(path_pickles + file_name + "_HSB1_Train.pkl")
    DF_HSB1_Test_list.to_pickle(path_pickles + file_name + "_HSB1_Test.pkl")
    
    
    DF_LSB0_Train_list.to_pickle(path_pickles + file_name + "_LSB0_Train.pkl")
    DF_LSB0_Test_list.to_pickle(path_pickles + file_name + "_LSB0_Test.pkl")
    DF_LSB1_Train_list.to_pickle(path_pickles + file_name + "_LSB1_Train.pkl")
    DF_LSB1_Test_list.to_pickle(path_pickles + file_name + "_LSB1_Test.pkl")
    

if create_train_and_test:

    #TRAIN

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_HSB0_Train.pkl') ) )
    X_train_0_HSB = pd.read_pickle(path_pickles + file_name +  '_HSB0_Train.pkl')

    if run_nominal:
        HSB_train_cut_categ0 = ((X_train_0_HSB['mH']>145) & (X_train_0_HSB['mH']<175))
    else if run_systematics:
        HSB_train_cut_categ0 = ((X_train_0_HSB['mH']>165) & (X_train_0_HSB['mH']<200))
    X_train_0_HSB = X_train_0_HSB[HSB_train_cut_categ0]

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_HSB1_Train.pkl') ) )
    X_train_1_HSB = pd.read_pickle(path_pickles + file_name +  '_HSB1_Train.pkl')
        
    if run_nominal:
        HSB_train_cut_categ1 = ((X_train_1_HSB['mH']>145) & (X_train_1_HSB['mH']<175))
    else if run_systematics:
        HSB_train_cut_categ1 = ((X_train_1_HSB['mH']>165) & (X_train_1_HSB['mH']<200))
    X_train_1_HSB = X_train_1_HSB[HSB_train_cut_categ1]

    
    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str( path_pickles + file_name +  '_LSB0_Train.pkl') ) )
    X_train_0_LSB = pd.read_pickle(path_pickles + file_name +  '_LSB0_Train.pkl')

    LSB_train_cut_categ0 = ((X_train_0_LSB['mH']>65) & (X_train_0_LSB['mH']<75))
    X_train_0_LSB = X_train_0_LSB[LSB_train_cut_categ0]

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_LSB1_Train.pkl') ) )
    X_train_1_LSB = pd.read_pickle(path_pickles + file_name +  '_LSB1_Train.pkl')  

    LSB_train_cut_categ1 = ((X_train_1_LSB['mH']>65) & (X_train_1_LSB['mH']<75))
    X_train_1_LSB = X_train_1_LSB[LSB_train_cut_categ1]
    

    #TEST

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_HSB0_Test.pkl') ) )
    X_test_0_HSB = pd.read_pickle(path_pickles + file_name +  '_HSB0_Test.pkl')

    if run_nominal:
        HSB_test_cut_categ0 = ((X_test_0_HSB['mH']>145) & (X_test_0_HSB['mH']<175))                                                                                                                       
    else if run_systematics:
        HSB_test_cut_categ0 = ((X_test_0_HSB['mH']>165) & (X_test_0_HSB['mH']<200))                                                                                                                       
    X_test_0_HSB = X_test_0_HSB[HSB_test_cut_categ0]

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_HSB1_Test.pkl') ) )
    X_test_1_HSB = pd.read_pickle(path_pickles + file_name +  '_HSB1_Test.pkl')

    if run_nominal:
        HSB_test_cut_categ1 = ((X_test_1_HSB['mH']>145) & (X_test_1_HSB['mH']<175))
    else if run_systematics:
        HSB_test_cut_categ1 = ((X_test_1_HSB['mH']>165) & (X_test_1_HSB['mH']<200))
    X_test_1_HSB = X_test_1_HSB[HSB_test_cut_categ1]

    
    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str( path_pickles + file_name +  '_LSB0_Test.pkl') ) )
    X_test_0_LSB = pd.read_pickle(path_pickles + file_name +  '_LSB0_Test.pkl')

    LSB_test_cut_categ0 = ((X_test_0_LSB['mH']>65) & (X_test_0_LSB['mH']<75))
    X_test_0_LSB = X_test_0_LSB[LSB_test_cut_categ0]

    print ('{:>20} {:<15}'.format('Reading DataFrame ', Fore.GREEN+str(path_pickles + file_name +  '_LSB1_Test.pkl') ) )
    X_test_1_LSB = pd.read_pickle(path_pickles + file_name +  '_LSB1_Test.pkl')  

    LSB_test_cut_categ1 = ((X_test_1_LSB['mH']>65) & (X_test_1_LSB['mH']<75))
    X_test_1_LSB = X_test_1_LSB[LSB_test_cut_categ1]
    

    print(Fore.BLUE+str('Creating datasets for training and test'))
        
    DF_train = [X_train_0_HSB] + [X_train_1_HSB]    
    DF_train = pd.concat(DF_train)
    DF_train = sklearn.utils.shuffle(DF_train, random_state=123) #shuffle the train dataset
    
    DF_test = [X_test_0_HSB] + [X_test_1_HSB]
    DF_test = pd.concat(DF_test)
    DF_test = sklearn.utils.shuffle(DF_test, random_state=123)  #shuffle the test dataset       
    
    
    twotrackjets = False
    for v in VariablesToModel :
        if 'trackjet' in v: 
            twotrackjets = True
            
    if twotrackjets==True:
        print(Fore.BLUE+str('Select only event with at least 2 track jets'))       
        
        sel_trackjets_train = ( (DF_train["trackjet1_pt"]>=0) & (DF_train["trackjet2_pt"]>=0) )
        sel_trackjets_test = ( (DF_test["trackjet1_pt"]>=0) & (DF_test["trackjet2_pt"]>=0) )
        
        print('{:<20} {:<15}'.format('Training set BEFORE selection: ', Fore.BLUE+str(DF_train.shape[0])))      
        DF_train = DF_train[sel_trackjets_train]
        print('{:<20} {:<15}'.format('Training set AFTER selection: ', Fore.BLUE+str(DF_train.shape[0])))
        
        print('{:<20} {:<15}'.format('Test set BEFORE selection: ', Fore.BLUE+str(DF_test.shape[0])))
        DF_test = DF_test[sel_trackjets_test]
        print('{:<20} {:<15}'.format('Test set AFTER selection: ', Fore.BLUE+str(DF_test.shape[0])))
        
        
        
    #create the matrix with features and the vector with labels
    for v in VariablesToModel:
        if v=='deltaR_trackjets' :
            DF_train["deltaR_trackjets"] = np.sqrt(np.power(DF_train["trackjet1_eta"] - DF_train["trackjet2_eta"], 2) + np.power(DF_train["trackjet1_phi"] - DF_train["trackjet2_phi"],2))
            DF_test["deltaR_trackjets"] = np.sqrt(np.power(DF_test["trackjet1_eta"] - DF_test["trackjet2_eta"], 2) + np.power(DF_test["trackjet1_phi"] - DF_test["trackjet2_phi"],2))
        
        if v=='deltaR_fatjets' : 
            DF_train["deltaR_fatjets"] = np.sqrt(np.power(DF_train["etaH"] - DF_train["etaV"], 2) + np.power(DF_train["phiH"] - DF_train["phiV"],2))
            DF_test["deltaR_fatjets"] = np.sqrt(np.power(DF_test["etaH"] - DF_test["etaV"], 2) + np.power(DF_test["phiH"] - DF_test["phiV"],2))
    
        if v=='deta_trackjets' :
            DF_train["deta_trackjets"] = np.abs(DF_train["trackjet1_eta"] - DF_train["trackjet2_eta"])
            DF_test["deta_trackjets"] = np.abs(DF_test["trackjet1_eta"] - DF_test["trackjet2_eta"])

        if v=='deta_fatjets' :
            DF_train["deta_fatjets"] = np.abs(DF_train["etaH"] - DF_train["etaV"])
            DF_test["deta_fatjets"] = np.abs(DF_test["etaH"] - DF_test["etaV"])

        if v=='dphi_trackjets' :
            DF_train["dphi_trackjets"] = DF_train["trackjet1_phi"]-DF_train["trackjet2_phi"]
            DF_train["dphi_trackjets"][DF_train["dphi_trackjets"]<0] += 2*np.pi 
        
            DF_test["dphi_trackjets"] = DF_test["trackjet1_phi"]-DF_test["trackjet2_phi"]
            DF_test["dphi_trackjets"][DF_test["dphi_trackjets"]<0] += 2*np.pi 
                
        if v=='dphi_fatjets' :
            DF_train["dphi_fatjets"] = DF_train["phiH"]-DF_train["phiV"]
            DF_train["dphi_fatjets"][DF_train["dphi_fatjets"]<0] += 2*np.pi 
            
            DF_test["dphi_fatjets"] = DF_test["phiH"]-DF_test["phiV"]
            DF_test["dphi_fatjets"][DF_test["dphi_fatjets"]<0] += 2*np.pi

        if v=='trackjet1_btag_score':
            inf_train_selection = (np.abs(DF_train['trackjet1_btag_score'])!=np.inf)
            DF_train = DF_train[(DF_train['trackjet1_btag_score']!= np.inf)]

            inf_test_selection = (np.abs(DF_test['trackjet1_btag_score'])!=np.inf)
            DF_test = DF_test[inf_test_selection]

        if v=='trackjet2_btag_score':
            DF_train = DF_train[(np.abs(DF_train['trackjet2_btag_score'])!= np.inf)]
            DF_test = DF_test[(np.abs(DF_test['trackjet2_btag_score'])!= np.inf)]

        if v=='trackjet1_e':
            theta_DF_train = 2* np.arctan(1/(np.exp(DF_train['trackjet1_eta'])))
            theta_DF_test = 2* np.arctan(1/(np.exp(DF_test['trackjet1_eta'])))
            DF_train['trackjet1_e'] = np.sqrt((DF_train['trackjet1_m']**2) + ((DF_train['trackjet1_pt']/np.cos(theta_DF_train))**2))
            DF_test['trackjet1_e'] = np.sqrt((DF_test['trackjet1_m']**2) + ((DF_test['trackjet1_pt']/np.cos(theta_DF_test))**2))

        if v=='trackjet2_e':
            theta_DF_train = 2* np.arctan(1/(np.exp(DF_train['trackjet2_eta'])))
            theta_DF_test = 2* np.arctan(1/(np.exp(DF_test['trackjet2_eta'])))
            DF_train['trackjet2_e'] = np.sqrt((DF_train['trackjet2_m']**2) + ((DF_train['trackjet2_pt']/np.cos(theta_DF_train))**2))
            DF_test['trackjet2_e'] = np.sqrt((DF_test['trackjet2_m']**2) + ((DF_test['trackjet2_pt']/np.cos(theta_DF_test))**2))

            
    VariablesToModel.append('weight')
    X_train = DF_train[VariablesToModel]
    y_train = DF_train['Category']
    X_test = DF_test[VariablesToModel]
    y_test = DF_test['Category']
    
    goodsel_train = pd.Series(True, index= X_train.index)
    goodsel_test = pd.Series(True, index = X_test.index)

    for v in VariablesToModel:
         goodsel_train &= (np.abs(X_train[v])!= np.inf)
         goodsel_test &= (np.abs(X_test[v])!= np.inf)

    print("Training events before infinities removing: ", X_train.shape[0])
    print("Testing events before infinities removing: ", X_test.shape[0])

    X_train = X_train[goodsel_train]
    y_train = y_train[goodsel_train]
    X_test = X_test[goodsel_test]
    y_test = y_test[goodsel_test]
    
    print("Training events after infinities removing: ", X_train.shape[0])
    print("Testing events after infinities removing: ", X_test.shape[0])


    #save matrix for training and test

    print(Fore.BLUE+str('Saving pickles with features and labels for training and test'))
    
    X_train.to_pickle(path_model + 'X_Train_' + mc_campaign + '.pkl')
    y_train.to_pickle(path_model + 'y_Train_' + mc_campaign + '.pkl')
    
    X_test.to_pickle(path_model + 'X_Test_' + mc_campaign + '.pkl')
    y_test.to_pickle(path_model + 'y_Test_' + mc_campaign + '.pkl')

