import os, re, pickle
import pandas as pd
import numpy as np
import sklearn.utils
import matplotlib.pyplot as plt
from keras.models import Sequential,Model,load_model
from sklearn.metrics import roc_curve, auc, roc_auc_score, classification_report, confusion_matrix, accuracy_score
from colorama import Fore
import matplotlib.colors as mcolors
from eli5.permutation_importance import get_score_importances

def drawVariables(DF_categ0, DF_categ1, dirname, dataset_type, weights_categ0=None,weights_categ1=None, normalization = False):
  
   variables_dir ='input_var/'
   try:
      os.mkdir(dirname + variables_dir)
      print("Directory " , variables_dir ,  " Created ") 
   except FileExistsError:
      print("Directory " , variables_dir,  " already exists")

   variables = list(DF_categ0)
   for i in variables:
      if i=='signal_jet1_pt': bins = np.linspace(0, 600, 60)
      elif i=='signal_jet2_pt': bins = np.linspace(0, 150, 30)
      elif i=='signal_jet1_E': bins = np.linspace(0, 2000, 100)
      elif i=='signal_jet2_E': bins = np.linspace(0, 1000, 100)
      elif(i=='l1_pt') |(i=='l2_pt'): bins = np.linspace(0, 500, 50)
      elif(i=='l1_e') | (i=='l2_e'): bins = np.linspace(0, 1000, 100)
      elif(i=='fat_jet_pt'): bins = np.linspace(0,1000,100)
      elif(i=='fat_jet_M'): bins = np.linspace(0,400, 40)
      elif(i=='fat_jet_D2'): bins = np.linspace(0,5,50)
      elif(i=='fat_jet_ntrack'): bins = np.linspace(0,80,80)
      elif("eta" in i): bins = np.linspace(-5,5,50)
      elif("phi" in i): bins = np.linspace(-4,4,50)
      else: bins = 50 
      (n_0, bins_0, patches_0)= plt.hist(DF_categ0[i].as_matrix(), bins=bins,  histtype='stepfilled', alpha = 0.5, lw=3, label = [r'Category0'], weights = weights_categ0, normed = normalization)
      (n_1, bins_1, patches_1) = plt.hist(DF_categ1[i].as_matrix(), bins=bins, histtype='stepfilled', alpha = 0.5, lw=3, label = [r'Category1'], weights = weights_categ1, normed = normalization)
      if(normalization & (weights_categ0[0]!=None)): plt.ylabel('Norm. Yields')
      elif(normalization & (weights_categ0[0]==None)): plt.ylabel('Norm. Entries')
      elif(not normalization & (weights_categ0[0]!=None)): plt.ylabel('Yields')
      elif(not normalization & (weights_categ0[0]==None)): plt.ylabel('Entries')
      if(not normalization): plt.yscale('log')
      plt.xlabel(i)
      plt.legend(loc="upper right")
      plt.savefig(dirname + variables_dir + i + '_' + dataset_type +'.pdf')
      plt.clf()

def comparisonPlots(DF_categ0_type1, DF_categ1_type1, DF_categ0_type2, DF_categ1_type2, selection1, selection2, dirname, dataset_type, weights=None):
   variables_dir ='input_var/'
   try:
      os.mkdir(dirname + variables_dir)
      print("Directory " , variables_dir ,  " Created ") 
   except FileExistsError:
      print("Directory " , variables_dir,  " already exists")
   variables = list(DF_categ0_type1)
   for i in variables:
      plt.hist(DF_categ0_type1[i].as_matrix(), bins=50, histtype='step',  label = [r'Category0'+ selection1], color= 'b', normed=True)
      plt.hist(DF_categ0_type2[i].as_matrix(), bins=50, histtype='stepfilled',  label = [r'Category0'+ selection2], alpha = 0.5, lw=3, color= 'b', normed=True)
      plt.hist(DF_categ1_type1[i].as_matrix(), bins=50, histtype='step',  label = [r'Category1' + selection1], color= 'r', normed=True)
      plt.hist(DF_categ1_type2[i].as_matrix(), bins=50, histtype='stepfilled',  label = [r'Category1' + selection2],  alpha = 0.5, lw=3,  color= 'r', normed=True)
      plt.ylabel('Entries')
      plt.xlabel(i)
      plt.legend(loc="upper right")
      plt.savefig(dirname + variables_dir + i + '_' + dataset_type +'_selectioncomparison.pdf')
      plt.clf()

