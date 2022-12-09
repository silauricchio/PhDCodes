#ifndef HISTOPLOTTER_H
#define HISTOPLOTTER_H

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include <iostream>
#include "TChain.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "THStack.h"

#include "../util/Debug.h"
#include "ReadFiles.h"
#include "GraphicsOptions.h"

using namespace std;

class HistoMaker
{
 public:
  HistoMaker(bool debug_mode=false, ReadFiles *reader=NULL, Graphics_options *GO=NULL)
    {
      if(reader == NULL)
	{
	  ERROR("The reader passed to the HistoMaker does not exist!!");
	  exit(EXIT_FAILURE);
	}
      reader_ = reader;
      debug_ = debug_mode;

      if(GO == NULL)
	{
	  ERROR("The class GraphicsOptions passed to the HistoMaker does not exist!!");
	  exit(EXIT_FAILURE);
	}
      GO_ = GO;
 };
  ~HistoMaker();

 private:
  
  ReadFiles *reader_;
  Graphics_options *GO_;
  bool debug_;

  map<string, TChain*> map_chains_; 
  map<string, map<string, map<string, TH1D*>>> map_histos_;
  map<string, map<string, map<string, TH1D*>>> map_histos_raw_;
  map<string, map<string, map<string, double>>> map_integral_histos_;
  map<string, map<string, map<string, TH1D*>>> map_ratios_;
  map<string, TH2D*> map_2Dhistos_;
  map<string, TH2D*> map_ratio2D_;
  map<string, map<string, THStack*>> map_stack_plots_;


  bool scale_to_bin_width_ = false;
  string unit_of_measure_ = "";

  bool histos_to_normalize_ = false;
  bool histo_to_normalize_ = false;
  bool stack_to_normalize_ = false;
  bool isStack_ = false;

  double normalizeHistos_;
  vector<pair<string, double>> normalizeHisto_;
  vector<pair<string, double>> normalizeStack_;


  void MakeHisto1D(string var_name, string sel_name, string chain_tag);
  void MakeHisto1D_weighted(string var_name, string sel_name, string chain_tag);
  string MakeHisto2D(string var_name1, string var_name2, string sel_name, string chain_tag);
  TH2D* MakeRatio2D(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag);
  pair<TH2D*,double> MakePulls2D(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag, double number);

  THStack* MakeStackPlot(string var_name, string sel_name, vector<string> stack_components);
  vector<string> Order_stack_components(string var_name, string sel_name, vector<string> stack_components);
  void CalculateStatUncertanties(TH1D* h, Double_t* err_bins);
  void CalculateRatioUncertanties(TH1D* h_num, TH1D* h_den, double integral_num, double integral_den, TH1D* h_ratio,  Double_t *err_bins);
  void PropagateScaleFactorUncertanty(TH1D* h, Double_t *err_bins, double SF, double err_SF);

  TPad* Create_Top_Pad(double xlow, double ylow, double xup, double yup);
  TPad* Create_Bottom_Pad(double xlow, double ylow, double xup, double yup);

  void Add_chain_to_map(string file_tag, TChain *chain);
  void Add_histo_to_map(string var_name, string sel_name, string file_tag, TH1D* histo);
  void Add_histo_to_map_raw(string var_name, string sel_name, string file_tag, TH1D* histo);
  void Add_ratio_to_map(string var_name, string sel_name, string ratio_name, TH1D* h_ratio);
  void Add_2Dhisto_to_map(string name, TH2D* h2);
  void Add_ratio2D_to_map(string name, TH2D* h2);
  void Add_stack_to_map(string var_name, string sel_name, THStack* stack);

  void MakeHistos(vector<string> var_names, vector<string> sel_names,  vector<string> chain_tags);


 public:
  
  ReadFiles* Get_reader()
  {
    ReadFiles *reader = reader_;
    return reader;
  }
  
  bool MakeChain( string file_tag);
  
 vector<string> Get_chain_tags()
   {
     
     vector<string> tags;
     map<string, TChain*>::iterator r = this->map_chains_.begin();
     while(r != this->map_chains_.end())
       {
	 tags.push_back(r->first);
	 r++;
       }
     return tags;
   }

 TChain* Get_chain(string tag)
 {
   TChain* chain = map_chains_[tag];
   if(chain == NULL)
     {
       ERROR("Impossible to find the chain with tag: " << tag);
       exit(EXIT_FAILURE);
     }
   return chain;
 }
 
 vector<string> Get_ratio_tags(string var_name, string sel_name)
   {
     if(! ((map_ratios_.find(var_name) != map_ratios_.end()) and (map_ratios_[var_name].find(sel_name) != map_ratios_[var_name].end())))
       {
	 ERROR("Map of ratios [" + var_name + "][" + sel_name + "] does not exist!");
	 return {};
       }
     
     vector<string> tags;
     map<string, TH1D*>::iterator r = this->map_ratios_[var_name][sel_name].begin();
     while(r != this->map_ratios_[var_name][sel_name].end())
       {
         tags.push_back(r->first);
         r++;
       }
     return tags;
   }


