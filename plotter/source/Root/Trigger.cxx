#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TEfficiency.h"
#include <iostream>
#include "TChain.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TGraphAsymmErrors.h"

#include "../Plotter/Trigger.h"
#include "../Plotter/ReadFiles.h"
#include "../Plotter/HistoPlotter.h"
#include "../Plotter/GraphicsOptions.h"
#include "../util/Debug.h"


using namespace std;

TEfficiency* TriggerEfficiency::Make_Efficiency_Curve(string var_name, string sel_name_num, string sel_name_den, string chain_tag)
{
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());
  
  TString name_histo_den = sel_name_den + "_" + chain_tag;
  TH1D *h_den = (TH1D*) file->Get(name_histo_den);

  TString name_histo_num = sel_name_num + "_" + chain_tag;
  TH1D *h_num = (TH1D*) file->Get(name_histo_num);
  double NBins_den = h_den->GetNbinsX();
 
  if(h_num->GetNbinsX() != h_den->GetNbinsX())
    {
      ERROR("The number of bins is different between num and den histos!!");
      exit(EXIT_FAILURE);
    }
  

  double integral_den = h_den->Integral(1, NBins_den+1);
  double integral_num = h_num->Integral(1, NBins_den+1);
  double efficiency = (double)(integral_num /integral_den);
  cout<<endl;
  cout << "EFFICIENCY FOR " << chain_tag << " " <<var_name << " " << sel_name_num << " over " << sel_name_den << " --> " << efficiency << endl;
  cout<<endl;
  string name = "Eff_" + sel_name_num + "_" + var_name + "_" + chain_tag;
  string title = "Efficiency of " + sel_name_num + ";" + h_num->GetXaxis()->GetTitle() + "; Efficiency";
 

  TEfficiency *eff = new TEfficiency(*h_num, *h_den);
  
  
  eff->SetName(name.c_str());
  eff->SetTitle(title.c_str());

  this->Add_efficiency_curve_to_map(name, eff);
  file->Close();
  return eff;
}

TEfficiency* TriggerEfficiency::Make_Efficiency_Map(string var_name1, string var_name2, string sel_name_num, string sel_name_den, string chain_tag)
{
  string name_num = "histo2D_" + var_name1 + "_" + var_name2 +  "_" + sel_name_num + "_" + chain_tag;
  string name_den = "histo2D_" + var_name1 + "_" + var_name2 +  "_" + sel_name_den + "_" + chain_tag;
  
  TFile *file_num = TFile::Open(("file_2D_" + sel_name_num + "_" + chain_tag + "_" + var_name1 + "_" + var_name2  + ".root").c_str());
  TH2D *histo_num = (TH2D*)file_num->Get(name_num.c_str());
  
  
  TFile *file_den =TFile::Open(("file_2D_" + sel_name_den + "_" + chain_tag + "_" + var_name1 + "_" + var_name2  + ".root").c_str());
  TH2D *histo_den = (TH2D*)file_den->Get(name_den.c_str());
  
  string name = "Eff_" + sel_name_num + "_" + var_name1 + "_" + var_name2 + "_" + chain_tag;
  string title = "Efficiency of " + sel_name_num + ";" + histo_num->GetXaxis()->GetTitle()+ ";"  +  histo_num->GetYaxis()->GetTitle();

  double nbinsX_num = histo_num->GetNbinsX();
  double nbinsY_num = histo_num->GetNbinsY();
  double nbinsX_den = histo_den->GetNbinsX();
  double nbinsY_den = histo_den->GetNbinsY();  
  if((nbinsX_num != nbinsX_den) or (nbinsY_num != nbinsY_den))
    {
      ERROR("The number of bins is different between num and den 2D histos!!");
      exit(EXIT_FAILURE);
    }

  TEfficiency *eff = new TEfficiency(*histo_num, *histo_den);

  eff->SetName(name.c_str());
  eff->SetTitle(title.c_str());

  this->Add_efficiency_map_to_map(name, eff);  

  return eff;
}



void TriggerEfficiency::Add_efficiency_curve_to_map(string name, TEfficiency* graph)
{
  this->map_efficiency_curves_[name] = graph;
}

vector<string>  TriggerEfficiency::Get_efficiency_curves_tags()
{
  vector<string> tags;
  map<string, TEfficiency*>::iterator s = this->map_efficiency_curves_.begin();
  while(s!= this->map_efficiency_curves_.end())
    {
      tags.push_back(s->first);
      s++;
    }
  return tags;
}

TEfficiency* TriggerEfficiency::Get_efficiency_curve(string tag)
{
  if(this->map_efficiency_curves_.find(tag) == this->map_efficiency_curves_.end())
    {
      ERROR("The efficiency curve with tag -> " << tag << " does not exist in the map!!");
      exit(EXIT_FAILURE);
    }
  TEfficiency *graph = this->map_efficiency_curves_[tag];
  return graph;
}

void TriggerEfficiency::Add_efficiency_map_to_map(string name, TEfficiency* graph)
{
  this->map_efficiency_maps_[name] = graph;
}

vector<string> TriggerEfficiency::Get_efficiency_maps_tags()
{
  vector<string> tags;
  map<string, TEfficiency*>::iterator s = this->map_efficiency_maps_.begin();
  while(s!= this->map_efficiency_maps_.end())
    {
      tags.push_back(s->first);
      s++;
    }
  return tags;
}