def comparisonPlots_2(DF_type1, DF_type2,  selection1, selection2, dirname, dataset_type, weights=None):
   variables_dir ='input_var/'
   try:
      os.mkdir(dirname + variables_dir)
      print("Directory " , variables_dir ,  " Created ") 
   except FileExistsError:
      print("Directory " , variables_dir,  " already exists")
   variables = list(DF_type1)
   for i in variables:
      plt.hist(DF_type1[i].as_matrix(), bins=50, histtype='step',  label = [r'Category0'+ selection1], normed=True)
      plt.hist(DF_type2[i].as_matrix(), bins=50, histtype='stepfilled',  label = [r'Category0'+ selection2],  normed=True)
      plt.ylabel('Entries')
      plt.xlabel(i)
      plt.legend(loc="upper right")
      plt.savefig(dirname + variables_dir + i + '_' + dataset_type +'_selectioncomparison.pdf')
      plt.clf()
   



def comparisonPlots_ratio(DF_type1=None, DF_type2=None, label1 = '', label2 = '', variable_list=None, a_list=None, b_list=None, nbin_list=None, var_bins=None, path_plots='', sample_name = '', name_histo = '', Weights1=None, Weights2=None, normalization=False):

   k = 0
   for i in variable_list:

      fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})

      plt.title(sample_name + ' ' + i)

      if var_bins==None :
         if a_list!=None : 
            min = a_list[k]
            max = b_list[k]
            nbins = nbin_list[k]
            bins=np.linspace(min, max, nbins)
            bin_size = (max-min)/nbins
            
         else : bins = 60
         
         ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=bins, histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
         ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=bins, histtype='stepfilled', alpha= 0.2, color = 'cyan', label = [label2],  density=normalization, weights=Weights2)

      else :
       ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=var_bins[k], histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
       ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=var_bins[k], histtype='stepfilled', alpha= 0.2, color = 'cyan', label = [label2],  density=normalization, weights=Weights2)


      ax1.set_ylabel('Data')
      ax1.set_yscale('log')   
      ax1.legend(loc="upper right")

      bins1 = bins1[:-1]
      ratio = (ns1[ns2!=0])/(ns2[ns2!=0])

      errors = np.array(ratio.shape)
      errors[(ratio!=0)] = np.multiply(ratio[ratio!=0], np.sqrt((1/ns1[ratio!=0]) + (1/ns2[ratio!=0])))
      errors[ratio==0] = 0

      ax2.plot(bins1[ns2!=0], ratio,  color = 'black', label=[label1+'/'+label2])
      ax2.errorbar(bins1[ns2!=0], ratio, yerr = errors)
      ax2.set_ylim((0.5,1.5))
      ax2.set_ylabel(label1 + '/' + label2)
      ax2.set_xlabel(i)
      ax2.axhline(y=1, linestyle='--')
      plt.savefig(path_plots + i + '_' + sample_name + '_' + name_histo + '.pdf')
      plt.clf()
      k+=1