 TH1D* Get_ratio(string var_name, string sel_name, string tag)
 {
   TH1D* ratio = map_ratios_[var_name][sel_name][tag];
   if(ratio == NULL)
     {
       ERROR("Impossible to find the ratio with tag: " << var_name + " " + sel_name + " "+ tag);
       exit(EXIT_FAILURE);
     }
   return ratio;
 }

 vector<string> Get_histo_tags(string var_name, string sel_name)
   {
     if(!((this->map_histos_.find(var_name) != this->map_histos_.end()) and (this->map_histos_[var_name].find(sel_name)!=this->map_histos_[var_name].end())));
       {
	 ERROR("Map of histos [" + var_name + "][" + sel_name + "] does not exist!");
	 return {};
       }
     vector<string> tags;
     map<string, TH1D*>::iterator r = this->map_histos_[var_name][sel_name].begin();
     while(r != this->map_histos_[var_name][sel_name].end())
       {
	 tags.push_back(r->first);
	 r++;
       }
     return tags;
   }

 TH1D* Get_histo(string var_name, string sel_name, string tag)
 {
   if((this->map_histos_.find(var_name) == this->map_histos_.end()) or (this->map_histos_[var_name].find(sel_name)==this->map_histos_[var_name].end()))
   {
     ERROR("Map of histos [" + var_name + "][" + sel_name + "] does not exist!");
     exit(EXIT_FAILURE);
   }
   TH1D* h = map_histos_[var_name][sel_name][tag];
   if(h == NULL)
     {
       ERROR("Impossible to find the histo " <<var_name + " " + sel_name + " " + tag);
       exit(EXIT_FAILURE);
     }
   return h;
 }
 
 TH1D* Get_histo_raw(string var_name, string sel_name, string tag)
 {
   
   if(!((this->map_histos_raw_.find(var_name) != this->map_histos_raw_.end()) and (this->map_histos_raw_[var_name].find(sel_name)!=this->map_histos_raw_[var_name].end())))
   {
     ERROR("Map of histos [" + var_name + "][" + sel_name + "] does not exist!");
     exit(EXIT_FAILURE);
   }
   TH1D* h = map_histos_raw_[var_name][sel_name][tag];
   if(h == NULL)
     {
       ERROR("Impossible to find the raw histo " <<var_name + " " + sel_name + " " + tag);
       exit(EXIT_FAILURE);
     }
   return h;
 }

 double Get_histo_integral(string var_name, string sel_name, string tag)
 {
   if((this->map_integral_histos_.find(var_name) == this->map_integral_histos_.end()) or (this->map_integral_histos_[var_name].find(sel_name)==this->map_integral_histos_[var_name].end()))
     {
       ERROR("Map with histo integral[" + var_name + "][" + sel_name + "] does not exist!");
       exit(EXIT_FAILURE);
     }
   double I = map_integral_histos_[var_name][sel_name][tag];
   return I;
 }

 vector<string> Get_2Dhisto_tags()
   {
     vector<string> tags;
     map<string, TH2D*>::iterator r = this->map_2Dhistos_.begin();
     while(r != this->map_2Dhistos_.end())
       {
         tags.push_back(r->first);
         r++;
       }
     return tags;
   }

 vector<string> Get_ratio2D_tags()
   {
     vector<string> tags;
     map<string, TH2D*>::iterator r = this->map_ratio2D_.begin();
     while(r != this->map_ratio2D_.end())
       {
         tags.push_back(r->first);
         r++;
       }
     return tags;
   }

 TH2D* Get_2Dhisto(string tag)
 {
   TH2D * h = map_2Dhistos_[tag];
   if(h==NULL)
     {
       ERROR("Impossible to find the 2D histo " << tag);
       exit(EXIT_FAILURE);
     }
   return h;
 }

 TH2D* Get_ratio2D(string tag)
 {
   TH2D * h = map_ratio2D_[tag];
   if(h==NULL)
     {
       ERROR("Impossible to find the 2D ratio " << tag);
       exit(EXIT_FAILURE);
     }
   return h;
 }

 vector<string> Get_stacks_var_names()
   {
     vector<string> tags;
     map<string, map<string, THStack*>>::iterator r = this->map_stack_plots_.begin();
     while(r != this->map_stack_plots_.end())
       {
         tags.push_back(r->first);
         r++;
       }
     return tags;
   }

