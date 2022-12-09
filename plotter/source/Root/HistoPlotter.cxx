#include <string>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <stdio.h>
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TFile.h"
#include <iostream>
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TPaletteAxis.h"
#include "TColor.h"
#include "THStack.h"
#include <iostream>
#include <iomanip>

#include "../util/Debug.h"
#include "../Plotter/HistoPlotter.h"

using namespace std;

string HistoMaker::MakeHisto2D(string var_name1, string var_name2, string sel_name, string chain_tag)
{
  cout << "Making 2D histo for vars-> " << var_name1 << ", " << var_name2 << " sel->" << sel_name << " and chain-> " << chain_tag << endl;

  string selection = this->reader_->Get_sel_string(sel_name);

  int nbin1 = this->reader_->Get_vars_Nbins(var_name1);
  int nbin2 = this->reader_->Get_vars_Nbins(var_name2);
  double A1 = (this->reader_->Get_vars_range(var_name1)).first;
  double A2 = (this->reader_->Get_vars_range(var_name2)).first;
  double B1 = (this->reader_->Get_vars_range(var_name1)).second;
  double B2 = (this->reader_->Get_vars_range(var_name2)).second;

  bool isVarBin1 = reader_->Get_varbins(var_name1);
  bool isVarBin2 = reader_->Get_varbins(var_name2);
  vector<Double_t> vec_binning1, vec_binning2;
  int NBins1 = 0;
  int NBins2 = 0;
  if(isVarBin1)
    {
      vec_binning1 = reader_->Get_binning(var_name1);
      NBins1 = vec_binning1.size() - 1;
    }
  if(isVarBin2)
    {
      vec_binning2 = reader_->Get_binning(var_name2);
      NBins2 = vec_binning2.size() - 1;
    }

  Double_t binning1[NBins1+1];
  Double_t binning2[NBins2+1];
  
  if(isVarBin1)
    {
      for(int i=0; i<vec_binning1.size(); i++) binning1[i] = vec_binning1.at(i);
    }
  if(isVarBin2)
    {
      for(int i=0; i<vec_binning2.size(); i++) binning2[i] = vec_binning2.at(i);
    }

  const Double_t *Binning1 = binning1;
  const Double_t *Binning2 = binning2;

  string title = var_name1 + " vs " + var_name2 + " " + sel_name + " " + chain_tag;
  string name = "histo2D_" + var_name1 + "_" + var_name2 +  "_" + sel_name + "_" + chain_tag;
  

  TH2D *h2;
  if(!isVarBin1 and !isVarBin2) h2  = new TH2D(name.c_str(), title.c_str(), nbin2, A2, B2, nbin1, A1, B1);
  if(isVarBin1 and !isVarBin2) h2 = new TH2D(name.c_str(), title.c_str(), nbin2, A2, B2, NBins1, Binning1);
  if(!isVarBin1 and isVarBin2) h2 = new TH2D(name.c_str(), title.c_str(), NBins2, Binning2, nbin1, A1, B1);
  if(isVarBin1 and  isVarBin2) h2 = new TH2D(name.c_str(), title.c_str(), NBins2, Binning2, NBins1, Binning1);

  string var_to_draw;
  string var_name_1, var_name_2;
  bool is_new_var1 = false;
  bool is_new_var2 = false;

  if(var_name1 == "deta_smalljets")
    {
      is_new_var1 = true;
      var_name_1 = "abs(SmallJet1_X_eta-SmallJet2_X_eta)";
    }
  if(var_name2 == "deta_smalljets")
    {
      is_new_var2 = true;
      var_name_2 = "abs(SmallJet1_X_eta-SmallJet2_X_eta)";
    }
  if(var_name1 == "deta_trackjets")
    {
      is_new_var1 = true;
      var_name_1 = "abs(trackjet1_eta-trackjet2_eta)";
    }
  if(var_name2 == "deta_trackjets")
    {
      is_new_var2 = true;
      var_name_2 = "abs(trackjet1_eta-trackjet2_eta)";
    }
  if(var_name1 == "deta_fatjets")
    {
      is_new_var1 = true;
      var_name_1 = "abs(etaH-etaV)";
    }
  if(var_name2 == "deta_fatjets")
    {
      is_new_var2 = true;
      var_name_2 = "abs(etaH-etaV)";
    }
  if(var_name1 == "deltaR_smalljets")
    {
      is_new_var1 = true;
      var_name_1 = "sqrt(pow((SmallJet1_X_eta-SmallJet2_X_eta),2) + pow((SmallJet1_X_phi-SmallJet2_X_phi),2))";
    }
  if(var_name2 == "deltaR_smalljets")
    {
      is_new_var2 = true;
      var_name_2 = "sqrt(pow((SmallJet1_X_eta-SmallJet2_X_eta),2) + pow((SmallJet1_X_phi-SmallJet2_X_phi),2))";
    }
  if(var_name1 == "deltaR_trackjets")
    {
      is_new_var1 = true;
      var_name_1 = "sqrt(pow((trackjet1_eta-trackjet2_eta),2) + pow((trackjet1_phi-trackjet2_phi),2))";
    }
  if(var_name2 == "deltaR_trackjets") 
    {
      is_new_var2 = true;
      var_name_2 = "sqrt(pow((trackjet1_eta-trackjet2_eta),2) + pow((trackjet1_phi-trackjet2_phi),2))";
    }
  if(var_name1 == "deltaR_fatjets")
    {
      is_new_var1 = true;
      var_name_1 = "sqrt(pow((etaH-etaV),2) + pow((phiH-phiV),2))";
    }
  if(var_name2 == "deltaR_fatjets") 
    {
      is_new_var2 = true;
      var_name_2 ="sqrt(pow((etaH-etaV),2) + pow((phiH-phiV),2))";
    }
  if(var_name1 == "NNScore_Exp")
    {
      is_new_var1 = true;
      var_name_1 = "exp(NNRewScore)";
    }
  if(var_name2 == "NNScore_Exp")
    {
      is_new_var2 = true;
      var_name_2 = "exp(NNRewScore)";
    }
  if(var_name1 == "SB2_Score_Exp")
    {
      is_new_var1 = true;
      var_name_1 = "exp(SB2_Score)";
    }
  if(var_name2 == "SB2_Score_Exp")
    {
      is_new_var2 = true;
      var_name_2 ="exp(SB2_Score)";
    }
  if(var_name1 == "trackjets_pt_balance")
    {
      is_new_var1 = true; 
      var_name_1 = "((trackjet1_pt - trackjet2_pt)/(trackjet1_pt + trackjet2_pt))"; 
    }
  if(var_name2 == "trackjets_pt_balance")
    {
      is_new_var2 = true;
      var_name_2 = "((trackjet1_pt - trackjet2_pt)/(trackjet1_pt + trackjet2_pt))";
    }
  if(var_name1 == "smalljets_pt_balance")
    {
      is_new_var1 = true;
      var_name_1 = "((SmallJet1_X_pT - SmallJet2_X_pT)/(SmallJet1_X_pT + SmallJet2_X_pT))";
    }
  if(var_name2 == "smalljets_pt_balance")
    {
      is_new_var2 = true;
      var_name_2 = "((SmallJet1_X_pT - SmallJet2_X_pT)/(SmallJet1_X_pT + SmallJet2_X_pT))";
    }
  
  if(var_name1 == "mXpTX")
    {
      is_new_var1 = true;
      var_name_1 = "(mV/pTV)";
    }
  if(var_name2 == "mXpTX")
    {
      is_new_var2 = true;
      var_name_2 = "(mV/pTV)";
    }
  
  if(var_name1 == "mXpTX_truth"){
    is_new_var1 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_1 = "(" + Xname + "/pTV)";
  }
  if(var_name2 == "mXpTX_truth"){
    is_new_var2 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_2 = "(" + Xname + "/pTV)";
  }

  if(var_name1 == "SmallJet1_X_m_pT"){
    is_new_var1 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_1 = "(SmallJet1_X_M/SmallJet1_X_pT)";
  }
 
  if(var_name2 == "SmallJet1_X_m_pT"){
    is_new_var2 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_2 = "(SmallJet1_X_M/SmallJet1_X_pT)";
  }

  if(var_name1 == "SmallJet2_X_m_pT"){
    is_new_var1 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_1 = "(SmallJet2_X_M/SmallJet2_X_pT)";
  }

  if(var_name2 == "SmallJet2_X_m_pT"){
    is_new_var2 = true;
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_name_2 = "(SmallJet2_X_M/SmallJet2_X_pT)";
  }
  


  if(is_new_var1 and !is_new_var2) var_to_draw = var_name_1 + ":" + var_name2 + ">>" + name;
  else if(!is_new_var1 and is_new_var2) var_to_draw = var_name1 + ":" + var_name_2 + ">>" + name;
  else if(is_new_var1 and is_new_var2)  var_to_draw = var_name_1 + ":" + var_name_2 + ">>" + name;
  else var_to_draw = var_name1 + ":" + var_name2 + ">>" + name;

  string sel_to_draw = selection += "*weight";

  this->map_chains_[chain_tag]->Draw(var_to_draw.c_str(), sel_to_draw.c_str(), "goff");
  this->Add_2Dhisto_to_map(name, h2);
  
  return name;
}

void HistoMaker::MakeHisto1D(string var_name, string sel_name, string chain_tag)
{
  string selection = this->reader_->Get_sel_string(sel_name);
  
  int nbin = this->reader_->Get_vars_Nbins(var_name);
  double A = (this->reader_->Get_vars_range(var_name)).first;
  double B = (this->reader_->Get_vars_range(var_name)).second;

  bool isVarBin = reader_->Get_varbins(var_name);
  vector<Double_t> vec_binning;
  int NBins = 0;
  if(isVarBin)
    {
      vec_binning = reader_->Get_binning(var_name);
      NBins = vec_binning.size() - 1;
    }
     
  Double_t binning[NBins+1];
  if(isVarBin)
    { 
      for(int i=0; i<vec_binning.size(); i++) binning[i] = vec_binning.at(i);
    }

  const Double_t *Binning = binning;

  TString title = var_name + "_" + sel_name ;
  TString name = sel_name + "_" + chain_tag + "_raw";

  if(debug_)
    {
      cout<< endl;
      cout << "-----------------------------------" << endl;
      cout << "Creating raw histo with: " << endl;
      cout << "Title: " << title << " Name histo :" << name << endl; 
      if(isVarBin)
	{
	  cout <<"NBins: " << NBins << " Binning: ( ";
	  int I = 0;
	  for(int I = 0; I<NBins; I++)  cout << binning[I] << " ";
	  cout << " ) " << endl;
	}								
      else cout << "NBins: " << nbin << " Range:[" << A << ", " << B << "] " << endl;
      cout << "-----------------------------------" << endl;
      cout<< endl;
    }

 
  TH1D *h_raw;
  if(!isVarBin) h_raw = new TH1D(name, title, nbin, A, B);
  else h_raw = new TH1D(name, title, NBins, Binning);

  TString var_to_draw = var_name + ">>" + name;
  if(var_name == "deta_trackjets") var_to_draw = "abs(trackjet1_eta-trackjet2_eta) >> " + name;
  if(var_name == "deta_fatjets") var_to_draw = "abs(etaH-etaV) >> " + name;
  if(var_name == "deltaR_trackjets") var_to_draw = "sqrt(pow((trackjet1_eta-trackjet2_eta),2) + pow((trackjet1_phi-trackjet2_phi),2)) >> " + name;
  if(var_name == "deltaR_fatjets") var_to_draw = "sqrt(pow((etaH-etaV),2) + pow((phiH-phiV),2)) >> " +name;
  if(var_name == "deta_smalljets") var_to_draw = "abs(SmallJet1_X_eta-SmallJet2_X_eta) >> " + name;
  if(var_name == "deltaR_smalljets") var_to_draw = "sqrt(pow((SmallJet1_X_eta-SmallJet2_X_eta),2) + pow((SmallJet1_X_phi-SmallJet2_X_phi),2)) >> " +name;
  if(var_name == "NNScore_exp") var_to_draw = "exp(NNRewScore) >> " + name;     
  if(var_name == "SB2_Score_Exp") var_to_draw = "exp(SB2_Score) >> " + name;
  if(var_name == "trackjets_pt_balance") var_to_draw = "((trackjet1_pt - trackjet2_pt)/(trackjet1_pt + trackjet2_pt)) >> " + name;
  if(var_name == "smalljets_pt_balance") var_to_draw = "((SmallJet1_X_pT - SmallJet2_X_pT)/(SmallJet1_X_pT + SmallJet2_X_pT)) >> " + name;
  if(var_name == "mXpTX") var_to_draw = "(mV/pTV) >> " + name;
  if(var_name == "mXpTX_truth"){
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_to_draw = "(" + Xname + "/pTV)>>" + name;
 }
  if(var_name == "SmallJet1_X_m_pT")  var_to_draw = "(SmallJet1_X_M/SmallJet1_X_pT) >> " + name;
  if(var_name == "SmallJet2_X_m_pT")  var_to_draw = "(SmallJet2_X_M/SmallJet2_X_pT) >> " + name;
  

  
  TString sel_to_draw = selection;

  this->map_chains_[chain_tag]->Draw(var_to_draw, sel_to_draw, "goff");
  this->Add_histo_to_map_raw(var_name, sel_name, chain_tag, h_raw);

  
  this->Get_histo_raw(var_name, sel_name, chain_tag)->Sumw2();
  if(this->Get_ScaletoBinWidth()) this->Get_histo_raw(var_name, sel_name, chain_tag)->Scale(1, "width");
  
}

void HistoMaker::MakeHisto1D_weighted(string var_name, string sel_name, string chain_tag)
{
  string selection = this->reader_->Get_sel_string(sel_name);


  //************ Temporary!!!!!!! ************ //
  if((strstr(sel_name.c_str(), "in3sigma")!=NULL) or (strstr(sel_name.c_str(), "out3sigma")!=NULL)) 
    {
      double mass_fit = -99.;
      double sigma_fit = -99.;
      
      //open file and read values
      string path_txt = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/util/results_fit.txt";
      ifstream txt_file (path_txt.c_str());
      string line;
      string word;

      if(!txt_file.is_open()) 
	{
	  ERROR("the file does not exist");
	  exit(EXIT_FAILURE);
	}
      if (txt_file.is_open())
	{
	  
	  while (!txt_file.eof())
	    {
	      getline(txt_file, line);
	      istringstream iss(line);
	      vector<string> results(istream_iterator<string>{iss},istream_iterator<string>());
	      
	      int n = chain_tag.length();
	      string chain = chain_tag;
	      if(strcmp(chain_tag.substr(n-2).c_str(), "_")!=0) chain = chain.substr(0, n-1); //silvia

	      if (results.size()>0 and (strstr(results[0].c_str(), chain.c_str())!=NULL))
		{
		  mass_fit = std::stod(results[3].c_str());
		  sigma_fit = std::stod(results[6].c_str());
		}
	    }
	}
      double mass_min = mass_fit - 3*sigma_fit;
      double mass_max = mass_fit + 3*sigma_fit;
      
      ostringstream strs1, strs2;
      strs1 << mass_min;
      string mass_min_str = strs1.str();
      strs2 << mass_max;
      string mass_max_str = strs2.str();

      string  new_selection = "";
      if((strstr(sel_name.c_str(), "in3sigma")!=NULL)) new_selection = "&& ( (mV > "+mass_min_str + ") && ( mV < "  + mass_max_str + ")) )" ; 
      if((strstr(sel_name.c_str(), "out3sigma")!=NULL)) new_selection = "&& (( mV < " +mass_min_str + ") || ( mV > " + mass_max_str + " )) )";
      if(mass_fit!= (-99.)) selection += new_selection;
      if(mass_fit==(-99.) and (strstr(sel_name.c_str(), "out3sigma")!=NULL)) selection += ")";

    }
  //************ //

  selection += "*weight";

  if(debug_) 
    {
      cout<< endl;
      cout << "-----------------------------------" << endl;
      cout<< "The string selection is --> " << selection << endl; 
    }
  
  Int_t nbin = this->reader_->Get_vars_Nbins(var_name);
  Double_t A = (this->reader_->Get_vars_range(var_name)).first;
  Double_t B = (this->reader_->Get_vars_range(var_name)).second;

  bool isVarBin = reader_->Get_varbins(var_name);
 
  vector<Double_t> vec_binning;
  int NBins = 0;
  if(isVarBin)
    {
      vec_binning = reader_->Get_binning(var_name);
      NBins = vec_binning.size() - 1;
    }
  
  Double_t binning[NBins+1];
  if(isVarBin)
    {
      for(int i=0; i<(NBins+1); i++) binning[i] = vec_binning.at(i);
    }
  const Double_t *Binning = binning;


  TString title = var_name + "_" + sel_name;
  TString name = sel_name + "_" + chain_tag;

  TH1D *h;
  if(!isVarBin) h = new TH1D(name, title, nbin, A, B);
  else h = new TH1D(name, title, NBins, Binning);

  if(debug_)
    { 
      cout << "Creating yields histo with: " << endl;
      cout << "Title: " << title << " Name histo :" << (string)name << endl; 
      if(isVarBin)
        {
          cout <<"NBins: " << NBins << " Binning: ( ";
          int I = 0;
          for(int I = 0; I<NBins; I++)  cout << binning[I] << " ";
          cout << " ) " << endl;
	}
      else cout << "NBins: " << nbin << " Range:[" << A << ", " << B << "] " << endl;
      cout << "-----------------------------------" << endl;
      cout<< endl;
    }

  TString var_to_draw = var_name + ">>" + name;
  if(var_name == "deta_trackjets") var_to_draw = "abs(trackjet1_eta-trackjet2_eta) >> " + name;
  if(var_name == "deta_fatjets") var_to_draw = "abs(etaH-etaV) >> " + name;
  if(var_name == "deltaR_trackjets") var_to_draw = "sqrt(pow((trackjet1_eta-trackjet2_eta),2) + pow((trackjet1_phi-trackjet2_phi),2)) >> " + name;
  if(var_name == "deltaR_fatjets") var_to_draw = "sqrt(pow((etaH-etaV),2) + pow((phiH-phiV),2)) >> " +name;
  if(var_name == "deta_smalljets") var_to_draw = "abs(SmallJet1_X_eta-SmallJet2_X_eta) >> " + name;
  if(var_name == "deltaR_smalljets") var_to_draw = "sqrt(pow((SmallJet1_X_eta-SmallJet2_X_eta),2) + pow((SmallJet1_X_phi-SmallJet2_X_phi),2)) >> " +name;
  if(var_name == "NNScore_exp") var_to_draw= "exp(NNRewScore) >> " + name; 
  if(var_name == "SB2_Score_Exp") var_to_draw = "exp(SB2_Score) >> " + name;
  if(var_name == "trackjets_pt_balance") var_to_draw = "((trackjet1_pt - trackjet2_pt)/(trackjet1_pt + trackjet2_pt)) >> " + name;
  if(var_name == "smalljets_pt_balance") var_to_draw = "((SmallJet1_X_pT - SmallJet2_X_pT)/(SmallJet1_X_pT + SmallJet2_X_pT)) >> " + name;
  if(var_name == "mXpTX") var_to_draw = "(mV/pTV) >> " +name;
  if(var_name == "mXpTX_truth"){
    unsigned last = chain_tag.find_last_of("_");
    string Xname = chain_tag.substr(last+1);
    var_to_draw = "(" + Xname + "/pTV) >>" + name;
  }
  if(var_name == "SmallJet1_X_m_pT")  var_to_draw = "(SmallJet1_X_M/SmallJet1_X_pT) >> " + name;
  if(var_name == "SmallJet2_X_m_pT")  var_to_draw = "(SmallJet2_X_M/SmallJet2_X_pT) >> " + name;

  TString sel_to_draw = selection;  

  this->map_chains_[chain_tag]->Draw(var_to_draw, sel_to_draw, "goff"); 
  h->Sumw2();

  if(strstr(sel_name.c_str(), "NominalRew")!=NULL) 
    {
      
      TString sel_to_draw_up = sel_to_draw;
      sel_to_draw_up = sel_to_draw_up.Replace(sel_to_draw_up.Index("NNScore_nominal"), sizeof("NNScore_nominal") - 1, "NNScore_up");

      TH1D *h_up;
      if(!isVarBin) 
	{
	  h_up = new TH1D(name + "_up", title, nbin, A, B);
	}
      else 
	{
	  h_up = new TH1D(name + "_up", title, NBins, Binning);
	}
      
      TString var_to_draw_up = var_to_draw + "_up";

      this->map_chains_[chain_tag]->Draw(var_to_draw_up, sel_to_draw_up, "goff");

      TH1D *h_noNorm = (TH1D*)h->Clone();

      if(this->Get_NormalizeHisto())
	{
	  this->NormalizeHisto(h, this->normalizeHistos_);
	  this->NormalizeHisto(h_up, this->normalizeHistos_);
	}

      double up_norm = 1.;
      double nom_norm = 1.;
      
      if(strstr(sel_name.c_str(), "HSB1_train_inclusive")!=NULL) {nom_norm=1.1921992453036606e-06; up_norm=1.1998734755431183e-06;};
      if(strstr(sel_name.c_str(), "HSB1_train_merged")!=NULL) {nom_norm=6.313326700442024e-06; up_norm=6.351324412847854e-06;};
      if(strstr(sel_name.c_str(), "HSB1_val_inclusive")!=NULL) {nom_norm=1.2171510127934273e-06; up_norm=1.2244859548383682e-06;};
      if(strstr(sel_name.c_str(), "HSB1_val_merged")!=NULL) {nom_norm=6.438264359274532e-06; up_norm=6.478663227815173e-06;};
      if(strstr(sel_name.c_str(), "LSB1_val_inclusive")!=NULL) {nom_norm=1.7333462072651842e-06; up_norm=1.7499528835788298e-06;};
      if(strstr(sel_name.c_str(), "LSB1_val_merged")!=NULL) {nom_norm=9.324440136993967e-06; up_norm=9.418634892789889e-06;};
      if(strstr(sel_name.c_str(), "LSB1_val2_inclusive")!=NULL) {nom_norm=9.652592206202547e-07; up_norm=9.75165805599311e-07;};
      if(strstr(sel_name.c_str(), "LSB1_val2_merged")!=NULL) {nom_norm=5.191394385790892e-06; up_norm=5.242728456126493e-06;};
      if(strstr(sel_name.c_str(), "SR_merged")!=NULL) {nom_norm=1.927524817011584e-06; up_norm=1.942000913462855e-06;};
      if(strstr(sel_name.c_str(), "LSB1_full_merged")!=NULL) {nom_norm=3.443660261837067e-06; up_norm=3.4766797512020275e-06;};
      if(strstr(sel_name.c_str(), "HSB1_train_resolved")!=NULL) {nom_norm=3.082451055015908e-06; up_norm=3.1018606745474338e-06;};
      if(strstr(sel_name.c_str(), "HSB1_val_resolved")!=NULL) {nom_norm=3.068539764516823e-06; up_norm=3.086180679831129e-06;};
      if(strstr(sel_name.c_str(), "LSB1_val_resolved")!=NULL) {nom_norm=5.236742852574289e-06; up_norm=5.283264710176913e-06;};
      if(strstr(sel_name.c_str(), "LSB1_val2_resolved")!=NULL) {nom_norm=2.9820765555640896e-06; up_norm=3.0143338749103367e-06;};
      if(strstr(sel_name.c_str(), "SR_resolved")!=NULL) {nom_norm=1.0040417761026917e-06; up_norm=1.0113410774758762e-06;};
      if(strstr(sel_name.c_str(), "LSB1_full_resolved")!=NULL) {nom_norm=1.9623439584393054e-06; up_norm=1.9822794001811417e-06;};

      h_up->Scale(up_norm/nom_norm);

      TH1D *error_up = (TH1D*)h_up->Clone();
      error_up->Add(h, -1);

      // for (int b=1; b <= h->GetNbinsX(); b++)   h->SetBinError(b, sqrt(((h->GetBinError(b))*(h->GetBinError(b))) + ((error_up->GetBinContent(b))*(error_up->GetBinContent(b))))); 
      
    }
    
  this->Add_histo_to_map(var_name, sel_name, chain_tag, h); 

  if(this->Get_ScaletoBinWidth()) this->Get_histo(var_name, sel_name, chain_tag)->Scale(1, "width");

}


