#ifndef STATISTICS_H
#define STATISTICS_H

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <iostream>
#include "TChain.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include <boost/optional.hpp>

#include "HistoPlotter.h"
#include "GraphicsOptions.h"
#include "../util/Debug.h"

class Statistics
{
 public:
  Statistics(HistoMaker *h_maker=NULL, bool debug_mode=false, Graphics_options *GO=NULL)
    {
      if(h_maker == NULL)
        {
          ERROR("The HistoMaker passed to the Efficiency class does not exist!!");
          exit(EXIT_FAILURE);
        }
      h_Maker_ = h_maker;

      if(GO == NULL)
        {
          ERROR("The GraphicsOptions passed to the Efficiency class does not exist!!");
          exit(EXIT_FAILURE);
        }
      GO_ = GO;
      debug_ = debug_mode;

    };
  ~Statistics();

 private:
  HistoMaker *h_Maker_;
  Graphics_options *GO_;
  bool debug_;

  double systematics_factor_ = 0.;
  bool apply_systematics_ = false;

  map<string, map<string,TGraphAsymmErrors*>> map_efficiencies_;
  map<string, map<string, TGraph*>> map_significancies_;
  map<string, map<string, TGraph*>> map_asym_significancies_;
  map<string, map<string, TGraph*>> map_asym_significancies_with_syst_;
  map<string, TGraph*> map_xsection_;

  TGraphAsymmErrors* MakeEfficiencyCurve(vector<string> sels_num, string sel_den, string var_name, string tag);
  TGraph* MakeYieldsCurve(vector<string> sels_num, string var_name, string tag);
  void MakeSignificanceCurve(vector<string> sels, string var_name, string tag);
  void MakeXSectionCurve(string sel, string var_name, vector<string> signal_tags, double N_sigma);
  void MakeXSectionCurve_Combined(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma);

  void Add_efficiency_to_map(string chain_tag, string sel_den, TGraphAsymmErrors *eff)
  {
    map_efficiencies_[chain_tag][sel_den] = eff;
  }

  void Add_significance_to_map(string var_name, string signal_tag, TGraph *sig)
  {
    map_significancies_[var_name][signal_tag] = sig;
  }
  
  void Add_asym_significance_to_map(string var_name, string signal_tag, TGraph *sig)
  {
    map_asym_significancies_[var_name][signal_tag] = sig;
  }


  void Add_asym_significance_with_Syst_to_map(string var_name, string signal_tag, TGraph *sig)
  {
    map_asym_significancies_with_syst_[var_name][signal_tag] = sig;
  }
  
  void Add_XSection_to_map(string var_name, TGraph *XSec)
  {
    map_xsection_[var_name] = XSec;
  }

 public: 

  void Set_apply_systematics(bool apply, double k=0.)
  {
    apply_systematics_ = apply;
    systematics_factor_ = k;
  }

  bool Get_apply_systematics()
  {
    bool apply = apply_systematics_;
    return apply;
  }

  double Get_systematics_factor()
  {
    double k = systematics_factor_;
    return k;
  }

  TGraphAsymmErrors* Get_efficiency(string chain_tag, string sel_den)
  {
    if((map_efficiencies_.find(chain_tag) == map_efficiencies_.end()) or (map_efficiencies_[chain_tag].find(sel_den) == map_efficiencies_[chain_tag].end()))
      {
	WARNING("Efficiency map with tag: " << chain_tag << " " << sel_den << " does not exist!!");
	return NULL;
      }
    TGraphAsymmErrors* graph = map_efficiencies_[chain_tag][sel_den];
    return graph;
  }

  TGraph* Get_significance(string var_name, string signal_tag)
  {
    if((map_significancies_.find(var_name) == map_significancies_.end()) or (map_significancies_[var_name].find(signal_tag) == map_significancies_[var_name].end()))
      {
	ERROR("Significance map with tags: " << var_name << " " <<signal_tag << " does not exist!!");
        exit(EXIT_FAILURE);
      }
    if(map_significancies_[var_name][signal_tag] == NULL)
      {
	ERROR("Significance " << var_name << " " <<signal_tag << " not found!!");
	exit(EXIT_FAILURE);
      }
    TGraph *sig = map_significancies_[var_name][signal_tag];
    return sig;
  }

  TGraph* Get_asym_significance_with_Syst(string var_name, string signal_tag)
  {
    if((map_asym_significancies_with_syst_.find(var_name) == map_asym_significancies_with_syst_.end()) or (map_asym_significancies_with_syst_[var_name].find(signal_tag) == map_asym_significancies_with_syst_[var_name].end()))
      {
        ERROR("Asym Significance with Syst map with tags: " << var_name << " " <<signal_tag << " does not exist!!");
        exit(EXIT_FAILURE);
      }
    if(map_asym_significancies_with_syst_[var_name][signal_tag] == NULL)
      {
        ERROR("Asym Significance with Syst " << var_name << " " <<signal_tag << " not found!!");
        exit(EXIT_FAILURE);
      }
    TGraph *sig = map_asym_significancies_with_syst_[var_name][signal_tag];
    return sig;
  }

  TGraph* Get_asym_significance(string var_name, string signal_tag)
  {
    if((map_asym_significancies_.find(var_name) == map_asym_significancies_.end()) or (map_asym_significancies_[var_name].find(signal_tag) == map_asym_significancies_[var_name].end()))
      {
        ERROR("Asym Significance map with tags: " << var_name << " " <<signal_tag << " does not exist!!");
        exit(EXIT_FAILURE);
      }
    if(map_asym_significancies_[var_name][signal_tag] == NULL)
      {
        ERROR("Asym Significance " << var_name << " " <<signal_tag << " not found!!");
        exit(EXIT_FAILURE);
      }
    TGraph *sig = map_asym_significancies_[var_name][signal_tag];
    return sig;
  }