 vector<string> Get_stacks_sel_names(string var_name)
   {
     if(!((this->map_stack_plots_.find(var_name) != this->map_stack_plots_.end())))
     {
       ERROR("Map of stack plots [" + var_name + "] does not exist!");
       return {};
     }
     vector<string> tags;
     map<string, THStack*>::iterator r = this->map_stack_plots_[var_name].begin();
     while(r != this->map_stack_plots_[var_name].end())
       {
         tags.push_back(r->first);
         r++;
       }
     return tags;
   }
 

 THStack* Get_stack(string var_name, string sel_name)
 {
   
   if(!((this->map_stack_plots_.find(var_name) != this->map_stack_plots_.end()) and (this->map_stack_plots_[var_name].find(sel_name)!=this->map_stack_plots_[var_name].end())))
   {
     ERROR("Map of stack plots [" + var_name + "][" + sel_name + "] does not exist!");
     exit(EXIT_FAILURE);
     }
   THStack *h = map_stack_plots_[var_name][sel_name];
   if(h==NULL)
     {
       ERROR("Impossible to find the stack " << var_name << " " << sel_name);
       exit(EXIT_FAILURE);
     }
   return h;
 }
 
 void Set_ScaletoBinWidth(bool set, string unit_of_measure="")
 {
   scale_to_bin_width_ = set;
   unit_of_measure_ = unit_of_measure;
 }
 
 bool Get_ScaletoBinWidth()
 {
   bool set =  scale_to_bin_width_;
   return set;
 }


 string Get_UnitOfMeasure()
 {
   string unit = unit_of_measure_;
   return unit;
 }


  void NormalizeHistos(double Norm);
  void NormalizeHisto(TH1D *h, double Norm);
  void NormalizeHisto2D(TH2D *h, double Norm);
  void NormalizeStack(string var_name, string sel_name);
  void ApplyNormalizations();


  void DrawHisto(string var_name, string sel_name,  vector<string> file_tags);
  void DrawHisto(string var_name, string sel_name,  vector<string> file_tags, vector<string> tags_num, vector<string> tags_den);
  void DrawComparisons_sels(string var_name, vector<string> sel_names,  vector<string> chain_tags);
  void DrawComparisons_sels(string var_name, vector<string> sel_names,  vector<string> chain_tags, vector<string> sels_num, vector<string> sels_den);
  void DrawComparisons_vars(vector<string> var_names, string sel_name, vector<string> chain_tags);
  void DrawStack(string var_name, string sel_name, vector<string> chain_tags);
  void DrawStack_Ratio(string var_name, string sel_name, vector<string> chain_tags);
  void DrawDataDnnBkg_Ratio(string var_name, string sel_name, vector<string> chain_tags, bool draw_obs_sign);
  void CompareHistos_fromFile(vector<string> file_names, vector<string> histo_names, vector<string> vars_to_compare);
  void CompareHistos_fromFile(vector<string> file_names, vector<string> histo_names, vector<string> histo_num, vector<string> histo_den, vector<string> vars_to_compare, vector<string> histo_signals={}, vector<string> histo_bkg={});  
  void CompareRatios_fromFile(vector<string> file_names, vector<string> histo_names, float number,  vector<string> vars_to_compare);

  void CreateNonClosurePlots(vector<string> sels_name, vector<string> vars_name, string nominal_path, string var_path, string noboot_nominal_path, string chain);

  void DrawHistos();
  void DrawHistos(vector<string> tags_num, vector<string> tags_den);
  void Draw2DHistos(vector<string> var_name1, vector<string> var_name2, vector<string> sel_names, vector<string> chain_tags, bool Profile=false);
  void DrawRatio2D(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names1, vector<string> var_names2, vector<string> chain_tags);
  void DrawPulls_2D(vector<string> var_name1, vector<string> var_name2, vector<string> sel_names_num, vector<string> sel_names_den,  vector<string> chain_tags, double number);

  void CompareVariables(vector<string> sel_names, vector<string> var_names, vector<string> file_tags);
  void CompareSelections(vector<string> sel_names, vector<string> var_names, vector<string> file_tags);
  void CompareSelections(vector<string> sel_names, vector<string> var_names, vector<string> file_tags, vector<string>sels_num, vector<string>sels_den);
  void DrawStacks();  
  void DrawDataDNNBkgPlots(bool draw_obs_sign);
 
  bool WriteInfoFile(string var_name, string sel_name, bool isStack);

  void Set_NormalizeHistos(double Norm);
  void Set_NormalizeHisto(string histo_tag, double Norm);
  void Set_NormalizeStack(string histo_tag, double Norm);
  bool Get_NormalizeHistos();
  bool Get_NormalizeHisto();
  bool Get_NormalizeStack();

  pair<TH1D*, TH1D*> Do_histo_sum(string var_name, string sel_name, vector<string> tags);

};






#endif