void HistoMaker::Add_chain_to_map(string file_tag, TChain *chain)
{
  this->map_chains_[file_tag] = chain;
}

void HistoMaker::Add_histo_to_map(string var_name, string sel_name, string file_tag, TH1D* histo)
{
  this->map_histos_[var_name][sel_name][file_tag] = histo;
  this->map_integral_histos_[var_name][sel_name][file_tag] = histo->Integral();
}

void HistoMaker::Add_histo_to_map_raw(string var_name, string sel_name, string file_tag, TH1D* histo)
{
  this->map_histos_raw_[var_name][sel_name][file_tag] = histo;
}

void HistoMaker::Add_ratio_to_map(string var_name, string sel_name, string ratio_name, TH1D* h_ratio)
{
  this->map_ratios_[var_name][sel_name][ratio_name] = h_ratio;
}

void HistoMaker::Add_2Dhisto_to_map(string name, TH2D* h2)
{
  this->map_2Dhistos_[name] = h2;
}

void HistoMaker::Add_ratio2D_to_map(string name, TH2D* h2)
{
  this->map_ratio2D_[name] = h2;
}


TPad* HistoMaker::Create_Top_Pad(double xlow, double ylow, double xup, double yup)
{
  TPad *pad1 = new TPad("pad1", "pad1", xlow, ylow, xup, yup, kWhite);
  pad1->Draw();
  pad1->SetBottomMargin(0.12);
  
  return pad1;
}

TPad* HistoMaker::Create_Bottom_Pad(double xlow, double ylow, double xup, double yup)
{
  TPad *pad2 = new TPad("pad2", "pad2", xlow, ylow, xup, yup, kWhite);
  pad2->Draw();
  pad2->SetBottomMargin(0.25);

  return pad2;
}


//public:

bool HistoMaker::MakeChain(string file_tag)
{
  TChain *chain = new TChain("Nominal");
  
  vector<string> tags = this->reader_->Get_file_tags();
  vector<string> tags_for_chain; 
  string file;

  for(string tag : tags)
    {
      if(strstr(tag.c_str(), file_tag.c_str())!=NULL) 
	{
	  tags_for_chain.push_back(tag);
	  file = this->reader_->Get_file(tag);
	  chain->Add(file.c_str());
	}
    }

  if(tags_for_chain.size() != chain->GetListOfFiles()->GetEntriesFast())
    {
      WARNING("Not all tree found");
    }
  
  if(chain->GetListOfFiles()->GetEntriesFast() == 0) 
    {
      WARNING("Chain " << file_tag << " not created");
      return false;
    }
  else
    {
      INFO("Chain " << file_tag << " created with " << chain->GetListOfFiles()->GetEntriesFast() << " files");
      this->Add_chain_to_map(file_tag, chain);

      for(auto tag : tags_for_chain)  INFO("--> "<< this->reader_->Get_file(tag));

      if(debug_)
	{
	  cout << "Chain has "<<map_chains_[file_tag]->GetNtrees() <<" trees inside"<<endl;
	  map_chains_[file_tag]->GetFile()->GetListOfKeys()->Print();
	  cout << "...the chain has " << map_chains_[file_tag]->GetEntries()<< " entries" << endl;
	}
      return true;
    }
}

void HistoMaker::Set_NormalizeHistos(double Norm)
{
  this->histos_to_normalize_ = true;
  this->normalizeHistos_ = Norm;
}

bool HistoMaker::Get_NormalizeHistos()
{
  bool set = this->histos_to_normalize_;
  return set;
}

void HistoMaker::Set_NormalizeHisto(string histo_tag, double Norm)
{
  this->histo_to_normalize_ = true;
  this->normalizeHisto_.push_back(make_pair(histo_tag, Norm));
}

bool HistoMaker::Get_NormalizeHisto()
{
  bool set = this->histo_to_normalize_;
  return set;
}

void HistoMaker::Set_NormalizeStack(string histo_tag, double Norm)
{
  this->stack_to_normalize_ = true;
  this->normalizeStack_.push_back(make_pair(histo_tag, Norm));
}

bool HistoMaker::Get_NormalizeStack()
{
  bool set = this->stack_to_normalize_;
  return set;
}


void  HistoMaker::NormalizeHistos(double Norm)
{
  map<string, map<string, map<string, TH1D*>>>::iterator m1;
  map<string, map<string, TH1D*>>::iterator m2;
  map<string, TH1D*>::iterator h_map;
  
  m1 = this->map_histos_.begin();
  while(m1 != this->map_histos_.end())
    {
      m2 = m1->second.begin();
      while(m2 != m1->second.end())
      {
	h_map = m2->second.begin();
	while(h_map != m2->second.end())
	{
	  this->NormalizeHisto(h_map->second, Norm);
	  h_map++;
	}
	m2++;
      }
      m1++;
    }
  
}

void HistoMaker::NormalizeHisto(TH1D *h, double Norm)
{
  if(debug_) cout << "Normalizing histo: " << h->GetName()<< endl;
  if(this->Get_ScaletoBinWidth()) 
    {
      if(h->Integral("width") != Norm) h->Scale(Norm/h->Integral(), "width");
    }
  else
    {
      if(h->Integral() != Norm)  h->Scale(Norm/h->Integral());
    }
}


void HistoMaker::NormalizeHisto2D(TH2D *h, double Norm)
{
  if(debug_) cout << "Normalizing histo 2D: " << h->GetName() << " to: " << Norm << endl;
  if(this->Get_ScaletoBinWidth())
    {
      if(h->Integral("width") != Norm) h->Scale(Norm/h->Integral(), "width");
    }
  else
    {
      if(h->Integral() != Norm)  h->Scale(Norm/h->Integral());
    }
  if(debug_) 
    {
      if(this->Get_ScaletoBinWidth()) cout<<  "now integral is -> "<<h->Integral("width")<<endl;
      else cout << "now integral is -> "<<h->Integral()<<endl;
    }
}

void HistoMaker::ApplyNormalizations()
{

  if(this->Get_NormalizeHistos())
    {
      double norm = this->normalizeHistos_;
      this->NormalizeHistos(norm);
    }

  if(this->Get_NormalizeHisto())
    {
      vector<pair<string, double>> histos_norms = this->normalizeHisto_;
      for(auto histo : histos_norms)
	{
          string tag = histo.first;
          vector<string> tags;
          double norm = histo.second;

          string parsed, input = tag;
          stringstream input_stringstream(input);

          while(getline(input_stringstream, parsed, ' '))
            {
              tags.push_back(parsed);
            }

	  if((this->map_histos_.find(tags.at(0)) == this->map_histos_.end()) or (this->map_histos_[tags.at(0)].find(tags.at(1))==this->map_histos_[tags.at(0)].end())) 
	    {
	      WARNING("Map of histos [" + tags.at(0) + "][" + tags.at(1) + "] does not exist! Histo not normalized");
	    }
	  else  this->NormalizeHisto(Get_histo(tags.at(0), tags.at(1), tags.at(2)), norm);
        }
    }

}

void HistoMaker::NormalizeStack(string var_name, string sel_name)
{
  vector<pair<string, double>> histos_norms = this->normalizeStack_;
  for(auto histo : histos_norms)
    {
      string tag = histo.first;
      vector<string> tags;
      double norm = histo.second;

      string parsed, input = tag;
      stringstream input_stringstream(input);

      while(getline(input_stringstream, parsed, ' '))
	{
	  tags.push_back(parsed);
	}
      if(tags.at(0) == var_name and tags.at(1) == sel_name )  
	{
	  bool isScaleBin = this->Get_ScaletoBinWidth();
	  TString int_options = "";
	  if(isScaleBin) int_options = "width";
	  double bkg_integral = this->Get_histo(tags.at(0), tags.at(1), "Bkg")->Integral(int_options);

	  this->NormalizeHisto(this->Get_histo(tags.at(0), tags.at(1), "Bkg"), norm);

	  TIter hist_component(this->Get_stack(var_name, sel_name)->GetHists());
	  TObject* object = 0;
	  while(object = hist_component())
	    {
	      ((TH1*)object)->Scale(norm/bkg_integral);
	    }
	}
      
    }

}


pair<TH1D*, TH1D*> HistoMaker::Do_histo_sum(string var_name, string sel_name, vector<string> tags)
{
  TH1D *sum = (TH1D*)(this->Get_histo(var_name, sel_name, tags.at(0)))->Clone();
  // TH1D *sum_raw = (TH1D*)(this->Get_histo_raw(var_name, sel_name, tags.at(0)))->Clone();
  TH1D *sum_raw;
  for(int i=1; i<tags.size(); i++)
    {
      sum->Add(this->Get_histo(var_name, sel_name, tags.at(i)));
      //sum_raw ->Add(this->Get_histo_raw(var_name, sel_name, tags.at(i)));
    }
  return make_pair(sum, sum_raw);
}

void HistoMaker::MakeHistos(vector<string> var_names, vector<string> sel_names,  vector<string> chain_tags)
{
  for(auto var_name : var_names)
    {
      cout << endl;
      cout << "VARIABLE --> " << var_name << endl;
      for(auto sel_name : sel_names)
	{
	  cout << "SELECTION --> " << sel_name<< endl;
	  
	  for(auto tag: chain_tags)
	    {
	      if ((strcmp(tag.c_str(),"Data")!=0) && ((strstr(sel_name.c_str(),"NominalRew")!=NULL) || (strstr(sel_name.c_str(),"VariatedRew")!=NULL))) // do not produce reweighted plots on signals or MC bkg
		{
		  continue;
		}
	      if((map_chains_.find(tag)!=map_chains_.end()) and  map_chains_[tag]!=NULL)
		{
		  cout << "Making histo with chain --> " << tag <<endl;
		  //this->MakeHisto1D(var_name, sel_name, tag);
		  this->MakeHisto1D_weighted(var_name, sel_name, tag);	 
		} 
	      else
		{
		  WARNING("Chain " + tag + " does not exist!! --> Histo for this sample not created!");
		}
	    }
	}
    }
}

