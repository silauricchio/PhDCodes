#This code provides useful function to plot variables and do basics statistical studies

There are 3 scripts you can use to speed up the use of the Plotter:
    1) script_start.sh : you need to source it at the beginning, so you can compile and run executable very easily
    2) script_compile.sh : script_start.sh sources this script. It does cmake --source and make -j. You don't need to source it if you launch first script_start.sh
    3) script_GitLab_submit.sh : this script helps to commit on GitLab only the source repository



To clone this repository from Git: git clone ssh://git@gitlab.cern.ch:7999/sauricch/plotter.git

The first time you download it, create the build directory for compiling and the run directory when you should run your plotter (to not confuse output and the other files). Build and run should be in the same directory of source one.

Once source the script_start.sh, you can easily compile with command "compile" in any directory you are in. 

The script also create alias for running the executable (e.g. run_Plotter) 
    	  ->REMEMBER: if you create a new main code, you have to add it on the CMakeList.txt file.



The source consists in 4 folders: app, Plotter, Root and util

   - app: put here the main codes and the config files
   - Plotter: contains the header files of libraries
   - Root: contains the cxx files of libraries
   - util: contains other libraries (e.g. for debug)


The main code (Plotter.cxx) reads info from a config file (config_Plotter.cfg)  and put them into the code.

The config file contains:
    - the input files to read with several tags and a flag (ON/ OFF) to enable/disable the file
    - the selections to apply
    - the variables to plot, with the bins number and the range for the histogram
    - a flag to enable/disable variable binning
    - the variable binning for the variables you want to plot with varying bin size


The default config file path is indicated in the main code, but you can change it at the moment to run the code:

e.g.: run_Plotter -c path_config

you can run with the following options:
    "-c": to overwrite the default config file path
    "-d": for enabling debug mode
    "-h": to print help for these options


Input files are managed by the class ReadFiles (ReadFiles.h and .cxx). 
In the main code you specify which chains to create, grouping files according to a common tag. If the corresponding file/files are disabled in the config file, the chain is not created, but the program does not have a crash.

                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The graphics options are managed by the class Graphics_options (GraphicsOptions.h) 

                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Histograms are created by the class HistoMaker (HistoMaker.h and HistoMaker.cxx)

The principal methods are:
  - HistoMaker::DrawStacks() : create stack plots for every variable indicated in the config file. Data and signals enabled in the config file are superimposed and the Data/MC ratio is drawn. If Data are set to OFF the ratio is not drawn.
  
  - HistoMaker::DrawHistos() : create plots for every variable and for every selection indicated in the config file, with all the chain superimposed in the same canvas.
  - HistoMaker::DrawHistos(histos_num, histos_den) : create plots for every variable indicated in the config file, with all the chain superimposed in the same canvas and the ratio plots among histos_num and histos_den
  - HistoMaker::CompareVariables(sels_to_compare, vars_to_compare, tags_to_compare) : this method compares variables for each selection in the "sels_to_compare" vector
  - HistoMaker::CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare): this method compares selections for each variable in the "vars_to_compare" vector
  - HistoMaker::CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare, sels_num, sels_den) : this method compares selections for each variable  in the "vars_to_compare" vector with ratio plot
  
  - HistoMaker::CompareHistos_fromFile(file_names, histo_names, vars_to_compare) : this method needs alredy existent histos for each var to compare. It opens each file_names and searches the corrispondent histo_names to compare, for each variable in vars_to_compare. file_names and histo_names should have the same dimensions.
  - HistoMaker::CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare) : the same of above, with ratio plots
  - HistoMaker::CompareRatios_fromFile(file_names, histo_names, comparison_value, vars_to_compare) :the same of above, but in the bottom plots with bin-by-bin pulls with respect to a comparison_value. Also chi2 is calculated and insert in the bottom pad as a legend entry for each curve.
  - HistoMaker::CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare, histo_signals, histo_bkg) : comparisons as above, with bin-by-bin significance (S/radB) with indicated  histo_signals and  histo_bkg.

  - HistoMaker::Draw2DHistos(var_name1, var_name2, sel_names, chain_tags) : this methos creates 2D histograms for each couple of var_name1 and var_name2, for each selection and tag indicated in the "sel_names" and "chain_tags" vectors
  - HistoMaker::DrawRatio2D(sel_names_num, sel_names_den, var_name1, var_name2, chain_tags) : this method requires alredy existent histograms to divide with selections equal to those indicated in the vectors sel_names_num and sel_names_den, for each couple of var_name1 and var_name2, for each tag in chain_tags.
  - HistoMaker::DrawPulls_2D(var_name1, var_name2, sel_names_num, sel_names_den, chain_tags, comparison_value) : this method creates pulls wrt a comparison_value in the 2D plane for each couple of var_name1 and var_name2, for each selection and tag indicated in the "sel_names" and "chain_tags" vectors



