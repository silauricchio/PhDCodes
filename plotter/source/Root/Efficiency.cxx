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

#include "../Plotter/HistoPlotter.h"
#include "../Plotter/GraphicsOptions.h"
#include "../util/Debug.h"
#include "../Plotter/Efficiency.h"

TGraphAsymmErrors* Efficiency::MakeEfficiencyCurve(vector<string> sels_num, string sel_den, string var_name, string tag)
{
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());

  TString name_histo_den = var_name + "_" + sel_den + "_" + tag;
  TH1D *h_den = (TH1D*) file->Get(name_histo_den);

  double yields_den = h_den->Integral(1, h_den->GetNbinsX()+1);
  vector<Double_t> efficiency;
  vector<Double_t> eff_err_h, eff_err_l;
 
  for(auto sel : sels_num)
    {
      TString name_histo_num = var_name + "_" + sel + "_" + tag;
      TH1D *h_num = (TH1D*) file->Get(name_histo_num);

      double yields_num = h_num->Integral(1, h_num->GetNbinsX()+1);
      
      TEfficiency *Eff = new TEfficiency();

      efficiency.push_back(yields_num/yields_den);

      Double_t err_h = TEfficiency::ClopperPearson(yields_den, yields_num,  0.683,  true) - (yields_num/yields_den);
      Double_t err_l = (yields_num/yields_den) - TEfficiency::ClopperPearson(yields_den, yields_num, 0.683,  false);

      eff_err_h.push_back(err_h);
      eff_err_l.push_back(err_l);
    }
  TGraphAsymmErrors *graph = new TGraphAsymmErrors();
  string graph_name = tag;
  graph->SetName(graph_name.c_str());

  for(int i=0; i<sels_num.size(); i++)
    {
      graph->SetPoint(i, i + 1., efficiency.at(i)); 
      graph->GetXaxis()->SetNdivisions(-510);
      graph->SetPointError(i, 0, 0, eff_err_l.at(i), eff_err_h.at(i));
    }
  
  for(int i=0; i<sels_num.size(); i++)  graph->GetXaxis()->SetBinLabel(graph->GetXaxis()->FindBin(i+1.), (sels_num.at(i)).c_str());
  graph->GetXaxis()->LabelsOption("d");
    

  this->Add_efficiency_to_map(tag, graph);

  string file_name = "Info_efficiencies_" + tag + ".txt";
  ofstream fout;
  fout.open(file_name.c_str());
  fout << endl;
  fout << " EFFICIENCIES CALCULATED WITH RESPECT TO THE SELECTION -> " << sel_den << endl;
  fout << endl;
  fout << "Info for sample -> " << tag << endl;
  fout << endl;
  for(int i=0; i<sels_num.size(); i++) fout << sels_num.at(i) << "  ->  " << efficiency.at(i) << " + " << eff_err_h.at(i) << " - " <<  eff_err_l.at(i) << endl;
  fout.close();

  return graph;
}


void Efficiency::DrawEfficiencies(vector<string> sels_num, string sel_den, string var_name, vector<string> chain_tag)
{

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;
  cout << " for selections : ";
  for(auto sel : sels_num) cout << sel << " ";
  cout<<endl;
  cout << " with respect to selection: " << sel_den<< endl;
  
  TCanvas *c = new TCanvas();
  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != chain_tag.size()))
    {
      WARNING("Number of drawing colors are different from number of efficiency graphs !!");
      graph_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and (this->GO_->Get_leg_entries().size() != chain_tag.size()))
    {
      WARNING("Number of legend entries are different from number of efficiency graphs!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  TString leg_entry;

  int s=0;
  for(auto tag : chain_tag)
    {
      cout<< endl;
      cout << "  --> On sample:  " << tag << endl;
      
      TGraphAsymmErrors *eff = this->MakeEfficiencyCurve(sels_num, sel_den, var_name, tag);

      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  eff->SetLineColor(color);
	}
      
      leg_entry = eff ->GetName();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
      leg->AddEntry(eff, leg_entry);

      if (s==0) eff->Draw("AL");
      else eff->Draw("same L");

      gPad->Update();
      if(this->GO_->Get_overwrite_Y_maximum()) eff->SetMaximum(this->GO_->Get_maximum());
      eff->SetMinimum(0);
      s++;
    }

  leg->Draw();

  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->SaveAs("Efficiencies.pdf");


  string file_name = "file_Efficiencies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
  for(auto tag : chain_tag)
    {
      TGraphAsymmErrors *eff = this->Get_efficiency(tag);
      eff->Write();
    }
  file_out->Close();

}


void Statistics::DrawSignificancies(vector<string> sels_num, string sel_den, string var_name, vector<string> chain_tag)
{

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;
  cout << " for selections : ";
  for(auto sel : sels_num) cout << sel << " ";
  cout<<endl;
  cout << " with respect to selection: " << sel_den<< endl;
  
  TCanvas *c = new TCanvas();
  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != chain_tag.size()))
    {
      WARNING("Number of drawing colors are different from number of efficiency graphs !!");
      graph_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and (this->GO_->Get_leg_entries().size() != chain_tag.size()))
    {
      WARNING("Number of legend entries are different from number of efficiency graphs!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);
  TString leg_entry;

  int s=0;
  for(auto tag : chain_tag)
    {
      cout<< endl;
      cout << "  --> On sample:  " << tag << endl;
      
      TGraphAsymmErrors *eff = this->MakeEfficiencyCurve(sels_num, sel_den, var_name, tag);

      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  eff->SetLineColor(color);
	}
      
      leg_entry = eff ->GetName();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
      leg->AddEntry(eff, leg_entry);

      if (s==0) eff->Draw("AL");
      else eff->Draw("same L");

      gPad->Update();
      if(this->GO_->Get_overwrite_Y_maximum()) eff->SetMaximum(this->GO_->Get_maximum());
      eff->SetMinimum(0);
      s++;
    }

  leg->Draw();

  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->SaveAs("Efficiencies.pdf");


  string file_name = "file_Efficiencies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
  for(auto tag : chain_tag)
    {
      TGraphAsymmErrors *eff = this->Get_efficiency(tag);
      eff->Write();
    }
  file_out->Close();

}

