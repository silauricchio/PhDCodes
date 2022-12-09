//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Oct 28 15:22:54 2020 by ROOT version 6.18/04
// from TTree Nominal/Nominal
// found on file: /eos/user/f/fcirotto/yxh/ReaderOutput/mc16a_07Sep20/MergedTrees/data.root
//////////////////////////////////////////////////////////

#ifndef Reweight2D_h
#define Reweight2D_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "Debug.h"

// Header file for the classes stored in the TTree if any.
#include "vector"

class Reweight2D {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   
   TString tree_path_ = "";
   TString tree_name_ = "";
   TString path_histo_ = "";
   TString name_histo1_ = "";
   TString name_histo2_ = "";
   TString var_name1X_ = "";
   TString var_name1Y_ = "";
   TString var_name2X_ = "";
   TString var_name2Y_ = "";


// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          isH;
   Bool_t          isW;
   Bool_t          isZ;
   Bool_t          isCR;
   Bool_t          isMC;
   Bool_t          isMH;
   Bool_t          isMW;
   Bool_t          isMZ;
   Bool_t          isSR;
   Bool_t          isHd2;
   Bool_t          isWd2;
   Bool_t          isZd2;
   Bool_t          isSRWH;
   Bool_t          isSRZH;
   Bool_t          isVR1A;
   Bool_t          isVR1B;
   Bool_t          isVR2A;
   Bool_t          isVR2B;
   Bool_t          isHnTrk;
   Bool_t          isWnTrk;
   Bool_t          isZnTrk;
   Bool_t          isWnTrkVVJJ;
   Bool_t          isZnTrkVVJJ;
   Bool_t          isX_smalljets;
   Bool_t          passYXHPreSel;
   Bool_t          Jet1_isHWindow;
   Bool_t          Jet2_isHWindow;
   Bool_t          Jet3_isHWindow;
   Bool_t          passLeptonVeto;
   Bool_t          HCand_isHWindow;
   Bool_t          VCand_isHWindow;
   Bool_t          trackjet1_btagged;
   Bool_t          trackjet2_btagged;
   Int_t           nTag;
   Int_t           nMuons;
   Int_t           nElectrons;
   Int_t           Jet1_nBTags;
   Int_t           Jet2_nBTags;
   Int_t           Jet3_nBTags;
   Int_t           nLargeRJets;
   Int_t           nSmallRJets;
   Int_t           nTrackJetsH;
   Int_t           HCand_nBTags;
   Int_t           VCand_nBTags;
   Int_t           Jet1_nTrackJets;
   Int_t           Jet2_nTrackJets;
   Int_t           Jet3_nTrackJets;
   Int_t           nTrackJetsEvent;
   Int_t           HCand_nTrackJets;
   Int_t           VCand_nTrackJets;
   Int_t           nCentralSmallRJets;
   Int_t           nForwardSmallRJets;
   UInt_t          runNumber;
   ULong64_t       eventNumber;
   Float_t         mu;
   Float_t         d2H;
   Float_t         d2V;
   Float_t         etaH;
   Float_t         etaV;
   Float_t         phiH;
   Float_t         phiV;
   Float_t         HdRjj;
   Float_t         etaVH;
   Float_t         phiVH;
   Float_t         mu_avg;
   Float_t         HdEtajj;
   Float_t         Jet1_D2;
   Float_t         Jet2_D2;
   Float_t         Jet3_D2;
   Float_t         mu_scaled;
   Float_t         pu_weight;
   Float_t         ntrkVVJJ_H;
   Float_t         ntrkVVJJ_V;
   Float_t         trackRatio1;
   Float_t         trackRatio2;
   Float_t         Jet1_XbbScore;
   Float_t         Jet2_XbbScore;
   Float_t         mu_avg_scaled;
   Float_t         HCand_XbbScore;
   Float_t         VCand_XbbScore;
   Float_t         trackJetPtAsym;
   Float_t         Jet1_XbbScoreQCD;
   Float_t         Jet1_XbbScoreTop;
   Float_t         Jet2_XbbScoreQCD;
   Float_t         Jet2_XbbScoreTop;
   Float_t         HCand_XbbScoreQCD;
   Float_t         HCand_XbbScoreTop;
   Float_t         VCand_XbbScoreQCD;
   Float_t         VCand_XbbScoreTop;
   Float_t         Jet1_XbbScoreHiggs;
   Float_t         Jet2_XbbScoreHiggs;
   Float_t         trackjet1_quantile;
   Float_t         trackjet1_radiusVR;
   Float_t         trackjet2_quantile;
   Float_t         trackjet2_radiusVR;
   Float_t         HCand_XbbScoreHiggs;
   Float_t         VCand_XbbScoreHiggs;
   Float_t         trackjet1_btag_score;
   Float_t         trackjet2_btag_score;
   Double_t        EH;
   Double_t        EV;
   Double_t        mH;
   Double_t        mV;
   Double_t        mVH;
   Double_t        pTH;
   Double_t        pTV;
   Double_t        dyVH;
   Double_t        pTVH;
   Double_t        mVres;
   Double_t        Jet1_E;
   Double_t        Jet1_M;
   Double_t        Jet2_E;
   Double_t        Jet2_M;
   Double_t        Jet3_E;
   Double_t        Jet3_M;
   Double_t        mVHres;
   Double_t        weight;
   Double_t        Jet1_pT;
   Double_t        Jet2_pT;
   Double_t        Jet3_pT;
   Double_t        Jet1_eta;
   Double_t        Jet1_phi;
   Double_t        Jet2_eta;
   Double_t        Jet2_phi;
   Double_t        Jet3_eta;
   Double_t        Jet3_phi;
   Double_t        mH_uncalib;
   Double_t        mV_uncalib;
   Double_t        btag_weight;
   Double_t        trackjet1_m;
   Double_t        trackjet2_m;
   Double_t        trackjet1_pt;
   Double_t        trackjet2_pt;
   Double_t        SmallJet1_X_E;
   Double_t        SmallJet1_X_M;
   Double_t        SmallJet2_X_E;
   Double_t        SmallJet2_X_M;
   Double_t        trackjet1_eta;
   Double_t        trackjet1_phi;
   Double_t        trackjet2_eta;
   Double_t        trackjet2_phi;
   Double_t        SmallJet1_X_pT;
   Double_t        SmallJet2_X_pT;
   Double_t        SmallJet1_X_eta;
   Double_t        SmallJet1_X_phi;
   Double_t        SmallJet2_X_eta;
   Double_t        SmallJet2_X_phi;
   vector<double>  *SmallRJets_E;
   vector<double>  *SmallRJets_M;
   vector<double>  *SmallRJets_pT;
   vector<double>  *SmallRJets_eta;
   vector<double>  *SmallRJets_phi;

