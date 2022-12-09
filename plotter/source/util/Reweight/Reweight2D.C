#define Reweight2D_cxx
#include "Reweight2D.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "Debug.h"

void Reweight2D::Loop()
{
   if (fChain == 0) return;

   TFile *f = TFile::Open(path_histo_);
   
   if(!f->IsOpen()) 
     {
       ERROR("Histogram path not found!! --> Exit");
       exit(EXIT_FAILURE);
     }
   
   
   TH2D *h1 = (TH2D*)f->Get(this->name_histo1_);
   TH2D *h2 = (TH2D*)f->Get(this->name_histo2_);
	       
   Long64_t nentries = fChain->GetEntriesFast();

   double reweight_map1(1.), reweight_map2(1.);


   TFile *new_f = new TFile(tree_path_ + "/Reweighted_" + tree_name_ + ".root", "RECREATE"); 
   TTree* tree = (TTree*) fChain->GetTree()->CopyTree("");

   Double_t var_value1X(-178.), var_value1Y(-178.), var_value2X(-178.), var_value2Y(-178.);
   auto variables = tree->GetListOfLeaves();
   TLeaf *var1X = NULL;
   TLeaf *var1Y = NULL;
   TLeaf *var2X = NULL;
   TLeaf *var2Y = NULL;
   for(auto variable : *variables)
     {    
       if(var_name1X_  == ((TLeaf*)variable)->GetName()) var1X = (TLeaf*)tree->GetLeaf(var_name1X_);
       if(var_name1Y_  == ((TLeaf*)variable)->GetName()) var1Y = (TLeaf*)tree->GetLeaf(var_name1Y_);
       
       if(var_name2X_  == ((TLeaf*)variable)->GetName()) var2X = (TLeaf*)tree->GetLeaf(var_name2X_);
       if(var_name2Y_  == ((TLeaf*)variable)->GetName()) var2Y = (TLeaf*)tree->GetLeaf(var_name2Y_);
     }

   if(var1X == NULL)
     {
       ERROR("Variable " << var_name1X_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }
   if(var1Y == NULL)
     {
       ERROR("Variable " << var_name1Y_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }

   if(var2X == NULL)
     {
       ERROR("Variable " << var_name2X_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }
   if(var2Y == NULL)
     {
       ERROR("Variable " << var_name2Y_ << " not found in the tree. --> Exit!!");
       exit(EXIT_FAILURE);
     }

   TBranch* newBranch1 = tree->Branch("reweight_map1", &reweight_map1, "reweight_map1/D");
   TBranch* newBranch2 = tree->Branch("reweight_map2", &reweight_map2, "reweight_map2/D");


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      var_value1X = var1X ->GetValue();
      var_value1Y = var1Y ->GetValue();

      var_value2X = var2X ->GetValue();
      var_value2Y = var2Y ->GetValue();
   
      double weight1 = 1.;
      weight1 = h1->GetBinContent(h1->FindBin(var_value1X, var_value1Y));
      //cout << var_name1X_ << ": " << var_value1X << " " <<var_name1Y_ << ": " << var_value1Y << " weight: " <<weight1 << endl;

      double weight2 = 1.;
      weight2 = h2->GetBinContent(h2->FindBin(var_value2X, var_value2Y));
      //cout << var_name2X_ << ": " << var_value2X << " " <<var_name2Y_ << ": " << var_value2Y << " weight: " <<weight2 << endl;

      if(weight1 >= 0) reweight_map1 = weight1;
      else reweight_map1 = 1.;

      if(weight2 >= 0) reweight_map2 = weight2;
      else reweight_map2 = 1.;      

      
      newBranch1->Fill();           
      newBranch2->Fill();

   }
   
   f->Close();

   //Store the new tree
   tree->Write();
   new_f->Close();
   
}