void  HistoMaker::DrawHisto(string var_name, string sel_name,  vector<string> chain_tags)
{
  
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  bool isStack = this->isStack_;
  this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c = new TCanvas("c", "c", 800, 600);  

  this->GO_->Draw_Options();

  double max = -9.;
  double min = -9.;
  for(auto tag: chain_tags)
    {
      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
        {
	  double tmp_max = this->map_histos_[var_name][sel_name][tag]->GetMaximum();
	  double tmp_min = this->map_histos_[var_name][sel_name][tag]->GetMinimum();
	  if(tmp_max>max) max = tmp_max;
	  if(tmp_min<min) min = tmp_min;
        }
    }
  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.25;
  double min_range = this->GO_->Get_minimum();
  
  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.; 
      max_range = max + interval*2;
      c->SetLogy();
    }
  
  if(this->GO_->Get_logx()) c->SetLogx();
  
  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";

  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
  
  if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < chain_tags.size())
    {
      WARNING("Number of drawing options are less than number of chains!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < chain_tags.size())
    {
      WARNING("Number of drawing colors are less than number of chains!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != chain_tags.size())
    {
      WARNING("Number of legend entries are different from number of chains!!");
      overwrite_leg = false;
    }
  

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();


  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  TString leg_entry;

  int i = 0;
  for(auto tag: chain_tags)
    {
      
      if(isScaleBin and !this->GO_->Get_overwrite_y_name())
	{
	  double bin_width = (double)this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	  std::stringstream stream;
	  stream << std::fixed << std::setprecision(2) << bin_width;
	  std::string s = stream.str();

	  (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
	}

      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
	{
	  if(histo_options) 
	    {
	      options.push_back(this->GO_->Get_draw_options().at(i));
	    }
	  else
	    {
	      if (tag =="Data") options.push_back("E");
	      else options.push_back("hist");
	    }
	  
	  if(options.at(i)=="E" or options.at(i)=="P") this->map_histos_[var_name][sel_name][tag]->SetMarkerStyle(20);
	  draw_options = "same " + options.at(i);

	  if(histo_colors)
	    {
	      color = this->GO_->Get_colors().at(i);
	      this->map_histos_[var_name][sel_name][tag]->SetLineColor(color);
	      //this->map_histos_[var_name][sel_name][tag]->SetFillColor(color);//delete
	    }
	  else draw_options+= "PLC";
  
	  this->map_histos_[var_name][sel_name][tag]->SetMaximum(max_range);
	  this->map_histos_[var_name][sel_name][tag]->SetMinimum(min_range);
	  this->map_histos_[var_name][sel_name][tag]->GetXaxis()->SetTitle(X_title);
	  this->map_histos_[var_name][sel_name][tag]->GetYaxis()->SetTitle(Y_title);
	  
	  if(this->GO_->Get_overwrite_histo_name()) this->map_histos_[var_name][sel_name][tag]->SetTitle(this->GO_->Get_histo_name());
	  
	  leg_entry = tag.c_str();
	  if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

	  this->map_histos_[var_name][sel_name][tag]->Draw(draw_options);	  
	  leg->AddEntry(map_histos_[var_name][sel_name][tag], leg_entry);
	}
      else
	{
	  WARNING("Histo: " + var_name + " " + sel_name + " " + tag +" does not exist!");
	}
      i++;
    }
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();  
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
    
  c->SaveAs((var_name + "_" + sel_name +".pdf").c_str());  
}

void  HistoMaker::DrawHisto(string var_name, string sel_name,  vector<string> chain_tags, vector<string>tags_num, vector<string>tags_den)
{
  bool isStack = this->isStack_;
  this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c1 = new TCanvas();
  this->GO_->Draw_Options();
  
  TPad *Top_pad = Create_Top_Pad(0, 0.2, 1, 1);
  Top_pad->cd();

  double max = -9.;
  double min = -9.;
  for(auto tag: chain_tags)
    {
      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
        {
          double tmp_max = this->map_histos_[var_name][sel_name][tag]->GetMaximum();
          double tmp_min = this->map_histos_[var_name][sel_name][tag]->GetMinimum();
          if(tmp_max>max) max = tmp_max;
          if(tmp_min<min) min = tmp_min;
        }
    }
  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = this->GO_->Get_minimum();

  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.;
      max_range = max + interval*2;
      Top_pad->SetLogy();
    }

  if(this->GO_->Get_logx()) c1->SetLogx();

  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";

  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < chain_tags.size())
    {
      WARNING("Number of drawing options are less than number of chains!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < chain_tags.size())
    {
      WARNING("Number of drawing colors are different less than number of chains!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != chain_tags.size())
    {
      WARNING("Number of legend entries are different from number of chains!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();


  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  TString leg_entry;

  int i = 0;
  for(auto tag: chain_tags)
    {
      
      if(isScaleBin and !this->GO_->Get_overwrite_y_name())
	{
	  double bin_width = (double) this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	  std::stringstream stream;
	  stream << std::fixed << std::setprecision(2) << bin_width;
	  std::string s = stream.str();

	  (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
	}

      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()) and (map_histos_[var_name][sel_name].find(tag)!=map_histos_[var_name][sel_name].end()))
	{
	  if(histo_options)
            {
              options.push_back(this->GO_->Get_draw_options().at(i));
              if(options.at(i)=="E" or options.at(i)=="P") this->map_histos_[var_name][sel_name][tag]->SetMarkerStyle(20);
            }
          else options.push_back("hist");
          draw_options = "same " + options.at(i);

	  if(histo_colors)
            {
              color = this->GO_->Get_colors().at(i);
              this->map_histos_[var_name][sel_name][tag]->SetLineColor(color);
            }
	  else draw_options+= "PLC";

          this->map_histos_[var_name][sel_name][tag]->SetMaximum(max_range);

          this->map_histos_[var_name][sel_name][tag]->SetMinimum(min_range);
          this->map_histos_[var_name][sel_name][tag]->GetXaxis()->SetTitle(X_title);
          this->map_histos_[var_name][sel_name][tag]->GetYaxis()->SetTitle(Y_title);

          if(this->GO_->Get_overwrite_histo_name()) this->map_histos_[var_name][sel_name][tag]->SetTitle(this->GO_->Get_histo_name());

          leg_entry = tag.c_str();
          if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);
	  
	  this->map_histos_[var_name][sel_name][tag]->Draw(draw_options);
	  leg->AddEntry(map_histos_[var_name][sel_name][tag], leg_entry);
	}
      else
        {
          WARNING("Histo: " + var_name + " " + sel_name + " " + tag +" does not exist!");
        }
      i++;
    }
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c1->cd();
  
  TPad *Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.29);
  Bottom_pad->cd();  
  gPad->SetGridy(1);

  if(tags_num.size()!=tags_den.size()) 
    {
      ERROR("Different number of histos to divide. Ratio plot not created!");
      exit(EXIT_FAILURE);
    }

  
  string tag_num, tag_den;
  bool IsNum = false;
  bool IsDen = false;

  TString draw_options_ratio;
  Color_t color_ratio;
  vector<TString> options_ratio;

  bool ratio_options = this->GO_->Get_overwrite_Draw_Options_ratio();
  bool ratio_colors = this->GO_->Get_overwrite_Colors_ratio();

  if(this->GO_->Get_overwrite_Draw_Options_ratio() and this->GO_->Get_draw_options_ratio().size() < tags_num.size())
    {
      WARNING("Number of drawing options for ratio are less than number of ratios!!");
      ratio_options = false;
    }

  if(this->GO_->Get_overwrite_Colors_ratio() and this->GO_->Get_colors_ratio().size() < tags_num.size())
    {
      WARNING("Number of drawing colors for ratio are less than number of ratios!!");
      ratio_colors = false;
    }

  for(int i=0; i< tags_num.size(); i++)
    {
      tag_num = tags_num.at(i);
      tag_den = tags_den.at(i);

      IsNum = false;
      IsDen = false;

      map<string, TH1D*>::iterator h = map_histos_[var_name][sel_name].begin();
      while(h!=map_histos_[var_name][sel_name].end())
	{
 
	  if(tag_num == h->first)
	    {
	      IsNum = true;
	    }
	  if(tag_den == h->first)
	    {
	      IsDen = true;
	    }
	  if(IsNum and IsDen) break;
	  h++;
	}

      if(!IsNum)
	{
	  WARNING("Histo " <<  " " << var_name << " " << sel_name << " " << tag_num << " does not exist!!");
	}
      if(!IsDen)
	{
          WARNING("Histo " << " " << var_name << " " << sel_name << " " << tag_den << " does not exist!!");
	}
      
      if(IsNum and IsDen)
	{

	  TH1D* h_num = map_histos_[var_name][sel_name][tag_num];
	  TH1D* h_den = map_histos_[var_name][sel_name][tag_den];
	  

	  TH1D* h_ratio = (TH1D*)h_num->Clone();
	  h_ratio->Divide(h_den);
	 
	  TString ratio_title = tag_num+"/"+tag_den;
	  if(GO_->Get_overwrite_ratio_name()) ratio_title = GO_->Get_ratio_name();

	  h_ratio->GetYaxis()->SetTitle(ratio_title);
	  h_ratio->SetTitle("");

	  string ratio_name = "ratio_"  + tag_num + "_" + tag_den;
	  h_ratio->SetName(ratio_name.c_str());

	  h_ratio->SetMaximum(GO_->Get_maximum_ratio());
	  h_ratio->SetMinimum(GO_->Get_minimum_ratio());

	  if(ratio_options)
            {
              options_ratio.push_back(this->GO_->Get_draw_options_ratio().at(i));
              if(options_ratio.at(i)=="E" or options_ratio.at(i)=="P") h_ratio->SetMarkerStyle(8);
            }
	  
	  options_ratio.push_back("E");
          draw_options_ratio = "same " + options_ratio.at(i);

          if(ratio_colors)
            {
              color_ratio = this->GO_->Get_colors_ratio().at(i);
              h_ratio->SetLineColor(color_ratio);
	      h_ratio->SetMarkerSize(0.5);
	      h_ratio->SetMarkerColor(color_ratio);
            }


	  h_ratio->GetYaxis()->SetNdivisions(3);
	  h_ratio->GetYaxis()->SetTitleSize(0.1);
	  h_ratio->GetYaxis()->SetTitleOffset(0.25);
	  h_ratio->GetYaxis()->SetLabelSize(0.1);

	  h_ratio->GetXaxis()->SetLabelSize(0.1);
	  h_ratio->GetXaxis()->SetTitleSize(0.12);
	  h_ratio->GetXaxis()->SetTitleOffset(0.8);
	  

	  this->Add_ratio_to_map(var_name, sel_name, ratio_name, h_ratio);

	  h_ratio->Draw(draw_options_ratio);
	}
    }
  c1->cd();
  c1->SaveAs((var_name + "_" + sel_name +".pdf").c_str());
}



void HistoMaker::DrawComparisons_vars(vector<string> var_names, string sel_name, vector<string> chain_tags)
{
  bool isStack = this->isStack_;
  for(auto var_name : var_names) this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c = new TCanvas();
  this->GO_->Draw_Options();

      double max = -9.;
      double min = -9.;

      for(auto var_name : var_names)
	{
	  
	  for(auto tag: chain_tags)
	    {
	      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
		{
		  double tmp_max = this->map_histos_[var_name][sel_name][tag]->GetMaximum();
		  double tmp_min = this->map_histos_[var_name][sel_name][tag]->GetMinimum();
		  if(tmp_max>max) max = tmp_max;
		  if(tmp_min<min) min = tmp_min;
		}
	    }
	}
      double interval = TMath::Abs(max-min);
      double max_range = max + interval*0.5;
      double min_range = this->GO_->Get_minimum();

      if(this->GO_->Get_logy())
	{
	  min_range += interval/100000.;
	  max_range = max + interval*2;
	  c->SetLogy();
	}

      if(this->GO_->Get_logx()) c->SetLogx();

      if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();

      bool isScaleBin = this->Get_ScaletoBinWidth();
      string unit_of_measure = this->Get_UnitOfMeasure();
      TString X_title, Y_title;
      if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
      if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
      else Y_title = "Entries";

      TString draw_options;
      Color_t color;
      vector<TString> options;

      bool histo_options = this->GO_->Get_overwrite_Draw_Options();
      bool histo_colors = this->GO_->Get_overwrite_Colors();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

      if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < (chain_tags.size()*var_names.size()))
	{
	  WARNING("Number of drawing options are less than number of chains!!");
	  histo_options = false;
	}

      if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < (chain_tags.size()*var_names.size()))
	{
	  WARNING("Number of drawing colors are less than number of chains!!");
	  histo_colors = false;
	}

      if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != (chain_tags.size()*var_names.size()))
	{
	  WARNING("Number of legend entries are different from number of chains!!");
	  overwrite_leg = false;
	}

      double xmin = this->GO_->Get_leg_xmin();
      double xmax = this->GO_->Get_leg_xmax();
      double ymin = this->GO_->Get_leg_ymin();
      double ymax = this->GO_->Get_leg_ymax();


      TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
      leg->SetBorderSize(0);
      TString leg_entry;
      
      int i = 0;
      for(auto var_name : var_names)
	{
	  for(auto tag: chain_tags)
	    {
	      
	      if(isScaleBin and !this->GO_->Get_overwrite_y_name())
		{
		  double bin_width = (double)this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
		  std::stringstream stream;
		  stream << std::fixed << std::setprecision(2) << bin_width;
		  std::string s = stream.str();
		  
		  (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
		  
		}
	      
	      if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
		{

		  if(histo_options)
		    {
		      options.push_back(this->GO_->Get_draw_options().at(i));
		      if(options.at(i)=="E" or options.at(i)=="P") this->map_histos_[var_name][sel_name][tag]->SetMarkerStyle(20);
		    }
		  else options.push_back("hist");
		  draw_options = "same " + options.at(i);

		  if(histo_colors)
		    {
		      color = this->GO_->Get_colors().at(i);
		      this->map_histos_[var_name][sel_name][tag]->SetLineColor(color);
		    }
		  else draw_options+= "PLC";

		  this->map_histos_[var_name][sel_name][tag]->SetMaximum(max_range);

		  this->map_histos_[var_name][sel_name][tag]->SetMinimum(min_range);
		  this->map_histos_[var_name][sel_name][tag]->GetXaxis()->SetTitle(X_title);
		  this->map_histos_[var_name][sel_name][tag]->GetYaxis()->SetTitle(Y_title);

		  if(this->GO_->Get_overwrite_histo_name()) this->map_histos_[var_name][sel_name][tag]->SetTitle(this->GO_->Get_histo_name());

		  leg_entry = (tag + " " + var_name).c_str();
		  if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

		  this->map_histos_[var_name][sel_name][tag]->Draw(draw_options);
		  leg->AddEntry(map_histos_[var_name][sel_name][tag], leg_entry);
		}
	      else
		{
		  WARNING("Histo: " + var_name + " " + sel_name + " " + tag +" does not exist!");
		}
	      i++;
	    }
	}

      leg->Draw();
      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();
      
      c->SaveAs((sel_name +".pdf").c_str());

}

void HistoMaker::DrawComparisons_sels(string var_name, vector<string> sel_names,  vector<string> chain_tags)
{
  bool isStack = this->isStack_;
  for(auto sel_name : sel_names) this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c = new TCanvas();
  this->GO_->Draw_Options();

  double max = -9.;
  double min = -9.;
  for(auto sel_name : sel_names)
    {
      for(auto tag: chain_tags)
	{
          if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
            {
              double tmp_max = this->Get_histo(var_name, sel_name, tag)->GetMaximum();
              double tmp_min = this->Get_histo(var_name, sel_name, tag)->GetMinimum();
              if(tmp_max>max) max = tmp_max;
              if(tmp_min<min) min = tmp_min;
            }
        }
    }
  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = this->GO_->Get_minimum();


  if(this->GO_->Get_logy())
    {
      min_range += interval/500000.;
      max_range = max + interval*2;
      c->SetLogy();
    }

  if(this->GO_->Get_logx()) c->SetLogx();

  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";

  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of drawing options are less than number of chains!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of drawing colors are less than number of chains!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of legend entries are different from number of chains!!");
      overwrite_leg = false;
    }
  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();


  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  TString leg_entry;
  int i = 0;
  for(auto sel_name : sel_names)
    {
      for(auto tag: chain_tags)
        {
	  if(!GO_->Get_overwrite_selection())  GO_->Set_selection(tag);
          else
            {
              string new_sel = GO_->Get_new_selection();
              GO_->Set_selection(new_sel);
	    }

	  if(isScaleBin and !this->GO_->Get_overwrite_y_name())
	    {
	      double bin_width = (double)this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	      std::stringstream stream;
	      stream << std::fixed << std::setprecision(2) << bin_width;
	      std::string s = stream.str();
	      
	      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
	    }
	  
          if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
            {
              if(histo_options)
                {
                  options.push_back(this->GO_->Get_draw_options().at(i));
                  if(options.at(i)=="E" or options.at(i)=="P") this->Get_histo(var_name, sel_name, tag)->SetMarkerStyle(20);
                }
              else options.push_back("hist");
              draw_options = "same " + options.at(i);

              if(histo_colors)
                {
                  color = this->GO_->Get_colors().at(i);
                  this->Get_histo(var_name, sel_name, tag)->SetLineColor(color);
		  this->Get_histo(var_name, sel_name, tag)->SetMarkerColor(color);
                }
	      else draw_options+= "PLC";

              this->Get_histo(var_name, sel_name, tag)->SetMaximum(max_range);

              this->Get_histo(var_name, sel_name, tag)->SetMinimum(min_range);
              this->Get_histo(var_name, sel_name, tag)->GetXaxis()->SetTitle(X_title);
              this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetTitle(Y_title);

              if(this->GO_->Get_overwrite_histo_name()) this->Get_histo(var_name, sel_name, tag)->SetTitle(this->GO_->Get_histo_name());

              leg_entry = (tag + " " + sel_name).c_str();
              if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

              this->Get_histo(var_name, sel_name, tag)->Draw(draw_options);
              leg->AddEntry(this->Get_histo(var_name, sel_name, tag), leg_entry);
            }
          else
            {
              WARNING("Histo: " + var_name + " " + sel_name + " " + tag +" does not exist!");
            }
          i++;
        }
    }
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->SaveAs((var_name + ".pdf").c_str());

}

void HistoMaker::DrawComparisons_sels(string var_name, vector<string> sel_names,  vector<string> chain_tags, vector<string>sels_num, vector<string>sels_den)
{
  
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();

  bool draw_ratio_errors = GO_ -> Get_draw_ratio_uncertainties();
  
  bool isStack = this->isStack_;
  for(auto sel_name : sel_names) this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c = new TCanvas("c", "c", 800, 600);
  this->GO_->Draw_Options();

  TPad *Top_pad = Create_Top_Pad(0, 0.2, 1, 1);
  Top_pad->cd();

  double max = -9.;
  double min = -9.;
  for(auto sel_name : sel_names)
    {
      for(auto tag: chain_tags)
	{
	  if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
	    {
	      double tmp_max = this->Get_histo(var_name, sel_name, tag)->GetMaximum();
	      double tmp_min = this->Get_histo(var_name, sel_name, tag)->GetMinimum();
	      if(tmp_max>max) max = tmp_max;
	      if(tmp_min<min) min = tmp_min;
	    }
	}
    }
  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = this->GO_->Get_minimum();


  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.;
      max_range = max + interval*2;
      Top_pad->SetLogy();
    }

  if(this->GO_->Get_logx()) Top_pad->SetLogx();

  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";


  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of drawing options are less than number of chains!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of drawing colors are less than number of chains!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != (chain_tags.size()*sel_names.size()))
    {
      WARNING("Number of legend entries are different from number of chains!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();


  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  TString leg_entry;

  int i = 0;
  for(auto sel_name : sel_names)
    {
      cout<<endl;
      cout<<" PROCESSING SELECTION: " << sel_name << endl;
      cout<< endl;
      
      for(auto tag: chain_tags)
	{

	  if(!GO_->Get_overwrite_selection())  GO_->Set_selection(tag);
	  else
	    {
	      string new_sel = GO_->Get_new_selection();
	      GO_->Set_selection(new_sel);
	    }
	  
	  if(isScaleBin and !this->GO_->Get_overwrite_y_name())
	    {
	      double bin_width = (double) this->Get_histo(var_name, sel_name, tag)->GetBinWidth(2);
	      std::stringstream stream;
	      stream << std::fixed << std::setprecision(2) << bin_width;
	      std::string s = stream.str();
	      
	      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
	    }
	  
	  if((map_histos_.find(var_name) != map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end()))
	    {
	      if(histo_options)
		{
		  options.push_back(this->GO_->Get_draw_options().at(i));
		  if(options.at(i)=="E" or options.at(i)=="P") this->map_histos_[var_name][sel_name][tag]->SetMarkerStyle(20);
		  if((options.at(i)=="hist") and (strstr(sel_name.c_str(), "0")!=NULL)) 
		    {
		      this->map_histos_[var_name][sel_name][tag]->SetFillColorAlpha(this->GO_->Get_colors().at(i),0.3);     
		    }
		  if(strstr(sel_name.c_str(), "NominalRew")!=NULL and (options.at(i)=="E" or options.at(i)=="P")) this->map_histos_[var_name][sel_name][tag]->SetMarkerStyle(4);
		  if(strstr(sel_name.c_str(), "_scaled")!=NULL) 
		    {
		      int nbins = this->Get_histo(var_name, sel_name, tag)->GetNbinsX();
		      Double_t err_bins[nbins];
		      if((strstr(sel_name.c_str(), "d2more3")==NULL) and (strstr(sel_name.c_str(), "d2more2")==NULL))
			{
			  this->PropagateScaleFactorUncertanty(this->Get_histo(var_name, sel_name, tag), err_bins, 0.0615, 0.0008); //attention, here I inserted an average value of ratio!!!
			}
		      if(strstr(sel_name.c_str(), "d2more3")!=NULL)
                        {
                          this->PropagateScaleFactorUncertanty(this->Get_histo(var_name, sel_name, tag), err_bins, 0.0620, 0.0011); //attention, here I inserted an average value of ratio!!!           
			} 
		      if(strstr(sel_name.c_str(), "d2more2")!=NULL)
                        {
                          this->PropagateScaleFactorUncertanty(this->Get_histo(var_name, sel_name, tag), err_bins, 0.0620, 0.0008); //attention, here I inserted an average value of ratio!!!         
                        }
		      this->map_histos_[var_name][sel_name][tag]->SetError(err_bins); 
		    }
		}
	      else options.push_back("hist");
	      draw_options = "same " + options.at(i);

	      if(histo_colors)
		{
		  color = this->GO_->Get_colors().at(i);
		  this->Get_histo(var_name, sel_name, tag)->SetLineColor(color);
		  this->Get_histo(var_name, sel_name, tag)->SetMarkerColor(color);
		}
	      else draw_options+= "PLC";

	      this->Get_histo(var_name, sel_name, tag)->SetMaximum(max_range);

	      this->Get_histo(var_name, sel_name, tag)->SetMinimum(min_range);
	 
	      this->Get_histo(var_name, sel_name, tag)->GetXaxis()->SetTitle(X_title);
	      this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetTitle(Y_title);

	      this->Get_histo(var_name, sel_name, tag)->SetTitle(var_name.c_str());
	      if(this->GO_->Get_overwrite_histo_name()) this->Get_histo(var_name, sel_name, tag)->SetTitle(this->GO_->Get_histo_name());
	      leg_entry = (tag + " " + sel_name).c_str();
	      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);
	      
	      this->Get_histo(var_name, sel_name, tag)->Draw(draw_options);
	      if (options.at(i)=="hist") leg->AddEntry(Get_histo(var_name, sel_name, tag), leg_entry, "f");
	      else leg->AddEntry(Get_histo(var_name, sel_name, tag), leg_entry);
	      this->Get_histo(var_name, sel_name, tag)->GetXaxis()->SetLabelOffset(3);
	      //this->Get_histo(var_name, sel_name, tag)->GetXaxis()->SetLabelSize(0);
	    }
	  else
	    {
	      WARNING("Histo: " + var_name + " " + sel_name + " " + tag +" does not exist!");
	    }
	  i++;
	}
    }
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();


  c->cd();

  TPad *Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.29);
  Bottom_pad->cd();
  gPad->SetGridy(1);


  if(sels_num.size()!=sels_den.size())
    {
      ERROR("Different number of selections to divide!");
      exit(EXIT_FAILURE);
    }

  string sel_num, sel_den;
  bool IsNum = false;
  bool IsDen = false;
  
  vector<TString> options_ratio;
  TString draw_options_ratio;
  Color_t color_ratio;

  bool ratio_options = this->GO_->Get_overwrite_Draw_Options_ratio();
  bool ratio_colors = this->GO_->Get_overwrite_Colors_ratio();

  
  if(this->GO_->Get_overwrite_Draw_Options_ratio() and this->GO_->Get_draw_options_ratio().size() < (sels_num.size()*chain_tags.size()))
    {
      WARNING("Number of drawing options for ratio are less than number of ratios!!");
      ratio_options = false;
    }

  if(this->GO_->Get_overwrite_Colors_ratio() and this->GO_->Get_colors_ratio().size() < (sels_num.size()*chain_tags.size()))
    {
      WARNING("Number of drawing colors for ratio are less than number of ratios!!");
      ratio_colors = false;
    }

  for(auto tag : chain_tags)
    {
      for(int i=0; i< sels_num.size(); i++)
	{
	  sel_num = sels_num.at(i);
	  sel_den = sels_den.at(i);

	  IsNum = false;
	  IsDen = false;
	  
	  map<string, map <string,TH1D*>>::iterator h = map_histos_[var_name].begin();
	  while(h!=map_histos_[var_name].end())
	    {
	      if(sel_num == h->first)
		{
		  IsNum = true;
		}
	      if(sel_den == h->first)
		{
		  IsDen = true;
		}
	      if(IsNum and IsDen) break;
	      h++;
	    }
	  
	  if(!IsNum)
	    {
	      WARNING("Histo " <<  " " << var_name << " " << sel_num <<  " does not exist!!");
	    }
	  if(!IsDen)
	    {
	      WARNING("Histo " << " " << var_name << " " << sel_den << " does not exist!!");
	    }
	  
	  if(IsNum and IsDen)
	    {
	      
	      TH1D* h_num = this->Get_histo(var_name, sel_num, tag);
	      TH1D* h_den = this->Get_histo(var_name, sel_den, tag);
	      

	      TH1D* h_ratio = (TH1D*)h_num->Clone();
	      h_ratio->Divide(h_den);
	      
	      TString ratio_title = sel_num+"/"+sel_den;
	      if(GO_->Get_overwrite_ratio_name()) ratio_title = GO_->Get_ratio_name();
	      
	      h_ratio->GetYaxis()->SetTitle(ratio_title);
	      h_ratio->SetTitle("");
	      
	      string ratio_name = "ratio_"  + sel_num + "_" + sel_den + "_" + tag;
	      h_ratio->SetName(ratio_name.c_str());
	      
	      if(GO_->Get_overwrite_Y_maximum_ratio()) h_ratio->SetMaximum(GO_->Get_maximum_ratio());
	      if(GO_->Get_overwrite_Y_minimum_ratio()) h_ratio->SetMinimum(GO_->Get_minimum_ratio());
	      
	      if(ratio_options)
		{
		  options_ratio.push_back(this->GO_->Get_draw_options_ratio().at(i));
		  if(options_ratio.at(i)=="E" or options_ratio.at(i)=="P") h_ratio->SetMarkerStyle(8);
		  if(options_ratio.at(i).Contains("TEXT")) 
		    {
		      h_ratio->SetMarkerSize(4);
		      h_ratio->SetMarkerStyle(1);
		    }
		  else h_ratio->SetMarkerSize(0.8);
		}
	      else 
		{
		  options_ratio.push_back("E");
		  h_ratio->SetMarkerSize(0.5);
		}
	
	      draw_options_ratio = "same " + options_ratio.at(i);
	      if(ratio_colors)
		{
		  color_ratio = this->GO_->Get_colors_ratio().at(i);
		  h_ratio->SetLineColor(color_ratio);
		  h_ratio->SetMarkerColor(color_ratio);
		}
	      
	      
	      h_ratio->GetYaxis()->SetNdivisions(3);
	      h_ratio->GetYaxis()->SetTitleSize(0.1);
	      h_ratio->GetYaxis()->SetTitleOffset(0.40);
	      h_ratio->GetYaxis()->SetLabelSize(0.1);
	      
	      
	      string sel_ratio = sel_num + "_" + sel_den;
	      
	      this->Add_ratio_to_map(var_name, sel_ratio, ratio_name, h_ratio);
	      h_ratio->Draw(draw_options_ratio);
	      
 	      if(draw_ratio_errors)
 		{
		  int nbins = h_ratio->GetNbinsX();
		  Double_t err_bins[nbins];
		  double I_num = 1;
		  double I_den = 1;
		  bool norm_all_histos = this->Get_NormalizeHistos();
		  if(norm_all_histos)
		    {
		      I_num = this->Get_histo_integral(var_name, sel_num, tag);
		      I_den = this->Get_histo_integral(var_name, sel_den, tag);
		    }
		  this->CalculateRatioUncertanties(h_num, h_den, I_num, I_den, h_ratio, err_bins);
		  h_ratio->SetError(err_bins);
		}  

	      h_ratio->GetXaxis()->SetLabelSize(0.1);
              h_ratio->GetXaxis()->SetLabelOffset(0.01);
	      h_ratio->GetXaxis()->SetTitleSize(0.12);
	      h_ratio->GetXaxis()->SetTitleOffset(0.9);

	      h_ratio->SetFillColor(color_ratio);
	      //h_ratio->SetFillStyle(1001);
	      //h_ratio->SetFillColorAlpha(color_ratio,0.3); 
	      h_ratio->Draw(draw_options_ratio);
		  
	    }
	}
    }

  c->cd();
  c->SaveAs((var_name + ".pdf").c_str());
}


void  HistoMaker::DrawHistos() //for a given variable, it compares different selections and tags 
{
  vector<string> vars_name = this->reader_->Get_vars_name();
  vector<string> sel_tags = this->reader_->Get_sel_tags();
  vector<string> file_tags = this->Get_chain_tags();

  this->MakeHistos(vars_name, sel_tags, file_tags);  
  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_all_histos or norm_single_histo) this->ApplyNormalizations();  


  for(auto var : vars_name)
    {

      cout<<endl;
      cout<<" PROCESSING VARIABLE: " << var << endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
      
      for(auto sel : sel_tags)
        {
	  cout<< endl;
	  cout<<" PROCESSING SELECTION: " <<sel << endl;
	  cout<< endl;

	  if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
          else
            {
              string new_sel = GO_->Get_new_selection();
              GO_->Set_selection(new_sel);
	    }

          this->DrawHisto(var, sel, file_tags);
	  
	  
	  for(auto tag: file_tags)
	    {	      
	      //this->map_histos_raw_[var][sel][tag]->Write();
	      string new_name = sel + "_" + tag;
              this->map_histos_[var][sel][tag]->SetName(new_name.c_str());
	      this->map_histos_[var][sel][tag]->Write();
	    }
	  
        }
      file_out->Close();
	  
    }
}

void  HistoMaker::DrawHistos(vector<string>tags_num, vector<string>tags_den) //for a given variable, it compares different selections and tags
{
  vector<string> vars_name = this->reader_->Get_vars_name();
  vector<string> sel_tags = this->reader_->Get_sel_tags();
  vector<string> file_tags = this->Get_chain_tags();

  this->MakeHistos(vars_name, sel_tags, file_tags);
  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_all_histos or norm_single_histo) this->ApplyNormalizations();  


  for(auto var : vars_name)
    {
      cout<<endl;
      cout<<" PROCESSING VARIABLE: " << var << endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");


      for(auto sel : sel_tags)
	{
	  cout<<" PROCESSING SELECTION: " <<sel << endl;
	  cout<< endl;

	  if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
          else
            {
              string new_sel = GO_->Get_new_selection();
              GO_->Set_selection(new_sel);
	    }

	  this->DrawHisto(var, sel, file_tags, tags_num, tags_den);
	  for(auto tag: file_tags)
            {
              //this->map_histos_raw_[var][sel][tag]->Write();
	      string new_name = sel + "_" + tag;
              this->map_histos_[var][sel][tag]->SetName(new_name.c_str());
              this->map_histos_[var][sel][tag]->Write();
            }

	  vector<string> ratio_tags = this->Get_ratio_tags(var, sel);
	  for(auto tag_ratio : ratio_tags)
	    {
	      this->map_ratios_[var][sel][tag_ratio]->Write();
	    }	  
	}
           
      file_out->Close();
    }
  
}

