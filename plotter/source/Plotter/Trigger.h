#ifndef TRIGGER_H
#define TRIGGER_H

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <iostream>
#include "TChain.h"
#include "TPad.h"
#include "TVirtualPad.h"

#include "../Plotter/ReadFiles.h"
#include "../Plotter/HistoPlotter.h"
#include "../Plotter/GraphicsOptions.h"
#include "../util/Debug.h"


class TriggerEfficiency
{
public:
TriggerEfficiency(HistoMaker *h_maker=NULL, bool debug_mode=false, Graphics_options *GO=NULL)
{
if(h_maker == NULL)
	{
	  ERROR("The HistoMaker passed to the TriggerEfficiency class does not exist!!");
	  exit(EXIT_FAILURE);
	}
h_Maker_ = h_maker;

 if(GO == NULL)
	{
	  ERROR("The GraphicsOptions passed to the TriggerEfficiency class does not exist!!");
	  exit(EXIT_FAILURE);
	}
GO_ = GO;
debug_ = debug_mode;

};
~TriggerEfficiency();

private:
HistoMaker *h_Maker_;
Graphics_options *GO_;
bool debug_;

map<string, TEfficiency*> map_efficiency_curves_;
map<string, TEfficiency*> map_efficiency_maps_;

TEfficiency* Make_Efficiency_Curve(string var_name, string sel_name_num, string sel_name_den, string chain_tag);
TEfficiency* Make_Efficiency_Map(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag);



public:

void Add_efficiency_curve_to_map(string name, TEfficiency* graph);
vector<string>  Get_efficiency_curves_tags();
TEfficiency* Get_efficiency_curve(string tag);
void Add_efficiency_map_to_map(string name, TEfficiency* graph);
vector<string>  Get_efficiency_maps_tags();
TEfficiency* Get_efficiency_map(string tag);

void Draw_Efficiency_Curves(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names, vector<string> chain_tags);
void Draw_Efficiency_Maps(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names1, vector<string> var_names2, vector<string> chain_tags); 


};











#endif