TEfficiency* TriggerEfficiency::Get_efficiency_map(string tag)
{
  if(this->map_efficiency_maps_.find(tag) == this->map_efficiency_maps_.end())
    {
      ERROR("The efficiency map with tag -> " << tag << " does not exist in the map!!");
      exit(EXIT_FAILURE);
    }
  TEfficiency *graph = this->map_efficiency_maps_[tag];
  return graph;
}



void TriggerEfficiency::Draw_Efficiency_Curves(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names, vector<string> chain_tags)
{

  if(sel_names_num.size() != sel_names_den.size())
    {
      ERROR("Vector with selections have different lenght between num and den");
      exit(EXIT_FAILURE);
    }
 

  for(auto var : var_names)
    {
      
      TCanvas *c = new TCanvas();
      this->GO_->Draw_Options();	  

      Color_t color;
      bool histo_colors = this->GO_->Get_overwrite_Colors();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

      if(this->GO_->Get_overwrite_Colors() and this->GO_->Get_colors().size() != (chain_tags.size() * sel_names_num.size()))
	{
	  WARNING("Number of drawing colors are different from number of efficiency curves !!");
	  histo_colors = false;
	}

      if(this->GO_->Get_overwrite_Legend() and this->GO_->Get_leg_entries().size() != (chain_tags.size() * sel_names_num.size()))
	{
	  WARNING("Number of legend entries are different from number of efficiency curves!!");
	  overwrite_leg = false;
	}

      
      double xmin = this->GO_->Get_leg_xmin();
      double xmax = this->GO_->Get_leg_xmax();
      double ymin = this->GO_->Get_leg_ymin();
      double ymax = this->GO_->Get_leg_ymax();

      TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
      TString leg_entry;

 
      int s = 0;
      for(int i = 0; i<sel_names_num.size(); i++)
	{
	  
	  string sel_num = sel_names_num.at(i);
	  string sel_den = sel_names_den.at(i);

	  for(auto tag : chain_tags)
	    {

	      TEfficiency *trigger_eff = this->Make_Efficiency_Curve(var, sel_num, sel_den, tag);
	      trigger_eff->SetMarkerStyle(8);

	      if(histo_colors)
		{
		  color = this->GO_->Get_colors().at(s);
		  trigger_eff->SetMarkerColor(color);
		}

	      leg_entry = trigger_eff ->GetName();
	      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
       	      leg->AddEntry(trigger_eff, leg_entry);

	      if (s==0) trigger_eff->Draw("AP");
	      else trigger_eff->Draw("same P");

	      

	      gPad->Update();
	      (trigger_eff->GetPaintedGraph())->SetMaximum(1.3);
	      if(GO_->Get_overwrite_Y_maximum()) (trigger_eff->GetPaintedGraph())->SetMaximum(GO_->Get_maximum());
	      if(GO_->Get_overwrite_Y_minimum()) (trigger_eff->GetPaintedGraph())->SetMinimum(GO_->Get_minimum());

	      
	      s++;
	    }
	}
      leg->Draw();

      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();

      c->SaveAs(("Trigger_eff_curve_" + var + ".pdf").c_str());

      string file_name = "file_Trigger_efficiencies_" + var + ".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");

      for(int i = 0; i<sel_names_num.size(); i++)
	{
	  string sel_num = sel_names_num.at(i);
          string sel_den = sel_names_den.at(i);

	  for(auto tag : chain_tags)
            {
	      string name = "Eff_" + sel_num + "_" + var + "_" + tag;
	      this->Get_efficiency_curve(name)->Write();
	    }
	}

      file_out->Close();
    }
}

void TriggerEfficiency::Draw_Efficiency_Maps(vector<string> sel_names_num, vector<string> sel_names_den, vector<string> var_names1, vector<string> var_names2, vector<string> chain_tags)
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


  for(int i = 0; i<var_names1.size(); i++)
    {
      string var1 = var_names1.at(i);
      string var2 = var_names2.at(i);

      for(int j = 0; j< sel_names_num.size(); j++)
	{

	  string sel_num = sel_names_num.at(i);
          string sel_den = sel_names_den.at(i);

	  for(auto tag : chain_tags)
	    {
	      TCanvas *c = new TCanvas();
	      GO_->Draw_Options();
	   
	      string name = "Eff_" + sel_num + "_" + var1 + "_" + var2 + "_" + tag;
	      TEfficiency *trigger_map = this->Make_Efficiency_Map(var1, var2, sel_num, sel_den, tag);
              trigger_map->Draw("colz");
	      
	      c->SaveAs(("Trigger_eff_map_" + var1+"_"+var2 +"_"+sel_num+ ".pdf").c_str());
	      

	      
	    }
	}

    }


  string file_name = "file_Trigger_efficiencies_maps.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  for(int i = 0; i<var_names1.size(); i++)
    {
      string var1 = var_names1.at(i);
      string var2 = var_names2.at(i);

      for(int j = 0; j< sel_names_num.size(); j++)
        {

          string sel_num = sel_names_num.at(i);
          string sel_den = sel_names_den.at(i);

          for(auto tag : chain_tags)
            {
	      string name = "Eff_" + sel_num + "_" + var1 + "_" + var2 + "_" + tag;
	      this->Get_efficiency_map(name)->Write();
	    }  
	}
    }
  file_out->Close();
}



