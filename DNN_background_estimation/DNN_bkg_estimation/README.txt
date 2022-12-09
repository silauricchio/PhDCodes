THIS IS A README FILE FOR DNN TRAINING, TEST AND INTEGRATION INSIDE THE ANALYSIS FRAMEWORK FOR THE BACKGROUND ESTIMATION IN YXH FULL HADRONIC ANALYSIS

***** STEPS in a nutshell *******

------------------------------------------------------------------------------
1) Create skimmed trees, needed as a starting point to create pandas dataframe:
   python SkimTree.py (using pyROOT, you don't need additional settings)
   
   (for training dataframe, don't split the output in mini trees. See 6)

------------------------------------------------------------------------------
2) Install the anaconda environment with the command: 
   conda env create -f DNN_environment.yml

   -- activate the new environment:
   conda activate condaEnv

------------------------------------------------------------------------------
3) Prepare pandas dataframes for training and test:

   python NN_dataset.py 

   -- pay attention to the higgs mass window (set true to the needed one in NN_dataset.py):
      Nominal: mH in [145, 175] GeV
      Systematic: mH in [165, 200] GeV

------------------------------------------------------------------------------
4) Training procedures with bootstrap:
   The set of weights used for the Nominal background estimation is obtained with the bootstrap procedure

   -- Launch the bash script to start N trainings (N=100 in the analysis) (it takes several hours)
   source Launch_Ntrains_Bootstrap.sh 

   -- Perform checks at the end of bootstrap trainings to delete empty folders
   python check_Ntrains_Bootstrap.py 

------------------------------------------------------------------------------
5) Training for the systematic related to the training region choice:

   python NN_train_test.py (set Save_json_model=True if you need to save the model to decorate ntuples within the reader framework (4*)) 

   -- use python NN_ModelsComparison.py to compare reweighting with different models



------------------------------------------------------------------------------
6) Decorate trees with bootstrap:
   (Tipically done on Ntuples containing DNNScore for systematics (see 5) and Anomaly Score (AS) yet, but this is not mandatory)

   -- skim trees, if they aren't already skimmed (run outside conda environment, after ROOT setup, like in 1)
   python SkimTree.py (trees with AS are too big, we need to split the output in mini trees)

   -- create dataframe of the full tree (repeat this for each mini tree you have created)
   python Create_fulltree_DF.py 

   -- read the N training over dataframe and create trees with nominal scores and bootstrap variations (repeat this for each mini dataframe you have created)
   python Add_BootstrapScore_ToTree.py 

   -- eventually, hadd the output trees


------------------------------------------------------------------------------
(4*) Save model inside reader:
     -- We decorate data Ntuples with the weights for systematics variations (related to training region choice) using the lwtnn within the reader framework 
     -- the trained network is read, event by event, directly when Ntuples are produced

     #How do I convert the json model to a format for the lwtnn tool? (only used for training region systematics)

     Follow the instructions on lwtnn Github page : https://github.com/lwtnn/lwtnn
     NOTE: you need to install lwtnn only for the conversion of the model, not for running lwtnn inside the reader. For the latter purpose, an external link to lwtnn has been added inside the CMakeLists.txt of CxAODReader_YXH framework.

     1) Clone the project from Github:
     	git clone git@github.com:lwtnn/lwtnn.git

     2) Build with cmake:
     	mkdir build
   	cd build 
   	cmake -DBUILTIN_BOOST=true -DBUILTIN_EIGEN=true ..
   	make -j 4

     3) To run the tests first install h5py in a Python 3 environment, e.g. using pip
     	python -m pip install -r tests/requirements.txt
   
	 Starting from the directory where you built the project, run:	
            ./tests/test-GRU.sh

     4) After building, there are some required steps:
     	(The following instructions apply to the model/functional API in Keras. To see the instructions relevant to the sequential API, go to Quick Start With sequential API (on the Github wiki).)
   
	4.1)Make sure you saved your architecture and weights file from Keras, and created your input variable file (https://github.com/lwtnn/lwtnn/wiki/Keras-Converter):
   		You can generate a template file by calling:  
   		kerasfunc2json.py architecture.json weights.h5 > variables.json

		This gives you a file to modify with variables names, offset and scale for each variable scaling.
   		When lwtnn runs, it will apply the transformation f(x) = (x + offset) * scale.  
   		(
			relations with sklearn quantity:
			(lwtnn) offset = (sklearn) -scaler.mean_ 
			(lwtnn) scale = (sklearn) 1/scaler.scale_
  		 )

        4.2) Then save your network output file. Run:
      	     lwtnn/converters/kerasfunc2json.py architecture.json weights.h5 inputs.json > neural_net.json
   	      --> this output file will be the one linked in the config file of the reader framework 

------------------------------------------------------------------------------




