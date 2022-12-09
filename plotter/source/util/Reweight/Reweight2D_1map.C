#define Reweight2D_1map_cxx
#include "Reweight2D_1map.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "../Debug.h"

void Reweight2D_1map::Loop()
{
   if (fChain == 0) return;

   TFile *f = TFile::Open(path_histo_);
   
   if(!f->IsOpen()) 
     {
       ERROR("Histogram path not found!! --> Exit");
       exit(EXIT_FAILURE);
     }
   
   
   TH2D *h1 = (TH2D*)f->Get(this->name_histo_);
	       
   Long64_t nentries = fChain->GetEntriesFast();

   double reweight_map(1.);


   TFile *new_f = new TFile(tree_path_ + "/Reweighted_onFatJetH_" + tree_name_ + ".root", "RECREATE"); 
   TTree* tree = (TTree*) fChain->GetTree()->CopyTree("");

   Double_t var_valueX(-178.), var_valueY(-178.);
   auto variables = tree->GetListOfLeaves();
   TLeaf *varX = NULL;
   TLeaf *varY = NULL;
   for(auto variable : *variables)
     {    
       if(var_nameX_  == ((TLeaf*)variable)->GetName()) varX = (TLeaf*)tree->GetLeaf(var_nameX_);
       if(var_nameY_  == ((TLeaf*)variable)->GetName()) varY = (TLeaf*)tree->GetLeaf(var_nameY_);
     }

   if(varX == NULL)
     {
       ERROR("Variable " << var_nameX_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }
   if(varY == NULL)
     {
       ERROR("Variable " << var_nameY_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }

 
   TBranch* newBranch = tree->Branch("reweight_map", &reweight_map, "reweight_map/D");


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      var_valueX = varX ->GetValue();
      var_valueY = varY ->GetValue();
   
      double weight = 1.;
      weight = h1->GetBinContent(h1->FindBin(var_valueX, var_valueY));
      //cout << var_nameX_ << ": " << var_valueX << " " <<var_nameY_ << ": " << var_valueY << " weight: " <<weight << endl;

      reweight_map = weight;
      
      newBranch->Fill();           
 
   }
   
   f->Close();

   //Store the new tree
   tree->Write();
   new_f->Close();
   
}