bool HistoMaker::WriteInfoFile(string var_name, string sel_name, bool isStack)
{
  bool outcome = false;

  if(!((map_histos_.find(var_name)!=map_histos_.end()) and (map_histos_[var_name].find(sel_name)!=map_histos_[var_name].end())))
    {
      ERROR("Histos with tags " << var_name << " " << sel_name << "are not in the histo map!");
      return outcome;
    }

  string file_name = "Info_" + var_name + "_" +sel_name + ".txt";
   
  vector<string> chain_tags = this->Get_chain_tags();

  ofstream fout;

  fout.open(file_name.c_str());
  fout << "Info for Histo -> " << var_name << " " << sel_name << endl;
  fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  fout << endl;
  fout << "Chain              Yields[Overflows]            Raw" << endl;
  fout << "----------------------------------------------------------------------------" << endl;
  fout << endl;
  
  bool isData = false;
  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString int_options = "";
  if(isScaleBin) int_options = "width";
  
  if(isStack)chain_tags.push_back("Bkg");
  for(auto tag : chain_tags)
    {
      if((map_histos_[var_name][sel_name].find(tag)!=map_histos_[var_name][sel_name].end()))
	{
	  fout << tag << "               "
	       << "                     " << this->map_histos_[var_name][sel_name][tag]->Integral(1, this->map_histos_[var_name][sel_name][tag]->GetNbinsX()+1, int_options)
	       << "                     " << this->Get_histo(var_name, sel_name, tag)->GetEntries() << endl;
	}
      
      if(strstr(tag.c_str(), "Data")!= NULL) isData = true;
    }
  if(isData and isStack)
    {
      double Data = this->Get_histo(var_name, sel_name, "Data")->Integral(1, this->Get_histo(var_name, sel_name, "Data")->GetNbinsX()+1, int_options);
      double MC = this->Get_histo(var_name, sel_name, "Bkg")->Integral(1, this->Get_histo(var_name, sel_name, "Bkg")->GetNbinsX()+1, int_options);
      double ratio = Data/MC;
      fout << endl;
      fout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
      fout << "Data/MC = " << ratio << endl;
    }
  fout.close();

  return outcome;
}


void HistoMaker::Draw2DHistos(vector<string> var_name1, vector<string> var_name2, vector<string> sel_names, vector<string> chain_tags, bool Profile)
{

  vector<string> var_names(var_name1);
  var_names.insert(var_names.end(), var_name2.begin(), var_name2.end());
 

  GO_->Draw_Options();
  
  if(var_name1.size() != var_name2.size())
    {
      ERROR("Variables are not all coupled to fill a 2D TH1!! (var_names vector size is different between variable 1 and 2)");
      return;
    }

    
  ////// REWEIGHTING  //  comment if you don't want to apply reweighting
  /*
  cout<< endl;
  cout << "  REWEIGHTING PROCEDURE " << endl;
  
  
  vector<string> more_tags = {"Bkg_Vjets", "Bkg_tt"}; 
  for(auto sel: sel_names)
    {
      if(strstr(sel.c_str(), "rew")!=NULL) continue;
      for(auto tag : more_tags)
        {
          if(this->map_chains_.find(tag) == this->map_chains_.end() )
            {
              WARNING("Chain with tag " << tag << " does not exist!!" << endl);
              continue;
            }

          for(int i = 0; i<var_name1.size(); i++)
	    {
	      string var1 = var_name1.at(i);
              string var2 = var_name2.at(i);

              this->MakeHisto2D(var1, var2, sel, tag);
	    }
	}
    }
  */
    //    //    //    

  for(auto sel: sel_names)
    {
      for(auto tag : chain_tags)
	{
	  if(this->map_chains_.find(tag) == this->map_chains_.end() ) 
	    {
	      WARNING("Chain with tag " << tag << " does not exist!!" << endl);
	      continue;
	    }
	  
	  for(int i = 0; i<var_name1.size(); i++)
	    {
	      string var1 = var_name1.at(i);
	      string var2 = var_name2.at(i);
	      
	      string name_histo = this->MakeHisto2D(var1, var2, sel, tag);
	      TString draw_options = GO_->Get_2D_draw_option();

	      TCanvas *c = new TCanvas();
	      
	      if(this->GO_->Get_logy()) c->SetLogy();
	      if(this->GO_->Get_logx()) c->SetLogx();

	      
	      TH2D* h2 = this->Get_2Dhisto(name_histo);

	      ////// REWEIGHTING  //  comment if you don't want to apply reweighting  
	      /*
	      //to create 2d maps
	      
	      string name_Vjets = "histo2D_" + var1 + "_" + var2 + "_" + sel + "_Bkg_Vjets";
	      string name_tt = "histo2D_" + var1 + "_" + var2 + "_" + sel + "_Bkg_tt";   

	      cout<<endl;                                                                                                                                                                               
	      cout <<"subtracting Vjets and tt bar shapes from data"<< endl;                                                                                                                            
	      cout<<endl;                                                                                                                                                                               

	      h2->Add(this->Get_2Dhisto(name_Vjets), -1.);                                                                                                                                              
	      h2->Add(this->Get_2Dhisto(name_tt), -1.); 
	      
	      */
	      //to apply reweighting AFTER
	      /*
	      string new_sel = sel;
	  
	      if(strstr(sel.c_str(), "rew")!=NULL)
	      {
		  string to_find = "_rew";
		  size_t pos = new_sel.find(to_find);
		  if (pos != std::string::npos) new_sel.erase(pos, to_find.length());
	      }

	      //cout << "sel = " << sel << endl;
	      //cout<< "new_sel = " << new_sel << endl;
	      string name_Vjets = "histo2D_" + var1 + "_" + var2 + "_" + new_sel + "_Bkg_Vjets";
	      string name_tt = "histo2D_" + var1 + "_" + var2 + "_" + new_sel + "_Bkg_tt";
	      
	      cout << endl;
	      cout<<" from 2D histo with data ->" << sel << " subtracted histow with tt and Vjets " << new_sel << endl;
	      
	      h2->Add(this->Get_2Dhisto(name_Vjets), -1.); 
	      h2->Add(this->Get_2Dhisto(name_tt), -1.);
	      */
	      // // //

	      double norm = this->normalizeHistos_;
	      if(this->Get_NormalizeHistos()) this->NormalizeHisto2D(h2, norm);
		

	      h2->GetXaxis()->SetTitle(reader_->Get_vars_Xtitle(var2).c_str());
	      h2->GetYaxis()->SetTitle(reader_->Get_vars_Xtitle(var1).c_str());

	      if(this->GO_->Get_overwrite_Y_minimum_2D()) h2->SetMinimum( this->GO_->Get_minimum_2D());
	      if(this->GO_->Get_overwrite_Y_maximum_2D()) h2->SetMaximum( this->GO_->Get_maximum_2D());
	      h2->Draw(draw_options);
	      TProfile *prof = h2->ProfileX("prof");
	      if(Profile)
		{
		  prof->SetLineColor(kRed);
		  prof->SetMarkerColor(kRed);
		  prof->SetMarkerStyle(8);
		  prof->SetMarkerSize(0.5);
		  prof->Draw("same");
		}
	      
	      if(this->Get_NormalizeHistos()) h2->GetZaxis()->SetLabelSize(0.02); //silvia
	      //h2->GetZaxis()->SetLabelOffset(0.1);
	      c->SaveAs((name_histo + ".pdf").c_str());
	      
	      TFile *file_out = new TFile(("file_2D_"+ sel+"_"+tag+"_"+var1+"_"+var2+".root").c_str(),"RECREATE");
	      h2->Write();
	      file_out->Close();
	    }
	}
    }

}

 
void  HistoMaker::CompareVariables(vector<string> sel_names, vector<string> var_names, vector<string> file_tags) //use it to compare different varibales with a given selections
{
  
  this->MakeHistos(var_names, sel_names, file_tags);  
  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_all_histos or norm_single_histo) this->ApplyNormalizations();


  for(auto sel : sel_names)
    {
      cout<<endl;
      cout<<" PROCESSING SELECTION: " << sel << endl;

      if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
      else
	{
	  string new_sel = GO_->Get_new_selection();
	  GO_->Set_selection(new_sel);
	}

      string file_name = "file_"+sel+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
      
      this->DrawComparisons_vars(var_names, sel, file_tags);
      
      for(auto var : var_names)
        {
          cout<<" PROCESSING VARIABLE: " <<var << endl;
          cout<< endl;
          
          for(auto tag: file_tags)
	    {
              //this->map_histos_raw_[var][sel][tag]->Write();
              this->map_histos_[var][sel][tag]->Write();
            }

        }
      file_out->Close();
    }
}

void  HistoMaker::CompareSelections(vector<string> sel_names, vector<string> var_names, vector<string> file_tags)
{
  this->MakeHistos(var_names, sel_names, file_tags);
  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_all_histos or norm_single_histo) this->ApplyNormalizations();


  for(auto var : var_names)
    {
      cout<<" PROCESSING VARIABLE: " <<var << endl;
      cout<< endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
      
      this->DrawComparisons_sels(var, sel_names, file_tags);
      
      for(auto sel : sel_names)
	{

	  for(auto tag: file_tags)
            {
	      string new_name = sel + "_" + tag;
              this->map_histos_[var][sel][tag]->SetName(new_name.c_str());
              this->Get_histo(var, sel, tag)->Write();
            }
	}
      file_out->Close();
    }
}

void  HistoMaker::CompareSelections(vector<string> sel_names, vector<string> var_names, vector<string> file_tags, vector<string> sels_num, vector<string> sels_den)
{

    
// REWEIGHTING  // comment if you don't want to apply reweighting
  /*
  cout<< endl;
  cout << "  REWEIGHTING PROCEDURE " << endl;

  vector<string> data_tags ={"Data"}; 
  this->MakeHistos(var_names, sel_names, data_tags); 
  
  vector<string> more_tags = {"Bkg_Vjets", "Bkg_tt"};
  vector<string> new_sels = sel_names;

  //cout << "old vector" << endl;
  //for(auto sel: new_sels) cout << sel << endl;
  for(int i=0; i<new_sels.size();i++)
    {
      if(strstr(new_sels.at(i).c_str(), "_rew")!=NULL) new_sels.erase(new_sels.begin() + i);
    }
  //cout << "new vector" << endl;
  //for(auto sel: new_sels) cout << sel << endl;
  this->MakeHistos(var_names, new_sels, more_tags);


  for(auto var : var_names)
    {
      for(auto sel:sel_names)
	{
	  string new_sel = sel;
	 	 
	  if(strstr(sel.c_str(), "_rew")!=NULL)
	    {
	      string to_find = "_rew";
	      size_t pos = new_sel.find(to_find);
	      if (pos != std::string::npos) new_sel.erase(pos, to_find.length());
	    }
	  cout << endl;
	  cout<<" from histo with data ->" << sel << " subtracted histow with tt and Vjets " << new_sel << endl;
	  
	  (this->Get_histo(var, sel, "Data"))->Add(this->Get_histo(var, new_sel, "Bkg_Vjets"), -1.);
	  (this->Get_histo(var, sel, "Data"))->Add(this->Get_histo(var, new_sel, "Bkg_tt"), -1.);
	}
    }
  */
  //     //    //
  
  this->MakeHistos(var_names, sel_names, file_tags); //comment this if reweight is decommented
  
  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_all_histos or norm_single_histo) this->ApplyNormalizations();

  for(auto var : var_names)
    {
      cout<< endl;
      cout<<" PROCESSING VARIABLE: " <<var << endl;
      cout<< endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
      
      this->DrawComparisons_sels(var, sel_names, file_tags, sels_num, sels_den);

      for(auto sel : sel_names)
	{
          cout<<endl;
          cout<<" PROCESSING SELECTION: " << sel << endl;
	  cout<< endl;

          for(auto tag: file_tags)
	    {
              //this->map_histos_raw_[var][sel][tag]->Write();
	      string new_name = sel + "_" + tag;
	      this->map_histos_[var][sel][tag]->SetName(new_name.c_str());
              this->map_histos_[var][sel][tag]->Write();
            }
	  for(int g=0; g<sels_num.size(); g++)
	    {
	      string sel_ratio = sels_num.at(g) + "_" + sels_den.at(g);
	      vector<string> ratio_tags = this->Get_ratio_tags(var, sel_ratio);
	      for(auto tag_ratio : ratio_tags)
		{
		  this->Get_ratio(var, sel_ratio, tag_ratio)->Write();
		}
	    }
	}
      file_out->Close();
    }
}



THStack* HistoMaker::MakeStackPlot(string var_name, string sel_name, vector<string> stack_components)
{
  string name = "Stack_" + sel_name;
  string title = var_name + "_" + sel_name;

  THStack *stack = new THStack(name.c_str(), title.c_str());
  
  vector<Color_t> colors_stack;
  bool apply_colors = GO_->Get_overwrite_Colors_stack();
  if(apply_colors)
    {
      colors_stack = GO_->Get_colors_stack();
      if(colors_stack.size() < stack_components.size())
	{
	  ERROR("Number of colors less than number of stack components!!");
	  exit(EXIT_FAILURE);
	}
    }
  bool istt = false;
  bool isVjets = false;
  bool ismultijets = false;
  for(auto comp : stack_components)
    {
      if(strstr(comp.c_str(), "tt")!=NULL) istt = true;
      if(strstr(comp.c_str(), "Vjets")!=NULL) isVjets = true;
      if(strstr(comp.c_str(), "QCD_dijet")!=NULL) ismultijets = true;
    }

  stack_components.clear();
  if(istt) stack_components.push_back("Bkg_tt"); 
  if(isVjets) stack_components.push_back("Bkg_Vjets"); 
  if(ismultijets) stack_components.push_back("Bkg_QCD_dijet");

  stack_components = this->Order_stack_components(var_name, sel_name, stack_components);

  int s = 0;
  for(auto tag : stack_components)
    {
      TH1D *h = this->Get_histo(var_name, sel_name, tag);
      if (apply_colors) 
	{
	  h->SetFillColor(colors_stack.at(s));
	  h->SetLineColor(colors_stack.at(s));
	}
      stack->Add(h);
      s++;
    }

  TH1D* h_sum = (this->Do_histo_sum(var_name, sel_name, stack_components)).first;
  h_sum->SetName(("Bkg_" + sel_name).c_str());
  this->Add_histo_to_map(var_name, sel_name, "Bkg" , h_sum);
  /*
  TH1D* h_sum_raw = (this->Do_histo_sum(var_name, sel_name, stack_components)).second;
  h_sum_raw->SetName(("Bkg_" + "d_" + sel_name).c_str());
  this->Add_histo_to_map_raw(var_name, sel_name, "Bkg" , h_sum_raw);
  */
  this->Add_stack_to_map(var_name, sel_name, stack);
  return stack;
}

vector<string> HistoMaker::Order_stack_components(string var_name, string sel_name, vector<string> stack_components)
{
  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString int_options ="";
  if(isScaleBin) int_options = "width";

  vector<string> components;
  double int_max = -9.;
  string comp_max = "";
  vector<string> comps_max;
  while(components.size() != stack_components.size())
    {
      for(int t=0; t<stack_components.size(); t++)
	{
	  if(find(comps_max.begin(), comps_max.end(), stack_components.at(t)) != comps_max.end()) continue;
	  double int_provv = this->Get_histo(var_name, sel_name, stack_components.at(t))->Integral(int_options);
	  if(int_provv>int_max)
	    {
	      int_max = int_provv;
	      comp_max = stack_components.at(t);
	    }
	}
      components.push_back(comp_max);
      comps_max.push_back(comp_max);
      int_max = -9.;
    }
  reverse(components.begin(), components.end());
  return components;
}

void HistoMaker::Add_stack_to_map(string var_name, string sel_name, THStack* stack)
{
  this->map_stack_plots_[var_name][sel_name] = stack;
}


