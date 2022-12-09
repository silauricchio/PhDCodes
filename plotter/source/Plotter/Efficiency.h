#ifndef EFFICIENCY_H
#define EFFICIENCY_H

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

#include "HistoPlotter.h"
#include "GraphicsOptions.h"
#include "../util/Debug.h"

class Efficiency
{
 public:
  Efficiency(HistoMaker *h_maker=NULL, bool debug_mode=false, Graphics_options *GO=NULL)
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
  ~Efficiency();

 private:
  HistoMaker *h_Maker_;
  Graphics_options *GO_;
  bool debug_;

  map<string, TGraphAsymmErrors*> map_efficiencies_;
  

  TGraphAsymmErrors* MakeEfficiencyCurve(vector<string> sels_num, string sel_den, string var_name, string tag);

  void Add_efficiency_to_map(string chain_tag, TGraphAsymmErrors *eff)
  {
    map_efficiencies_[chain_tag] = eff;
  }


 public: 

  TGraphAsymmErrors* Get_efficiency(string chain_tag)
  {
    if((map_efficiencies_.find(chain_tag) == map_efficiencies_.end()) or (map_efficiencies_[chain_tag] == NULL))
      {
	ERROR("Efficiency map with tag: " << chain_tag << " does not exist!!");
	exit(EXIT_FAILURE);
      }
    TGraphAsymmErrors* graph = map_efficiencies_[chain_tag];
    return graph;
  }

  void DrawEfficiencies(vector<string> sels_num, string sel_den, string var_name, vector<string> chain_tag);

};



#endif