  TGraph* Get_XSection(string var_name)
  {
    if(map_xsection_.find(var_name) == map_xsection_.end())
      {
        ERROR("XSection map with tag: " << var_name << " does not exist!!");
        exit(EXIT_FAILURE);
      }
    if(map_xsection_[var_name] == NULL)
      {
        ERROR("XSection for " << var_name << " not found!!");
        exit(EXIT_FAILURE);
      }
    TGraph *xsec = map_xsection_[var_name];
    return xsec;
  }


  vector<string> Get_significance_variables()
    {
      vector<string> vars;
      map<string, map<string, TGraph*>>::iterator s = map_significancies_.begin();
      while(s!=map_significancies_.end())
	{
	  vars.push_back(s->first);
	  s++;
	}
      return vars;
    }


  vector<string> Get_asym_significance_variables()
    {
      vector<string> vars;
      map<string, map<string, TGraph*>>::iterator s = map_asym_significancies_.begin();
      while(s!=map_asym_significancies_.end())
        {
          vars.push_back(s->first);
          s++;
        }
      return vars;
    }

  vector<string> Get_asym_significance_with_syst_variables()
    {
      vector<string> vars;
      map<string, map<string, TGraph*>>::iterator s = map_asym_significancies_with_syst_.begin();
      while(s!=map_asym_significancies_with_syst_.end())
        {
          vars.push_back(s->first);
          s++;
        }
      return vars;
    }

  vector<string> Get_xsection_variables()
    {
      vector<string> vars;
      map<string, TGraph*>::iterator s = map_xsection_.begin();
      while(s!=map_xsection_.end())
        {
          vars.push_back(s->first);
          s++;
        }
      return vars;
    }

  vector<string> Get_significance_tags(string var_name)
    {
      if(map_significancies_.find(var_name) == map_significancies_.end())
	{
	  ERROR("Significances for variable: " << var_name << " does not exist!!");
	  exit(EXIT_FAILURE);
	}
      vector<string> tags;
      map<string, TGraph*>::iterator s = map_significancies_[var_name].begin();
      while(s!=map_significancies_[var_name].end())
	{
          tags.push_back(s->first);
	  s++;
        }
      return tags;
    }


  vector<string> Get_asym_significance_tags(string var_name)
    {
      if(map_asym_significancies_.find(var_name) == map_asym_significancies_.end())
        {
          ERROR("Asym Significances for variable: " << var_name << " does not exist!!");
          exit(EXIT_FAILURE);
        }
      vector<string> tags;
      map<string, TGraph*>::iterator s = map_asym_significancies_[var_name].begin();
      while(s!=map_asym_significancies_[var_name].end())
        {
          tags.push_back(s->first);
          s++;
        }
      return tags;
    }

  vector<string> Get_asym_significance_with_syst_tags(string var_name)
    {
      if(map_asym_significancies_with_syst_.find(var_name) == map_asym_significancies_with_syst_.end())
        {
          ERROR("Asym Significances with systematics for variable: " << var_name << " does not exist!!");
          exit(EXIT_FAILURE);
        }
      vector<string> tags;
      map<string, TGraph*>::iterator s = map_asym_significancies_with_syst_[var_name].begin();
      while(s!=map_asym_significancies_with_syst_[var_name].end())
        {
          tags.push_back(s->first);
          s++;
        }
      return tags;
    }

  void DrawEfficiencies(vector<string> sels_num, string sel_den, string var_name, vector<string> chain_tag);
  void DrawEfficiencies_fromConfig(string sel_den, string var_name);
  void DrawEfficiencies_SignalMap(string sel_num, string sel_den, string var_name);
  void Cuts_studies(vector<string>sels_den, string var_name, double WP);
  void Cuts_studies_withROC(vector<string>sels_den_s, string sel_s_substr, string sel_b_substr, string var_name);
  boost::optional<pair<TGraph*, TGraph*>> ROC_curve(vector<string> sels_num_s, vector<string> sels_num_b, string sel_den_s, string sel_den_b, string var_name, string tag, string nameaxis_eff_s="", string nameaxis_eff_b="", bool is_conventionalROC=true);

  void DrawSignificancies(vector<string> sels, string var_name, vector<string> signal_tags);
  void DrawSignificancies(vector<string> sels, vector<string> var_names, vector<string> signal_tags);
  void DrawSignificancies_fromConfig(vector<string> var_names);
  void CombineSignificancies(string path1, string path2);
  string CreateCombinedSignificancies(string path1, string path2);

  void XSectionsUpperLimits(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma);
  void XSectionsUpperLimits_Combined(vector<vector<string>> sels, vector<vector<string>> var_names, vector<string> signal_tags, double N_sigma);
  void DrawXSectionsUpperLimits(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma);
  void DrawXSectionsUpperLimits_Combined(vector<vector<string>> sels, vector<vector<string>> var_names, vector<string> signal_tags, double N_sigma);

  double CalculateSignificance(double yields_sig, double yields_Bkg);
  double CalculateAsymptoticSignificance(TH1D* h_sig, TH1D* h_Bkg);
  double CalculateAsymptoticSignificance_with_Syst(TH1D* h_sig, TH1D* h_Bkg);
  double CalculateXSection(TH1D* h_Bkg, TH1D* h_sig, double N_sigma);
  double CalculateXSection_Combined(vector<TH1D*> h_Bkg, vector<TH1D*> h_Sig, double N_sigma);

};



#endif