def comparisonPlots_ratio(DF_type1=None, DF_type2=None, DF_type3=None, label1 = '', label2 = '', label3 = '', variable_list=None, a_list=None, b_list=None, nbin_list=None, var_bins=None, path_plots='', sample_name = '', name_histo = '', Weights1=None, Weights2=None, Weights3=None, normalization=False):

   k = int(0)
   for i in variable_list:

      fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})

      bin_size = 0.8

      if var_bins==None :
         if a_list!=None : 
            min = a_list[k]
            max = b_list[k]
            nbins = nbin_list[k]
            bins=np.linspace(min, max, nbins)
            bin_size = (max-min)/nbins
            
         else : bins = 60
         
         ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=bins, histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
         ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=bins, histtype='stepfilled', alpha= 0.2, color = 'cyan', label = [label2],  density=normalization, weights=Weights2)
         ns3, bins3, patches3 = ax1.hist(DF_type3[i].values, bins=bins, histtype='step', color = 'orange', label = [label3],  density=normalization, weights=Weights3)

      else :
       ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=var_bins[k], histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
       ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=var_bins[k], histtype='stepfilled', alpha= 0.2, color = 'cyan', label = [label2],  density=normalization, weights=Weights2)
       ns3, bins3, patches3 = ax1.hist(DF_type3[i].values, bins=var_bins[k], histtype='step', color = 'orange', label = [label3],  density=normalization, weights=Weights3)

      ax1.set_ylabel('Data')
      ax1.set_yscale('log')   
      ax1.set_ylim(bottom=1*np.float_power(10,-9)) 
      ax1.legend(loc="upper right")

      ratio1 = (ns1[ns2!=0])/(ns2[ns2!=0])
      ratio2 = (ns1[ns3!=0])/(ns3[ns3!=0])
      bins1 = bins1[:-1]
      ax2.plot(bins1[ns3!=0], ratio2, marker = '.', color = 'orange', label=[label1+'/'+label3])
      ax2.plot(bins1[ns2!=0], ratio1, marker = '.', color = 'cyan', label=[label1+'/'+label2])
      ax2.axhline(y=1, linestyle='--')
      #ax2.set_ylim((0.5,1.5))
      ax2.set_xlabel(i)
      ax2.legend(loc='best')
      plt.savefig(path_plots + i + '_' + sample_name + '_' + name_histo + '.pdf')
      plt.clf()
      k+=1

def comparison_multiplePlots_ratio(DFs_vector=None, labels_vector = None, colors_vector=None, variable_list=None, a_list=None, b_list=None, nbin_list=None, var_bins=None, path_plots='', sample_name = '', name_histo = '', Weights_vector=None, normalization=False):

   k = int(0)
   for i in variable_list:

      fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})

      bin_size = 0.8

      if var_bins==None :
         if a_list!=None : 
            min = a_list[k]
            max = b_list[k]
            nbins = nbin_list[k]
            bins=np.linspace(min, max, nbins)
            bin_size = (max-min)/nbins
            
         else : bins = 60

         l = int(0)
         ns = {}
         Bins = {}
         patches = {}
         ns_nodensity = {}
         Bins_nodensity = {}
         patches_nodensity = {}
         for DF in DFs_vector:
            Label = labels_vector[l]
            Weights = Weights_vector[l]
            if (('NN' in Label)):  #cancellare
               Linestyle = ':'
            else:
               Linestyle = '-'
            if colors_vector!=None:
               Color = colors_vector[l]
            else:
               Color = ''
            ns[l], Bins[l], patches[l] = ax1.hist(DF[i].values, bins=bins, histtype='step', ls=Linestyle, color=Color, label = [Label], density=normalization, weights=Weights)
            ns_nodensity[l], Bins_nodensity[l], patches_nodensity[l] = ax1.hist(DF[i].values, bins=bins, histtype='step', ls=Linestyle, alpha=0, color='white', density=False, weights=Weights)
            l = l + 1

      else :
         l = int(0)
         ns = {}
         Bins = {}
         patches = {}
         ns_nodensity = {}
         Bins_nodensity = {}
         patches_nodensity = {}
         for DF in DFs_vector:
            Label = labels_vector[l]
            Weights = Weights_vector[l]
            if (('NN' in Label)):  #cancellare                                                                                                                                                             
               Linestyle = ':'
            else:
               Linestyle = '-'
            if colors_vector!=None:
               Color = colors_vector[l]
            else:
               Color = ''
            ns[l], Bins[l], patches[l] = ax1.hist(DF[i].values, bins=var_bins[k], histtype='step',  ls=Linestyle, color=Color, label = [Label], density=normalization, weights=Weights)
            ns_nodensity[l], Bins_nodensity[l], patches_nodensity[l] = ax1.hist(DF[i].values, bins=var_bins[k], histtype='step',  ls=Linestyle, alpha=0, color='white', density=False, weights=Weights)
            l = l + 1

      ax1.set_ylabel('Data')
      ax1.set_yscale('log')   
      #ax1.set_ylim(bottom=1*np.float_power(10,-9)) 
      ax1.set_ylim(top=1*np.float_power(10, 1))
      ax1.legend(loc="best")

      errors = {}
      ratio = {}
      for l in range(1, len(labels_vector)):
         condition = ns[l]!=0
         ratio[l] = (ns[0][condition])/(ns[l][condition])
         
         errors[l] = np.zeros(shape=ratio[l].shape)

         ratio_bins = Bins[l][:-1][condition]
         if colors_vector!=None:
            Color = colors_vector[l]
         else:
            Color = ''
         #ax2.plot(ratio_bins, ratio[l], marker = '.', ls='-',  color=Color)

         condition_2 = (ns[l]!=0) & (ns[0]!=0)
         
         errors[l][ratio[l]!=0] = ratio[l][ratio[l]!=0] * np.sqrt((1/(ns_nodensity[0][condition_2])) + (1/(ns_nodensity[l][condition_2])))

         '''
         print('')
         print('variable ' + str(i))
         print('ratio shape ' + str(ratio[l].shape))
         print('num shape ' + str(ns[0][ns[l]!=0].shape))
         print('num shape ' + str(ns[l][ns[l]!=0].shape))
         print('errors shape ' + str(errors[l].shape))
         
         print('ratio_bins ' + str(ratio_bins))
         print('ratio' + str(ratio[l]))
         print('num ' + str(ns_nodensity[0][ns[l]!=0]))
         print('den ' + str(ns_nodensity[l][ns[l]!=0]))
         print('num norm ' + str(ns[0][ns[l]!=0]))
         print('den norm ' + str(ns[l][ns[l]!=0]))
         print('err ' + str(errors[l]))
         '''

         ax2.plot(ratio_bins, ratio[l], color = Color, marker='o', ms=2)
         ax2.fill_between(ratio_bins, ratio[l]-errors[l], ratio[l]+errors[l],  alpha=0.2, edgecolor=Color, facecolor=Color)
         

      ax2.axhline(y=1, linestyle='--')
      ax2.axhline(y=0.9, linestyle=':')
      ax2.axhline(y=1.1, linestyle=':')
      ax2.set_ylim((0.5,1.5))
      ax2.set_ylabel(labels_vector[0]+'/others')
      ax2.set_xlabel(i)
      plt.savefig(path_plots + i + '_' + sample_name + '_' + name_histo + '.pdf')
      plt.clf()
      k+=1


