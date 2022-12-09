#!/bin/python                                                                                                                                                                                              

import os
    
modelpath= '/afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/models/train_inHSB_mH_145_175_v1p6_schemeA_WP60/fatjet_HwithNTracks_andTrackJets/FULLSTAT/Bootstrap/'  


subd = int(0)
models = int(0)

print('path ' + str(modelpath))
for subdir in os.listdir(modelpath):
    
    subd = subd+1
    path_file = modelpath + subdir 
    file =  path_file+ '/model.h5'
    
    if(os.path.exists(file)):
        print(subdir + '-->FULL')
        models = models+1
    else:
        print(subdir + '-->EMPTY')
        os.rmdir(path_file)
    
print('models found: ' +str(models))
print('on number of bootstrap: ' + str(subd))
