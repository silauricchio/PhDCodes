import ROOT
import glob
import os

from ROOT import TLorentzVector

ROOT.ROOT.EnableImplicitMT()


NtuplesPath = "/eos/atlas/atlascerngroupdisk/phys-hdbs/dbl/YXH/superNtuples_v1.9/mc16a/*.root" #input path
OutPathDir = "/eos/user/s/sauricch/ReaderOutput/SuperNtuples_v1.9_skimmed/mc16a/" #output path


if not os.path.exists(OutPathDir):
    os.makedirs(OutPathDir)

variables = ["runNumber", "eventNumber", "DNNScore", "isX_smalljets", "weight", "mVH","mH","pTH","etaH","phiH","EH", "mV", "mVHres", "mVres", "pTV","etaV","phiV","EV", "HCand_isJet1","HCand_XbbScore", "Jet1_pT", "Jet2_pT", "Jet1_M", "Jet2_M", "Jet1_XbbScore_V3","Jet2_XbbScore_V3", "VCand_XbbScore","d2TracksV","ntrkVVJJ_H",  "trackjet1_pt", "trackjet2_pt", "trackjet1_eta", "trackjet1_phi", "trackjet2_eta", "trackjet2_phi", "trackjet1_m", "trackjet2_m", "trackjet1_btag_score", "trackJetPtAsym", "trackjet1_btagged", "nTrackJetsH", "HCand_nBTags", "nLargeRJets", "SmallJet1_X_pT", "SmallJet2_X_pT", "SmallJet1_X_eta", "SmallJet2_X_eta", "SmallJet1_X_E", "SmallJet2_X_E", "SmallJet1_X_M", "SmallJet1_X_M", "SmallJet1_X_phi", "SmallJet2_X_phi", "SmallJet_X_pT_imb", "SmallJet_X_deltaY", "jet1_AnomScore", "jet2_AnomScore"]

    
for samplePath in glob.glob('{NtuplesPath}'.format(NtuplesPath=NtuplesPath)): 
    print samplePath

    branchList = ROOT.vector('string')()
    for i in variables: branchList.push_back(i) 

    if ((os.path.getsize(samplePath)/(1024*1024)) < 1): 
        continue
    if 'merged_top2' not in samplePath:
        continue
    
    sampleName = samplePath.split('/')[-1].replace('merged_top2_','').replace('_VRNN_mc16a', '')
    
    outFileName = OutPathDir + sampleName
    d = ROOT.RDataFrame("Nominal", samplePath)
    
    d=d.Filter("HCand_XbbScore>(-2)")

    #the following trick is for big files                                                                                                                                                                                 

    nEntries = int(d.Count())
    divisions = 3 #number of mini trees
    nEntries_batch = int(nEntries/divisions)
    nEntries_div = 0

    print('Total entries ', nEntries)
    print('N batches ', divisions)


    for div in range(divisions):
        previous_nEntries_div = nEntries_div
        nEntries_div = nEntries_div + nEntries_batch

        print('current batch ', nEntries_div)

        sampleName_div = sampleName.replace('.root', '') + '_' + str(div) + '.root'
        outFileName_div = OutPathDir + sampleName_div

        d_cut=d.Range(previous_nEntries_div, nEntries_div)
        d_cut.Snapshot("Nominal",outFileName_div, branchList)


    #if you don't want to split the output in mini trees, comment above and uncomment below:
    #d.Snapshot("Nominal",outFileName, branchList) 