   // List of branches
   TBranch        *b_isH;   //!
   TBranch        *b_isW;   //!
   TBranch        *b_isZ;   //!
   TBranch        *b_isCR;   //!
   TBranch        *b_isMC;   //!
   TBranch        *b_isMH;   //!
   TBranch        *b_isMW;   //!
   TBranch        *b_isMZ;   //!
   TBranch        *b_isSR;   //!
   TBranch        *b_isHd2;   //!
   TBranch        *b_isWd2;   //!
   TBranch        *b_isZd2;   //!
   TBranch        *b_isSRWH;   //!
   TBranch        *b_isSRZH;   //!
   TBranch        *b_isVR1A;   //!
   TBranch        *b_isVR1B;   //!
   TBranch        *b_isVR2A;   //!
   TBranch        *b_isVR2B;   //!
   TBranch        *b_isHnTrk;   //!
   TBranch        *b_isWnTrk;   //!
   TBranch        *b_isZnTrk;   //!
   TBranch        *b_isWnTrkVVJJ;   //!
   TBranch        *b_isZnTrkVVJJ;   //!
   TBranch        *b_isX_smalljets;   //!
   TBranch        *b_passYXHPreSel;   //!
   TBranch        *b_Jet1_isHWindow;   //!
   TBranch        *b_Jet2_isHWindow;   //!
   TBranch        *b_Jet3_isHWindow;   //!
   TBranch        *b_passLeptonVeto;   //!
   TBranch        *b_HCand_isHWindow;   //!
   TBranch        *b_VCand_isHWindow;   //!
   TBranch        *b_trackjet1_btagged;   //!
   TBranch        *b_trackjet2_btagged;   //!
   TBranch        *b_nTag;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_Jet1_nBTags;   //!
   TBranch        *b_Jet2_nBTags;   //!
   TBranch        *b_Jet3_nBTags;   //!
   TBranch        *b_nLargeRJets;   //!
   TBranch        *b_nSmallRJets;   //!
   TBranch        *b_nTrackJetsH;   //!
   TBranch        *b_HCand_nBTags;   //!
   TBranch        *b_VCand_nBTags;   //!
   TBranch        *b_Jet1_nTrackJets;   //!
   TBranch        *b_Jet2_nTrackJets;   //!
   TBranch        *b_Jet3_nTrackJets;   //!
   TBranch        *b_nTrackJetsEvent;   //!
   TBranch        *b_HCand_nTrackJets;   //!
   TBranch        *b_VCand_nTrackJets;   //!
   TBranch        *b_nCentralSmallRJets;   //!
   TBranch        *b_nForwardSmallRJets;   //!
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_mu;   //!
   TBranch        *b_d2H;   //!
   TBranch        *b_d2V;   //!
   TBranch        *b_etaH;   //!
   TBranch        *b_etaV;   //!
   TBranch        *b_phiH;   //!
   TBranch        *b_phiV;   //!
   TBranch        *b_HdRjj;   //!
   TBranch        *b_etaVH;   //!
   TBranch        *b_phiVH;   //!
   TBranch        *b_mu_avg;   //!
   TBranch        *b_HdEtajj;   //!
   TBranch        *b_Jet1_D2;   //!
   TBranch        *b_Jet2_D2;   //!
   TBranch        *b_Jet3_D2;   //!
   TBranch        *b_mu_scaled;   //!
   TBranch        *b_pu_weight;   //!
   TBranch        *b_ntrkVVJJ_H;   //!
   TBranch        *b_ntrkVVJJ_V;   //!
   TBranch        *b_trackRatio1;   //!
   TBranch        *b_trackRatio2;   //!
   TBranch        *b_Jet1_XbbScore;   //!
   TBranch        *b_Jet2_XbbScore;   //!
   TBranch        *b_mu_avg_scaled;   //!
   TBranch        *b_HCand_XbbScore;   //!
   TBranch        *b_VCand_XbbScore;   //!
   TBranch        *b_trackJetPtAsym;   //!
   TBranch        *b_Jet1_XbbScoreQCD;   //!
   TBranch        *b_Jet1_XbbScoreTop;   //!
   TBranch        *b_Jet2_XbbScoreQCD;   //!
   TBranch        *b_Jet2_XbbScoreTop;   //!
   TBranch        *b_HCand_XbbScoreQCD;   //!
   TBranch        *b_HCand_XbbScoreTop;   //!
   TBranch        *b_VCand_XbbScoreQCD;   //!
   TBranch        *b_VCand_XbbScoreTop;   //!
   TBranch        *b_Jet1_XbbScoreHiggs;   //!
   TBranch        *b_Jet2_XbbScoreHiggs;   //!
   TBranch        *b_trackjet1_quantile;   //!
   TBranch        *b_trackjet1_radiusVR;   //!
   TBranch        *b_trackjet2_quantile;   //!
   TBranch        *b_trackjet2_radiusVR;   //!
   TBranch        *b_HCand_XbbScoreHiggs;   //!
   TBranch        *b_VCand_XbbScoreHiggs;   //!
   TBranch        *b_trackjet1_btag_score;   //!
   TBranch        *b_trackjet2_btag_score;   //!
   TBranch        *b_EH;   //!
   TBranch        *b_EV;   //!
   TBranch        *b_mH;   //!
   TBranch        *b_mV;   //!
   TBranch        *b_mVH;   //!
   TBranch        *b_pTH;   //!
   TBranch        *b_pTV;   //!
   TBranch        *b_dyVH;   //!
   TBranch        *b_pTVH;   //!
   TBranch        *b_mVres;   //!
   TBranch        *b_Jet1_E;   //!
   TBranch        *b_Jet1_M;   //!
   TBranch        *b_Jet2_E;   //!
   TBranch        *b_Jet2_M;   //!
   TBranch        *b_Jet3_E;   //!
   TBranch        *b_Jet3_M;   //!
   TBranch        *b_mVHres;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_Jet1_pT;   //!
   TBranch        *b_Jet2_pT;   //!
   TBranch        *b_Jet3_pT;   //!
   TBranch        *b_Jet1_eta;   //!
   TBranch        *b_Jet1_phi;   //!
   TBranch        *b_Jet2_eta;   //!
   TBranch        *b_Jet2_phi;   //!
   TBranch        *b_Jet3_eta;   //!
   TBranch        *b_Jet3_phi;   //!
   TBranch        *b_mH_uncalib;   //!
   TBranch        *b_mV_uncalib;   //!
   TBranch        *b_btag_weight;   //!
   TBranch        *b_trackjet1_m;   //!
   TBranch        *b_trackjet2_m;   //!
   TBranch        *b_trackjet1_pt;   //!
   TBranch        *b_trackjet2_pt;   //!
   TBranch        *b_SmallJet1_X_E;   //!
   TBranch        *b_SmallJet1_X_M;   //!
   TBranch        *b_SmallJet2_X_E;   //!
   TBranch        *b_SmallJet2_X_M;   //!
   TBranch        *b_trackjet1_eta;   //!
   TBranch        *b_trackjet1_phi;   //!
   TBranch        *b_trackjet2_eta;   //!
   TBranch        *b_trackjet2_phi;   //!
   TBranch        *b_SmallJet1_X_pT;   //!
   TBranch        *b_SmallJet2_X_pT;   //!
   TBranch        *b_SmallJet1_X_eta;   //!
   TBranch        *b_SmallJet1_X_phi;   //!
   TBranch        *b_SmallJet2_X_eta;   //!
   TBranch        *b_SmallJet2_X_phi;   //!
   TBranch        *b_SmallRJets_E;   //!
   TBranch        *b_SmallRJets_M;   //!
   TBranch        *b_SmallRJets_pT;   //!
   TBranch        *b_SmallRJets_eta;   //!
   TBranch        *b_SmallRJets_phi;   //!