void HistoMaker::DrawStack(string var_name, string sel_name, vector<string> chain_tags)
{
  this->isStack_ = true;

  vector<string> chain_to_draw;
  vector<string> stack_components;
  for(int i = 0; i< chain_tags.size(); i++)
    {
      if(strstr((chain_tags.at(i)).c_str(),"Bkg")!=NULL) stack_components.push_back(chain_tags.at(i));
      if(strstr((chain_tags.at(i)).c_str(),"Bkg")==NULL) chain_to_draw.push_back(chain_tags.at(i));
    }

  this->MakeStackPlot(var_name, sel_name, stack_components);

  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_stack = this->Get_NormalizeStack();
  if(norm_all_histos) 
    {
      string histo_tag = var_name + " " + sel_name;
      double Norm = this->normalizeHistos_;
      this->normalizeStack_.push_back(make_pair(histo_tag, Norm));
      this->NormalizeStack(var_name, sel_name);
    }
  if(norm_stack) this->NormalizeStack(var_name, sel_name);

  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_single_histo) this->ApplyNormalizations();
  if(norm_all_histos)
    {
      double Norm = this->normalizeHistos_;
      for(auto histo_tag : chain_to_draw) this->NormalizeHisto(this->Get_histo(var_name, sel_name, histo_tag), Norm);
    }

  bool isStack = this->isStack_;
  this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c1 = new TCanvas();
  this->GO_->Draw_Options();

  double max = -9.;
  double min = -9.;
  for(auto tag: chain_tags)
    {
      double tmp_max = this->Get_histo(var_name, sel_name, tag)->GetMaximum();
      double tmp_min = this->Get_histo(var_name, sel_name, tag)->GetMinimum();
      if(tmp_max>max) max = tmp_max;
      if(tmp_min<min) min = tmp_min;
    }  
  
  double stack_max = this->Get_stack(var_name, sel_name)->GetMaximum();
  double stack_min = this->Get_stack(var_name, sel_name)->GetMinimum();
  if(stack_max > max) max = stack_max;
  if(stack_min < min) min = stack_min;

  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = min;
  
  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.;
      max_range = max + interval*3;
      c1->SetLogy();
    }
  
  if(this->GO_->Get_logx()) c1->SetLogx();
  
  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";
  

  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool stack_colors = this->GO_->Get_overwrite_Colors_stack();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
  bool overwrite_leg_stack =  this->GO_->Get_overwrite_Legend_stack();

  if(this->GO_->Get_overwrite_Draw_Options() and (this->GO_->Get_draw_options().size() != chain_to_draw.size()))
    {
      WARNING("Number of drawing options are different from number of chains to draw (excluding stack)!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != chain_to_draw.size()))
    {
      WARNING("Number of drawing colors are different from number of chains to draw (excluding stack)!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != chain_to_draw.size())
    {
      WARNING("Number of legend entries are different from number of chains to draw (excluding stack)!!");
      overwrite_leg = false;
    }


  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  TString leg_entry;

       
  if(isScaleBin and !(this->GO_->Get_overwrite_y_name()))
    {
      this->Get_stack(var_name, sel_name)->Draw();
      double bin_width = (double) this->Get_stack(var_name, sel_name)->GetHistogram()->GetXaxis()->GetBinWidth(1);
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2) << bin_width;
      std::string s = stream.str();
      
      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
    }
  

  if(this->GO_->Get_overwrite_histo_name()) this->Get_stack(var_name, sel_name)->SetTitle(this->GO_->Get_histo_name());

  int st = 0;
  for(auto st_c : stack_components)
    {
      leg_entry = st_c.c_str();
      if(overwrite_leg_stack) leg_entry = this->GO_->Get_leg_entries_stack().at(st);
      leg->AddEntry(this->Get_histo(var_name, sel_name, st_c),leg_entry);
      st++;
    }


  this->Get_stack(var_name,sel_name)->SetMaximum(max_range);
  this->Get_stack(var_name,sel_name)->SetMinimum(min_range);

  if(!stack_colors)  this->Get_stack(var_name, sel_name)->Draw("PFC hist");
  else this->Get_stack(var_name, sel_name)->Draw("hist");

  this->Get_stack(var_name, sel_name)->GetHistogram()->SetMaximum(max_range);
  this->Get_stack(var_name, sel_name)->GetHistogram()->SetMinimum(min_range);

  this->Get_stack(var_name, sel_name)->GetHistogram()->GetYaxis()->SetTitle(Y_title);
  this->Get_stack(var_name, sel_name)->GetHistogram()->GetXaxis()->SetTitle(X_title);



  if(chain_to_draw.size()!=0)
    {
      int g = 0;
      for(auto tag : chain_to_draw)
	{ 
	  if(isScaleBin and !(this->GO_->Get_overwrite_y_name()))
	    {
	      double bin_width = (double) this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	      std::stringstream stream;
	      stream << std::fixed << std::setprecision(2) << bin_width;
	      std::string s = stream.str();
	      
	      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
	    }
	  
	  if(histo_options)
	    {
	      this->Get_histo(var_name, sel_name, tag)->SetMarkerStyle(20);
	      options.push_back(this->GO_->Get_draw_options().at(g));
	    }
	  else options.push_back("hist");
	  
	  draw_options = "same " + options.at(g);
	  
	  if(histo_colors)
	    {
	      color = this->GO_->Get_colors().at(g);
	      this->Get_histo(var_name, sel_name, tag)->SetLineColor(color);
	    }
	  else draw_options += "PLC";
	  
	  
	  this->Get_histo(var_name, sel_name, tag)->SetMaximum(max_range);
	  this->Get_histo(var_name, sel_name, tag)->SetMinimum(min_range);
	  this->Get_histo(var_name, sel_name, tag)->GetXaxis()->SetTitle(X_title);
	  this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetTitle(Y_title);
	  

	  if(this->GO_->Get_overwrite_histo_name()) this->Get_histo(var_name, sel_name, tag)->SetTitle(this->GO_->Get_histo_name());
	  leg_entry = tag.c_str();
	  if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(g);
	  
	  leg->AddEntry(this->Get_histo(var_name, sel_name, tag), leg_entry);
	  
	  this->Get_histo(var_name, sel_name, tag)->Draw(draw_options);
	  g++;
	}
    }

  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  c1->cd();
  c1->SaveAs(("Stack_" + var_name + "_" + sel_name +".pdf").c_str());
  
 
}

void HistoMaker::DrawStack_Ratio(string var_name, string sel_name, vector<string> chain_tags)
{
  this->isStack_ = true;

  bool draw_stats = GO_ -> Get_draw_stat_uncertainties();

  vector<string> chain_to_draw;
  vector<string> stack_components;
  for(int i = 0; i< chain_tags.size(); i++)
    {
      if(strstr((chain_tags.at(i)).c_str(),"Bkg")!=NULL) stack_components.push_back(chain_tags.at(i));
      if(strstr((chain_tags.at(i)).c_str(),"Bkg")==NULL) chain_to_draw.push_back(chain_tags.at(i));
    }

  this->MakeStackPlot(var_name, sel_name, stack_components);

  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_stack = this->Get_NormalizeStack();
  if(norm_all_histos)
    {
      string histo_tag = var_name + " " + sel_name;
      double Norm = this->normalizeHistos_;
      this->normalizeStack_.push_back(make_pair(histo_tag, Norm));
      this->NormalizeStack(var_name, sel_name);
    }
  if(norm_stack) this->NormalizeStack(var_name, sel_name);

  bool norm_single_histo = this->Get_NormalizeHisto();
  if(norm_single_histo) this->ApplyNormalizations();
  if(norm_all_histos)
    {
      double Norm = this->normalizeHistos_;
      for(auto histo_tag : chain_to_draw) this->NormalizeHisto(this->Get_histo(var_name, sel_name, histo_tag), Norm);
    }

  bool isStack = this->isStack_;
  this->WriteInfoFile(var_name, sel_name, isStack);

  TCanvas *c1 = new TCanvas();
  this->GO_->Draw_Options();

  TPad *Top_pad = Create_Top_Pad(0, 0.2, 1, 1);
  Top_pad->cd();
  
  double max = -9.;
  double min = -9.;
  for(auto tag: chain_tags)
    {

      double tmp_max = this->Get_histo(var_name, sel_name, tag)->GetMaximum();
      double tmp_min = this->Get_histo(var_name, sel_name, tag)->GetMinimum();
      if(tmp_max>max) max = tmp_max;
      if(tmp_min<min) min = tmp_min;
    }

  double stack_max = this->Get_stack(var_name, sel_name)->GetMaximum();
  double stack_min = this->Get_stack(var_name, sel_name)->GetMinimum();
  if(stack_max > max) max = stack_max;
  if(stack_min < min) min = stack_min;

  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = this->GO_->Get_minimum();

  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.;
      max_range = max + interval*2;
      Top_pad->SetLogy();
    }

  if(this->GO_->Get_logx()) Top_pad->SetLogx();

  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
  else X_title = reader_->Get_vars_Xtitle(var_name);
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";
   

  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool stack_colors = this->GO_->Get_overwrite_Colors_stack();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
  bool overwrite_leg_stack =  this->GO_->Get_overwrite_Legend_stack();

  if(this->GO_->Get_overwrite_Draw_Options() and (this->GO_->Get_draw_options().size() != chain_to_draw.size()))
    {
      WARNING("Number of drawing options are different from number of chains to draw (excluding stack)!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != chain_to_draw.size()))
    {
      WARNING("Number of drawing colors are different from number of chains to draw (excluding stack)!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != chain_to_draw.size())
    {
      WARNING("Number of legend entries are different from number of chains to draw (excluding stack)!!");
      overwrite_leg = false;
    }
  if(this->GO_->Get_overwrite_Legend_stack() and  this->GO_->Get_leg_entries_stack().size() != stack_components.size())
    {
      WARNING("Number of legend entries for stack are different from number of stack components!!");
      overwrite_leg_stack = false;
    }


  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
   TString leg_entry;
  
     
  this->Get_stack(var_name,sel_name)->SetMaximum(max_range);
  this->Get_stack(var_name,sel_name)->SetMinimum(min_range);
  if(this->GO_->Get_overwrite_histo_name()) this->Get_stack(var_name,sel_name)->SetTitle(this->GO_->Get_histo_name());
     
  int st = 0;
  for(auto st_c : stack_components)
    {
      leg_entry = st_c.c_str();
      if(overwrite_leg_stack) leg_entry = this->GO_->Get_leg_entries_stack().at(st);
      leg->AddEntry(this->Get_histo(var_name, sel_name, st_c),leg_entry);
      st++;
    }

  
  if(isScaleBin and !this->GO_->Get_overwrite_y_name())
    {
      double bin_width = (double) this->Get_histo(var_name, sel_name, chain_to_draw.at(0))->GetBinWidth(2);
      std::stringstream stream;
      stream << std::fixed << std::setprecision(0) << bin_width;
      std::string s = stream.str();
      
      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
    }

  
      
  if(stack_colors) this->Get_stack(var_name,sel_name)->Draw("hist");
  else this->Get_stack(var_name,sel_name)->Draw("hist PFC");
 

  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();
  
  this->Get_stack(var_name,sel_name)->GetHistogram()->SetMaximum(max_range);
  this->Get_stack(var_name,sel_name)->GetHistogram()->SetMinimum(min_range);
  this->Get_stack(var_name,sel_name)->GetHistogram()->GetYaxis()->SetTitle(Y_title);
 
  int g = 0;
  for(auto tag : chain_to_draw)
    {
             
      if(isScaleBin and !(this->GO_->Get_overwrite_y_name()))
	{
	  double bin_width = (double) this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	  std::stringstream stream;
	  stream << std::fixed << std::setprecision(2) << bin_width;
	  std::string s = stream.str();

	  (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " GeV").c_str());
	}


      if(histo_options)
	{
	  options.push_back(this->GO_->Get_draw_options().at(g));
	  if(options.at(g)=="E" or options.at(g)=="P") this->Get_histo(var_name, sel_name, tag)->SetMarkerStyle(20);
	}
      else if(tag == "Data") options.push_back("P");
      else options.push_back("hist");
      
      if(options.at(g)=="P" or options.at(g)=="E") this->Get_histo(var_name,sel_name, tag)->SetMarkerSize(0.8);
	
      
      draw_options = "same " + options.at(g);
      
      if(histo_colors)
	{
	  color = this->GO_->Get_colors().at(g);
	  this->Get_histo(var_name,sel_name, tag)->SetLineColor(color);
	}
      else draw_options += "PLC";
      
      this->Get_histo(var_name,sel_name, tag)->SetMaximum(max_range);
      this->Get_histo(var_name,sel_name, tag)->SetMinimum(min_range);
      this->Get_histo(var_name,sel_name, tag)->GetXaxis()->SetTitle(X_title);
      this->Get_histo(var_name,sel_name, tag)->GetYaxis()->SetTitle(Y_title);
      
      if(this->GO_->Get_overwrite_histo_name()) this->Get_histo(var_name,sel_name, tag)->SetTitle(this->GO_->Get_histo_name());
      
      leg_entry = tag.c_str();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(g);
      
      leg->AddEntry(this->Get_histo(var_name,sel_name, tag), leg_entry);
      
      this->Get_histo(var_name,sel_name, tag)->Draw(draw_options);
      
      g++;
    }
  
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c1->cd();

  TPad *Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.29);
  Bottom_pad->cd();
  gPad->SetGridy(1);
  if(this->GO_->Get_logx()) Bottom_pad->SetLogx();

  TString draw_options_ratio;
  Color_t color_ratio;

  bool ratio_options = this->GO_->Get_overwrite_Draw_Options_ratio();
  bool ratio_colors = this->GO_->Get_overwrite_Colors_ratio();
  
  string tag_num = "Data";
  string tag_den = "Bkg";

  TH1D* h_num = this->Get_histo(var_name,sel_name, tag_num);
  TH1D* h_den = this->Get_histo(var_name,sel_name, "Bkg");

  //TH1D* h_ratio = (TH1D*)h_den->Clone();//DELETE!!!
  //h_ratio->Divide(h_num); //DELETE!!

  TH1D* h_ratio = (TH1D*)h_num->Clone();
  h_ratio->Divide(h_den);

  TString ratio_title = tag_num+"/"+tag_den;
  if(GO_->Get_overwrite_ratio_name()) ratio_title = GO_->Get_ratio_name();

  h_ratio->GetYaxis()->SetTitle(ratio_title);
  h_ratio->SetTitle("");

  string ratio_name = "ratio_"  + tag_num + "_" + tag_den + "_" + sel_name;
  h_ratio->SetName(ratio_name.c_str());
  
  h_ratio->SetMaximum(GO_->Get_maximum_ratio());
  h_ratio->SetMinimum(GO_->Get_minimum_ratio());

  if(ratio_options) draw_options_ratio = this->GO_->Get_draw_options_ratio().at(0);
  else  draw_options_ratio = "P";
  
  if(draw_options_ratio =="E" or draw_options_ratio =="P") 
    {
      h_ratio->SetMarkerStyle(8);
      h_ratio->SetMarkerSize(0.5);
    }
  draw_options_ratio += " same";
  
  if(ratio_colors)
    {
      color_ratio = this->GO_->Get_colors_ratio().at(0);
      h_ratio->SetLineColor(color_ratio);
      h_ratio->SetMarkerColor(color_ratio);
    }

  h_ratio->GetYaxis()->SetNdivisions(3);
  h_ratio->GetYaxis()->SetTitleSize(0.1);
  h_ratio->GetYaxis()->SetTitleOffset(0.25);
  h_ratio->GetYaxis()->SetLabelSize(0.1);

  h_ratio->GetXaxis()->SetLabelSize(0.1);
  h_ratio->GetXaxis()->SetTitleSize(0.12);
  h_ratio->GetXaxis()->SetTitleOffset(0.8);

  this->Add_ratio_to_map(var_name, sel_name, ratio_name, h_ratio);

  TH1D *errors;
  if(draw_stats)
    {
      errors = (TH1D*)h_den->Clone();
      errors->Divide(h_den);
      errors->SetFillColor(kRed-9);
      //errors->SetFillStyle(3104);
      int nbins = h_den->GetNbinsX();
      Double_t err_bins[nbins];
      this->CalculateStatUncertanties(h_den, err_bins);
      errors->SetError(err_bins);
      leg->AddEntry(errors, "Stat uncertanties");
    }
  
  h_ratio->Draw(draw_options_ratio);
  if(draw_stats) 
    {
      errors->Draw("E3 same");      
      h_ratio->Draw(draw_options_ratio);
    }
  

  c1->cd();
  Top_pad->cd();
  leg->Draw();

  c1->cd();
  c1->SaveAs(("Stack_" + var_name + "_" + sel_name +".pdf").c_str());

}


void HistoMaker::DrawDataDnnBkg_Ratio(string var_name, string sel_name, vector<string> chain_tags, bool draw_obs_sign)
{
  this->isStack_ = false;

  bool norm_all_histos = this->Get_NormalizeHistos();
  bool norm_single_histo = this->Get_NormalizeHisto();

  if(norm_single_histo) this->ApplyNormalizations();
  if(norm_all_histos)
    {
      double Norm = this->normalizeHistos_;
      for(auto histo_tag : chain_tags) this->NormalizeHisto(this->Get_histo(var_name, sel_name, histo_tag), Norm);
    }

  this->WriteInfoFile(var_name, sel_name, false);
  
  TCanvas *c = new TCanvas("","",  700, 700);

  gStyle->SetOptStat(0); 
  gStyle->SetOptTitle(0);

  TPad *Top_pad;
  TPad *Bottom_pad_up;
  TPad *Bottom_pad_down;
  
  if(draw_obs_sign)
    {
      Top_pad = new TPad("", "", 0,  0.4,  1, 1);
      Bottom_pad_up = new TPad("","", 0,  0.2,  1, 0.45);
      Bottom_pad_down = new TPad("","", 0., 0.,   1, 0.27);

      Top_pad->SetTickx(false);
      Top_pad->SetTicky(false);
      //Top_pad->SetTopMargin(0.05);
      //Top_pad->SetBottomMargin(0.02);
      Top_pad->SetLeftMargin(0.14);
      Top_pad->SetRightMargin(0.05);
      Top_pad->SetFrameBorderMode(0);
  
      Bottom_pad_up->SetTickx(false);
      Bottom_pad_up->SetTicky(false);
      //Bottom_pad_up->SetTopMargin(0.5);
      Bottom_pad_up->SetBottomMargin(0.4);
      Bottom_pad_up->SetLeftMargin(0.14);
      Bottom_pad_up->SetRightMargin(0.05);
      Bottom_pad_up->SetFrameBorderMode(0);

      Bottom_pad_down->SetTickx(false);
      Bottom_pad_down->SetTicky(false);
      //Bottom_pad_down->SetTopMargin(0.06);
      Bottom_pad_down->SetBottomMargin(0.4);
      Bottom_pad_down->SetLeftMargin(0.14);
      Bottom_pad_down->SetRightMargin(0.05);
      Bottom_pad_down->SetFrameBorderMode(0);

    }
  else
    {
      Top_pad = new TPad("","",0,0.35,1,1);
      Bottom_pad_up = new TPad("","",0,0,1,0.35);
 
      Top_pad->SetTickx(false);
      Top_pad->SetTicky(false);
      Top_pad->SetTopMargin(0.05);
      Top_pad->SetBottomMargin(0.011);
      Top_pad->SetLeftMargin(0.14);
      Top_pad->SetRightMargin(0.05);
      Top_pad->SetFrameBorderMode(0);
      Top_pad->SetTopMargin(0.06);

      Bottom_pad_up->SetTickx(false);
      Bottom_pad_up->SetTicky(false);
      Bottom_pad_up->SetTopMargin(0.06);
      Bottom_pad_up->SetBottomMargin(0.4);
      Bottom_pad_up->SetLeftMargin(0.14);
      Bottom_pad_up->SetRightMargin(0.05);
      Bottom_pad_up->SetFrameBorderMode(0);

    }
  
  Top_pad->Draw();
  Bottom_pad_up->Draw();
  if(draw_obs_sign) Bottom_pad_down->Draw();

  this->GO_->Draw_Options();
  c->cd();
  Top_pad->cd();
  
  double max = -9.;
  double min = -9.;
  for(auto tag: chain_tags)
    {

      double tmp_max = this->Get_histo(var_name, sel_name, tag)->GetMaximum();
      double tmp_min = this->Get_histo(var_name, sel_name, tag)->GetMinimum();
      if(tmp_max>max) max = tmp_max;
      if(tmp_min<min) min = tmp_min;
    }

  
  double interval = TMath::Abs(max-min);
  double max_range = max + interval*0.5;
  double min_range = this->GO_->Get_minimum();

  if(this->GO_->Get_logy())
    {
      min_range += interval/100000.;
      max_range = max + interval*2;
      Top_pad->SetLogy();
    }

  if(this->GO_->Get_logx()) Top_pad->SetLogx();

  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();


  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  X_title = reader_->Get_vars_Xtitle(var_name);
  Y_title = "Entries";


  TString draw_options;
  Color_t color;
  vector<TString> options;

  bool histo_options = this->GO_->Get_overwrite_Draw_Options();
  bool histo_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Draw_Options() and (this->GO_->Get_draw_options().size() != chain_tags.size()))
    {
      WARNING("Number of drawing options are different from number of chains to draw (excluding stack)!!");
      histo_options = false;
    }

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != chain_tags.size()))
    {
      WARNING("Number of drawing colors are different from number of chains to draw (excluding stack)!!");
      histo_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != chain_tags.size())
    {
      WARNING("Number of legend entries are different from number of chains to draw (excluding stack)!!");
      overwrite_leg = false;
    }


  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  leg->SetBorderSize(0);
  TString leg_entry;


  if(isScaleBin)
    {
      double bin_width = (double) this->Get_histo(var_name, sel_name, chain_tags.at(0))->GetBinWidth(2);
      std::stringstream stream;
      stream << std::fixed << std::setprecision(0) << bin_width;
      std::string s = stream.str();

      (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
    }

  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

  //start to modify from here
  int g = 0;
  for(auto tag : chain_tags)
    {

      if(isScaleBin and !(this->GO_->Get_overwrite_y_name()))
        {
          double bin_width = (double) this->Get_histo(var_name, sel_name, tag)->GetBinWidth(1);
	  std::stringstream stream;
          stream << std::fixed << std::setprecision(2) << bin_width;
	  std::string s = stream.str();

          (reader_->Get_varbins(var_name)) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " GeV").c_str());
        }

      if(histo_options)
        {
          options.push_back(this->GO_->Get_draw_options().at(g));
        }
      else if(tag == "Data")
	{
	  options.push_back("P");
	}
      else options.push_back("hist");

      if(options.at(g)=="E" or options.at(g)=="P") this->Get_histo(var_name, sel_name, tag)->SetMarkerStyle(20);

      draw_options = "same " + options.at(g);

      if(histo_colors)
        {
          color = this->GO_->Get_colors().at(g);
          this->Get_histo(var_name,sel_name, tag)->SetLineColor(color);
        }
      else draw_options += "PLC";

      this->Get_histo(var_name,sel_name, tag)->SetMaximum(max_range);
      this->Get_histo(var_name,sel_name, tag)->SetMinimum(min_range);
      this->Get_histo(var_name,sel_name, tag)->GetXaxis()->SetLabelOffset(9999);
      this->Get_histo(var_name,sel_name, tag)->GetXaxis()->SetLabelSize(0);
      this->Get_histo(var_name,sel_name, tag)->GetYaxis()->SetTitle(Y_title);
      this->Get_histo(var_name,sel_name, tag)->GetXaxis()->SetTitle(X_title);
      this->Get_histo(var_name,sel_name, tag)->GetXaxis()->SetTitleSize(0);    
      // this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetTitleSize(0.1);
      //this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetTitleOffset(0.3);
      //this->Get_histo(var_name, sel_name, tag)->GetYaxis()->SetLabelSize(0.1);

      if(this->GO_->Get_overwrite_histo_name()) this->Get_histo(var_name,sel_name, tag)->SetTitle(this->GO_->Get_histo_name());

      leg_entry = tag.c_str();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(g);

      if(tag == "Data")
	{
	  TString draw_options_bkg = "same hist";
	  string sel_name_bkg = sel_name + "_NominalRew";
	  leg->AddEntry(this->Get_histo(var_name,sel_name_bkg, tag), "DNN-reweighted Bkg");
	  double data_integral = this->Get_histo(var_name,sel_name, tag)->Integral();
	  if(this->Get_ScaletoBinWidth())
	    {
	      data_integral = this->Get_histo(var_name,sel_name, tag)->Integral("width");
	    }
	  this->NormalizeHisto(this->Get_histo(var_name, sel_name_bkg, tag), data_integral); //normalize bkg to data
	  this->Get_histo(var_name, sel_name_bkg, tag)->GetYaxis()->SetTitle(Y_title);
	  this->Get_histo(var_name,sel_name_bkg, tag)->SetMaximum(max_range);
	  this->Get_histo(var_name,sel_name_bkg, tag)->SetMinimum(min_range);
	  //this->Get_histo(var_name, sel_name_bkg, tag)->GetYaxis()->SetTitleSize(0.1);
	  //this->Get_histo(var_name, sel_name_bkg, tag)->GetYaxis()->SetTitleOffset(0.3);
	  //this->Get_histo(var_name, sel_name_bkg, tag)->GetYaxis()->SetLabelSize(0.1);
	  this->Get_histo(var_name, sel_name_bkg, tag)->GetXaxis()->SetLabelSize(0);
	  this->Get_histo(var_name, sel_name_bkg, tag)->SetLineColor(kBlack);
	  this->Get_histo(var_name, sel_name_bkg, tag)->Draw(draw_options_bkg);
	} 

      leg->AddEntry(this->Get_histo(var_name,sel_name, tag), leg_entry);
      this->Get_histo(var_name,sel_name, tag)->Draw(draw_options);

      g++;
    }

  leg->Draw();

  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->cd();
  Bottom_pad_up->cd();

  gPad->SetGridy(1);
  if(this->GO_->Get_logx()) Bottom_pad_up->SetLogx();

  TString draw_options_ratio;
  Color_t color_ratio;

  bool ratio_options = this->GO_->Get_overwrite_Draw_Options_ratio();
  bool ratio_colors = this->GO_->Get_overwrite_Colors_ratio();

  TH1D* h_num = this->Get_histo(var_name,sel_name,"Data");
  string sel_name_bkg = sel_name + "_NominalRew";
  TH1D* h_den = this->Get_histo(var_name,sel_name_bkg,"Data");
 
  TH1D* h_ratio = (TH1D*)h_num->Clone();
  h_ratio->Divide(h_den);

  TString ratio_title = "Data/Bkg";
  if(GO_->Get_overwrite_ratio_name()) ratio_title = GO_->Get_ratio_name();

  h_ratio->GetYaxis()->SetTitle(ratio_title);
  h_ratio->SetTitle("");

  string ratio_name = "ratio_Data_Bkg_" + sel_name;
  h_ratio->SetName(ratio_name.c_str());

  h_ratio->SetMaximum(GO_->Get_maximum_ratio());
  h_ratio->SetMinimum(GO_->Get_minimum_ratio());

  if(ratio_options) draw_options_ratio = this->GO_->Get_draw_options_ratio().at(0);
  else  draw_options_ratio = "P";

  if(draw_options_ratio =="E" or draw_options_ratio =="P")
    {
      h_ratio->SetMarkerStyle(8);
      h_ratio->SetMarkerSize(0.5);
    }
  draw_options_ratio += " same";

  if(ratio_colors)
    {
      color_ratio = this->GO_->Get_colors_ratio().at(0);
      h_ratio->SetLineColor(color_ratio);
      h_ratio->SetMarkerColor(color_ratio);
    }

  h_ratio->GetYaxis()->SetNdivisions(5);
  h_ratio->GetYaxis()->SetTitleSize(0.1);
  h_ratio->GetYaxis()->SetTitleOffset(0.3);
  h_ratio->GetYaxis()->SetLabelSize(0.1);

  //h_ratio->GetXaxis()->SetTitle(h_num->GetXaxis()->GetTitle());
  h_ratio->GetXaxis()->SetLabelSize(0);

  this->Add_ratio_to_map(var_name, sel_name, ratio_name, h_ratio);

  h_ratio->Draw(draw_options_ratio);

  c->cd();
  
  if(draw_obs_sign)
    {
      
      Bottom_pad_down->cd();
      if(this->GO_->Get_logx()) Bottom_pad_down->SetLogx();

      TH1D* h_sig = this->Get_histo(var_name,sel_name,"Data");
      TH1D* h_bkg = this->Get_histo(var_name,sel_name_bkg,"Data");

      bool isVarBin = reader_->Get_varbins(var_name);	
      int NBins;
      
      TH1D *h_S = nullptr;
      double max_x = 0;
      if(isVarBin) 
	{
	  auto vec_binning = reader_->Get_binning(var_name);
	  NBins = vec_binning.size() - 1;
	  Double_t binning[NBins+1];
	  for(int ib=0; ib<vec_binning.size(); ib++) binning[ib] = vec_binning.at(ib);
	  max_x = binning[vec_binning.size() - 1];
	  const Double_t *Binning = binning;
	  h_S = new TH1D("h_S", "", NBins, Binning);
	}
      else
	{
	  double A = (this->reader_->Get_vars_range(var_name)).first;
	  double B = (this->reader_->Get_vars_range(var_name)).second;
	  NBins = this->reader_->Get_vars_Nbins(var_name);
	  h_S = new TH1D("h_S", "", NBins, A, B);
	  max_x = B;
	  
	}

      
      for(int b=1;b<=NBins;b++)
	{
	  if(h_bkg->GetBinContent(b)>0)
	    {
	      h_S->SetBinContent(b, ((double)h_sig->GetBinContent(b)-(double)h_bkg->GetBinContent(b))/(double)sqrt(h_bkg->GetBinContent(b)));
	    }
	  else h_S->SetBinContent(b, 0);
	 
	}
      
      
      h_S->SetTitle("");
      
      
      TString draw_options_sig = "same hist";
      h_S->SetLineColor(kRed);
      h_S->SetFillColor(kRed);
  
      string sig_name = "Obs_sig_" + sel_name;
      h_S->SetName(sig_name.c_str());
      
      h_S->SetMaximum(int(h_S->GetMaximum()+1.));
      this->Add_histo_to_map("obs_sign_" + var_name, sel_name, "Data", h_S);
      h_S->GetYaxis()->SetTitle("obs sig");
      h_S->GetYaxis()->SetNdivisions(5);
      h_S->GetYaxis()->SetTitleSize(0.1);
      h_S->GetYaxis()->SetTitleOffset(0.3);
      h_S->GetYaxis()->SetLabelSize(0.1);

      h_S->GetXaxis()->SetTitle(h_sig->GetXaxis()->GetTitle());
      h_S->GetXaxis()->SetTitleOffset(0.8);
      h_S->GetXaxis()->SetLabelSize(0.1);
      h_S->GetXaxis()->SetTitleSize(0.12);
      
      h_S->Draw(draw_options_sig);
            
    }
    c->SaveAs(("Data_DNNBkg_" + var_name + "_" + sel_name +".pdf").c_str());
}

