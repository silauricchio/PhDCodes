#define Reweight1D_cxx
#include "Reweight1D.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "Debug.h"

void Reweight1D::Loop()
{
   if (fChain == 0) return;

   TFile *f = TFile::Open(path_histo_);
   
   if(!f->IsOpen()) 
     {
       ERROR("Histogram path not found!! --> Exit");
       exit(EXIT_FAILURE);
     }
   

   TH1D *h = (TH1D*)f->Get(name_histo_);
   int nBins_h = h->GetNbinsX(); 

   Long64_t nentries = fChain->GetEntriesFast();

   double reweight;
   TTree* tree =(TTree*) (f->Get<TTree>("Nominal"))->Clone();

   Double_t var_value(-178.);
   auto variables = tree->GetListOfLeaves();
   for(auto variable : *variables)    if(var_name_  == ((TLeaf*)variable)->GetName()) var_value = ((TLeaf*)variable)->GetValue();
   if(var_value == -178.)
     {
       ERROR("Variable " << var_name_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }

   TBranch* newBranch = tree->Branch("reweight", &reweight, "reweight/LF");
   

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      reweight = h->GetBinContent(h->FindBin(var_value));
      newBranch->Fill();           
      
   }
   
   f->Close();

   //Store the new tree
   TFile *new_f = new TFile(tree_path_ + "/Reweighted_" + tree_name_ + ".root", "RECREATE"); 
   tree->Write();
   new_f->Close();

}