   Reweight2D(TTree *tree=0, TString tree_path="",  TString tree_name="", TString path_histo="", TString histo_name1="", TString histo_name2="", TString var_name1X="", TString var_name1Y="", TString var_name2X="", TString var_name2Y="");
   virtual ~Reweight2D();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   
   void  Set_histo_path(TString path_histo)
   {
     path_histo_ = path_histo;
   }

   void  Set_histo_name1(TString histo_name)
   {
     name_histo1_ =  histo_name;
   }
   void  Set_histo_name2(TString histo_name)
   {
     name_histo2_ =  histo_name;
   }
   void     Set_var_name1X(TString var_name1X){var_name1X_ = var_name1X;}
   void     Set_var_name1Y(TString var_name1Y){var_name1Y_ = var_name1Y;}
   void     Set_var_name2X(TString var_name2X){var_name2X_ = var_name2X;}
   void     Set_var_name2Y(TString var_name2Y){var_name2Y_ = var_name2Y;}
   void     Set_tree_name(TString tree_name){tree_name_ = tree_name;}
   void     Set_tree_path(TString tree_path){tree_path_ = tree_path;}

};
#endif

#ifdef Reweight2D_cxx
Reweight2D::Reweight2D(TTree *tree, TString tree_path="",  TString tree_name="", TString path_histo="", TString histo_name1="", TString histo_name2="", TString var_name1X="", TString var_name1Y="", TString var_name2X="", TString var_name2Y="") : fChain(0) 