void HistoMaker::DrawStacks()
{

  vector<string> vars_name = this->reader_->Get_vars_name();
  vector<string> sel_tags = this->reader_->Get_sel_tags();
  vector<string> file_tags = this->Get_chain_tags();

  this->MakeHistos(vars_name, sel_tags, file_tags);

  for(auto var : vars_name)
    {
      cout<<endl;
      cout<<" PROCESSING VARIABLE: " << var << endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");


      for(auto sel : sel_tags)
        {
          cout<<" PROCESSING SELECTION: " <<sel << endl;

	  if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
          else
            {
              string new_sel = GO_->Get_new_selection();
              GO_->Set_selection(new_sel);
            }

	  bool isData = true;
	  if(this->map_chains_.find("Data") == map_chains_.end()) isData = false;

	  if(isData) this->DrawStack_Ratio(var, sel, file_tags);
          else this->DrawStack(var, sel, file_tags);

	  file_out->cd();

          for(auto tag: file_tags)
            {
              //this->map_histos_raw_[var][sel][tag]->Write();
              this->Get_histo(var, sel, tag)->Write();	      
            }

	  this->Get_stack(var, sel)->Write();
	  this->Get_histo(var, sel, "Bkg")->Write();
	  string ratio_name = "ratio_Data_Bkg_" + sel;
	  if(isData) this->Get_ratio(var, sel, ratio_name)->Write();
        }

      file_out->Close();
    }

}

void HistoMaker::DrawDataDNNBkgPlots(bool draw_obs_sign)
{
  vector<string> vars_name = this->reader_->Get_vars_name();
  vector<string> sel_tags = this->reader_->Get_sel_tags();
  vector<string> file_tags = this->Get_chain_tags();

  this->MakeHistos(vars_name, sel_tags, file_tags);
 
  for(auto var : vars_name)
    {
      cout<<endl;
      cout<<" PROCESSING VARIABLE: " << var << endl;

      string file_name = "file_"+var+".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");


      for(auto sel : sel_tags)
        {
	  if ((strstr(sel.c_str(),"NominalRew")!=NULL) || (strstr(sel.c_str(),"VariatedRew")!=NULL))
	    {
	      continue;
	    }

          cout<<" PROCESSING SELECTION: " <<sel << endl;

          if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
          else
            {
              string new_sel = GO_->Get_new_selection();
              GO_->Set_selection(new_sel);
            }

          bool isData = true;
          if(this->map_chains_.find("Data") == map_chains_.end()) isData = false;
	  
          if(isData) this->DrawDataDnnBkg_Ratio(var, sel, file_tags, draw_obs_sign);
          else 
	    {
	      ERROR("NO DATA TO DRAW");
	      exit(EXIT_FAILURE);
	    }

          file_out->cd();

          for(auto tag: file_tags)
            {
              this->Get_histo(var, sel, tag)->Write();
            }

          string ratio_name = "ratio_Data_Bkg_" + sel;
	  this->Get_ratio(var, sel, ratio_name)->Write();
	  if(draw_obs_sign) this->Get_histo("obs_sign_" + var, sel, "Data")->Write();
	}

      file_out->Close();
    }

}

void HistoMaker::CalculateStatUncertanties(TH1D* h, Double_t *err_bins)
{
  int nbins = h->GetNbinsX();

  for(int i=1; i<=nbins;i++)
    {
      double err = (1/((double)h->GetBinContent(i)));
      if(this->Get_ScaletoBinWidth()) err = (1/((double)h->GetBinContent(i)*h->GetBinWidth(i)));

      err = sqrt(err);		    
      err_bins[i] = err;
    }
}

void HistoMaker::CalculateRatioUncertanties(TH1D* h_num, TH1D* h_den, double integral_num=1, double integral_den=1, TH1D* h_ratio=NULL,  Double_t *err_bins=NULL)
{
  int nbins = h_ratio->GetNbinsX();

  for(int i=1; i<=nbins;i++)
    {
      double err = 0;
      if((h_num->GetBinContent(i)!=0) and (h_den->GetBinContent(i)!=0))
	{
	  
	  if(this->Get_ScaletoBinWidth())
	    {
	      err = ((double)h_ratio->GetBinContent(i)) * sqrt((double)(1/(h_num->GetBinContent(i)*integral_num*h_num->GetBinWidth(i))) +(double)(1/(h_den->GetBinContent(i)*integral_den*h_den->GetBinWidth(i))));
	    }
	  else
	    {
	      err = ((double)h_ratio->GetBinContent(i)) * sqrt((double)(1/(h_num->GetBinContent(i)*integral_num)) +(double)(1/(h_den->GetBinContent(i)*integral_den)));
	    }
	}
      err_bins[i] = err;
    }
}

void HistoMaker::PropagateScaleFactorUncertanty(TH1D* h, Double_t *err_bins, double SF, double err_SF)
{
  int nbins = h->GetNbinsX();

  for(int i=1; i<=nbins;i++)
    {
      double bin = (double)h->GetBinContent(i);
      if(this->Get_ScaletoBinWidth()) bin = ((double)h->GetBinContent(i)*h->GetBinWidth(i));
      double err = bin * (SF + bin*((double)(err_SF/SF))*((double)(err_SF/SF)));
      err = sqrt(err);
      err_bins[i] = err;
    }
}

void HistoMaker::DrawRatio2D(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names1, vector<string> var_names2, vector<string> chain_tags)
{
  if(sel_names_num.size() != sel_names_den.size())
    {
      ERROR("Vector with selections have different lenght between num and den");
      exit(EXIT_FAILURE);
    }

  if(var_names1.size() != var_names2.size())
    {
      ERROR("Vector with variables (for 2D histos) have different lenghts between X and Y coordinates!");
      exit(EXIT_FAILURE);
    }
  
  TString draw_option = "colz";
  bool overwrite_options = GO_->Get_overwrite_2D_draw_option();

  if(overwrite_options) draw_option = GO_->Get_2D_draw_option();
  
  for(int i = 0; i<var_names1.size(); i++)
    {
      string var1 = var_names1.at(i);
      string var2 = var_names2.at(i);
      
      for(int j = 0; j< sel_names_num.size(); j++)
	{

          string sel_num = sel_names_num.at(j);
          string sel_den = sel_names_den.at(j);

          for(auto tag : chain_tags)
            {
              TCanvas *c = new TCanvas();
              GO_->Draw_Options();

              TH2D *ratio2D = this->MakeRatio2D(var1, var2, sel_num, sel_den, tag);

	      if(this->GO_->Get_overwrite_Y_minimum_2D()) ratio2D->SetMinimum( this->GO_->Get_minimum_2D());
	      if(this->GO_->Get_overwrite_Y_maximum_2D()) ratio2D->SetMaximum( this->GO_->Get_maximum_2D());

	      //if(overwrite_options) ratio2D->SetMarkerColor(kWhite);
              ratio2D->Draw(draw_option);
	      // ratio2D->GetZaxis()->SetLabelSize(0.02);
	      //ratio2D->GetZaxis()->SetLabelOffset(0.1);
              c->SaveAs(("Ratio2D_" + var1+"_"+var2 +"_"+sel_num+"_"+ sel_den + ".pdf").c_str());
	      
            }
        }
      
    }
  
  string file_name = "file_Ratio2D.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  for(int i = 0; i<var_names1.size(); i++)
    {
      string var1 = var_names1.at(i);
      string var2 = var_names2.at(i);

      for(int j = 0; j< sel_names_num.size(); j++)
        {

          string sel_num = sel_names_num.at(j);
          string sel_den = sel_names_den.at(j);

          for(auto tag : chain_tags)
            {
              string name = "Ratio_" + sel_num + "_" + sel_den +"_" + var1 + "_" + var2 + "_" + tag;
              this->Get_ratio2D(name)->Write();
            }
        }
    }
  file_out->Close();
}

void HistoMaker::DrawPulls_2D(vector<string> var_names1, vector<string> var_names2, vector<string> sel_names_num, vector<string> sel_names_den,  vector<string> chain_tags, double number)
{
  if(sel_names_num.size() != sel_names_den.size())
    {
      ERROR("Vector with selections have different lenght between num and den");
      exit(EXIT_FAILURE);
    }
  
  if(var_names1.size() != var_names2.size())
    {
      ERROR("Vector with variables (for 2D histos) have different lenghts between X and Y coordinates!");
      exit(EXIT_FAILURE);
    }

  TString draw_option = "colz";
  bool overwrite_options = GO_->Get_overwrite_2D_draw_option();

  if(overwrite_options) draw_option = GO_->Get_2D_draw_option();

  for(int i = 0; i<var_names1.size(); i++)
    {
      string var1 = var_names1.at(i);
      string var2 = var_names2.at(i);

      for(int j = 0; j< sel_names_num.size(); j++)
	{
	  
	  string sel_num = sel_names_num.at(j);
          string sel_den = sel_names_den.at(j);
	  
	  for(auto tag : chain_tags)
	    {
	      TCanvas *c = new TCanvas();
	      GO_->Draw_Options();

	      pair<TH2D*, double> Pulls = this->MakePulls2D(var1, var2, sel_num, sel_den, tag, number);
	      TH2D *pulls2D = Pulls.first;
	      double Chi2rid = Pulls.second;
	      
	      if(this->GO_->Get_overwrite_Y_minimum_2D()) pulls2D->SetMinimum( this->GO_->Get_minimum_2D());
	      if(this->GO_->Get_overwrite_Y_maximum_2D()) pulls2D->SetMaximum( this->GO_->Get_maximum_2D());
	      
	      if(overwrite_options) pulls2D->SetMarkerColor(kWhite);                                                                                                                                      
	      pulls2D->Draw(draw_option);
	      //pulls2D->GetZaxis()->SetLabelSize(0.02);                                                                                                                                                   
	      //pullls2D->GetZaxis()->SetLabelOffset(0.1);                                                                                                                                                   
	      c->SaveAs(("Pulls2D_" + var1+"_"+var2 +"_"+sel_num + "_" + sel_den +  ".pdf").c_str());
	      
	      std::stringstream Stream;
	      Stream << std::fixed << std::setprecision(4) << Chi2rid;
	      std::string chisquare_rid = Stream.str();
	      cout << "Plot 2D -> " << var1 << ", " << var2 << ", " << sel_num << " " << sel_den <<  "--> Chi^{2} / Ndof: " << chisquare_rid << endl; 
	    }

	}
      
    }

}

pair<TH2D*,double> HistoMaker::MakePulls2D(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag, double number)
{
  string name = "Ratio_" + sel_name_num + "_" + sel_name_den + "_" + var_name1 + "_" + var_name2 + "_" + chain_tag;
  
  TFile *file = TFile::Open("file_Ratio2D.root");
  TH2D *histo = (TH2D*)file->Get(name.c_str());

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(0) << number;
  std::string Constant = Stream.str();


  string pulls_name = "Pulls_" + Constant + "_" +  sel_name_num + "_" +  sel_name_den +  "_" + var_name1 + "_" + var_name2 + "_" + chain_tag;
  string pulls_title = "Pulls wrt " + Constant + " " +  sel_name_num + "_" +  sel_name_den + ";" + var_name2 + ";" +  var_name1;
  
  double nbinsX = histo->GetNbinsX();
  double nbinsY = histo->GetNbinsY();
  double NBins = nbinsX * nbinsY;

  double Chi2 = 0;
  double ndof = 0;

  TH2D *new_histo = (TH2D*)histo->Clone();
  TH2D *const_histo = (TH2D*)histo->Clone();

  for(int b=1;b<=NBins;b++) 
    {
      if((histo->GetBinContent(b)) != 0) const_histo->SetBinContent(b, number);
    }

  new_histo->Add(const_histo, -1);

  TH2D *new_histo_abs = (TH2D*)new_histo->Clone();
  for(int b=1;b<=NBins;b++)
    {
      new_histo_abs->SetBinContent(b, abs(new_histo->GetBinContent(b)));
      Chi2 += pow(abs(new_histo->GetBinContent(b)), 2);
    }

  Chi2 = sqrt(Chi2);
  ndof = NBins;

  double chi2rid = (double)(Chi2/ndof);
  
  new_histo_abs->SetName(pulls_name.c_str());
  new_histo_abs->SetTitle(pulls_title.c_str());

  pair<TH2D*,double> pulls_pair = make_pair(new_histo_abs, chi2rid);

  return pulls_pair;

}



TH2D* HistoMaker::MakeRatio2D(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag)
{
  string name_num = "histo2D_" + var_name1 + "_" + var_name2 +  "_" + sel_name_num + "_" + chain_tag;
  string name_den = "histo2D_" + var_name1 + "_" + var_name2 +  "_" + sel_name_den + "_" + chain_tag;

  TFile *file_num = TFile::Open(("file_2D_" + sel_name_num + "_" + chain_tag + "_" + var_name1 + "_" + var_name2  + ".root").c_str());
  TH2D *histo_num = (TH2D*)file_num->Get(name_num.c_str());


  TFile *file_den =TFile::Open(("file_2D_" + sel_name_den + "_" + chain_tag + "_" + var_name1 + "_" + var_name2  + ".root").c_str());
  TH2D *histo_den = (TH2D*)file_den->Get(name_den.c_str());

  string name = "Ratio_" + sel_name_num + "_" + sel_name_den + "_" + var_name1 + "_" + var_name2 + "_" + chain_tag;
  string title = "Ratio " + sel_name_num + " " +  sel_name_den + ";" + var_name2 + ";" +  var_name1;

  double nbinsX_num = histo_num->GetNbinsX();
  double nbinsY_num = histo_num->GetNbinsY();
  double nbinsX_den = histo_den->GetNbinsX();
  double nbinsY_den = histo_den->GetNbinsY();
  if((nbinsX_num != nbinsX_den) or (nbinsY_num != nbinsY_den))
    {
      ERROR("The number of bins is different between num and den 2D histos!!");
      exit(EXIT_FAILURE);
    }
  
  TH2D *ratio = (TH2D*)histo_num->Clone();
  ratio->Divide(histo_den);

  ratio->SetName(name.c_str());
  ratio->SetTitle(title.c_str());

  this->Add_ratio2D_to_map(name, ratio);

  return ratio;

}


void HistoMaker::CompareHistos_fromFile(vector<string> file_names, vector<string> histo_names, vector<string> vars_to_compare)
{
  for(auto var : vars_to_compare)
    {

      TCanvas *c = new TCanvas();
      GO_->Draw_Options();
  
      bool histo_options = this->GO_->Get_overwrite_Draw_Options();
      bool histo_colors = this->GO_->Get_overwrite_Colors();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

      if(GO_->Get_overwrite_selection())
	{
	  string new_sel = GO_->Get_new_selection();
	  GO_->Set_selection(new_sel);
	}
      
      if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < histo_names.size())
	{
	  WARNING("Number of drawing colors are different less than number of histos to draw!!");
	  histo_colors = false;
	}

      if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != histo_names.size())
	{
	  WARNING("Number of legend entries are different from number of histos to draw!!");
	  overwrite_leg = false;
	}


      double xmin = this->GO_->Get_leg_xmin();
      double xmax = this->GO_->Get_leg_xmax();
      double ymin = this->GO_->Get_leg_ymin();
      double ymax = this->GO_->Get_leg_ymax();
    
      TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
      leg->SetBorderSize(0);
      TString leg_entry;
      vector<TString> options;

      vector<TH1D*> histos;
      TH1D *h =  nullptr;
      TFile *file = nullptr;
      
      int i = 0;

      for(auto name : histo_names)
	{
	  
	  TString draw_options = "";
	  TString file_name = (file_names.at(i) + "file_" + var + ".root").c_str();

          file = TFile::Open(file_name);
          if (!file->IsOpen())
            {
              WARNING("File with name " << file_name << " does not exist!" << endl);
              continue;
            }


	  cout << "OPENING HISTO -> " << name << endl;

          h = (TH1D*)(file->Get(name.c_str()))->Clone();

          histos.push_back(h);
	  if(this->GO_->Get_logy()) c->SetLogy();

	  if(this->GO_->Get_overwrite_y_name())
            {
              TString X_title = this->GO_->Get_X_axis_name();
              h->GetXaxis()->SetTitle(X_title);
            }
	  if(this->GO_->Get_overwrite_y_name())
	    {
	     TString Y_title = this->GO_->Get_Y_axis_name();
	      h->GetYaxis()->SetTitle(Y_title);
	    }


	  double max_range, min_range;
          if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
          if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

	  leg_entry = name.c_str();
          if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

	  leg->AddEntry(h, leg_entry);
	  
	  if(histo_options)
            {
              options.push_back(this->GO_->Get_draw_options().at(i));
              if(options.at(i).Contains("P")) h->SetMarkerStyle(20);
            }
          else options.push_back("");

          draw_options = "same " + options.at(i);

	  if(histo_colors)
	  {
	    Color_t color = this->GO_->Get_colors().at(i);
	    h->SetLineColor(color);
	    if(strcmp(options.at(i), "E2")==0) 
	    {
	      h->SetFillColor(color);
	      h->SetFillStyle(1001);
	      h->SetFillColorAlpha(color,0.3);
	    }
	    else h->SetFillColor(kWhite);
	  }
	  else draw_options += " PLC";
	  
	  h->SetTitle(var.c_str());
	  h->GetYaxis()->SetTitleSize(0.04);
	  h->GetYaxis()->SetTitleOffset(0.5);
	  h->GetYaxis()->SetLabelSize(0.02);
	  h->GetXaxis()->SetTitleSize(0.04);
          h->GetXaxis()->SetTitleOffset(0.7);
          h->GetXaxis()->SetLabelSize(0.02);

	  if(this->GO_->Get_overwrite_Y_maximum()) h->SetMaximum(max_range);
          if(this->GO_->Get_overwrite_Y_minimum()) h->SetMinimum(min_range);

	  
	  h->Draw(draw_options);
	  i++;
	}
      leg->Draw("same");
      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();      
      
      c->SaveAs((var + "_comparedFromFile.pdf").c_str());

    }
}