def comparisonPlots_3ratios(DF_type1=None, DF_type2=None, DF_type3=None, DF_type4=None, label1 = '', label2 = '', label3 = '', label4 = '', variable_list=None, a_list=None, b_list=None, nbin_list=None, var_bins=None, path_plots='', sample_name = '', name_histo = '', Weights1=None, Weights2=None, Weights3=None, Weights4=None, normalization=False):

   k = int(0)
   for i in variable_list:

      fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})

      bin_size = 0.8

      if var_bins==None :
         if a_list!=None : 
            min = a_list[k]
            max = b_list[k]
            nbins = nbin_list[k]
            bins=np.linspace(min, max, nbins)
            bin_size = (max-min)/nbins
            
         else : bins = 60
         
         ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=bins, histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
         ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=bins, histtype='stepfilled', alpha= 0.2, color = 'cyan', label = [label2],  density=normalization, weights=Weights2)
         ns3, bins3, patches3 = ax1.hist(DF_type3[i].values, bins=bins, histtype='stepfilled', alpha= 0.2, color = 'red', label = [label3],  density=normalization, weights=Weights3)
         ns4, bins4, patches4 = ax1.hist(DF_type4[i].values, bins=bins, histtype='step', color = 'orange', label = [label4],  density=normalization, weights=Weights4)

      else :
       ns1, bins1, patches1 = ax1.hist(DF_type1[i].values, bins=var_bins[k], histtype='step', label = [label1], color = 'black', density=normalization, weights=Weights1)
       ns2, bins2, patches2 = ax1.hist(DF_type2[i].values, bins=var_bins[k], histtype='step', linestyle = '--',  color = 'cyan', label = [label2],  density=normalization, weights=Weights2)
       ns3, bins3, patches3 = ax1.hist(DF_type3[i].values, bins=var_bins[k], histtype='step', linestyle = ':', color = 'red', label = [label3],  density=normalization, weights=Weights3)
       ns4, bins4, patches4 = ax1.hist(DF_type4[i].values, bins=var_bins[k], histtype='step', color = 'orange', label = [label4],  density=normalization, weights=Weights4)

      ax1.set_ylabel('Data')
      ax1.set_yscale('log')   
      #ax1.set_ylim(bottom=1*np.float_power(10,-9)) 
      ax1.legend(loc="upper right", fontsize = 'x-small', frameon = False)

      ratio1 = (ns1[ns2!=0])/(ns2[ns2!=0])
      ratio2 = (ns1[ns3!=0])/(ns3[ns3!=0])
      ratio3 = (ns1[ns4!=0])/(ns4[ns4!=0])

      bins1 = bins1[:-1]
      ax2.plot(bins1[ns4!=0], ratio3, marker = '.', color = 'orange')
      ax2.plot(bins1[ns2!=0], ratio1, marker = '.', color = 'cyan')
      ax2.plot(bins1[ns3!=0], ratio2, marker = '.', color = 'red')
      ax2.axhline(y=1, linestyle='--')
      #ax2.set_ylim((0.5,1.5))
      ax2.set_ylabel(label1 + '/' + label4 + '(rew)')
      ax2.set_xlabel(i)
      plt.savefig(path_plots + i + '_' + sample_name + '_' + name_histo + '.pdf')
      plt.clf()
      k+=1