{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  
  Set_tree_path(tree_path);
  
  if (tree == 0) 
    {
    TString tree_file = tree_path + tree_name + ".root";
    
    TFile *f = TFile::Open(tree_file);
    if (!f || !f->IsOpen()) 
      {
	ERROR("ROOT file "<< tree_file <<" not found!!");
	exit(EXIT_FAILURE);
      }
    f->GetObject("Nominal",tree);   
    }
Init(tree);
 
 Set_histo_path(path_histo);
 Set_histo_name1(histo_name1);
 Set_histo_name2(histo_name2);
 Set_var_name1X(var_name1X);
 Set_var_name1Y(var_name1Y);
 Set_var_name2X(var_name2X);
 Set_var_name2Y(var_name2Y);
 Set_tree_name(tree_name);
 
 Loop();
}

Reweight2D::~Reweight2D()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Reweight2D::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Reweight2D::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Reweight2D::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SmallRJets_E = 0;
   SmallRJets_M = 0;
   SmallRJets_pT = 0;
   SmallRJets_eta = 0;
   SmallRJets_phi = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("isH", &isH, &b_isH);
   fChain->SetBranchAddress("isW", &isW, &b_isW);
   fChain->SetBranchAddress("isZ", &isZ, &b_isZ);
   fChain->SetBranchAddress("isCR", &isCR, &b_isCR);
   fChain->SetBranchAddress("isMC", &isMC, &b_isMC);
   fChain->SetBranchAddress("isMH", &isMH, &b_isMH);
   fChain->SetBranchAddress("isMW", &isMW, &b_isMW);
   fChain->SetBranchAddress("isMZ", &isMZ, &b_isMZ);
   fChain->SetBranchAddress("isSR", &isSR, &b_isSR);
   fChain->SetBranchAddress("isHd2", &isHd2, &b_isHd2);
   fChain->SetBranchAddress("isWd2", &isWd2, &b_isWd2);
   fChain->SetBranchAddress("isZd2", &isZd2, &b_isZd2);
   fChain->SetBranchAddress("isSRWH", &isSRWH, &b_isSRWH);
   fChain->SetBranchAddress("isSRZH", &isSRZH, &b_isSRZH);
   fChain->SetBranchAddress("isVR1A", &isVR1A, &b_isVR1A);
   fChain->SetBranchAddress("isVR1B", &isVR1B, &b_isVR1B);
   fChain->SetBranchAddress("isVR2A", &isVR2A, &b_isVR2A);
   fChain->SetBranchAddress("isVR2B", &isVR2B, &b_isVR2B);
   fChain->SetBranchAddress("isHnTrk", &isHnTrk, &b_isHnTrk);
   fChain->SetBranchAddress("isWnTrk", &isWnTrk, &b_isWnTrk);
   fChain->SetBranchAddress("isZnTrk", &isZnTrk, &b_isZnTrk);
   fChain->SetBranchAddress("isWnTrkVVJJ", &isWnTrkVVJJ, &b_isWnTrkVVJJ);
   fChain->SetBranchAddress("isZnTrkVVJJ", &isZnTrkVVJJ, &b_isZnTrkVVJJ);
   fChain->SetBranchAddress("isX_smalljets", &isX_smalljets, &b_isX_smalljets);
   fChain->SetBranchAddress("passYXHPreSel", &passYXHPreSel, &b_passYXHPreSel);
   fChain->SetBranchAddress("Jet1_isHWindow", &Jet1_isHWindow, &b_Jet1_isHWindow);
   fChain->SetBranchAddress("Jet2_isHWindow", &Jet2_isHWindow, &b_Jet2_isHWindow);
   fChain->SetBranchAddress("Jet3_isHWindow", &Jet3_isHWindow, &b_Jet3_isHWindow);
   fChain->SetBranchAddress("passLeptonVeto", &passLeptonVeto, &b_passLeptonVeto);
   fChain->SetBranchAddress("HCand_isHWindow", &HCand_isHWindow, &b_HCand_isHWindow);
   fChain->SetBranchAddress("VCand_isHWindow", &VCand_isHWindow, &b_VCand_isHWindow);
   fChain->SetBranchAddress("trackjet1_btagged", &trackjet1_btagged, &b_trackjet1_btagged);
   fChain->SetBranchAddress("trackjet2_btagged", &trackjet2_btagged, &b_trackjet2_btagged);
   fChain->SetBranchAddress("nTag", &nTag, &b_nTag);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("Jet1_nBTags", &Jet1_nBTags, &b_Jet1_nBTags);
   fChain->SetBranchAddress("Jet2_nBTags", &Jet2_nBTags, &b_Jet2_nBTags);
   fChain->SetBranchAddress("Jet3_nBTags", &Jet3_nBTags, &b_Jet3_nBTags);
   fChain->SetBranchAddress("nLargeRJets", &nLargeRJets, &b_nLargeRJets);
   fChain->SetBranchAddress("nSmallRJets", &nSmallRJets, &b_nSmallRJets);
   fChain->SetBranchAddress("nTrackJetsH", &nTrackJetsH, &b_nTrackJetsH);
   fChain->SetBranchAddress("HCand_nBTags", &HCand_nBTags, &b_HCand_nBTags);
   fChain->SetBranchAddress("VCand_nBTags", &VCand_nBTags, &b_VCand_nBTags);
   fChain->SetBranchAddress("Jet1_nTrackJets", &Jet1_nTrackJets, &b_Jet1_nTrackJets);
   fChain->SetBranchAddress("Jet2_nTrackJets", &Jet2_nTrackJets, &b_Jet2_nTrackJets);
   fChain->SetBranchAddress("Jet3_nTrackJets", &Jet3_nTrackJets, &b_Jet3_nTrackJets);
   fChain->SetBranchAddress("nTrackJetsEvent", &nTrackJetsEvent, &b_nTrackJetsEvent);
   fChain->SetBranchAddress("HCand_nTrackJets", &HCand_nTrackJets, &b_HCand_nTrackJets);
   fChain->SetBranchAddress("VCand_nTrackJets", &VCand_nTrackJets, &b_VCand_nTrackJets);
   fChain->SetBranchAddress("nCentralSmallRJets", &nCentralSmallRJets, &b_nCentralSmallRJets);
   fChain->SetBranchAddress("nForwardSmallRJets", &nForwardSmallRJets, &b_nForwardSmallRJets);
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("mu", &mu, &b_mu);
   fChain->SetBranchAddress("d2H", &d2H, &b_d2H);
   fChain->SetBranchAddress("d2V", &d2V, &b_d2V);
   fChain->SetBranchAddress("etaH", &etaH, &b_etaH);
   fChain->SetBranchAddress("etaV", &etaV, &b_etaV);
   fChain->SetBranchAddress("phiH", &phiH, &b_phiH);
   fChain->SetBranchAddress("phiV", &phiV, &b_phiV);
   fChain->SetBranchAddress("HdRjj", &HdRjj, &b_HdRjj);
   fChain->SetBranchAddress("etaVH", &etaVH, &b_etaVH);
   fChain->SetBranchAddress("phiVH", &phiVH, &b_phiVH);
   fChain->SetBranchAddress("mu_avg", &mu_avg, &b_mu_avg);
   fChain->SetBranchAddress("HdEtajj", &HdEtajj, &b_HdEtajj);
   fChain->SetBranchAddress("Jet1_D2", &Jet1_D2, &b_Jet1_D2);
   fChain->SetBranchAddress("Jet2_D2", &Jet2_D2, &b_Jet2_D2);
   fChain->SetBranchAddress("Jet3_D2", &Jet3_D2, &b_Jet3_D2);
   fChain->SetBranchAddress("mu_scaled", &mu_scaled, &b_mu_scaled);
   fChain->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   fChain->SetBranchAddress("ntrkVVJJ_H", &ntrkVVJJ_H, &b_ntrkVVJJ_H);
   fChain->SetBranchAddress("ntrkVVJJ_V", &ntrkVVJJ_V, &b_ntrkVVJJ_V);
   fChain->SetBranchAddress("trackRatio1", &trackRatio1, &b_trackRatio1);
   fChain->SetBranchAddress("trackRatio2", &trackRatio2, &b_trackRatio2);
   fChain->SetBranchAddress("Jet1_XbbScore", &Jet1_XbbScore, &b_Jet1_XbbScore);
   fChain->SetBranchAddress("Jet2_XbbScore", &Jet2_XbbScore, &b_Jet2_XbbScore);
   fChain->SetBranchAddress("mu_avg_scaled", &mu_avg_scaled, &b_mu_avg_scaled);
   fChain->SetBranchAddress("HCand_XbbScore", &HCand_XbbScore, &b_HCand_XbbScore);
   fChain->SetBranchAddress("VCand_XbbScore", &VCand_XbbScore, &b_VCand_XbbScore);
   fChain->SetBranchAddress("trackJetPtAsym", &trackJetPtAsym, &b_trackJetPtAsym);
   fChain->SetBranchAddress("Jet1_XbbScoreQCD", &Jet1_XbbScoreQCD, &b_Jet1_XbbScoreQCD);
   fChain->SetBranchAddress("Jet1_XbbScoreTop", &Jet1_XbbScoreTop, &b_Jet1_XbbScoreTop);
   fChain->SetBranchAddress("Jet2_XbbScoreQCD", &Jet2_XbbScoreQCD, &b_Jet2_XbbScoreQCD);
   fChain->SetBranchAddress("Jet2_XbbScoreTop", &Jet2_XbbScoreTop, &b_Jet2_XbbScoreTop);
   fChain->SetBranchAddress("HCand_XbbScoreQCD", &HCand_XbbScoreQCD, &b_HCand_XbbScoreQCD);
   fChain->SetBranchAddress("HCand_XbbScoreTop", &HCand_XbbScoreTop, &b_HCand_XbbScoreTop);
   fChain->SetBranchAddress("VCand_XbbScoreQCD", &VCand_XbbScoreQCD, &b_VCand_XbbScoreQCD);
   fChain->SetBranchAddress("VCand_XbbScoreTop", &VCand_XbbScoreTop, &b_VCand_XbbScoreTop);
   fChain->SetBranchAddress("Jet1_XbbScoreHiggs", &Jet1_XbbScoreHiggs, &b_Jet1_XbbScoreHiggs);
   fChain->SetBranchAddress("Jet2_XbbScoreHiggs", &Jet2_XbbScoreHiggs, &b_Jet2_XbbScoreHiggs);
   fChain->SetBranchAddress("trackjet1_quantile", &trackjet1_quantile, &b_trackjet1_quantile);
   fChain->SetBranchAddress("trackjet1_radiusVR", &trackjet1_radiusVR, &b_trackjet1_radiusVR);
   fChain->SetBranchAddress("trackjet2_quantile", &trackjet2_quantile, &b_trackjet2_quantile);
   fChain->SetBranchAddress("trackjet2_radiusVR", &trackjet2_radiusVR, &b_trackjet2_radiusVR);
   fChain->SetBranchAddress("HCand_XbbScoreHiggs", &HCand_XbbScoreHiggs, &b_HCand_XbbScoreHiggs);
   fChain->SetBranchAddress("VCand_XbbScoreHiggs", &VCand_XbbScoreHiggs, &b_VCand_XbbScoreHiggs);
   fChain->SetBranchAddress("trackjet1_btag_score", &trackjet1_btag_score, &b_trackjet1_btag_score);
   fChain->SetBranchAddress("trackjet2_btag_score", &trackjet2_btag_score, &b_trackjet2_btag_score);
   fChain->SetBranchAddress("EH", &EH, &b_EH);
   fChain->SetBranchAddress("EV", &EV, &b_EV);
   fChain->SetBranchAddress("mH", &mH, &b_mH);
   fChain->SetBranchAddress("mV", &mV, &b_mV);
   fChain->SetBranchAddress("mVH", &mVH, &b_mVH);
   fChain->SetBranchAddress("pTH", &pTH, &b_pTH);
   fChain->SetBranchAddress("pTV", &pTV, &b_pTV);
   fChain->SetBranchAddress("dyVH", &dyVH, &b_dyVH);
   fChain->SetBranchAddress("pTVH", &pTVH, &b_pTVH);
   fChain->SetBranchAddress("mVres", &mVres, &b_mVres);
   fChain->SetBranchAddress("Jet1_E", &Jet1_E, &b_Jet1_E);
   fChain->SetBranchAddress("Jet1_M", &Jet1_M, &b_Jet1_M);
   fChain->SetBranchAddress("Jet2_E", &Jet2_E, &b_Jet2_E);
   fChain->SetBranchAddress("Jet2_M", &Jet2_M, &b_Jet2_M);
   fChain->SetBranchAddress("Jet3_E", &Jet3_E, &b_Jet3_E);
   fChain->SetBranchAddress("Jet3_M", &Jet3_M, &b_Jet3_M);
   fChain->SetBranchAddress("mVHres", &mVHres, &b_mVHres);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("Jet1_pT", &Jet1_pT, &b_Jet1_pT);
   fChain->SetBranchAddress("Jet2_pT", &Jet2_pT, &b_Jet2_pT);
   fChain->SetBranchAddress("Jet3_pT", &Jet3_pT, &b_Jet3_pT);
   fChain->SetBranchAddress("Jet1_eta", &Jet1_eta, &b_Jet1_eta);
   fChain->SetBranchAddress("Jet1_phi", &Jet1_phi, &b_Jet1_phi);
   fChain->SetBranchAddress("Jet2_eta", &Jet2_eta, &b_Jet2_eta);
   fChain->SetBranchAddress("Jet2_phi", &Jet2_phi, &b_Jet2_phi);
   fChain->SetBranchAddress("Jet3_eta", &Jet3_eta, &b_Jet3_eta);
   fChain->SetBranchAddress("Jet3_phi", &Jet3_phi, &b_Jet3_phi);
   fChain->SetBranchAddress("mH_uncalib", &mH_uncalib, &b_mH_uncalib);
   fChain->SetBranchAddress("mV_uncalib", &mV_uncalib, &b_mV_uncalib);
   fChain->SetBranchAddress("btag_weight", &btag_weight, &b_btag_weight);
   fChain->SetBranchAddress("trackjet1_m", &trackjet1_m, &b_trackjet1_m);
   fChain->SetBranchAddress("trackjet2_m", &trackjet2_m, &b_trackjet2_m);
   fChain->SetBranchAddress("trackjet1_pt", &trackjet1_pt, &b_trackjet1_pt);
   fChain->SetBranchAddress("trackjet2_pt", &trackjet2_pt, &b_trackjet2_pt);
   fChain->SetBranchAddress("SmallJet1_X_E", &SmallJet1_X_E, &b_SmallJet1_X_E);
   fChain->SetBranchAddress("SmallJet1_X_M", &SmallJet1_X_M, &b_SmallJet1_X_M);
   fChain->SetBranchAddress("SmallJet2_X_E", &SmallJet2_X_E, &b_SmallJet2_X_E);
   fChain->SetBranchAddress("SmallJet2_X_M", &SmallJet2_X_M, &b_SmallJet2_X_M);
   fChain->SetBranchAddress("trackjet1_eta", &trackjet1_eta, &b_trackjet1_eta);
   fChain->SetBranchAddress("trackjet1_phi", &trackjet1_phi, &b_trackjet1_phi);
   fChain->SetBranchAddress("trackjet2_eta", &trackjet2_eta, &b_trackjet2_eta);
   fChain->SetBranchAddress("trackjet2_phi", &trackjet2_phi, &b_trackjet2_phi);
   fChain->SetBranchAddress("SmallJet1_X_pT", &SmallJet1_X_pT, &b_SmallJet1_X_pT);
   fChain->SetBranchAddress("SmallJet2_X_pT", &SmallJet2_X_pT, &b_SmallJet2_X_pT);
   fChain->SetBranchAddress("SmallJet1_X_eta", &SmallJet1_X_eta, &b_SmallJet1_X_eta);
   fChain->SetBranchAddress("SmallJet1_X_phi", &SmallJet1_X_phi, &b_SmallJet1_X_phi);
   fChain->SetBranchAddress("SmallJet2_X_eta", &SmallJet2_X_eta, &b_SmallJet2_X_eta);
   fChain->SetBranchAddress("SmallJet2_X_phi", &SmallJet2_X_phi, &b_SmallJet2_X_phi);
   fChain->SetBranchAddress("SmallRJets_E", &SmallRJets_E, &b_SmallRJets_E);
   fChain->SetBranchAddress("SmallRJets_M", &SmallRJets_M, &b_SmallRJets_M);
   fChain->SetBranchAddress("SmallRJets_pT", &SmallRJets_pT, &b_SmallRJets_pT);
   fChain->SetBranchAddress("SmallRJets_eta", &SmallRJets_eta, &b_SmallRJets_eta);
   fChain->SetBranchAddress("SmallRJets_phi", &SmallRJets_phi, &b_SmallRJets_phi);
   Notify();
}

Bool_t Reweight2D::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Reweight2D::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Reweight2D::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


#endif // #ifdef Reweight2D_cxx