void HistoMaker::CompareHistos_fromFile(vector<string> file_names, vector<string> histo_names,vector<string>histo_num, vector<string>histo_den,  vector<string> vars_to_compare, vector<string> histo_signals, vector<string> histo_bkg)
{
  for(auto var : vars_to_compare)
    {
      

      TCanvas *c = new TCanvas();
      GO_->Draw_Options();
  
      bool histo_colors = this->GO_->Get_overwrite_Colors();
      bool histo_options = this->GO_->Get_overwrite_Draw_Options();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
      
      if(strstr(histo_names.at(0).c_str(),"SB1")!=NULL )GO_->Set_selection("SB1");
      if(strstr(histo_names.at(0).c_str(),"SB2")!=NULL )GO_->Set_selection("SB2");
      if(strstr(histo_names.at(0).c_str(),"CR0")!=NULL )GO_->Set_selection("CR0");
      if(strstr(histo_names.at(0).c_str(),"CR1")!=NULL )GO_->Set_selection("CR1");
      if(strstr(histo_names.at(0).c_str(),"CR2")!=NULL )GO_->Set_selection("CR2");
      if(strstr(histo_names.at(0).c_str(),"SR")!=NULL )GO_->Set_selection("SR");


      if(GO_->Get_overwrite_selection())
	{
	  string new_sel = GO_->Get_new_selection();
	  GO_->Set_selection(new_sel);
	}

      
      if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < histo_names.size())
	{
	  WARNING("Number of drawing options are less than number of chains!!");
	  histo_options = false;
	}      

      if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < histo_names.size())
	{
	  WARNING("Number of drawing colors are less than number of histos to draw!!");
	  histo_colors = false;
	}

      if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != histo_names.size())
	{
	  WARNING("Number of legend entries are different from number of histos to draw!!");
	  overwrite_leg = false;
	}


      double xmin = this->GO_->Get_leg_xmin();
      double xmax = this->GO_->Get_leg_xmax();
      double ymin = this->GO_->Get_leg_ymin();
      double ymax = this->GO_->Get_leg_ymax();

      vector<TH1D*> histos;
      TH1D *h =  nullptr;
      TFile *file = nullptr;

      TPad *Top_pad = Create_Top_Pad(0, 0.2, 1, 1);
      Top_pad->cd();
     
      gPad->SetGridy(1);

      TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
      leg->SetBorderSize(0);
      TString leg_entry;
      vector<TString> options;
      
      int i = 0;
      cout<<endl;
      for(auto name : histo_names)
	{

	  
	  TString draw_options = "";
	  TString file_name = (file_names.at(i) + "file_" + var + ".root").c_str();

	  file = TFile::Open(file_name);
	  if (!file->IsOpen())
	    {
	      WARNING("File with name " << file_name << " does not exist!" << endl);
	      continue;
	    }


	  h = (TH1D*)((TH1D*)file->Get(name.c_str())->Clone());
	  
	  cout << " get histo " << h->GetName() << " from file " << file_name << endl;
	  
	  string new_ = h->GetName(); //delete                                                                                                                                                             
          TString new_name = new_.c_str(); //delete      
	  if(file_name.Contains("1p6"))                                                                                                                                                              
            {                                                                                                                                                                                             
              new_name += "_new"; //delete                                                                                                                                                                 
	      if(file_name.Contains("1p6_schemeE")) new_name += "E";
              h->SetName(new_name); //delete                                                                                                                                                               
              //cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                                 
	    } 

	  /*
	  string new_ = h->GetName(); //delete
	  TString new_name = new_.c_str(); //delete
	  if(file_name.Contains("trackjets"))
	    {
	     new_name += "_trk"; //delete
	       h->SetName(new_name); //delete
	       cout<< "new name histo -> " << h->GetName()<< endl;//delete
	    }

	  if(file_name.Contains("fatjetH_"))
	    {
	      new_name += "_H"; //delete                                                                                                                                                            
	      h->SetName(new_name); //delete                                                                                                                                                             
	      cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                               
	    }

	  if(file_name.Contains("Reweighting_NNfatjetH") and !file_name.Contains("withE"))
            {
              new_name += "_H_withM"; //delete                                                                                                                                                           
              h->SetName(new_name); //delete                                                                                                                                                               
              cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                                  
            }

	  if(file_name.Contains("fatjetsHX_"))
            {
              new_name += "_XH"; //delete                                                                                                                                                              
              h->SetName(new_name); //delete                                                                                                                                                              
              cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                                 
            }
	  */
	  histos.push_back(h);

	  if(this->GO_->Get_logy()) Top_pad->SetLogy();

	  double max_range, min_range;
	  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
	  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

	  leg_entry = name.c_str();
          if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

	  leg->AddEntry(h, leg_entry);

	  if(histo_options)
            {
              options.push_back(this->GO_->Get_draw_options().at(i));
	      if(options.at(i).Contains("P")) h->SetMarkerStyle(20);
            }
          else options.push_back("");
	  
          draw_options = "same " + options.at(i);
	 
	  h->GetYaxis()->SetNdivisions(5);


	  if(strstr(options.at(i), "TEXT")!=NULL)
	    {
	      gStyle->SetPaintTextFormat("1.4f");
	      h->SetMarkerStyle(2);
	      h->SetMarkerSize(1);
	    }
	  
	  if(histo_colors)
	  {
	    Color_t color = this->GO_->Get_colors().at(i);
	    h->SetLineColor(color);
	    h->SetMarkerColor(color);
	    if((options.at(i).Contains("E6")) || (options.at(i).Contains("E5")) || (options.at(i).Contains("E2")) )
	      {
		h->SetFillStyle(1001);
		h->SetFillColor(color);
		h->SetFillColorAlpha(color,0.3);
		h->SetMarkerSize(0.5);
		h->SetMarkerStyle(20);
	      }
	    else h->SetFillColor(kWhite);
	    //if(options.at(i)=="HIST") h->SetFillColor(color);
	    /*
	    if(strstr(h->GetName(), "Bkg")!=NULL)
	      {
		h->SetFillStyle(1001);
		h->SetFillColorAlpha(color,0.2);
		draw_options += "HIST";
		}
	    */
	  }
	  else draw_options += " PLC";

	  if(this->GO_->Get_overwrite_y_name())
            {
	      TString X_title = this->GO_->Get_X_axis_name();
              h->GetXaxis()->SetTitle(X_title);
            }
	  if(this->GO_->Get_overwrite_y_name())
	    {
	     TString Y_title = this->GO_->Get_Y_axis_name();
	      h->GetYaxis()->SetTitle(Y_title);
	    }
	  h->SetTitle(var.c_str());
	  h->GetYaxis()->SetTitleSize(0.04);
	  h->GetYaxis()->SetTitleOffset(0.5);
	  h->GetYaxis()->SetLabelSize(0.02);
	  h->GetXaxis()->SetTitleSize(0.04);
          h->GetXaxis()->SetTitleOffset(0.7);
          h->GetXaxis()->SetLabelSize(0.02);
	  if(this->GO_->Get_overwrite_Y_maximum()) h->SetMaximum(max_range);
	  if(this->GO_->Get_overwrite_Y_minimum()) h->SetMinimum(min_range);
	  h->Draw(draw_options);
	  i++;
	}
      leg->Draw("same");
      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();      

      c->cd();

      TPad *Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.29);
      Bottom_pad->cd();
      gPad->SetGridy(1);


      if(histo_num.size()!=histo_den.size())
	{
	  ERROR("Different number of histos to divide!");
	  exit(EXIT_FAILURE);
	}

      string h_num_name, h_den_name;

      vector<TString> options_ratio;
      TString draw_options_ratio;
      Color_t color_ratio;

      bool ratio_options = this->GO_->Get_overwrite_Draw_Options_ratio();
      bool ratio_colors = this->GO_->Get_overwrite_Colors_ratio();


      if(this->GO_->Get_overwrite_Draw_Options_ratio() and this->GO_->Get_draw_options_ratio().size() < (histo_num.size()))
	{
	  WARNING("Number of drawing options for ratio are less than number of ratios!!");
	  ratio_options = false;
	}

      if(this->GO_->Get_overwrite_Colors_ratio() and this->GO_->Get_colors_ratio().size() < (histo_num.size()))
	{
	  WARNING("Number of drawing colors for ratio are less than number of ratios!!");
	  ratio_colors = false;
	}

	for(int r=0; r<histo_num.size(); r++)
	  {


	    h_num_name = histo_num.at(r);
	    h_den_name = histo_den.at(r);

	    TH1D *h_num, *h_den;
	    for(auto histo : histos)
	      {
		if(strcmp(histo->GetName(), h_num_name.c_str())==0) h_num = histo; 
		if(strcmp(histo->GetName(), h_den_name.c_str())==0) h_den = histo;
	      }

	    cout << "histo num " << h_num->GetName() << " histo den " << h_den->GetName() << endl;//canc

	    if(h_num == nullptr) 
	      {
		WARNING("histo with name " <<  h_num_name << " does not exist! Ratio not drawn");
		continue;
	      }
	    if(h_den == nullptr)
              {
		WARNING("histo with name " <<  h_den_name << " does not exist! Ratio not drawn");
                continue;
              }

	    TH1D* h_ratio = (TH1D*)h_num->Clone();
	    h_ratio->Divide(h_den);

	    TString ratio_title= "";
	    if(GO_->Get_overwrite_ratio_name()) ratio_title = GO_->Get_ratio_name();

	    h_ratio->GetYaxis()->SetTitle(ratio_title);
	    h_ratio->SetTitle("");

	    h_ratio->SetMaximum(GO_->Get_maximum_ratio());
	    h_ratio->SetMinimum(GO_->Get_minimum_ratio());

	    if(ratio_options)
	      {
		options_ratio.push_back(this->GO_->Get_draw_options_ratio().at(r));
		if(options_ratio.at(r)=="E" or options_ratio.at(r)=="P") h_ratio->SetMarkerStyle(8);
	      }
	    else options_ratio.push_back("E");
	    draw_options_ratio = "same " + options_ratio.at(r);
	    if(ratio_colors)
	      {
		color_ratio = this->GO_->Get_colors_ratio().at(r);
		h_ratio->SetLineColor(color_ratio);
		h_ratio->SetMarkerSize(0.5);
		h_ratio->SetMarkerColor(color_ratio);
	      }


	    h_ratio->GetYaxis()->SetNdivisions(3);
	    h_ratio->GetYaxis()->SetTitleSize(0.1);
	    h_ratio->GetYaxis()->SetTitleOffset(0.25);
	    h_ratio->GetYaxis()->SetLabelSize(0.1);

	    h_ratio->GetXaxis()->SetLabelSize(0.1);
	    h_ratio->GetXaxis()->SetTitleSize(0.12);
	    h_ratio->GetXaxis()->SetTitleOffset(0.8);

	    h_ratio->Draw(draw_options_ratio);

	  }
	if(!histo_bkg.empty()) //for significance on ratio plot
	  {
	    if(histo_signals.size()!=histo_bkg.size())
	      {
		ERROR("Different number of histos for significance! Significance not calculated");
		exit(EXIT_FAILURE);
	      }

	    vector<TString> options_sig;
	    TString draw_options_sig;
	    Color_t color_sig;

	    bool sig_options = this->GO_->Get_overwrite_Draw_Options_sig();
	    bool sig_colors = this->GO_->Get_overwrite_Colors_sig();


	    if(this->GO_->Get_overwrite_Draw_Options_sig() and (this->GO_->Get_draw_options_sig().size() < histo_signals.size()))
	      {
		WARNING("Number of drawing options for significance are less than number of ratios!!");
		sig_options = false;
	      }

	    if(this->GO_->Get_overwrite_Colors_sig() and (this->GO_->Get_colors_sig().size() < histo_num.size()))
	      {
		WARNING("Number of drawing colors for significance are less than number of ratios!!");
		sig_colors = false;
	      }
	    
	    string h_sig_name, h_bkg_name;
	    
	    for(int r=0; r<histo_signals.size(); r++)
	      {
		
		h_sig_name = histo_signals.at(r);
		h_bkg_name = histo_bkg.at(r);
		
		TH1D *h_sig, *h_bkg;
		for(auto histo : histos)
		  {
		    if(strstr(histo->GetName(), h_sig_name.c_str())!=NULL) h_sig = histo;
		    if(strstr(histo->GetName(), h_bkg_name.c_str())!=NULL) h_bkg = histo;
		  }
	        
		if(h_sig == nullptr) 
		  {
		    WARNING("histo with name " <<  h_sig_name << " does not exist! Significance not calculated!");
		    continue;
		  }
		if(h_bkg == nullptr)
		  {
		    WARNING("histo with name " <<  h_bkg_name << " does not exist! Significance not calculated");
		    continue;
		  }
		
		bool isVarBin = reader_->Get_varbins(var);	
		int NBins;
		
		TH1D *h_S = nullptr;
		double max_x = 0;
		if(isVarBin) 
		  {
		    auto vec_binning = reader_->Get_binning(var);
		    NBins = vec_binning.size() - 1;
		    Double_t binning[NBins+1];
		    for(int ib=0; ib<vec_binning.size(); ib++) binning[ib] = vec_binning.at(ib);
		    max_x = binning[vec_binning.size() - 1];
		    const Double_t *Binning = binning;
		    h_S = new TH1D("h_S", "", NBins, Binning);
		  }
		else
		  {
		    double A = (this->reader_->Get_vars_range(var)).first;
		    double B = (this->reader_->Get_vars_range(var)).second;
		    NBins = this->reader_->Get_vars_Nbins(var);
		    h_S = new TH1D("h_S", "", NBins, A, B);
		    max_x = B;
 
		  }

		for(int b=1;b<=NBins;b++)
		  {
		    h_S->SetBinContent(b, ((double)h_sig->GetBinContent(b)/(double)sqrt(h_bkg->GetBinContent(b))) );
		  }
		
	       
		h_S->SetTitle("");
		 
		h_S->SetMaximum(GO_->Get_maximum_sig());
		h_S->SetMinimum(GO_->Get_minimum_sig());
		
		if(sig_options)
		  {
		    options_sig.push_back(this->GO_->Get_draw_options_sig().at(r));
		  }
		else 
		  {
		    options_sig.push_back("E");
		  }
		
		if(options_sig.at(r)=="E" or options_sig.at(r)=="P") h_S->SetMarkerStyle(8);
		
		draw_options_sig = "same Y+" + options_sig.at(r);
		if(sig_colors)
		  {
		    color_sig = this->GO_->Get_colors_sig().at(r);
		    h_S->SetLineColor(color_sig);
		    h_S->SetMarkerSize(0.5);
		    h_S->SetMarkerColor(color_sig);
		  }
		
		
		// scale h_S to the pad coordinates
		double scale = (GO_->Get_maximum_ratio()/ GO_->Get_maximum_sig());
		h_S->Scale(scale);
		h_S->Draw(draw_options_sig);
	
		// draw an axis on the right side
		
		auto axis = new TGaxis(max_x, GO_->Get_minimum_ratio(), max_x, GO_->Get_maximum_ratio(), GO_->Get_minimum_sig(), GO_->Get_maximum_sig(), 4, "L+");
		axis->SetLineColor(kRed);
		axis->SetTextColor(kRed);
		axis->SetLabelColor(kRed);
		string sig_title= "S/#sqrt{B}";
		axis->SetTitle(sig_title.c_str());
		axis->SetTitleSize(0.1);
		axis->SetTitleOffset(0.25);
		axis->SetLabelSize(0.1);		
		axis->Draw();
		
	      }
	  }  
	    
	c->cd();
	
	c->SaveAs((var + "_comparedFromFile.pdf").c_str());
    }
  
}


void HistoMaker::CompareRatios_fromFile(vector<string> file_names, vector<string> histo_names, float number,  vector<string> vars_to_compare)
{
  for(auto var : vars_to_compare)
    {
      

      TCanvas *c = new TCanvas();
      GO_->Draw_Options();
  
      bool histo_colors = this->GO_->Get_overwrite_Colors();
      bool histo_options = this->GO_->Get_overwrite_Draw_Options();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
      
      if(strstr(histo_names.at(0).c_str(),"SB1")!=NULL )GO_->Set_selection("SB1");
      if(strstr(histo_names.at(0).c_str(),"SB2")!=NULL )GO_->Set_selection("SB2");
      if(strstr(histo_names.at(0).c_str(),"CR0")!=NULL )GO_->Set_selection("CR0");
      if(strstr(histo_names.at(0).c_str(),"CR1")!=NULL )GO_->Set_selection("CR1");
      if(strstr(histo_names.at(0).c_str(),"CR2")!=NULL )GO_->Set_selection("CR2");
      if(strstr(histo_names.at(0).c_str(),"SR")!=NULL )GO_->Set_selection("SR");

      if(GO_->Get_overwrite_selection())
	{
	  string new_sel = GO_->Get_new_selection();
	  GO_->Set_selection(new_sel);
	}

      
      if(this->GO_->Get_overwrite_Draw_Options() and this->GO_->Get_draw_options().size() < histo_names.size())
	{
	  WARNING("Number of drawing options are less than number of chains!!");
	  histo_options = false;
	}      

      if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() < histo_names.size())
	{
	  WARNING("Number of drawing colors are different less than number of histos to draw!!");
	  histo_colors = false;
	}

      if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != histo_names.size())
	{
	  WARNING("Number of legend entries are different from number of histos to draw!!");
	  overwrite_leg = false;
	}


      double xmin = this->GO_->Get_leg_xmin();
      double xmax = this->GO_->Get_leg_xmax();
      double ymin = this->GO_->Get_leg_ymin();
      double ymax = this->GO_->Get_leg_ymax();

      vector<TH1D*> histos;
      TH1D *h =  nullptr;
      TFile *file = nullptr;

      TPad *Top_pad = Create_Top_Pad(0, 0.35, 1, 1);
      Top_pad->cd();
      
      gPad->SetGridy(1);
      //gStyle->SetTextSize(0.5);

      TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);

      TString leg_entry;
      vector<TString> options;
      
      int i = 0;
      cout<<endl;
      vector<TH1D*> Top_histos;
      for(auto name : histo_names)
	{

	  
	  TString draw_options = "";
	  TString file_name = (file_names.at(i) + "file_" + var + ".root").c_str();

	  file = TFile::Open(file_name);
	  if (!file->IsOpen())
	    {
	      WARNING("File with name " << file_name << " does not exist!" << endl);
	      continue;
	    }


	  h = (TH1D*)((TH1D*)file->Get(name.c_str())->Clone());
	  
	  cout << " get histo " << h->GetName() << " from file " << file_name << endl;
	  
	  string new_ = h->GetName(); //delete
	  TString new_name = new_.c_str(); //delete
	  if(file_name.Contains("trackjets"))
	    {
	     new_name += "_trk"; //delete
	       h->SetName(new_name); //delete
	       cout<< "new name histo -> " << h->GetName()<< endl;//delete
	    }
	  /*
	  if(file_name.Contains("fatjetH_"))
	    {
	      new_name += "_H"; //delete                                                                                                                                                            
	      h->SetName(new_name); //delete                                                                                                                                                             
	      cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                               
	    }
	  if(file_name.Contains("Reweighting_NNfatjetH") and !file_name.Contains("withE"))
            {
              new_name += "_H_withM"; //delete                                                                                                                                                                   
              h->SetName(new_name); //delete                                                                                                                                                              
              cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                                  
            }
	  
	  if(file_name.Contains("fatjetsHX_"))
            {
              new_name += "_XH"; //delete                                                                                                                                                              
              h->SetName(new_name); //delete                                                                                                                                                              
              cout<< "new name histo -> " << h->GetName()<< endl;//delete                                                                                                                                 
            }
	  */
	  histos.push_back(h);

	  if(this->GO_->Get_logy()) Top_pad->SetLogy();

	  double max_range, min_range;
	  if(this->GO_->Get_overwrite_Y_maximum()) max_range = this->GO_->Get_maximum();
	  if(this->GO_->Get_overwrite_Y_minimum()) min_range = this->GO_->Get_minimum();

	  leg_entry = name.c_str();
          if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(i);

	  leg->AddEntry(h, leg_entry);

	  if(histo_options)
            {
              options.push_back(this->GO_->Get_draw_options().at(i));
              if(options.at(i).Contains("E") or options.at(i).Contains("P")) h->SetMarkerStyle(20);
            }
          else options.push_back("hist");

          draw_options = "same " + options.at(i);
	  h->GetYaxis()->SetNdivisions(5);


	  if(strstr(options.at(i), "TEXT")!=NULL)
	    {
	      gStyle->SetPaintTextFormat("1.4f");
	      h->SetMarkerStyle(2);
	      h->SetMarkerSize(1);
	    }
	  
	  
	  if(histo_colors)
	  {
	    Color_t color = this->GO_->Get_colors().at(i);
	    h->SetLineColor(color);
	    h->SetMarkerColor(color);
	    
	    if(strstr(options.at(i), "E")!=NULL)
	      {
		h->SetFillColor(color);
 		//h->SetFillStyle(1001);
		h->SetFillStyle(3004); 
		h->SetFillColorAlpha(color,0.1);
	      }
	    
	    if(strstr(options.at(i), "hist")!=NULL)
              {
		h->SetFillStyle(0);
	      }

	    //if(options.at(i)=="HIST") h->SetFillColor(color);
	    /*
	    if(strstr(h->GetName(), "QCD_dijet")!=NULL)
	      {
		h->SetFillStyle(3003);
		draw_options += "HIST";
		}*/
	  }
	  else draw_options += " PLC";

	  if(this->GO_->Get_overwrite_y_name())
            {
	      TString X_title = this->GO_->Get_X_axis_name();
              h->GetXaxis()->SetTitle(X_title);
            }
	  if(this->GO_->Get_overwrite_y_name())
	    {
	     TString Y_title = this->GO_->Get_Y_axis_name();
	      h->GetYaxis()->SetTitle(Y_title);
	    }

	  h->SetTitle(var.c_str());
	  h->GetYaxis()->SetTitleSize(0.05);
	  h->GetYaxis()->SetTitleOffset(0.5);
	  h->GetYaxis()->SetLabelSize(0.05);
	  h->GetXaxis()->SetTitleSize(0);
	  
	  if(this->GO_->Get_overwrite_Y_maximum()) h->SetMaximum(max_range);
	  if(this->GO_->Get_overwrite_Y_minimum()) h->SetMinimum(min_range);

	  h->Draw(draw_options);
	  Top_histos.push_back(h);

	  TH1D *h_errors = (TH1D*)(h->Clone());
	  int nbins_original = h->GetNbinsX();
	  for(int b=1; b<=nbins_original; b++ )
	    {
	      h_errors->SetBinContent(b, 1);
	      h_errors->SetBinError(b, h->GetBinError(b));
	    }
 	  h_errors ->SetFillStyle(3004);
	  h_errors ->SetFillColorAlpha(kBlack,0.5);
	  h_errors ->SetMarkerStyle(0);
	  h_errors ->SetLineColor(kGray);
	  h_errors ->Draw("E2 SAME");
	  leg->AddEntry(h_errors, "stat errors");
	  i++;
	}
      leg->Draw("same");
      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();      

      c->cd();

      TPad *Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.42);
      Bottom_pad->cd();
      Bottom_pad->SetGridy(1);

      //Bottom_pad->SetLogy();//canc


      std::stringstream stream;
      stream << std::fixed << std::setprecision(0) << number;
      std::string Number = stream.str();

      TLegend *bottom_leg = new TLegend(xmin-0.1, ymin+0.05, xmax, ymax+0.05);
      bottom_leg->SetBorderSize(0);
      bottom_leg ->SetFillStyle(0);
      
      map<string, TH1D*>h_pulls;

      int t = 0;
      for(auto histo_top : Top_histos)
	{
	  TH1D *new_histo = (TH1D*)histo_top->Clone();
	  TH1D *const_histo = NULL;
	  TH1D *new_histo_abs = NULL;
	  h_pulls[new_histo->GetName()] = new TH1D(new_histo->GetName(), ("pulls wrt " + Number).c_str(), 40, -4, 4); //canc

	  bool isVarBin = reader_->Get_varbins(var);
	  
	  int Nerrors_bins = 0;
	  if(isVarBin) Nerrors_bins = ((reader_->Get_binning(var)).size() -1);
	  else Nerrors_bins = this->reader_->Get_vars_Nbins(var);

	  Double_t errors[Nerrors_bins];
	  Double_t pulls_errors[Nerrors_bins];

	  double Chi2 = 0;
	  double mean = 0;
	  double std_dev = 0;
	  double ndof = 0;
	  if(isVarBin)
	    {
	      auto vec_binning = reader_->Get_binning(var);
	      int NBins = vec_binning.size() - 1;
	      Double_t binning[NBins+1];
	      for(int ib=0; ib<vec_binning.size(); ib++) binning[ib] = vec_binning.at(ib);
	      const Double_t *Binning = binning;
	      
	      const_histo = new TH1D("const_histo", "", NBins, Binning);
	      for(int b=1;b<=NBins;b++) 
		{
		  const_histo->SetBinContent(b, number);
		  errors[b] = histo_top->GetBinError(b);
		  pulls_errors[b] = 0;
		}

	      new_histo->Add(const_histo, -1);
	      
	      new_histo_abs = new TH1D("new_histo_abs", "", NBins, Binning);
	      for(int b=1;b<=NBins;b++)
		{
		  if(errors[b]!=0 and (histo_top->GetBinContent(b)!=0))
		    {
		      new_histo_abs->SetBinContent(b, abs(new_histo->GetBinContent(b))/errors[b]);
		      //new_histo_abs->SetBinContent(b, new_histo->GetBinContent(b));    
		      h_pulls[new_histo->GetName()] ->Fill(new_histo->GetBinContent(b)/errors[b]); //canc
		      Chi2 += pow((new_histo_abs->GetBinContent(b)), 2);
		      mean += new_histo_abs->GetBinContent(b);
		      ndof++;
		    }
		}
	    }
	  else 
	    {
	      int nbins = this->reader_->Get_vars_Nbins(var);
	      double A = (this->reader_->Get_vars_range(var)).first;
	      double B = (this->reader_->Get_vars_range(var)).second;
	      
	      const_histo = new TH1D("const_histo", "", nbins, A, B);
	      for(int b=1;b<=nbins;b++) 
		{
		  const_histo->SetBinContent(b, number);
		  errors[b] = histo_top->GetBinError(b);
		  pulls_errors[b] = 0;
		}
		  
	      new_histo->Add(const_histo, -1);
	      
	      new_histo_abs = new TH1D("new_histo_abs", "", nbins, A, B);
   
	      for(int b=1;b<=nbins;b++)
		{
		  if(errors[b]!=0 and (histo_top->GetBinContent(b)!=0))
		    {
		      new_histo_abs->SetBinContent(b, abs(new_histo->GetBinContent(b))/errors[b]);
		      h_pulls[new_histo->GetName()] ->Fill(new_histo->GetBinContent(b)/errors[b]);//canc
		      //new_histo_abs->SetBinContent(b, new_histo->GetBinContent(b));
		      Chi2 += pow((new_histo_abs->GetBinContent(b)), 2);
		      mean += new_histo_abs->GetBinContent(b);
		      ndof++;
		    }
		}
	    }

	  mean = (double)(mean/ndof);
	 
	  for (int b=1; b<=ndof; b++) std_dev += pow((new_histo_abs->GetBinContent(b)-mean), 2);
	  std_dev = (double)(std_dev/(ndof-1));
	  std_dev = sqrt(std_dev);

	  Chi2 = sqrt(Chi2);
	  std::stringstream Stream;
          Stream << std::fixed << std::setprecision(4) << Chi2;
	  std::string chisquare = Stream.str();

	  std::stringstream Stream2;
          Stream2 << std::fixed << std::setprecision(0) << ndof;
	  std::string number_dof = Stream2.str();

	  std::stringstream Stream3;
          Stream3 << std::fixed << std::setprecision(3) << Chi2/ndof;
	  std::string chisquare_rid = Stream3.str();

	  std::stringstream Stream4;
          Stream4 << std::fixed << std::setprecision(3) << mean;
	  std::string pulls_mean = Stream4.str();
	  
	  std::stringstream Stream5;
          Stream5 << std::fixed << std::setprecision(3) << std_dev;
	  std::string pulls_std_dev = Stream5.str();
	  

	  //TString bottom_leg_entry = "#Chi ^{2}/Ndof= " + chisquare + "/" + number_dof + "=" + chisquare_rid;
	  //TString bottom_leg_entry = "#sqrt{#Chi ^{2}}/Ndof= " + chisquare_rid; 
	  //TString bottom_leg_entry = "#sqrt{#Sigma(bin_{i} - " + Number+ ")^{2}/#sigma_{i}^{2}}/Ndof = " + chisquare_rid;
	  TString bottom_leg_entry = "|pulls| average = " + pulls_mean + ", std_dev = " + pulls_std_dev;
	  
	  
	  
	  bottom_leg->AddEntry(new_histo_abs, bottom_leg_entry);
	  
	  new_histo_abs->SetLineColor(this->GO_->Get_colors().at(t));
	  h_pulls[new_histo->GetName()]->SetLineColor(this->GO_->Get_colors().at(t));

	  new_histo_abs->GetYaxis()->SetNdivisions(4);
	  new_histo_abs->GetYaxis()->SetTitleSize(0.1);
	  new_histo_abs->GetYaxis()->SetTitleOffset(0.29);
	  new_histo_abs->GetYaxis()->SetLabelSize(0.07);
	  
	  new_histo_abs->GetXaxis()->SetLabelSize(0.07);
	  new_histo_abs->GetXaxis()->SetTitleSize(0.1);
	  new_histo_abs->GetXaxis()->SetTitleOffset(0.8);

	  new_histo_abs->SetMaximum(GO_->Get_maximum_ratio());
	  new_histo_abs->SetMinimum(GO_->Get_minimum_ratio());

	  
	  new_histo_abs->GetYaxis()->SetTitle(("|bin_{i} - " + Number+"|/#sigma_{i}").c_str());
	  //new_histo_abs->GetYaxis()->SetTitle(("bin_{i} - " + Number ).c_str()); 
	  TString X_title = reader_->Get_vars_Xtitle(var);
	  new_histo_abs->GetXaxis()->SetTitle(X_title);
	  new_histo_abs->SetError(pulls_errors);
	  new_histo_abs->Draw("E SAME");
	  
	  t++;
	}
      bottom_leg->Draw();
      c->cd();
      
      c->SaveAs((var + "_RatioscomparedFromFile.pdf").c_str());

      //draw the pulls in another canvas
      TCanvas *c_pulls = new TCanvas();
      TLegend *leg_pulls = new TLegend(0.5, 0.55, 0.79, 0.89);
      leg_pulls->SetBorderSize(0);
      leg_pulls->SetFillStyle(0);
      leg_pulls->SetTextSize(0.03);
      
      
      //TF1 *fit_func = new TF1("my_gaus", "gaus", -2, 2);

      map<string, TH1D*>::iterator iter = h_pulls.begin();
      while(iter != h_pulls.end())
	{

	  iter->second->SetTitle((iter->first).c_str());
	  iter->second->GetXaxis()->SetTitle("pulls wrt 1");
	  iter->second->GetYaxis()->SetTitle("Entries");
	  //fit_func->SetParameter(1, 0);
	  //fit_func->SetParameter(2, 1);
	  //iter->second->Fit("my_gaus");
	  iter->second->Draw("hist sames");
	  //fit_func->SetLineColor(iter->second->GetLineColor());
	  //fit_func ->Draw("same");

	  double mean = iter->second->GetMean() ;
	  double std_dev = iter->second->GetStdDev();

	  std::stringstream Stream_A;
          Stream_A << std::fixed << std::setprecision(3) << std_dev;
	  std::string str_std_dev = Stream_A.str();
	  
	  std::stringstream Stream_B;
          Stream_B << std::fixed << std::setprecision(3) << mean;
	  std::string str_mean = Stream_B.str();

	  TString l_entry = "mean = " + str_mean + ", std_dev = " + str_std_dev;
	  
	  leg_pulls->AddEntry(iter->second, l_entry);
	  iter++;
	}

      leg_pulls->Draw();
      c_pulls->SaveAs((var + "_pulls.pdf").c_str()); //canc

      
    }
      
}