Info files with raw, yields and data/MC ratio are created for each variable present in the config file.
Root files with histos, stacks and ratios are created for each variable present in the config file.

                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The class TriggerEfficiency manages the trigger efficiency tools:

  - TriggerEfficiency::Draw_Efficiency_Curves(sel_names_num_for_trigger , sel_names_den_for_trigger , var_names_for_trigger , chain_tags_for_trigger) :
    									 this method produces efficiency curves with respect to "sel_names_den_for_trigger" selections, for each variable in the "var_names_for_trigger" vector. For a given variable, efficiencies curves for each selection in the "sel_names_num_for_trigger" vector and for each chain in the "chain_tags_for_trigger" vector are superimposed.

  - TriggerEfficiency::Draw_Efficiency_Maps(sel_names_num_for_trigger , sel_names_den_for_trigger, var_names1, var_names2, chain_tags_for_trigger) :
    								       this method produced 2d efficiency maps with respect to "sel_names_den_for_trigger" selections, for each var_names1-var_names2 pair, for each variable selection in the "sel_names_num_for_trigger" vector and for each chain in the "chain_tags_for_trigger" vector.
    
Global efficiencies are also printed on screen.

                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The class Statistics allows to calculate efficiencies and significances, varying the selections to apply
    
    
  -Statistics::DrawEfficiencies(selections, selection_den, variable, samples) : with this method you create efficiency curves, calculated on the "variable" chosen, for each sample in "samples". The efficiencies are calculated as a function of "selections", with respect to "selection_den". All the samples curves are superimposed on the same canvas.
  
  -Statistics::DrawEfficiencies(selection_den, variable) : with this method you create efficiency curves, calculated on the "variable" chosen, for each sample in the config file. The effici\
encies are calculated as a function of the selections in the config file, with respect to "selection_den". All the samples curves are superimposed on the same canvas.

   For these last two methods ---> efficiency is also drawn as 2D plot with the signal name on the Y axis and the selections names on the X axis.

  -Statistics::DrawEfficiencies_SignalMap(selection_num, selection_den, variable) : this method calculate a grid of efficiency (selection num/selection_den) for mY vs mX/mY (truth masses) on the Y and X axes, respectively; chains are taken from config.

  This methods create a txt file with efficiencies information.
  A root files with all the efficiency curves is also created


  -Statistics::DrawSignificancies(selections, variables_for_sig, samples) : with this method you create significancies curves varying selections, for each variable in "variables for sig", for each signal in "samples" vector. 
  Two canvas are created: one with S/Radq(B) significance and another with asymptotic significance.
  If the flag for the significance with systematics is set to "true", also another canvas with asymptotic significance with systematics is created.

  -Statistics::DrawSignificancies_fromConfig(variables_for_sig) : with this method you create significancies curves, varying selections indicated in the config file, for each variable in "variables for sig", for each signal in the config file.
  Two canvas are created: one with S/Radq(B) significance and another with asymptotic significance.
  If the flag for the significance with systematics is set to "true", also another canvas with asymptotic significance with systematics is created.

  This methods create a txt file with significancies information.  A root files with all the significanc curves is also created.	


  -Statistics::XSectionsUpperLimits(selections, variables_for_sig, samples, N_sigma) : with this method you create xsection upper limits, corresponding to an asymptotic significance equal to N_sigma. For each selection in selections and for each variable in variables_for_sig, curves varying samples are created. 
  !!selections and variables_for_sig must have the same dimension!!
 The method creates a pdf with curves at different variables superimposed, for each selection. The method als creates a root file per each selection, with all the curves and a txt file for each selection with xsection upper limits number for each sample.


  -Statistics::XSectionsUpperLimits_Combined(sels_to_combine, vars_for_sels_to_combine, samples, N_sigma) : with this method you create xsection upper limits, corresponding to a combined asymptotic significance equal to N_sigma. For each selection vector in "sels_to_combine", on  each variable vector in variables_for_sig, combined significance curves varying samples are created.
  The combined significance is calculated as the square root of the sum of each significance^2 in vectors in "sels_to_combine".
  !! sels_to_combine, vars_for_sels_to_combine must have exactly the same dimensions !!
  

FOR TRIGGER, SIGNIFICANCE AND EFFICIENCY STUDIES, ROOT FILES WITH HISTOS MUST ALREDY EXIST!!!!! --> TriggerEfficiency and Statistics class take histos from root files. IF FILES OR HISTOS DO NOT EXIST, THIS WILL CAUS A SEGMENTATION VIOLATION!!



