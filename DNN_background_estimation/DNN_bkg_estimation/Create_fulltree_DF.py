#!/bin/python  

import os, re, pickle
import uproot
import pandas as pd
pd.options.mode.chained_assignment = None  


mc_campaign = 'mc16a'
#mc_campaign = 'mc16d'                                                                                                                                                                                 
#mc_campaign = 'mc16e'                                                                                                                                                                                 
  
#file_name = 'data' #use this if you didn't split tree in mini trees
file_name = 'data_0' #_0,_1,_2 are mini trees indexes
#file_name = 'data_1'
#file_name = 'data_2'

model_name = 'fatjet_HwithNTracks_andTrackJets'

dataset_path = '/eos/user/s/sauricch/ReaderOutput/SuperNtuples_v1.9_skimmed/'+ mc_campaign +'/'
path_pickles = '/eos/user/s/sauricch/MachineLearning/YXH/pickle_files/SuperNtuples_v1p9_WP60/' + mc_campaign + '/'
path_model= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_145_175_v1p6_schemeA_WP60/fatjet_HwithNTracks_andTrackJets/'


if file_name == 'data':
    files = ["data.root"]
if file_name == 'data_0':    
    files = ["data_0.root"]  
if file_name == 'data_1':
    files = ["data_1.root"]
if file_name == 'data_2':
    files = ["data_2.root"]


DF_list = []
    
for f in files:
    
    print(f)                                                                                                                                                                                       
    file = dataset_path + f
    print(file)
    
    if os.path.isfile(file):
        theFile = uproot.open(file)
        tree = theFile["Nominal"]                                                                                                                                                                   
        DF = tree.pandas.df()
        
        #save DataFrames                                                                                                                                                                         
        file_name_simple = f
        file_name_simple = file_name_simple.replace(".root","")
        DF['sample_name'] = file_name_simple
        DF_list += [DF]
        
        
DF_list = pd.concat(DF_list)
print(Fore.BLUE+'Saving DataFrame as ' + path_pickles + file_name + '_fulltree.pkl')
DF_list.to_pickle(path_pickles + file_name + "_fulltree.pkl")
        