void HistoMaker::CreateNonClosurePlots(vector<string> sels_name, vector<string> vars_name, string nominal_path, string var_path, string noboot_nominal_path, string chain)
{
  if(sels_name.size() != vars_name.size())
    {
      ERROR("Selections strings and variables strings have not the same dimension!!");
      exit(EXIT_FAILURE);   
    }


  bool isScaleBin = this->Get_ScaletoBinWidth();
  string unit_of_measure = this->Get_UnitOfMeasure();
  TString X_title, Y_title;
  if(this->GO_->Get_overwrite_y_name())  Y_title = this->GO_->Get_Y_axis_name();
  else Y_title = "Entries";

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();


  TH1D *h_dati, *h_nom, *h_var, *h_errors, *h_var_up, *h_var_symm, *h_pulls, *h_distr_pulls, *h_nom_clone;
  TCanvas *c;
  TLegend *leg, *bottom_leg;
  TVirtualPad *Top_pad, *Bottom_pad_up, *Bottom_pad_down;
  TFile *nominal_file,  *variated_file, *noboot_nominal_file;

  int i=0;
  for(auto sel : sels_name)
    {
      string var = vars_name.at(i);
      
      if(this->GO_->Get_overwrite_x_name())  X_title = this->GO_->Get_X_axis_name();
      else X_title = var.c_str();

      string nominal_file_name = nominal_path + "file_" + var + ".root";
      string variated_file_name = var_path + "file_" + var + ".root";
      string noboot_nominal_file_name = noboot_nominal_path + "file_" + var + ".root";

      nominal_file = TFile::Open(nominal_file_name.c_str());
      variated_file = TFile::Open(variated_file_name.c_str());
      noboot_nominal_file = TFile::Open(noboot_nominal_file_name.c_str());

      string histo_dati = sel + "_" + chain;
      string histo_nominal = sel + "_NominalRew_" + chain;
      string histo_variated = sel + "_VariatedRew_" + chain; 
     
      h_dati = (TH1D*) nominal_file->Get(histo_dati.c_str());
      h_nom = (TH1D*) nominal_file->Get(histo_nominal.c_str());
      h_var = (TH1D*) variated_file->Get(histo_variated.c_str());
      h_nom_clone = (TH1D*) noboot_nominal_file->Get(histo_nominal.c_str());

      gStyle->SetCanvasDefH(700);
      gStyle->SetCanvasDefW(600);
      
      TCanvas *c = new TCanvas();
      gStyle->SetOptStat(0);
      gStyle->SetOptTitle(0);

      c->Divide(1,3,0,0);
      
      //Top_pad = Create_Top_Pad(0, 0.35, 1, 1);
      //Bottom_pad = Create_Bottom_Pad(0, 0.03, 1, 0.43);
      //Top_pad->cd();
      
      Top_pad = c->cd(1);
      //gPad->SetPad(0., 0.30, 1, 0.6);
      gPad->SetTickx(1);
      
      if(this->GO_->Get_logy()) Top_pad->SetLogy();
      
      
      if(!GO_->Get_overwrite_selection())  GO_->Set_selection(sel);
      else
	{
	  string new_sel = GO_->Get_new_selection();
	  GO_->Set_selection(new_sel);
	}


      leg = new TLegend(xmin, ymin, xmax, ymax);
      leg->SetBorderSize(0);

      h_dati->SetMarkerColor(kSpring+9);
      h_dati->SetLineColor(kGreen+1);
      h_dati->SetMarkerStyle(1);


      h_nom->SetLineColor(kBlack);
      
      h_errors = (TH1D*)(h_nom->Clone());
      int nbins = h_nom->GetNbinsX();
      for(int b=1; b<=nbins; b++ )
	{
	  h_errors->SetBinContent(b, h_nom->GetBinContent(b));
	  h_errors->SetBinError(b, h_nom->GetBinError(b));
	}
      h_errors ->SetFillStyle(3004);
      h_errors ->SetFillColorAlpha(kBlack,0.5);
      h_errors ->SetMarkerStyle(0);
      h_errors ->SetLineColorAlpha(kWhite,0);


      h_var->SetLineColor(kBlue);
      
      
      h_var_up = (TH1D*)(h_var->Clone());
      h_var_up->Add(h_nom, -1);

      h_var_symm = (TH1D*)(h_nom->Clone());
      h_var_symm->Add(h_var_up, -1);

      h_var_symm->SetLineColor(kRed);
      

      if(isScaleBin and !this->GO_->Get_overwrite_y_name())
	{
          double bin_width = (double)h_nom->GetBinWidth(1);
	  double last_bin_width = (double)h_nom->GetBinWidth(h_nom->GetNbinsX()-1);
	  std::stringstream stream;
          stream << std::fixed << std::setprecision(2) << bin_width;
	  std::string s = stream.str();

          (bin_width!=last_bin_width) ? (Y_title = "Entries/bin_size") : (Y_title = ("Entries/" + s  + " " + unit_of_measure).c_str());
        }

      h_nom_clone->GetYaxis()->SetTitle(Y_title);
      h_nom_clone->Draw("hist");

      if(this->GO_->Get_overwrite_Y_maximum()) h_nom_clone->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum()) h_nom_clone->SetMinimum(this->GO_->Get_minimum());
      
      h_errors ->Draw("E2 same");
      h_var->Draw("hist same");
      h_var_symm->Draw("hist same");
      h_dati->Draw("E same");


      leg->AddEntry(h_dati, "Tagged Data");
      leg->AddEntry(h_nom, "Tagged rew Data (Nominal model)");
      leg->AddEntry(h_var, "Untagged rew Data (Alternative model)");
      leg->AddEntry(h_var_symm, "symmet. Untagged rew Data (Alternative model)");
      leg->AddEntry(h_errors, "stat+boot error");

      leg->Draw();
      TLatex l;
      l.SetTextFont(72);
      l.SetTextSize(0.039);
      l.DrawLatexNDC(0.14, 0.9, "ATLAS");
      l.SetTextFont(42);
      l.DrawLatexNDC(0.22, 0.9, "Work in progress");
      l.DrawLatexNDC(0.14, 0.84, TString::Format("#scale[0.8]{#sqrt{s} = 13 TeV,   #int Ldt = %.1f fb^{-1}}", GO_->Get_lumi()));
      TString selection = ("#font[12]{" + sel + "}").c_str();
      l.DrawLatexNDC(0.15, 0.76, "#scale[0.8]{" + selection +"}");

      Bottom_pad_up = c->cd(2);

      gPad->SetTickx(1);

      bottom_leg = new TLegend();
      bottom_leg->SetBorderSize(0);
      bottom_leg->SetNColumns(3);
      
      TH1D *h_ratio_dati = (TH1D*)(h_dati->Clone());
      h_ratio_dati->Divide(h_nom_clone);
      for(int b=1; b<=nbins; b++ ) if((h_dati->GetBinContent(b)==0) || (h_nom_clone->GetBinContent(b)==0)) h_ratio_dati->SetBinError(b, 0);

      h_ratio_dati->SetLineColor(h_dati->GetLineColor());
      h_ratio_dati->SetMarkerColor(h_dati->GetMarkerColor());

      TH1D *h_ratio_syst_up = (TH1D*)(h_var->Clone());
      h_ratio_syst_up->Divide(h_nom);
      
      TH1D *h_ratio_syst = (TH1D*)(h_var->Clone());
      for(int b=1; b<=nbins; b++ )
        {
          h_ratio_syst->SetBinContent(b, 1);
          if(h_nom->GetBinContent(b)>0) h_ratio_syst->SetBinError(b, (h_ratio_syst_up->GetBinContent(b))-1);
	  else h_ratio_syst->SetBinError(b, 0);
	  if(b==1) h_ratio_syst->SetBinError(b, 0);
        }
      //h_ratio_syst->SetFillColorAlpha(kBlue-10,0.5);
      h_ratio_syst->SetFillColor(kBlue);
      //h_ratio_syst->SetFillStyle(3001);


      TH1D *h_ratio_syst_boot = (TH1D*)(h_var->Clone());

      for(int b=1; b<=nbins; b++ )
        {
	  h_ratio_syst_boot->SetBinContent(b, 1);
	  double err_boot2 = (h_nom->GetBinError(b)*h_nom->GetBinError(b)) - (h_nom_clone->GetBinError(b)*h_nom_clone->GetBinError(b)); //err_tot^2 - err_stat^2 = err_boot^2
	  //if(h_nom->GetBinContent(b)>0) h_ratio_syst_boot->SetBinError(b, sqrt((err_boot2/(h_nom->GetBinContent(b)*h_nom->GetBinContent(b)))+((h_ratio_syst->GetBinError(b))*(h_ratio_syst->GetBinError(b)))));
	  if(h_nom->GetBinContent(b)>0) h_ratio_syst_boot->SetBinError(b, sqrt(err_boot2)/(h_nom->GetBinContent(b)));
	}
      
      
      TH1D *h_ratio_all = (TH1D*)(h_var->Clone());
      for(int b=1; b<=nbins; b++)
	{
          h_ratio_all->SetBinContent(b, 1);
          if(h_nom->GetBinContent(b)>0) h_ratio_all->SetBinError(b, sqrt((h_ratio_syst->GetBinError(b)*h_ratio_syst->GetBinError(b)) + ((h_nom->GetBinError(b)/h_nom->GetBinContent(b))*(h_nom->GetBinError(b)/h_nom->GetBinContent(b)))));
	  else h_ratio_all->SetBinError(b, 0);
	  if(b==1) h_ratio_all->SetBinError(b, 0);
        }
      
      h_ratio_all->SetFillColor(kGray);
      
      bottom_leg->AddEntry(h_ratio_dati, "Tagged data");
      bottom_leg->AddEntry(h_ratio_syst, "shape syst");
      //bottom_leg->AddEntry(h_ratio_syst_boot, "shape syst + boot");
      bottom_leg->AddEntry(h_ratio_syst_boot, "bootstrap");
      //bottom_leg->AddEntry(h_ratio_all, "shape syst + boot + stat");


      h_ratio_syst_boot->Draw("E5 ][");
	    
      h_ratio_syst_boot->GetYaxis()->SetTitle("Tagged/Untagged(Nominal rew)");
      if(GO_->Get_overwrite_Y_maximum_ratio()) h_ratio_syst_boot->SetMaximum(GO_->Get_maximum_ratio());
      if(GO_->Get_overwrite_Y_minimum_ratio()) h_ratio_syst_boot->SetMinimum(GO_->Get_minimum_ratio());
 
      h_ratio_syst_boot->SetFillColor(kViolet);
      //h_ratio_syst_boot->SetFillStyle(3001);

      //h_ratio_syst->Draw("E5 same ][");
      h_ratio_dati->Draw("E same");
      bottom_leg->Draw();

      Bottom_pad_down = c->cd(3);

      gPad->SetTickx(1);
      gPad->SetTicky(2);
      gPad->SetGridy(1);
      

      h_pulls = (TH1D*)(h_dati->Clone());
      h_pulls->Reset("ICE");

      h_distr_pulls = new TH1D("h_distr_pulls", ("Data pulls for " + var + " " + sel).c_str(), 10, -4, 4);

      for(int b=1; b<=nbins; b++ )
        {
	  double err_den = sqrt((h_nom->GetBinError(b)*h_nom->GetBinError(b)) + (h_ratio_syst->GetBinError(b)*h_nom->GetBinContent(b)*h_ratio_syst->GetBinError(b)*h_nom->GetBinContent(b)));
	  //double err_den = h_nom->GetBinError(b);

	  double pulls_err = sqrt(  ((double)(h_dati->GetBinError(b)/h_nom_clone->GetBinContent(b))*(double)(h_dati->GetBinError(b)/h_nom_clone->GetBinContent(b))) +					  (((double)(h_dati->GetBinContent(b)/(h_nom_clone->GetBinContent(b)*h_nom_clone->GetBinContent(b)))*err_den) * ((double)(h_dati->GetBinContent(b)/(h_nom_clone->GetBinContent(b)*h_nom_clone->GetBinContent(b)))*err_den)));
	  
	  double pull = (double) (h_ratio_dati->GetBinContent(b)-h_ratio_all->GetBinContent(b));
	  if((pulls_err>0) && (pull!=-1)) h_pulls->SetBinContent(b, (double)(pull/pulls_err));
	  if(pull==-1) h_pulls->SetBinContent(b, -99);
	  //cout <<pull<< " " <<  h_pulls->GetBinContent(b)<<endl;
	  if(h_pulls->GetBinContent(b)!=(-99)) h_distr_pulls->Fill(h_pulls->GetBinContent(b));

	  //cout << h_pulls->GetBinContent(b)<<endl;
	}
      
      h_distr_pulls->Sumw2();

      h_pulls->SetMarkerStyle(2);
      h_pulls->SetMarkerColor(kBlack);
      h_pulls->SetLineColor(kBlack);

      h_pulls->SetMaximum(4);
      h_pulls->SetMinimum(-4);
      
      h_pulls->Draw("hist P");

      h_pulls->GetYaxis()->SetTitle("pulls");
      h_pulls->GetXaxis()->SetTitle(h_nom->GetXaxis()->GetTitle());
      

      c->cd(0);
      string pdf_name = "NonClosure_" + var + "_" + sel + ".pdf";
      c->SaveAs(pdf_name.c_str());


      TF1 *fit_func = new TF1("my_gaus", "gaus", -4, 4);
      TCanvas *c_pulls = new TCanvas();
      c_pulls->cd();
      TLegend *leg_pulls = new TLegend();
      leg_pulls->SetBorderSize(0);
      leg_pulls->SetFillStyle(0);
      leg_pulls->SetTextSize(0.03);

      h_distr_pulls->GetXaxis()->SetTitle("pulls wrt 1");
      h_distr_pulls->GetYaxis()->SetTitle("Entries");
      fit_func->FixParameter(0, h_distr_pulls->GetMaximum());
      fit_func->SetParameter(1, 0);
      fit_func->SetParameter(2, 2);
      h_distr_pulls->Fit("my_gaus");
      h_distr_pulls->SetMarkerColor(kBlack);
      h_distr_pulls->SetLineColor(kBlack);
      h_distr_pulls->Draw("hist");
      fit_func->SetLineColor(kRed);
      fit_func->Draw("same");
      
      
      double mean = fit_func->GetParameter(1);
      double std_dev = fit_func->GetParameter(2);
      
      std::stringstream Stream_A;
      Stream_A << std::fixed << std::setprecision(3) << std_dev;
      std::string str_std_dev = Stream_A.str();
      
      std::stringstream Stream_B;
      Stream_B << std::fixed << std::setprecision(3) << mean;
      std::string str_mean = Stream_B.str();
      
      TString l_entry = "mean = " + str_mean + ", std_dev = " + str_std_dev;
      
      leg_pulls->AddEntry(h_distr_pulls, l_entry);
      
      leg_pulls->Draw();
      c_pulls->SaveAs((var + "_" + sel +  "_pulls.pdf").c_str()); //canc

      
      TFile*file_pulls = new TFile((var + "_" + sel +  "_pulls").c_str(), "RECREATE");
      h_distr_pulls->Write();


      
      
      h_var_up = NULL;
      h_var_symm = NULL;
      h_errors = NULL;
      h_dati = NULL;
      h_nom = NULL;
      h_nom_clone = NULL;
      h_var = NULL;
      h_pulls = NULL;
      h_distr_pulls = NULL;
      c = NULL;
      Top_pad = NULL;
      Bottom_pad_up = NULL;
      Bottom_pad_down = NULL;
      leg = NULL;
      bottom_leg = NULL;
      nominal_file->Close();
      variated_file->Close();
      noboot_nominal_file->Close();
      i++;
    }
}