def printNNScore(newdirName, yhat_train_categ0, yhat_train_categ1, yhat_test_categ0, yhat_test_categ1, weight_train_categ0, weight_train_categ1, weight_test_categ0, weight_test_categ1):
   bins=np.linspace(0,1, 50)
   plt.hist(yhat_train_categ0, bins=bins, histtype='step', lw=2, alpha=0.5, label=[r'Category0 Train'], normed=True, weights=weight_train_categ0)
   plt.hist(yhat_train_categ1, bins=bins, histtype='step', lw=2, alpha=0.5, label=[r'Category1 Train'], normed=True, weights=weight_train_categ1)
   plt.hist(yhat_test_categ0, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0 Test'], normed=True, weights=weight_test_categ0)
   plt.hist(yhat_test_categ1, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1 Test'], normed=True, weights=weight_test_categ1)
   #plt.yscale('log')
   plt.ylabel('Norm. Yields')
   plt.xlabel('DNN score')
   plt.legend(loc="upper center")
   plt.savefig(newdirName + "MC_TrainTest_Score.pdf")
   plt.clf()

   plt.hist(yhat_train_categ0, bins=bins, histtype='step',  label=[r'Category0 Train'], normed=True)
   plt.hist(yhat_test_categ0, bins=bins, histtype='stepfilled', label=[r'Category0 Test'], normed=True)
   plt.ylabel('Norm. Entries')
   plt.xlabel('DNN score')
   plt.legend(loc="upper center")
   plt.savefig(newdirName + "MC_TrainTest_Score_categ0.pdf")
   plt.clf()

   plt.hist(yhat_train_categ1, bins=bins, histtype='step', label=[r'Category1 Train'], normed=True)
   plt.hist(yhat_test_categ1, bins=bins, histtype='stepfilled', label=[r'Category1 Test'], normed=True)
   plt.ylabel('Norm. Entries')
   plt.xlabel('DNN score')
   plt.legend(loc="upper center")
   plt.savefig(newdirName + "MC_TrainTest_Score_categ1.pdf")
   plt.clf()

   (n_train0, bins_train0, patches_train0) = plt.hist(yhat_train_categ0, bins=bins, histtype='step', lw=2, alpha=0.5, label=[r'Category0 Train'], weights= weight_train_categ0)
   (n_train1, bins_train1, patches_train1) = plt.hist(yhat_train_categ1, bins=bins, histtype='step', lw=2, alpha=0.5, label=[r'Category1 Train'],  weights= weight_train_categ1)
   (n_test0, bins_test0, patches_test0) = plt.hist(yhat_test_categ0, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0 Test'], weights= weight_test_categ0)
   (n_test1, bins_test1, patches_test1) = plt.hist(yhat_test_categ1, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1 Test'], weights= weight_test_categ1)

   #asymptotic significance
   sigma_train = 0
   sigma_test = 0
   for i in range(len(bins_test0)-1):
      if n_train0[i]>0: sigma_train += 2*((n_train1[i] + n_train0[i])*np.log(1+(n_train1[i]/n_train0[i])) - n_train1[i])
      if n_test0[i]>0: sigma_test += 2*((n_test1[i] + n_test0[i])*np.log(1+(n_test1[i]/n_test0[i])) - n_test1[i])
   sigma_train = np.sqrt(sigma_train)
   sigma_test = np.sqrt(sigma_test)
   sigma_train = round(sigma_train,3)
   sigma_test = round(sigma_test,3)
   plt.text(0.01, 10000, 'Asysig (train) = ' + str(sigma_train) + '\n' + 'Asysig (test) = ' + str(sigma_test))
   plt.yscale('log')
   plt.ylabel('Yields')
   plt.xlabel('DNN score')
   plt.legend(loc="lower center")
   plt.savefig(newdirName + "MC_weighted_TrainTest_Score.pdf")
   plt.clf()

   plt.hist(yhat_train_categ0, bins=bins, weights=weight_train_categ0, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0 Train'])
   plt.hist(yhat_train_categ1, bins=bins, weights= weight_train_categ1, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1 Train'])
   plt.yscale('log')
   plt.ylabel('Yields')
   plt.xlabel('DNN score')
   plt.legend(loc="upper right")
   plt.savefig(newdirName + "MC_weighted_Train_Score.pdf")
   plt.clf()

   plt.hist(yhat_test_categ0, bins=bins, weights= weight_test_categ0, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0 Test'])
   plt.hist(yhat_test_categ1, bins=bins, weights= weight_test_categ1, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1 Test'])
   plt.yscale('log')
   plt.ylabel('Yields')
   plt.xlabel('DNN score')
   plt.legend(loc="lower center")
   plt.savefig(newdirName + "MC_weighted_Test_Score.pdf")
   plt.clf()



def printNNScore_Allsamples(newdirName, model_name, yhat_categ0, yhat_categ1, weight_categ0, weight_categ1, AsySig=False):

   #bins=np.linspace(0,1, 50)
   bins = np.linspace(0,3,60)


   if AsySig:   #asymptotic significance
      fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})
      (n_0, bins_0, patches_0) = ax1.hist(yhat_categ0, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0'], weights= weight_categ0)
      (n_1, bins_1, patches_1) = ax1.hist(yhat_categ1, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1'],  weights= weight_categ1)

      sigma = 0
      for i in range(len(bins_0)-1):
         if n_0[i]>0: sigma += 2*((n_1[i] + n_0[i])*np.log(1+(n_1[i]/n_0[i])) - n_1[i])
      sigma = np.sqrt(sigma)
      sigma = round(sigma,4)
      ax1.text(0.01, 50, 'Asysig = ' + str(sigma))
      
      sigma_bin = 0
      sigma_list = []
      for i in range(len(bins_0)-1):
         if n_0[i]>0: 
            sigma_bin += 2*((n_1[i] + n_0[i])*np.log(1+(n_1[i]/n_0[i])) - n_1[i])
         sigma_list.append(np.sqrt(sigma_bin))
      bins2 = bins_0[:-1]
      ax2.plot(bins2, sigma_list, color = 'blue')
      ax2.set_ylim((0,3.2))
      ax2.set_ylabel('asym sig')
      ax2.set_xlabel('Score')

      ax1.set_title(model_name)
      ax1.set_yscale('log')
      ax1.set_ylabel('Yields')
      ax1.legend(loc="upper right")
   else:
      plt.hist(yhat_categ0, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0'], weights= weight_categ0)
      plt.hist(yhat_categ1, bins=bins, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1'],  weights= weight_categ1)
      plt.title(model_name)

   plt.savefig(newdirName + "MC_weighted_allsamples_" + model_name + "_Score.pdf")
   plt.clf()


def printVariable_withAsysig_subplot(newdirName, variable_name, binning, DF_categ0, DF_categ1, weight_categ0, weight_categ1):

   fig, (ax1, ax2) = plt.subplots(nrows=2, sharex = True, gridspec_kw = {'height_ratios': [3, 1]})
   (n_0, bins_0, patches_0) = ax1.hist(DF_categ0[variable_name].values, bins=binning, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category0'], weights= weight_categ0)
   (n_1, bins_1, patches_1) = ax1.hist(DF_categ1[variable_name].values, bins=binning, histtype='stepfilled', lw=2, alpha=0.5, label=[r'Category1'],  weights= weight_categ1)

   sigma = 0
   for i in range(len(bins_0)-1):
      if n_0[i]>0: sigma += 2*((n_1[i] + n_0[i])*np.log(1+(n_1[i]/n_0[i])) - n_1[i])
   sigma = np.sqrt(sigma)
   sigma = round(sigma,4)
   ax1.text(0.01, 50, 'Asysig = ' + str(sigma))
      
   sigma_bin = 0
   sigma_list = []
   for i in range(len(bins_0)-1):
      if n_0[i]>0: 
         sigma_bin += 2*((n_1[i] + n_0[i])*np.log(1+(n_1[i]/n_0[i])) - n_1[i])
      sigma_list.append(np.sqrt(sigma_bin))
   bins2 = bins_0[:-1]
   ax2.plot(bins2, sigma_list, color = 'blue')
   ax2.set_ylim((0,3.2))
   ax2.set_ylabel('asym sig')
   ax2.set_xlabel(variable_name)
   
   ax1.set_yscale('log')
   ax1.set_ylabel('Yields')
   ax1.legend(loc="upper right")

   plt.savefig(newdirName + variable_name + '_asysig.pdf')
   plt.clf()


def printROCCurve(newdirName, fpr_w, tpr_w,fpr_train_w, tpr_train_w, roc_auc_w, roc_auc_train_w):
   plt.plot(fpr_w, tpr_w, color='darkorange', lw=2, label='Full curve Test (area = %0.3f)' % roc_auc_w)
   plt.plot(fpr_train_w, tpr_train_w, color='c', lw=2, label='Full curve Train (area = %0.3f)' % roc_auc_train_w)
   plt.plot([0, 0], [1, 1], color='navy', lw=2, linestyle='--')
   plt.xlim([-0.05, 1.0])
   plt.ylim([0.0, 1.05])
   plt.ylabel('True Positive Rate (weighted)')
   plt.xlabel('False Positive Rate (weighted)')
   plt.title('curve for Category1 vs Category0')
   plt.legend(loc="lower right")
   plt.savefig(newdirName + "ROC.png")
   plt.clf()
   
def plotTrainPerformance(newdirName, FitHistory, logY=False):  
  # plt.plot(FitHistory.history['acc'])
  # plt.plot(FitHistory.history['val_acc'])
  # plt.title('Model accuracy')
  # plt.ylabel('Accuracy')
  # plt.xlabel('Epoch')
  # if logY==True : plt.yscale('log')
  # plt.legend(['Train', 'Val'], loc='lower right')
  # plt.savefig(newdirName + "/Accuracy.png")
  # plt.clf()
   
   # summarize history for loss
   plt.plot(FitHistory.history['loss'])
   #plt.plot(FitHistory.history['val_loss'])
   plt.plot(FitHistory.history['val_myloss'])
   #plt.ylim(top=2.1)
   plt.title('Model loss')
   plt.ylabel('Loss')
   plt.xlabel('Epoch')
   
   if logY==True : plt.yscale('log')
   #plt.legend(['Train loss', 'Train loss on Val'], loc='best')  
   plt.legend(['Train loss', 'Validation loss'], loc='best')
   plt.savefig(newdirName + "/Loss.png")
   plt.clf()

'''
   #'summarize history for loss
   plt.plot(FitHistory.history['precision_m'])
   plt.plot(FitHistory.history['val_precision_m'])
   plt.title('Precicion')
   plt.ylabel('Precision')
   plt.xlabel('Epoch')
   if logY==True : plt.yscale('log')  
   plt.legend(['Train', 'Val'], loc='upper right')
   plt.savefig(newdirName + "/Precision.png")
   plt.clf()

   #summarize history for loss
   plt.plot(FitHistory.history['recall_m'])
   plt.plot(FitHistory.history['val_recall_m'])
   plt.title('Recall')
   plt.ylabel('Recall')
   plt.xlabel('Epoch')
   if logY==True : plt.yscale('log') 
   plt.legend(['Train', 'Val'], loc='upper right')
   plt.savefig(newdirName + "/Recall.png")
   plt.clf()

#summarize history for loss
   plt.plot(FitHistory.history['f1_m'])
   plt.plot(FitHistory.history['f1_m'])
   plt.title('F1')
   plt.ylabel('F1')
   plt.xlabel('Epoch')
   if logY==True : plt.yscale('log') 
   plt.legend(['Train', 'Val'], loc='upper right')
   plt.savefig(newdirName + "/F1.png")
   plt.clf()
'''

def MyNNScore(X, path_output):
   print(Fore.RED + 'Opening model in ' + path_output)
   model = load_model(path_output+'model.h5', compile=False)
   yhat = model.predict(X, verbose = True, batch_size=2048)
   return yhat



#with the model just trained one can test the importance of each variable simply evaluating the model on a test set where one variable at a time is shuffled (this is quite similar to eliminate the variable)
def DoFeaturesRanking(path_output, X, y, variables):

   def score(X, y):
      y_pred = MyNNScore(X, path_output)
      #fpr_w, tpr_w, thresholds_w = roc_curve(y, y_pred)
      #return auc(fpr_w, tpr_w)
      return y_pred

   print("DOING FEATURES RANKING")
   base_score, score_decreases = get_score_importances(score, X, y) #base_score: array with the score referring to the net evaluated to the full dataset (score0); score dec: 3D-matrix(#randomizations, variables, score0-score)
   feature_importances = np.mean(score_decreases, axis=0)
   fpr0, tpr0, thresholds0 = roc_curve(y, base_score)
   AUC0 = auc(fpr0, tpr0)
   score = base_score - score_decreases
   nRandom = score.shape[0] #number of shuffling operations
   nVars = score.shape[1] #number of variables
   nEvents = score.shape[2] #dataset dimension
   AUC = np.zeros( (nRandom, nVars) )
   for i in range(0, nRandom):#iteration over the number of shuffering 
      for j in range(0, nVars):#iteration over variables
         score_all = score[i][j][:] 
         score_1 = score_all[y==1]
         score_0 = score_all[y==0]
         fpr, tpr, thresholds = roc_curve(y, score_all)
         AUC[i][j] = auc(fpr, tpr)
   AUCs = np.mean(AUC, axis=0)
   AUCs_min = np.min(AUC, axis=0)
   AUCs_max = np.max(AUC, axis=0)
   AUCs_error = ( AUCs_max-AUCs_min) / 2
   plt.figure()
   AUC0_vec = np.zeros( ( len(variables) ) )
   AUC0_vec[:] = AUC0
   plt.plot(variables, AUC0_vec, label='AUC0')
   plt.errorbar(variables, AUCs, yerr=AUCs_error, label='AUC variables')
   plt.xticks(rotation='vertical')
   plt.legend(loc="upper right")
   plt.xlabel('')
   plt.ylabel('AUC')
   plt.savefig(path_output + 'plot_FR_AUC.pdf')
   AUC_mean = np.zeros( (nVars) )
   plt.figure()

   for j in range(0, nVars):
      score_all = np.mean( score, axis=0 )
      score_all = score_all[j][:]
      print(score_all.shape)
      score_1 = score_all[y==1]
      score_0 = score_all[y==0]
      label = ', Var' + str(j)
      label = 'Removing ' + variables[j]
      plt.hist(score_1, range=[0,1], bins=50, label=label, histtype='step', density=False)
      fpr, tpr, thresholds = roc_curve(y, score_all)
      AUC_mean[j] = auc(fpr, tpr)
   plt.xticks(rotation='vertical')
   plt.legend(loc="upper right")
   plt.xlabel('DNN Score')
   plt.ylabel('Events')
   plt.savefig(path_output + 'plot_FR_scores.pdf')
   plt.figure()
   plt.plot(variables, AUC0_vec, label='AUC0')
   plt.plot(variables, (1-AUC_mean)/(1-AUC0_vec), label='Model Inefficiency')
   plt.xticks(rotation='vertical')
   plt.legend(loc="upper right")
   plt.xlabel('')
   plt.ylabel('Rank')
   plt.savefig(path_output + 'plot_FR_Rank.pdf')
   return base_score, score_decreases, feature_importances

def compareROCcurves(dirName, ModelsNames, fpr_w, tpr_w, roc_auc_w):
   i=0
   for model in ModelsNames:
      plt.plot(fpr_w[i], tpr_w[i], label = [model + ' (AUC: ' + roc_auc_w[i] + ')'])
      i = i+1
   plt.ylabel('True Positive Rate (weighted)')
   plt.xlabel('False Positive Rate (weighted)')
   plt.title('ROCs for Category1 vs Category0')
   plt.legen(loc="lower right")
   plt.savefig(newdirName + "ROCs_compared.png")
   plt.clf()

