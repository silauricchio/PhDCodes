#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TTree.h"
#include "TPaletteAxis.h"
#include "TEfficiency.h"
#include "TChain.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TLatex.h"

#include <math.h>
#include <iostream>
#include <iomanip>
#include <boost/algorithm/string/replace.hpp>
#include <boost/optional.hpp>
#include <typeinfo> 

#include "../Plotter/HistoPlotter.h"
#include "../Plotter/GraphicsOptions.h"
#include "../util/Debug.h"
#include "../Plotter/Statistics.h"

using namespace std;

TGraphAsymmErrors* Statistics::MakeEfficiencyCurve(vector<string> sels_num, string sel_den, string var_name, string tag)
{
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());

  TString name_histo_den = sel_den + "_" + tag;
  TH1D *h_den = (TH1D*) file->Get(name_histo_den);

  double yields_den = h_den->Integral(1, h_den->GetNbinsX()+1);
  if (h_Maker_->Get_ScaletoBinWidth()) yields_den = h_den->Integral(1, h_den->GetNbinsX()+1, "width");

  if(yields_den<=0)
    {
      ERROR("Yields at denominator of efficency are not >0!!");
      return NULL;
    }
  
  if(yields_den<=20) //canc
    {
      ERROR("Yields at denominator of efficency are to few!!"); //canc
      return NULL;//canc
    }//canc
  
  vector<Double_t> efficiency;
  vector<Double_t> eff_err_h, eff_err_l;

  for(auto sel : sels_num)
    {
      TString name_histo_num = sel + "_" + tag;

      TH1D *h_num = (TH1D*) file->Get(name_histo_num);

      double yields_num = h_num->Integral(1, h_num->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_num = h_num->Integral(1, h_num->GetNbinsX()+1, "width");

      efficiency.push_back((Double_t)(yields_num/yields_den));

      Double_t err_h = TEfficiency::ClopperPearson(yields_den, yields_num,  0.683,  true) - (yields_num/yields_den);
      Double_t err_l = (yields_num/yields_den) - TEfficiency::ClopperPearson(yields_den, yields_num, 0.683,  false);

      eff_err_h.push_back(err_h);
      eff_err_l.push_back(err_l);
    }

  TGraphAsymmErrors *graph = new TGraphAsymmErrors();
  string graph_name = tag;
  graph->SetName(graph_name.c_str());

  string graph_title = "Efficiencies wrt " + sel_den;
  graph->SetTitle(graph_title.c_str());

  

  for(int i=0; i<sels_num.size(); i++)
    {
      /*
      unsigned last = sels_num.at(i).find_last_of("_");
      std::string d2_str = sels_num.at(i).substr(last+1);
      boost::replace_all(d2_str, "p", ".");
      double cutd2 = std::stod(d2_str); 
      */
      //graph->SetPoint(i, cutd2, efficiency.at(i)); 
      graph->SetPoint(i, i + 1., efficiency.at(i)); 
      graph->GetXaxis()->SetNdivisions(-510);
      graph->SetPointError(i, 0, 0, eff_err_l.at(i), eff_err_h.at(i));
    }
  //graph->GetXaxis()->SetTitle("D2X<cut");
  //graph->GetXaxis()->SetLabelSize(0.02); 
  
  graph->GetXaxis()->SetAlphanumeric();
  for(int i=0; i<sels_num.size(); i++)  graph->GetXaxis()->SetBinLabel(graph->GetXaxis()->FindBin(i+1.), (sels_num.at(i)).c_str());
  graph->GetXaxis()->LabelsOption("d");
  

  this->Add_efficiency_to_map(tag, sel_den, graph);

  string file_name = "Info_efficiencies_" + tag + "_" + sel_den +".txt";
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


TGraph* Statistics::MakeYieldsCurve(vector<string> sels_num, string var_name, string tag)
{
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());

  vector<Double_t> efficiency;
  vector<Double_t> eff_err_h, eff_err_l;

  for(auto sel : sels_num)
    {
      TString name_histo_num = sel + "_" + tag;

      TH1D *h_num = (TH1D*) file->Get(name_histo_num);

      double yields_num = h_num->Integral(1, h_num->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_num = h_num->Integral(1, h_num->GetNbinsX()+1, "width");

      efficiency.push_back((Double_t)(yields_num));
    }

  TGraph *graph = new TGraph();
  string graph_name = tag;
  graph->SetName(graph_name.c_str());

  string graph_title = "Yields";
  graph->SetTitle(graph_title.c_str());

  
  for(int i=0; i<sels_num.size(); i++)
    {
      graph->SetPoint(i, i + 1., efficiency.at(i)); 
      graph->GetXaxis()->SetNdivisions(-510);
    }
  
  graph->GetXaxis()->SetLabelSize(0.02); 
  
  graph->GetXaxis()->SetAlphanumeric();
  for(int i=0; i<sels_num.size(); i++)  graph->GetXaxis()->SetBinLabel(graph->GetXaxis()->FindBin(i+1.), (sels_num.at(i)).c_str());
  graph->GetXaxis()->LabelsOption("d");
  
  string file_name = "Info_Yields_" + tag + ".txt";
  ofstream fout;
  fout.open(file_name.c_str());
  fout << endl;
  fout << " CALCULATING YIELDS" << endl;
  fout << endl;
  fout << "For sample -> " << tag << endl;
  fout << endl;
  for(int i=0; i<sels_num.size(); i++) fout << sels_num.at(i) << "  ->  " << efficiency.at(i) << endl;
  fout.close();

  return graph;
}


void Statistics::DrawEfficiencies(vector<string> sels_num, string sel_den, string var_name, vector<string> chain_tag)
{

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;
  cout << " for selections : ";
  for(auto sel : sels_num) cout << sel << " ";
  cout<<endl;
  cout << " with respect to selection: " << sel_den<< endl;
  
  TCanvas *c = new TCanvas();
  if(this->GO_->Get_logy())  c->SetLogy();

  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
  bool graph_options = this->GO_->Get_overwrite_Draw_Options();

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
  
  TString map_eff_name = "Eff_" + sel_den + "_" + var_name;
  TString map_eff_title = "Efficiency wrt " + sel_den;
  Int_t nbinsX = sels_num.size();
  Int_t nbinsY = chain_tag.size();

  TH2D *map_eff = new TH2D(map_eff_name, map_eff_title, nbinsX, 0, nbinsX, nbinsY, 0, nbinsY);

  for(int i=0; i<nbinsY; i++)  map_eff->GetYaxis()->SetBinLabel(i+1, (chain_tag.at(i)).c_str());
  for(int i=0; i<nbinsX; i++)  map_eff->GetXaxis()->SetBinLabel(i+1, (sels_num.at(i)).c_str());
  map_eff->GetYaxis()->LabelsOption("u");
  map_eff->GetXaxis()->LabelsOption("d");
  map_eff->GetYaxis()->SetLabelSize(0.02);
  map_eff->GetXaxis()->SetLabelSize(0.02);

  int s=0;
  for(auto tag : chain_tag)
    {
      cout<< endl;
      cout << "  --> On sample:  " << tag << endl;
      
      TGraphAsymmErrors *eff = this->MakeEfficiencyCurve(sels_num, sel_den, var_name, tag);

      for(int i=0; i<nbinsX; i++) map_eff -> SetBinContent((i+1), (s+1), eff->GetPointY(i));

      TString draw_options;

      if(graph_options)
	{
	  draw_options = this->GO_->Get_draw_options().at(s);
	}
      else draw_options = "L";

      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  eff->SetLineColor(color);
	}
      else draw_options+= "PLC";

      leg_entry = eff ->GetName();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
      leg->AddEntry(eff, leg_entry);

      if (s==0) draw_options+="A";
      else draw_options+= "same";

      eff->GetYaxis()->SetTitle("Efficiency");
      eff->GetXaxis()->LabelsOption("d");

      eff ->Draw(draw_options);

      eff->SetMaximum(1);
      eff->SetMinimum(0);
      
      if(this->GO_->Get_overwrite_Y_maximum()) eff->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum()) eff->SetMinimum(this->GO_->Get_minimum());

      s++;
    }

  leg->Draw();

  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->SaveAs(("Efficiencies_wrt_"+ sel_den + ".pdf").c_str());

  TCanvas *c2 = new TCanvas();
  map_eff->SetMarkerSize(1);
  map_eff->SetMarkerColor(kWhite);
  gStyle->SetPalette(104);
  map_eff->Draw("COLZ TEXT");
  c2->SaveAs(("Efficiencies_2Dhisto_wrt_"+ sel_den + ".pdf").c_str());

  string file_name = "file_Efficiencies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
  for(auto tag : chain_tag)
    {
      TGraphAsymmErrors *eff = this->Get_efficiency(tag, sel_den);
      eff->Write();
    }
  file_out->Close();

}

void Statistics::DrawEfficiencies_fromConfig(string sel_den, string var_name)
{
  vector<string> sels_num = h_Maker_->Get_reader()->Get_sel_tags();
  reverse(sels_num.begin(), sels_num.end()); 
  vector<string> chain_tag =  h_Maker_->Get_chain_tags();

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;
  cout << " for selections : ";
  for(auto sel : sels_num) cout << sel << " ";
  cout<<endl;
  cout << " with respect to selection: " << sel_den<< endl;
  
  TCanvas *c = new TCanvas();
  if(this->GO_->Get_logy())  c->SetLogy();
  TCanvas *c_yields = new TCanvas();
  if(this->GO_->Get_logy())  c_yields->SetLogy();

  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
  bool graph_options = this->GO_->Get_overwrite_Draw_Options();

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
  TLegend *leg_yields = new TLegend(xmin, ymin, xmax, ymax);

  TString leg_entry;
  
  TString map_eff_name = "Eff_" + sel_den + "_" + var_name;
  TString map_eff_title = "Efficiency wrt " + sel_den;
  TString map_yields_name = "Yields";
  TString map_yields_title = "Yields";
  Int_t nbinsX = sels_num.size();
  Int_t nbinsY = chain_tag.size();

  TH2D *map_eff = new TH2D(map_eff_name, map_eff_title, nbinsX, 0, nbinsX, nbinsY, 0, nbinsY);
  TH2D *map_yields = new TH2D(map_yields_name, map_yields_title, nbinsX, 0, nbinsX, nbinsY, 0, nbinsY);

  for(int i=0; i<nbinsY; i++)
    {
      map_eff->GetYaxis()->SetBinLabel(i+1, (chain_tag.at(i)).c_str());
      map_yields->GetYaxis()->SetBinLabel(i+1, (chain_tag.at(i)).c_str());
    }
  for(int i=0; i<nbinsX; i++)
    {
      map_eff->GetXaxis()->SetBinLabel(i+1, (sels_num.at(i)).c_str());
      map_yields->GetXaxis()->SetBinLabel(i+1, (sels_num.at(i)).c_str());
    }
  map_eff->GetYaxis()->LabelsOption("u");
  map_eff->GetXaxis()->LabelsOption("d");
  map_eff->GetYaxis()->SetLabelSize(0.02);
  map_eff->GetXaxis()->SetLabelSize(0.02);
  map_yields->GetYaxis()->LabelsOption("u");
  map_yields->GetXaxis()->LabelsOption("d");
  map_yields->GetYaxis()->SetLabelSize(0.02);
  map_yields->GetXaxis()->SetLabelSize(0.02);

  int s=0;
  for(auto tag : chain_tag)
    {
      cout<< endl;
      cout << "  --> On sample:  " << tag << endl;
      
      TGraphAsymmErrors *eff = this->MakeEfficiencyCurve(sels_num, sel_den, var_name, tag);
      TGraph *yields = this->MakeYieldsCurve(sels_num, var_name, tag);

      if(eff==NULL)
	{
	  s++;
	  continue;
	}

      for(int i=0; i<nbinsX; i++)
	{
	  map_eff -> SetBinContent((i+1), (s+1), eff->GetPointY(i));
	  map_yields -> SetBinContent((i+1), (s+1), yields->GetPointY(i));
	}

      TString draw_options;

      if(graph_options)
	{
	  draw_options = this->GO_->Get_draw_options().at(s);
	}
      else draw_options = "L";

      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  eff->SetLineColor(color);
	  yields->SetLineColor(color);
	}
      else draw_options+= "PLC";

      leg_entry = eff ->GetName();
      if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);

      leg->AddEntry(eff, leg_entry);
      leg_yields->AddEntry(yields, leg_entry);

      if (s==0) draw_options+="A";
      else draw_options+= "same";

      eff->GetYaxis()->SetTitle("Efficiency");
      eff->GetXaxis()->LabelsOption("d");

      yields->GetYaxis()->SetTitle("Yields");
      yields->GetXaxis()->LabelsOption("d");

      c->cd();
      eff ->Draw(draw_options);

      eff->SetMaximum(1);
      eff->SetMinimum(0);
      
      if(this->GO_->Get_overwrite_Y_maximum()) eff->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum()) eff->SetMinimum(this->GO_->Get_minimum());

      c_yields->cd();
      yields ->Draw(draw_options);

      if(this->GO_->Get_overwrite_Y_maximum()) yields->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum()) yields->SetMinimum(this->GO_->Get_minimum());

      s++;
    }

  c->cd();
  leg->Draw();

  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();

  c->SaveAs(("Efficiencies_wrt_" + sel_den +".pdf").c_str());

  c_yields->cd();
  leg_yields->Draw();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  c_yields->SaveAs(("Yields_"  + sels_num.at(0)+ ".pdf").c_str());

  TCanvas *c2 = new TCanvas();
  gStyle->SetPaintTextFormat("1.3f");
  map_eff->SetMarkerSize(1);
  map_eff->SetMarkerColor(kWhite);
  gStyle->SetPalette(104);
  map_eff->Draw("COLZ TEXT45");
  c2->SaveAs(("Efficiencies_2Dhisto_wrt_" + sel_den + ".pdf").c_str());

  TCanvas *c2_yields = new TCanvas();
  gStyle->SetPaintTextFormat("7.0f");
  map_yields->SetMarkerSize(1);
  map_yields->SetMarkerColor(kWhite);
  gStyle->SetPalette(104);
  map_yields->Draw("COLZ TEXT45");
  c2_yields->SaveAs(("Yields_2Dhisto_for_" + sels_num.at(0)+ ".pdf").c_str());

  string file_name = "file_Efficiencies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
  for(auto tag : chain_tag)
    {
      TGraphAsymmErrors *eff = this->Get_efficiency(tag, sel_den);
      eff->Write();
    }
  file_out->Close();

}



void Statistics::DrawSignificancies(vector<string> sels, string var_name, vector<string> signal_tags)
{

  bool isApplySystematics = this->Get_apply_systematics();
  
  cout<<endl;
  cout << " CALCULATING SIGNIFICANCE ON VARIABLE: " << var_name << endl;
  cout << " for selections : ";
  for(auto sel : sels) cout << sel << " ";
  cout<<endl;
  
  TCanvas *c_sig = new TCanvas();
  TCanvas *c_asysig = new TCanvas();
  TCanvas *c_asysig_with_syst;
  if (isApplySystematics) c_asysig_with_syst = new TCanvas();
  
  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != signal_tags.size()))
    {
      WARNING("Number of drawing colors are different from number of significance graphs !!");
      graph_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and (this->GO_->Get_leg_entries().size() != signal_tags.size()))
    {
      WARNING("Number of legend entries are different from number of significance graphs!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg_sig = new TLegend(xmin, ymin, xmax, ymax);
  TLegend *leg_asysig = new TLegend(xmin, ymin, xmax, ymax);
  TLegend *leg_asysig_with_syst;
  if(isApplySystematics) leg_asysig_with_syst = new TLegend(xmin, ymin, xmax, ymax);
  TString leg_entry_sig, leg_entry_asysig, leg_entry_asysig_with_syst;

  int s=0;
  for(auto tag : signal_tags)
    {
      cout<< endl;
      cout << "  --> On signal:  " << tag << endl;
      
      this->MakeSignificanceCurve(sels, var_name, tag);
      
      TGraph *sig = this->Get_significance(var_name, tag);
      TGraph *asysig = this->Get_asym_significance(var_name, tag);
      TGraph *asysig_with_syst;
      if(isApplySystematics) asysig_with_syst = this->Get_asym_significance_with_Syst(var_name, tag);

      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  sig->SetLineColor(color);
	  asysig->SetLineColor(color);
	  if(isApplySystematics) asysig_with_syst->SetLineColor(color);
	}
      
      leg_entry_sig = sig ->GetName();
      leg_entry_asysig = asysig ->GetName();
      if(isApplySystematics) leg_entry_asysig_with_syst = asysig_with_syst ->GetName();
      if(overwrite_leg)
	{
	  leg_entry_sig = this->GO_->Get_leg_entries().at(s);
	  leg_entry_asysig = this->GO_->Get_leg_entries().at(s);
	  if(isApplySystematics) leg_entry_asysig_with_syst = this->GO_->Get_leg_entries().at(s);
	}
      leg_sig->AddEntry(sig, leg_entry_sig);
      leg_asysig->AddEntry(asysig, leg_entry_asysig);
      if(isApplySystematics) leg_asysig_with_syst->AddEntry(asysig_with_syst, leg_entry_asysig_with_syst);
      
      c_sig->cd();
      if (s==0) sig->Draw("AL");
      else sig->Draw("same L");
      gPad->Update();
      if(this->GO_->Get_overwrite_Y_maximum()) sig->SetMaximum(this->GO_->Get_maximum());
      sig->SetMinimum(0);
      if(this->GO_->Get_overwrite_Y_minimum()) sig->SetMinimum(this->GO_->Get_minimum());
      
      c_asysig->cd();
      if (s==0) asysig->Draw("AL");
      else asysig->Draw("same L");
      gPad->Update();
      if(this->GO_->Get_overwrite_Y_maximum()) asysig->SetMaximum(this->GO_->Get_maximum());
      asysig->SetMinimum(0);
      if(this->GO_->Get_overwrite_Y_minimum()) asysig->SetMinimum(this->GO_->Get_minimum());

      if(isApplySystematics) 
	{
	  c_asysig_with_syst->cd();
	  if (s==0) asysig_with_syst->Draw("AL");
	  else asysig_with_syst->Draw("same L");
	  gPad->Update();
	  if(this->GO_->Get_overwrite_Y_maximum()) asysig_with_syst->SetMaximum(this->GO_->Get_maximum());
	  asysig_with_syst->SetMinimum(0);
	  if(this->GO_->Get_overwrite_Y_minimum()) asysig_with_syst->SetMinimum(this->GO_->Get_minimum());
	}

      s++;
    }

  c_sig->cd();
  leg_sig->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  TLatex l;
  l.SetTextFont(72);
  l.SetTextSize(0.035);
  l.DrawLatexNDC(0.14, 0.75, ("#scale[0.8]{On variable: " + var_name +"}").c_str());
  c_sig->SaveAs(("Significancies_" + var_name + ".pdf").c_str());

  c_asysig->cd();
  leg_asysig->Draw();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  l.DrawLatexNDC(0.14, 0.75, ("#scale[0.8]{On variable: " + var_name +"}").c_str());
  c_asysig->SaveAs(("Asymptotic_Significancies_" + var_name + ".pdf").c_str());

  if(isApplySystematics) 
    {
      c_asysig_with_syst->cd();
      leg_asysig_with_syst->Draw();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();
      l.DrawLatexNDC(0.14, 0.75, ("#scale[0.8]{On variable: " + var_name +"}").c_str());
      c_asysig_with_syst->SaveAs(("Asymptotic_Significancies_with_Syst_" + var_name + ".pdf").c_str());
    }
}

void Statistics::DrawSignificancies(vector<string> sels, vector<string> var_names, vector<string> signal_tags)
{
  
  bool isApplySystematics = this->Get_apply_systematics();

  for(auto var : var_names) this->DrawSignificancies(sels, var, signal_tags);


  string file_name = "file_Significancies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
 
  for(auto var : var_names)
    {
      for(auto tag : signal_tags)
	{
	  TGraph *sig = this->Get_significance(var, tag);
	  sig->Write();

	  sig = this->Get_asym_significance(var, tag);
	  sig->Write();

	  if(isApplySystematics) sig = this->Get_asym_significance_with_Syst(var, tag);
	  sig->Write();
	}
    }
  file_out->Close();    
}

void Statistics::DrawSignificancies_fromConfig(vector<string> var_names)
{
  
  vector<string> sels = h_Maker_->Get_reader()->Get_sel_tags();
  vector<string> chain_tag =  h_Maker_->Get_chain_tags();
  vector<string> signal_tags;

  for(int i = 0; i<chain_tag.size(); i++) 
    {
      if(strstr((chain_tag.at(i)).c_str(),"Bkg")==NULL) signal_tags.push_back(chain_tag.at(i));
    }

  bool isApplySystematics = this->Get_apply_systematics();

  for(auto var : var_names) this->DrawSignificancies(sels, var, signal_tags);


  string file_name = "file_Significancies_.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
 
  for(auto var : var_names)
    {
      for(auto tag : signal_tags)
	{
	  TGraph *sig = this->Get_significance(var, tag);
	  sig->Write();

	  sig = this->Get_asym_significance(var, tag);
	  sig->Write();

	  if(isApplySystematics) sig = this->Get_asym_significance_with_Syst(var, tag);
	  sig->Write();
	}
    }
  file_out->Close();    
}


void Statistics::CombineSignificancies(string path1, string path2)
{
  string file_name = this->CreateCombinedSignificancies(path1, path2);
  
  TFile *file = TFile::Open(file_name.c_str());

  bool isApplySystematics = false;

  TObject *obj;
  TKey *key;
  TIter next( file->GetListOfKeys());
  while ((key = (TKey *) next()))
    {
      obj = file->Get(key->GetName());
      if(strstr(((std::string)key->GetName()).c_str(), "withsys")!=NULL) isApplySystematics = true;
    }

  TCanvas *c_sig = new TCanvas();
  TCanvas *c_asysig = new TCanvas();
  TCanvas *c_asysig_with_syst;
  
  if (isApplySystematics) c_asysig_with_syst = new TCanvas();
  
  this->GO_->Draw_Options();
  
  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg_sig = new TLegend(xmin, ymin, xmax, ymax);
  TLegend *leg_asysig = new TLegend(xmin, ymin, xmax, ymax);
  TLegend *leg_asysig_with_syst = new TLegend(xmin, ymin, xmax, ymax);

  TString leg_entry_sig;

  TObject *Obj;
  TKey *Key;
  TIter Next( file->GetListOfKeys());

  int s = 0;
  while ((Key = (TKey *) Next()))
    {
      Obj = file->Get(Key->GetName());
      //printf(" found object:%s\n",key->GetName());                                                                                                                                                        
      if(!Obj->InheritsFrom(TGraph::Class())) continue;

      TGraph *sig = (TGraph*) Obj->Clone();
 
      leg_entry_sig = sig ->GetName();
     
     
      if(strstr(((std::string)Key->GetName()).c_str(), "asy")==NULL)
	{
	  leg_sig->AddEntry(sig, leg_entry_sig);
	  c_sig->cd();
	}
      if((strstr(((std::string)Key->GetName()).c_str(), "asy")!=NULL) and ((((std::string)Key->GetName()).c_str(), "sys")==NULL)) 
	{
	  leg_asysig->AddEntry(sig, leg_entry_sig);
	  c_asysig->cd();
	}
      if(strstr(((std::string)Key->GetName()).c_str(), "sys")!=NULL) 
	{ 
	  leg_asysig_with_syst->AddEntry(sig, leg_entry_sig);
	  c_asysig_with_syst->cd(); 
	}

      if (s==0) sig->Draw("AL PLC");
      else sig->Draw("same L PLC");
      gPad->Update();
      if(this->GO_->Get_overwrite_Y_maximum()) sig->SetMaximum(this->GO_->Get_maximum());
      sig->SetMinimum(0);
      if(this->GO_->Get_overwrite_Y_minimum()) sig->SetMinimum(this->GO_->Get_minimum());

      s++;
    }

  c_sig->cd();
  leg_sig->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  c_sig->SaveAs("Combined_significancies.pdf");

  c_asysig->cd();
  leg_asysig->Draw();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  c_asysig->SaveAs("Combined_asymptotic_Significancies.pdf");

  if(isApplySystematics) 
    {
      c_asysig_with_syst->cd();
      leg_asysig_with_syst->Draw();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();
      c_asysig_with_syst->SaveAs("Combined_asymptotic_Significancies_with_Syst.pdf");
    }
 
}


string Statistics::CreateCombinedSignificancies(string path1, string path2)
{
  TFile *file1 = TFile::Open(path1.c_str());
  TFile *file2= TFile::Open(path2.c_str());

  string file_name = "file_Significancies_combined.root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
  TObject *obj;
  TKey *key;
  TIter next (file1->GetListOfKeys());
  
  TIter list2 (file2->GetListOfKeys());

  while ((key = (TKey *) next()))
    {

      TString graph_name = ((std::string)key->GetName()).c_str();
      
      bool is_infile2 = false;
      
      TKey *key2;
      while ((key2 = (TKey *) list2()))
	{
	  if(!strcmp(((std::string)key2->GetName()).c_str(), graph_name)) is_infile2 = true;
	  if(is_infile2) break;
	}

      if(!is_infile2) continue;

      if(strstr(graph_name, "asy")==NULL)
	{
 
	  TGraph *sig_1 = (TGraph*)file1->Get(graph_name);
	  TGraph *sig_2 = (TGraph*)file2->Get(graph_name);
	  
	  if(sig_1->GetN() != sig_2->GetN())
	    {
	      ERROR("Different number of points between graphs with significancies");
	      exit(EXIT_FAILURE);
	    }
	  
	  TGraph *combined_sig = new TGraph();
	  combined_sig->SetName(graph_name);
	  
	  for(int i = 0; i <sig_1->GetN(); i++ ) 
	    {
	      combined_sig->SetPoint(i, i + 1., sqrt(((sig_1->GetPointY(i))*(sig_1->GetPointY(i)))+((sig_2->GetPointY(i))*(sig_2->GetPointY(i)))));
	    }
	  
	  for(int i = 0; i <sig_1->GetN(); i++ )
	    {
	      combined_sig->GetXaxis()->SetBinLabel(combined_sig->GetXaxis()->FindBin(i+1.), sig_1->GetXaxis()->GetBinLabel(sig_1->GetXaxis()->FindBin(i+1.)));
	    }
	  
	  
	  file_out->cd();
	  combined_sig->Write();
	}
	 
      if((strstr(graph_name, "asy")!=NULL) and (strstr(graph_name, "sys")==NULL) )
	{
	  TGraph *asysig_1 = (TGraph*)file1->Get(graph_name);
          TGraph *asysig_2 = (TGraph*)file2->Get(graph_name);

	  if(asysig_1->GetN() != asysig_2->GetN())
	    {
	      ERROR("Different number of points between graphs with significancies");
	      exit(EXIT_FAILURE);
	    }
		  
	  TGraph *combined_asysig = new TGraph();
	  combined_asysig->SetName(graph_name);

	  for(int i = 0; i <asysig_1->GetN(); i++ )
	    {
	      combined_asysig->SetPoint(i, i + 1., sqrt(((asysig_1->GetPointY(i))*(asysig_1->GetPointY(i)))+((asysig_2->GetPointY(i))*(asysig_2->GetPointY(i)))));
	    }
		  
	  for(int i = 0; i<asysig_1->GetN(); i++ )
	    {
	      combined_asysig->GetXaxis()->SetBinLabel(combined_asysig->GetXaxis()->FindBin(i+1.), asysig_1->GetXaxis()->GetBinLabel(asysig_1->GetXaxis()->FindBin(i+1.)));
	    }
	

	  file_out->cd();
	  if(combined_asysig != NULL) combined_asysig->Write();
	  
	}

      if(strstr(graph_name, "sys")!=NULL)
	{
	  TGraph *asysig_withsys_1 = (TGraph*)file1->Get(graph_name);
          TGraph *asysig_withsys_2 = (TGraph*)file2->Get(graph_name);

	  if(asysig_withsys_1->GetN() != asysig_withsys_2->GetN())
	    {
	      ERROR("Different number of points between graphs with significancies");
	      exit(EXIT_FAILURE);
	    }
	  
	  TGraph *combined_asysig_withsys = new TGraph();
	  combined_asysig_withsys->SetName(graph_name);	  

	  for(int i = 0; i<asysig_withsys_1->GetN(); i++ )
	    {
	      combined_asysig_withsys->SetPoint(i, i + 1., sqrt(((asysig_withsys_1->GetPointY(i))*(asysig_withsys_1->GetPointY(i)))+((asysig_withsys_2->GetPointY(i))*(asysig_withsys_2->GetPointY(i)))));
	    }
	  
	  for(int i = 0; i<asysig_withsys_1->GetN(); i++ )
	    {
	      combined_asysig_withsys->GetXaxis()->SetBinLabel(combined_asysig_withsys->GetXaxis()->FindBin(i+1.), asysig_withsys_1->GetXaxis()->GetBinLabel(asysig_withsys_1->GetXaxis()->FindBin(i+1.)));
	    }
	  
	  file_out->cd();
	  if(combined_asysig_withsys != NULL) combined_asysig_withsys->Write();
	}
    }
    
  file_out->Close();    
  file1->Close();
  file2->Close();
  
  return file_name;
}

void Statistics::MakeSignificanceCurve(vector<string> sels, string var_name, string tag)
{
  bool isApplySystematics = this->Get_apply_systematics();
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());

  vector<double> Sig, AsymSig, AsymSig_with_Syst;
  for(auto sel : sels)
    {
      TString name_histo_signal = sel + "_" + tag;
      TH1D *h_signal = (TH1D*) file->Get(name_histo_signal);
      double yields_signal = h_signal->Integral(1, h_signal->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_signal = h_signal->Integral(1, h_signal->GetNbinsX()+1, "width");      

      TString name_histo_Bkg =  "Bkg_" + sel;
      TH1D *h_bkg = (TH1D*) file->Get(name_histo_Bkg); 
      double yields_bkg = h_bkg->Integral(1, h_bkg->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_bkg = h_bkg->Integral(1, h_bkg->GetNbinsX()+1, "width");

      Sig.push_back(this->CalculateSignificance(yields_signal, yields_bkg));
      AsymSig.push_back(this->CalculateAsymptoticSignificance(h_signal, h_bkg));
      if(isApplySystematics) AsymSig_with_Syst.push_back(this->CalculateAsymptoticSignificance_with_Syst(h_signal, h_bkg));
    }

  string graph_name = var_name + "_" + tag;

  TGraph *graphSig = new TGraph();
  graphSig->SetName(graph_name.c_str());
  
  graph_name = var_name + "_" + tag + "_asym";
  TGraph *graphAsySig = new TGraph();
  graphAsySig->SetName(graph_name.c_str());

  TGraph *graphAsySig_with_Syst;
  if(isApplySystematics) 
    {
      graph_name = var_name + "_" + tag + "_asym_withsys";
      graphAsySig_with_Syst = new TGraph();
      graphAsySig_with_Syst->SetName(graph_name.c_str());
    }

  for(int i=0; i<sels.size(); i++)
    {
      graphSig->SetPoint(i, i + 1., Sig.at(i));
      graphAsySig->SetPoint(i, i + 1., AsymSig.at(i)); 
      if(isApplySystematics) graphAsySig_with_Syst->SetPoint(i, i + 1., AsymSig_with_Syst.at(i));
      
      graphSig->GetXaxis()->SetNdivisions(-510);
      graphAsySig->GetXaxis()->SetNdivisions(-510);
      if(isApplySystematics) graphAsySig_with_Syst->GetXaxis()->SetNdivisions(-510);
    }
  
  for(int i=0; i<sels.size(); i++)
    {
      graphSig->GetXaxis()->SetBinLabel(graphSig->GetXaxis()->FindBin(i+1.), (sels.at(i)).c_str());
      graphAsySig->GetXaxis()->SetBinLabel(graphAsySig->GetXaxis()->FindBin(i+1.), (sels.at(i)).c_str());
      if(isApplySystematics) graphAsySig_with_Syst->GetXaxis()->SetBinLabel(graphAsySig_with_Syst->GetXaxis()->FindBin(i+1.), (sels.at(i)).c_str());
    }
  graphSig->GetXaxis()->LabelsOption("d");
  graphAsySig->GetXaxis()->LabelsOption("d");
  if(isApplySystematics) graphAsySig_with_Syst->GetXaxis()->LabelsOption("d");
    

  this->Add_significance_to_map(var_name, tag, graphSig);
  this->Add_asym_significance_to_map(var_name, tag, graphAsySig);
  if(isApplySystematics) this->Add_asym_significance_with_Syst_to_map(var_name, tag, graphAsySig_with_Syst);

  string file_name = "Info_significancies_" + var_name + "_" + tag + ".txt";
  ofstream fout;
  fout.open(file_name.c_str());
  fout << endl;
  fout << " SIGNIFICANCIES CALCULATED WITH RESPECT TO THE VARIABLE -> " << var_name << endl;
  fout << endl;
  fout << "Info for sample -> " << tag << endl;
  fout << endl;
  for(int i=0; i<sels.size(); i++)
    {
      fout << sels.at(i) << "  ->  Significance: " << Sig.at(i) << "  Asymptotic Significance:  " << AsymSig.at(i); 
      if(isApplySystematics) fout << "  Asymptotic Significance with systematics:  " << AsymSig_with_Syst.at(i);
      fout << endl;
    }
  fout.close();
}

double Statistics::CalculateSignificance(double yields_sig, double yields_Bkg)
{
  if(yields_Bkg == 0)
    {
      ERROR("Bkg yields are equal to 0. Impossible to calculate significance!!");
      exit(EXIT_FAILURE);
    }
  double sig = yields_sig/sqrt(yields_Bkg);
  return sig;
}


double Statistics::CalculateAsymptoticSignificance(TH1D* h_sig, TH1D* h_Bkg)
{
  double sig = 0;
  
  int nbins = h_sig->GetNbinsX();
  if(nbins != h_Bkg->GetNbinsX())
    {
      ERROR("Signal and Bkg have different number of bins!!");
      exit(EXIT_FAILURE);
    }
  
  
  double Si = 0.;
  double Bi = 0.;

  for(int i=1; i<=nbins; i++)
    {
      Si = h_sig -> GetBinContent(i);
      Bi = h_Bkg -> GetBinContent(i);
      
      if (h_Maker_->Get_ScaletoBinWidth())
	{
	  Si = h_sig->GetBinContent(i)*h_sig->GetBinWidth(i);
	  Bi = h_Bkg->GetBinContent(i)*h_Bkg->GetBinWidth(i);
	}

      if(Bi>0.)
	{
	  sig += 2 * ( (Si+Bi)*log(1+(Si/Bi)) - Si);
	}
    }
  
  sig = sqrt(sig);
  return sig;
}


double Statistics::CalculateAsymptoticSignificance_with_Syst(TH1D* h_sig, TH1D* h_Bkg)
{
  double sig = 0;

  int nbins = h_sig->GetNbinsX();
  if(nbins != h_Bkg->GetNbinsX())
    {
      ERROR("Signal and Bkg have different number of bins!!");
      exit(EXIT_FAILURE);
    }

  double Si = 0.;
  double Bi = 0.;
  double sig_nosys = 0.;
  double factor_sys = this->Get_systematics_factor();

  for(int i=1; i<=nbins; i++)
    {
      Si = h_sig -> GetBinContent(i);
      Bi = h_Bkg -> GetBinContent(i);
      
      if (h_Maker_->Get_ScaletoBinWidth())
	{
	  Si = h_sig->GetBinContent(i)*h_sig->GetBinWidth(i);
          Bi = h_Bkg->GetBinContent(i)*h_Bkg->GetBinWidth(i);
        }

      double sys = Bi * factor_sys;
      double ln1 = log((Si+Bi)*(Bi+sys*sys)/(Bi*Bi+(Si+Bi)*sys*sys));
      double ln2 = log(1 + (sys*sys*Si)/(Bi*(Bi+sys*sys)));

      if(Bi>0.) sig_nosys += 2*((Si+Bi)*log(1+(Si/Bi)) - Si);
      
      if( factor_sys!=0.)
	{
	  if(Bi>0.) sig += 2*((Si+Bi)*ln1 - (Bi*Bi/(sys*sys))*ln2);
	}
      else sig = sig_nosys;
    }

  sig = sqrt(sig);

  return sig;
}


void Statistics::XSectionsUpperLimits(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma)
{

  if(sels.size() != var_names.size())
    {
      ERROR("Variables and selections vectors have different size!!");
      exit(EXIT_FAILURE);
    }

  this->DrawXSectionsUpperLimits(sels, var_names, signal_tags, N_sigma);

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();
  
  string file_name = "file_XsecUpperLimits_Z_" + N_sigma_str + "_" + signal_tags.at(0) + ".root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
    
  int v = 0;
  for(auto sel : sels)
    {
      string var = var_names.at(v);
      TGraph *xsec = this->Get_XSection(var);
      xsec->Write();
      
      v++;
    }
  file_out->Close();
}

void Statistics::XSectionsUpperLimits_Combined(vector<vector<string>> sels, vector<vector<string>> var_names, vector<string> signal_tags, double N_sigma)
{

  if(sels.size() != var_names.size())
    {
      ERROR("Variables and selections vectors have different size!!");
      exit(EXIT_FAILURE);
    }

  this->DrawXSectionsUpperLimits_Combined(sels, var_names, signal_tags, N_sigma);

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();
  
  string file_name = "file_XsecUpperLimits_Z_" + N_sigma_str + "_" + signal_tags.at(0) + ".root";
  TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
    
  int v = 0;
  for(auto var : var_names)
    {
      string name_vars = "";
      for(int i=0;i<var.size();i++)  name_vars += (var.at(i) + "_");

      TGraph *xsec = this->Get_XSection(name_vars);
      xsec->Write();
      
      v++;
    }
  file_out->Close();
}
   
  
void Statistics::DrawXSectionsUpperLimits(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma)
{

  cout<<endl;
  cout << " CALCULATING XSECTION UPPER LIMITS at Z: " << N_sigma << endl;
  cout << " ON SIGNALS: ";
  for(auto tag : signal_tags) cout << tag << " ";
  cout<<endl;

  TCanvas *c_xsec = new TCanvas();

  if(this->GO_->Get_logy())  c_xsec->SetLogy();


  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != var_names.size()))
    {
      WARNING("Number of drawing colors are different from number of XSection graphs !!");
      graph_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and (this->GO_->Get_leg_entries().size() != var_names.size()))
    {
      WARNING("Number of legend entries are different from number of XSection graphs!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);

  TString leg_entry;

  int s=0;
  for(auto var : var_names)
    {
      string sel = sels.at(s);
      cout<< endl;

      cout << "  --> ON VARIABLE:  " << var << endl;
      cout << "  --> ON SELECTION:  " << sel << endl;

      this->MakeXSectionCurve(sel, var, signal_tags, N_sigma);
      
      TGraph *xsec = this->Get_XSection(var);
 
      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  xsec->SetLineColor(color);
	  xsec->SetMarkerColor(color);
	}
      
      //xsec->SetMarkerStyle(20+s);
      if(strstr(sel.c_str(), "resolved")!=NULL) xsec->SetMarkerStyle(22); 
      else xsec->SetMarkerStyle(20);

      xsec->SetMarkerSize(1);

      leg_entry = xsec ->GetName();

      if(overwrite_leg)	leg_entry = this->GO_->Get_leg_entries().at(s);
      leg->AddEntry(xsec, leg_entry);
      
      c_xsec->cd();
      if (s==0) xsec->Draw("APL");
      else xsec->Draw("same PL");
      gPad->Update();

      if(this->GO_->Get_overwrite_Y_maximum())  xsec->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum())  xsec->SetMinimum(this->GO_->Get_minimum());
  
      xsec->GetYaxis()->SetTitle("xsec(Y->XH)*BR(XH->qqbb) [pb]");
      xsec->GetYaxis()->SetTitleOffset(1.3);

      s++;
    }

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();

  c_xsec->cd();
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  TLatex l;
  l.SetTextFont(72);
  l.SetTextSize(0.035);
  string sel = "";
  if(strstr(sels.at(0).c_str(),"SR")!=NULL) sel = "SR";
  l.DrawLatexNDC(0.14, 0.75, ("#scale[0.8]{" + sel +"}").c_str());
  c_xsec->SaveAs(("Xsection_for_Z_" + N_sigma_str + "_" + signal_tags.at(0) + ".pdf").c_str());

}

void Statistics::DrawXSectionsUpperLimits_Combined(vector<vector<string>> sels, vector<vector<string>> var_names, vector<string> signal_tags, double N_sigma)
{
  cout<<endl;
  cout << " CALCULATING XSECTION UPPER LIMITS at Z: " << N_sigma << endl;
  cout << " ON SIGNALS: ";
  for(auto tag : signal_tags) cout << tag << " ";
  cout<<endl;

  TCanvas *c_xsec = new TCanvas();

  if(this->GO_->Get_logy())  c_xsec->SetLogy();

  this->GO_->Draw_Options();
  
  Color_t color;
  bool graph_colors = this->GO_->Get_overwrite_Colors();
  bool overwrite_leg =  this->GO_->Get_overwrite_Legend();

  if(this->GO_->Get_overwrite_Colors() and (this->GO_->Get_colors().size() != var_names.size()))
    {
      WARNING("Number of drawing colors are different from number of XSection graphs !!");
      graph_colors = false;
    }

  if(this->GO_->Get_overwrite_Legend() and (this->GO_->Get_leg_entries().size() != var_names.size()))
    {
      WARNING("Number of legend entries are different from number of XSection graphs!!");
      overwrite_leg = false;
    }

  double xmin = this->GO_->Get_leg_xmin();
  double xmax = this->GO_->Get_leg_xmax();
  double ymin = this->GO_->Get_leg_ymin();
  double ymax = this->GO_->Get_leg_ymax();

  TLegend *leg = new TLegend(xmin, ymin, xmax, ymax);

  TString leg_entry;

  int s=0;
  for(auto var : var_names) //cycle on curve to draw
    {
      cout<< endl;
      cout << "  --> Combining SELECTIONs:  "; 
      for(int i=0;i<var.size();i++) cout << sels.at(s).at(i) << " ";
      cout << endl;
      cout << "  --> EACH ON VARIABLE:  ";
      for(int i=0;i<var.size();i++) cout << var.at(i) << " ";
      cout << endl;

      this->MakeXSectionCurve_Combined(sels.at(s), var, signal_tags, N_sigma);
      
      string name_vars = "";
      for(int i=0;i<var.size();i++)  name_vars += (var.at(i) + "_");

      TGraph *xsec = this->Get_XSection(name_vars);
 
      if(graph_colors)
	{
	  color = this->GO_->Get_colors().at(s);
	  xsec->SetLineColor(color);
	  xsec->SetMarkerColor(color);
	}
      xsec->SetMarkerStyle(20+s);
      xsec->SetMarkerSize(1);

      leg_entry = xsec ->GetName();

      if(overwrite_leg)	leg_entry = this->GO_->Get_leg_entries().at(s);
      leg->AddEntry(xsec, leg_entry);
      
      c_xsec->cd();
      if (s==0) xsec->Draw("APL");
      else xsec->Draw("same PL");
      gPad->Update();

      if(this->GO_->Get_overwrite_Y_maximum())  xsec->SetMaximum(this->GO_->Get_maximum());
      if(this->GO_->Get_overwrite_Y_minimum())  xsec->SetMinimum(this->GO_->Get_minimum());
  
      xsec->GetYaxis()->SetLabelSize(0.02);
      xsec->SetMarkerSize(0.5);
      xsec->GetYaxis()->SetTitle("xsec(Y->XH)*BR(XH->qqbb) [pb]");
      xsec->GetYaxis()->SetTitleOffset(1.3);

      s++;
    }

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();

  c_xsec->cd();
  leg->Draw();
  bool atlas_legend = this->GO_->Get_ATLAS_option();
  if(atlas_legend) this->GO_->Draw_ATLAS_legend();
  TLatex l;
  l.SetTextFont(72);
  l.SetTextSize(0.035);
  string sel = "";
  if(strstr(sels.at(0).at(0).c_str(),"SR")!=NULL) sel = "SR";
  l.DrawLatexNDC(0.14, 0.75, ("#scale[0.8]{" + sel +"}").c_str());
  c_xsec->SaveAs(("Xsection_for_Z_" + N_sigma_str + "_" + signal_tags.at(0) + ".pdf").c_str());

}



void Statistics::MakeXSectionCurve(string sel, string var_name, vector<string> signal_tags, double N_sigma)
{
  TFile *file = TFile::Open(("file_"+ var_name + ".root").c_str());

  vector<double> XSec;

 
  TString name_histo_Bkg =  "Bkg_" + sel;
  TH1D *h_bkg = (TH1D*) file->Get(name_histo_Bkg); 
  

  for(auto tag : signal_tags)
    {
      TString name_histo_signal = sel + "_" + tag;
      TH1D *h_signal = (TH1D*) file->Get(name_histo_signal);
      XSec.push_back(this->CalculateXSection(h_bkg, h_signal, N_sigma));
    }

  string graph_name = var_name + "_" + sel;

  TGraph *graphXSec = new TGraph();
  graphXSec->SetName(graph_name.c_str());
  
  for(int i=0; i<signal_tags.size(); i++)
    {
      graphXSec->SetPoint(i, i + 1., XSec.at(i));
      graphXSec->GetXaxis()->SetNdivisions(-510);
    }
  
  for(int i=0; i<signal_tags.size(); i++)   graphXSec->GetXaxis()->SetBinLabel(graphXSec->GetXaxis()->FindBin(i+1.), (signal_tags.at(i)).c_str());

  graphXSec->GetXaxis()->LabelsOption("d");
    
  this->Add_XSection_to_map(var_name, graphXSec);

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();

  string file_name = "Info_XSections_" + var_name + "_" + sel + "_" +  signal_tags.at(0) + ".txt";
  ofstream fout;
  fout.open(file_name.c_str());
  fout << endl;
  
  fout << " XSECTIONS UPPER LIMITS CORRESPONDING TO AN ASYMPTOTIC SIGNIFICANCE -> " << N_sigma_str << " sigma" <<endl;
  fout << endl;
  fout << " CALCULATED ON VARIABLES -> " << var_name << endl;
  
  for(int i=0; i<signal_tags.size(); i++)
    {
      fout << signal_tags.at(i) << "  ->  Xsec: " << XSec.at(i) << endl;
    }
  fout.close();   
}


void Statistics::MakeXSectionCurve_Combined(vector<string> sels, vector<string> var_names, vector<string> signal_tags, double N_sigma)
{
  //this method allow to create a xsec curve combining significance among sels (and corresponding var_names)

  int N_sels = sels.size();
  int N_vars = var_names.size();

  if(N_sels != N_vars)
    {
      ERROR("Numbers of selections and variables are different!!!");
      exit(EXIT_FAILURE);
    } 

  vector<TFile*> files;
  for(int i=0; i<N_vars;i++)
    {
      string name_file = "file_"+ var_names.at(i)+ ".root";
      files.push_back(TFile::Open(name_file.c_str()));
    }
   
   //Now let's create xsection curves

  vector<double> XSec; //Xsec dimensions = signal_tags.size()
  vector<TH1D*> h_Bkgs; //h_Bkgs dimensions = N_vars=N_sels   
  vector<vector<TH1D*>> h_Sigs; //h_Sigs dimensions = N_vars*signal_tags.size()

  for(int n=0; n<N_vars; n++)
    { 
      files.at(n)->cd();
      TString name_histo_Bkg =  "Bkg_" + sels.at(n);
      h_Bkgs.push_back((TH1D*)(files.at(n)->Get(name_histo_Bkg)->Clone()));
      
      vector<TH1D*> histos_sig;
      for(auto tag : signal_tags)
	{
	  TString name_histo_signal = sels.at(n) + "_" + tag;
 	  histos_sig.push_back((TH1D*)(files.at(n)->Get(name_histo_signal)->Clone()));
	}
      h_Sigs.push_back(histos_sig);    
    }

  vector<vector<TH1D*>> h_sigs(signal_tags.size(), vector<TH1D*>(N_vars, 0));
  for(int i=0; i<signal_tags.size(); i++)
    {
      for(int n=0; n<N_vars; n++)
	{
	  h_sigs.at(i).at(n)= h_Sigs.at(n).at(i);
	}
    }

  for(int i=0; i<signal_tags.size(); i++)
    {
      XSec.push_back(this->CalculateXSection_Combined(h_Bkgs, h_sigs.at(i), N_sigma));
    }
  
  string name = "";  //this name is for map
  string graph_name = "Combined_";
  for(int n=0; n<N_vars; n++)
    { 
      name += (var_names.at(n) + "_");
      graph_name += (sels.at(n) + "_");
    }
      
  TGraph *graphXSec = new TGraph();
  graphXSec->SetName(graph_name.c_str());
  
  for(int i=0; i<signal_tags.size(); i++)
    {
      graphXSec->SetPoint(i, i + 1., XSec.at(i));
      graphXSec->GetXaxis()->SetNdivisions(-510);
    }
  
  for(int i=0; i<signal_tags.size(); i++)   graphXSec->GetXaxis()->SetBinLabel(graphXSec->GetXaxis()->FindBin(i+1.), (signal_tags.at(i)).c_str());

  graphXSec->GetXaxis()->LabelsOption("d");
    
  this->Add_XSection_to_map(name, graphXSec);

  std::stringstream Stream;
  Stream << std::fixed << std::setprecision(1) << N_sigma;
  std::string N_sigma_str = Stream.str();

  string file_name = "Info_XSections_combined_" + sels.at(0) + "_" + signal_tags.at(0) + ".txt";
  ofstream fout;
  fout.open(file_name.c_str());
  fout << endl;
  
  fout << " XSECTIONS UPPER LIMITS CORRESPONDING TO A COMBINED ASYMPTOTIC SIGNIFICANCE -> " << N_sigma_str << " sigma" <<endl;
  fout << endl;
  fout << " CALCULATED ON VARIABLES -> " << name << endl;
  
  for(int i=0; i<signal_tags.size(); i++)
    {
      fout << signal_tags.at(i) << "  ->  Xsec: " << XSec.at(i) << endl;
    }
  fout.close();   
}


double Statistics::CalculateXSection(TH1D* h_Bkg, TH1D* h_sig, double N_sigma)
{

  int nbins = h_sig->GetNbinsX();
  if(nbins != h_Bkg->GetNbinsX())
    {
      ERROR("Signal and Bkg have different number of bins!!");
      exit(EXIT_FAILURE);
    }

  double Si = 0.;
  double Bi = 0.;

  double final_Sig = 0.;
  double final_Xsec = 0.;

  double Xsec = 0.00001;
  for(int k = 1; k<=50000; k++)
    {
      double temp_Xsec = Xsec * k;
      double sig = 0;
      for(int i=1; i<=nbins; i++)
	{
	  Si = h_sig -> GetBinContent(i);
	  Bi = h_Bkg -> GetBinContent(i);

	  if (h_Maker_->Get_ScaletoBinWidth())
	    {
	      Si = h_sig->GetBinContent(i)*h_sig->GetBinWidth(i);
	      Bi = h_Bkg->GetBinContent(i)*h_Bkg->GetBinWidth(i);
	    }
	  
	  Si *= temp_Xsec;
	  
	  if(Bi>0.) sig += 2*((Si+Bi)*log(1+(Si/Bi)) - Si);	  
	}
      sig = sqrt(sig);
      final_Sig = sig;
      final_Xsec = temp_Xsec;

      if (final_Sig>=3) break;
    }
  cout <<" for " << h_sig ->GetName() << endl;
  cout << "At Z = " << final_Sig << " XSection = " << final_Xsec << endl;
  return final_Xsec;
}

double Statistics::CalculateXSection_Combined(vector<TH1D*> h_Bkg, vector<TH1D*> h_Sig, double N_sigma)
{
  //this method calculates xsec corresponding to a combined significance among N different selections (N = h_Bkg->size() = h_Sig->size()) equal to N_sigma

  int N_Sig = h_Sig.size();
  int N_Bkg = h_Bkg.size();

  if(N_Sig != N_Bkg)
    {
      ERROR("Numbers of signals and background are different!!");
      exit(EXIT_FAILURE);
    }

  for(int n=0; n<N_Sig; n++)
    {
      string sig_name = h_Sig.at(n)->GetName();
      unsigned last = sig_name.find_last_of("_");                                                                                                                                                   
      std::string sig_name1 = sig_name.substr(last+1);      

      for(int m=n+1; m<N_Sig; m++)
	{
	  sig_name = h_Sig.at(m)->GetName();
	  last = sig_name.find_last_of("_");
	  std::string sig_name2 = sig_name.substr(last+1);

	  if(sig_name1 != sig_name2)
	    {
	      ERROR("Signals are not the same!! I don't want to combine significances of different signals!!");
	      exit(EXIT_FAILURE);
	    }
	}
    }

  vector<int> nbins;
  for(int n=0; n<N_Sig; n++) nbins.push_back(h_Sig.at(n)->GetNbinsX());
  
  vector<int> nbins_Bkg;
  for(int n=0; n<N_Bkg; n++) nbins_Bkg.push_back(h_Bkg.at(n)->GetNbinsX());

  for(int n=0; n<N_Bkg; n++)
    {
      if(nbins.at(n)!= nbins_Bkg.at(n))
	{
	  ERROR("Signal and Bkg number " << n <<" have different number of bins!!");
	  exit(EXIT_FAILURE);
	}
    }

  double Si = 0.;
  double Bi = 0.;

  double final_Sig = 0.;
  double final_Xsec = 0.;

  double Xsec = 0.00001;
  for(int k = 1; k<=50000; k++) // for on Xsections
    {
      double temp_Xsec = Xsec * k;
      double sig_combined = 0;
      vector<double> single_sig;

      for(int n=0; n<N_Sig; n++) //for on signals (-->on selections)
	{
	  double sig =0;
	  for(int i=1; i<=nbins.at(n); i++) //for on bins 
	    {
	      Si = h_Sig.at(n)->GetBinContent(i);
	      Bi = h_Bkg.at(n)->GetBinContent(i);
	      
	      if (h_Maker_->Get_ScaletoBinWidth())
		{
		  Si = h_Sig.at(n)->GetBinContent(i)*h_Sig.at(n)->GetBinWidth(i);
		  Bi = h_Bkg.at(n)->GetBinContent(i)*h_Bkg.at(n)->GetBinWidth(i);
		}
	      
	      Si *= temp_Xsec;
	      
	      if(Bi>0.) sig += 2*((Si+Bi)*log(1+(Si/Bi)) - Si);
	    }
	  
	  single_sig.push_back(sig);
	}
      
      for(auto single_s : single_sig)  sig_combined += (single_s);
      sig_combined = sqrt(sig_combined);

      final_Sig = sig_combined;
      final_Xsec = temp_Xsec;
      
      if (sig_combined>=N_sigma) break;
    }

  cout <<" for " << h_Sig.at(0)->GetName() << endl;
  cout << "At Zcombined = " << final_Sig << " XSection = " << final_Xsec << endl;

  return final_Xsec;
}

boost::optional<pair<TGraph*, TGraph*>> Statistics::ROC_curve(vector<string> sels_num_s, vector<string> sels_num_b, string sel_den_s, string sel_den_b, string var_name, string tag,  string nameaxis_eff_s, string nameaxis_eff_b, bool is_conventionalROC)
{
  boost::optional<pair<TGraph*, TGraph*>> pair;
    
  if(sels_num_s.size() != sels_num_b.size())
    {
      WARNING("The two vectors with selections s and b have not the same size! ROC curve not created!!");
      return pair;
    }

  const int n_points = sels_num_s.size();

  vector<string> sels;
  
  for(int i=0; i<n_points; i++)
    {
      string str_temp = sels_num_s.at(i);
      boost::replace_all(str_temp, "in3sigma_", "");
      sels.push_back(str_temp);
    }

  vector<double>eff_s, eff_b;
  
  TGraphAsymmErrors *graph_eff_s = this->MakeEfficiencyCurve(sels_num_s, sel_den_s, var_name, tag);
  TGraphAsymmErrors *graph_eff_b = this->MakeEfficiencyCurve(sels_num_b, sel_den_b, var_name, tag);

  if(graph_eff_s==NULL)
    {
      WARNING("Efficiency curve wrt " << sel_den_s << " not created. --> ROC not calculated!!");
      return pair;
    }
  if(graph_eff_b==NULL)
    {
      WARNING("Efficiency curve wrt " << sel_den_b << " not created. --> ROC not calculated!!");
      return pair;
    }

  for(int p=0; p<n_points;p++)
    {
      eff_s.push_back(graph_eff_s->GetPointY(p));
      eff_b.push_back(graph_eff_b->GetPointY(p));
    }
  

  TGraph *roc = new TGraph();
  TGraph *estimator = new TGraph(); 
  
  double d2cut=-9.;

  for(int i=0; i<n_points; i++)
    {
      if(!is_conventionalROC) roc->SetPoint(i, (1. - eff_b.at(i)), eff_s.at(i));
      else roc->SetPoint(i, eff_b.at(i), eff_s.at(i));

      
      unsigned last = sels.at(i).find_last_of("_");
      std::string d2_str = sels.at(i).substr(last+1);
      boost::replace_all(d2_str, "p", ".");
      d2cut = std::stod(d2_str);

      estimator->SetPoint(i, d2cut, (eff_s.at(i)*(1. - eff_b.at(i))) ); //estimator 1
      //estimator->SetPoint(i, d2cut, (eff_s.at(i)/sqrt(eff_b.at(i) + eff_s.at(i)))); //estimator 2
      //if(eff_b.at(i)>0) estimator->SetPoint(i, d2cut, (eff_s.at(i)/sqrt(eff_b.at(i))));  //estimator 3
      //else estimator->SetPoint(i, d2cut, 0); //estimator 3
    }
     

  estimator->GetYaxis()->SetTitle("Eff(in3sigma)*(1-Eff(out3sigma))"); //estimator 1
  //estimator->GetYaxis()->SetTitle("Eff(in3sigma)/(sqrt(Eff(in3sigma) + Eff(out3sigma)))"); //estimator 2  
  //estimator->GetYaxis()->SetTitle("Eff(in3sigma)/(sqrt(Eff(out3sigma)))"); //estimator 3  
  
  
  if(nameaxis_eff_b.size()>0) roc->GetXaxis()->SetTitle(nameaxis_eff_b.c_str());
  else
    {
      if(is_conventionalROC) roc->GetXaxis()->SetTitle("Bkg efficiency");
      else roc->GetXaxis()->SetTitle("1 - Bkg efficiency");
    }
     
  if(nameaxis_eff_s.size()>0)  roc->GetYaxis()->SetTitle(nameaxis_eff_s.c_str());
  else roc->GetYaxis()->SetTitle("Signal efficiency");

  estimator->GetXaxis()->SetTitle("D2X<cut");
  roc->SetTitle(("ROC_wrt_" + sel_den_s).c_str());
  estimator->SetTitle(("Estimator_wrt_" + sel_den_s).c_str());

  roc->SetName(("ROC_wrt_" + sel_den_s).c_str());
  estimator->SetName(("Estimator_wrt_" + sel_den_s).c_str());

  pair = make_pair(roc, estimator);
  return pair;
}



void Statistics::Cuts_studies(vector<string>sels_den, string var_name, double WP)
{
  vector<string> sels = h_Maker_->Get_reader()->Get_sel_tags();
  vector<string> chain_tag =  h_Maker_->Get_chain_tags();

  map<string, map<string, double>> map_cuts;

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;

  for(auto sel_den : sels_den)
    {
 
      vector<string> sels_num;

      for(auto sel : sels) 
	{
	  if(strstr(sel.c_str(), (sel_den + "_d2").c_str())!=NULL) sels_num.push_back(sel);
	}
      
      cout<<endl;
      cout << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*" << endl;
      cout << " for selections : ";
      for(auto sel_num : sels_num) cout << sel_num << " ";
      cout<<endl;
      cout << " with respect to selection: " << sel_den << endl;

      TCanvas *c = new TCanvas();
      if(this->GO_->Get_logy())  c->SetLogy();

      this->GO_->Draw_Options();
  
      Color_t color;
      bool graph_colors = this->GO_->Get_overwrite_Colors();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
      bool graph_options = this->GO_->Get_overwrite_Draw_Options();

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
  
      TString map_eff_name = "Eff_" + sel_den + "_" + var_name;
      TString map_eff_title = "Efficiency wrt " + sel_den;
      Int_t nbinsX = sels_num.size();
      Int_t nbinsY = chain_tag.size();

      TH2D *map_eff = new TH2D(map_eff_name, map_eff_title, nbinsX, 0, nbinsX, nbinsY, 0, nbinsY);

      for(int i=0; i<nbinsY; i++)  map_eff->GetYaxis()->SetBinLabel(i+1, (chain_tag.at(i)).c_str());
      for(int i=0; i<nbinsX; i++)  map_eff->GetXaxis()->SetBinLabel(i+1, (sels_num.at(i)).c_str());
      map_eff->GetYaxis()->LabelsOption("u");
      map_eff->GetXaxis()->LabelsOption("d");
      map_eff->GetYaxis()->SetLabelSize(0.02);
      map_eff->GetXaxis()->SetLabelSize(0.02);

      int s=0, d=0;
      for(auto tag : chain_tag)
	{
	  cout<< endl;
	  cout << "  --> On sample:  " << tag << endl;
	  
	  TGraphAsymmErrors *eff = this->MakeEfficiencyCurve(sels_num, sel_den, var_name, tag);

	  if(eff==NULL)
	    {
	      s++;
	      continue;
	    }

	  double eff_WP = -9.;
	  string sel_WP = "";
	  for(int i=0; i<nbinsX; i++)
	    {
	      map_eff -> SetBinContent((i+1), (s+1), eff->GetPointY(i));
	      
	      eff_WP = eff->GetPointY(i);
	      sel_WP = sels_num.at(i);
	      if(eff_WP > WP) break;
	    }
	  
	  unsigned last = sel_WP.find_last_of("_");
	  std::string d2_str = sel_WP.substr(last+1);
	  boost::replace_all(d2_str, "p", ".");
	  double cut2 = std::stod(d2_str); 
	  
	  map_cuts[tag][sel_den]=cut2;
	  
	  TString draw_options;

	  if(graph_options)
	    {
	      draw_options = this->GO_->Get_draw_options().at(s);
	    }
	  else draw_options = "L";

	  if(graph_colors)
	    {
	      color = this->GO_->Get_colors().at(s);
	      eff->SetLineColor(color);
	    }
	  else draw_options+= "PLC";

	  leg_entry = eff ->GetName();
	  if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
	  leg->AddEntry(eff, leg_entry);

	  if(d==0) draw_options+="A";
	  if(d>0) draw_options+= "same";
	  d++;
	  
	  eff->GetYaxis()->SetTitle("Efficiency");
	  eff->GetXaxis()->LabelsOption("d");

	  eff ->Draw(draw_options);
	  
	  eff->GetXaxis()->SetNdivisions(50);

	  eff->SetMaximum(1);
	  eff->SetMinimum(0);
      
	  if(this->GO_->Get_overwrite_Y_maximum()) eff->SetMaximum(this->GO_->Get_maximum());
	  if(this->GO_->Get_overwrite_Y_minimum()) eff->SetMinimum(this->GO_->Get_minimum());

	  s++;
	}

      leg->Draw();

      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();

      c->SaveAs(("Efficiencies_wrt_" + sel_den +".pdf").c_str());

      TCanvas *c2 = new TCanvas();
      map_eff->SetMarkerSize(1);
      map_eff->SetMarkerColor(kWhite);
      gStyle->SetPalette(104);
      map_eff->Draw("COLZ TEXT");
      c2->SaveAs(("Efficiencies_2Dhisto_wrt_" + sel_den + ".pdf").c_str());

      string file_name = "file_Efficiencies_wrt_" + sel_den + ".root";
      TFile *file_out = new TFile(file_name.c_str(), "RECREATE");
  
      for(auto tag : chain_tag)
	{
	  TGraphAsymmErrors *eff = this->Get_efficiency(tag, sel_den);
	  if(eff==NULL) continue;
	  eff->Write();
	}
      file_out->Close();

    }
  
  //TCanvas *can = new TCanvas();                                                                                                                                                                       
  //TLegend *lg = new TLegend();  
  //int d=0;
  string previous_tag = "";

  ostringstream strs;
  strs << WP;
  string WP_str = strs.str();

  string file_name = "file_" + var_name + "_cuts_atWP" + WP_str +".root";
  TFile *file = new TFile(file_name.c_str(), "RECREATE");
	  
  
  for(auto Tag : chain_tag)
    {
      unsigned first = Tag.find("_");
      unsigned end_pos_of_delim = first + 1;
      unsigned last = Tag.find_first_of("_", end_pos_of_delim);
      string big_tag = Tag.substr(end_pos_of_delim, last - end_pos_of_delim);

      if(big_tag == previous_tag) continue;
      previous_tag = big_tag;

      TCanvas *can = new TCanvas();
      TLegend *lg = new TLegend();
      
      int d = 0;
	  
      for(auto tag : chain_tag)
	{
	  can->cd();
	  if(strstr(tag.c_str(), (big_tag).c_str())==NULL) continue;

	  TGraph *gr = new TGraph();
	  gr->SetName(tag.c_str());

	  map<string,double>::iterator m;
    
	  int i = 0;
	  m = map_cuts[tag].begin();
	  while(m != map_cuts[tag].end())
	    {
	      gr->SetPoint(i, i + 1., m->second);
	      m++;
	      i++;
	    }

	  lg->AddEntry(gr, tag.c_str());

	  gr->GetXaxis()->SetAlphanumeric();
	  
	  i=0;
	  m = map_cuts[tag].begin();
	  while(m != map_cuts[tag].end())
	    {
	      gr->GetXaxis()->SetBinLabel(gr->GetXaxis()->FindBin(i+1.), m->first.c_str());
	      m++;
	      i++;
	    }
	  
	  gr->GetXaxis()->SetLabelSize(0.02);
	  gr->GetXaxis()->LabelsOption("d");
	  gr->SetMaximum(5);
	  gr->SetMinimum(0);
	  
	  gr->SetTitle(("WP at " + WP_str).c_str());
	  gr->GetYaxis()->SetTitle(("Cuts on " + var_name).c_str());
	  if(d==0)gr->Draw("A L PLC");
	  if(d!=0)gr->Draw("same L PLC");
	  d++;

	  file->cd();
	  gr->Write();
	  file->Write();
	}


      can->cd();
      lg->Draw();
      can->SaveAs(("Cuts_on_" + var_name + "_mY_" + big_tag + ".pdf").c_str());
    }

  file->cd();
  file->Close();

  //lg->Draw();                                                                                                                                                                                         
  //can->SaveAs(("Cuts_on_" + var_name + ".pdf").c_str());         
 
}

void Statistics::Cuts_studies_withROC(vector<string> sels_den_s, string sel_s_substr, string sel_b_substr, string var_name) 
{
  vector<string> sels = h_Maker_->Get_reader()->Get_sel_tags();
  vector<string> chain_tag =  h_Maker_->Get_chain_tags();

  vector<string> sels_den_b;
  for(auto sel_den : sels_den_s)
    {
      string sel_den_b = sel_den;
      boost::replace_all(sel_den_b, sel_s_substr, sel_b_substr);
      sels_den_b.push_back(sel_den_b);
    }


  map<string, map<string, double>> map_eff_max_s, map_eff_max_b, map_max_est, map_d2cut_max_est;
  
  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;

  for(auto sel_den : sels_den_s)
    {
      string sel_den_b = sel_den;
      boost::replace_all(sel_den_b, sel_s_substr, sel_b_substr);

      vector<string> sels_num_s, sels_num_b;

      for(auto sel : sels) 
	{
	  if(strstr(sel.c_str(), (sel_den + "_d2").c_str())!=NULL) sels_num_s.push_back(sel);
	  if(strstr(sel.c_str(), (sel_den_b + "_d2").c_str())!=NULL) sels_num_b.push_back(sel);
	}
      
      cout<<endl;
      cout << "~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*~*" << endl;
      cout << " for signals selections : ";
      for(auto sel_num : sels_num_s) cout << sel_num << " ";
      cout<<endl;
      cout << " with respect to selection: " << sel_den << endl;
      cout<<endl;
      cout << " and for background selections: " ;
      for(auto sel_num_b: sels_num_b) cout << sel_num_b << " ";
      cout<<endl;
      cout << " with respect to selection: " << sel_den_b << endl;
      cout<<endl;
      cout<<endl;

      
      TCanvas *c_roc = new TCanvas();
      if(this->GO_->Get_logy())  c_roc->SetLogy();

      TCanvas *c_est = new TCanvas();
      c_est->SetLogy();

      this->GO_->Draw_Options();
  
      Color_t color;
      bool graph_colors = this->GO_->Get_overwrite_Colors();
      bool overwrite_leg =  this->GO_->Get_overwrite_Legend();
      bool graph_options = this->GO_->Get_overwrite_Draw_Options();

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
      
      TLegend *leg_roc = new TLegend(xmin, ymin, xmax, ymax);
      TLegend *leg_est = new TLegend(xmin, ymin, xmax, ymax);
      TString leg_entry;
  

      int s=0, d=0;
      for(auto tag : chain_tag)
	{
	  cout<< endl;
	  cout << "  --> On sample:  " << tag << endl;
	  
	  boost::optional<pair<TGraph*, TGraph*>> roc = this->ROC_curve(sels_num_s, sels_num_b, sel_den, sel_den_b, var_name, tag, "in3sigma eff", "1 - out3sigma eff", false); //silvia

	  
	  if(!roc)
	    {
	      s++;
	      continue;
	    }
	  
	  //search the max value of the estimator and the corresponding signal and background efficiencies
	  double max_est=-9.;
	  int max_est_index = -9.;
	  for(int s=0; s<sels_num_s.size(); s++)
	    {
	      double temp_est = roc->second->GetPointY(s);
	      if(temp_est>max_est)
		{
		  max_est=temp_est;
		  max_est_index = s;
		}	      
	    }

	  unsigned last = sels_num_s.at(max_est_index).find_last_of("_");
	  std::string d2_str = sels_num_s.at(max_est_index).substr(last+1);
	  boost::replace_all(d2_str, "p", ".");

	  double d2cut_max_est = std::stod(d2_str);
	  
	  TGraphAsymmErrors *graph_eff_s = this->Get_efficiency(tag, sel_den);
	  TGraphAsymmErrors *graph_eff_b = this->Get_efficiency(tag, sel_den_b);
	  
	  double max_eff_s = graph_eff_s->GetPointY(max_est_index);
	  double max_eff_b = graph_eff_b->GetPointY(max_est_index);

	  max_est = roc->second->GetPointY(max_est_index);
  	  
	  map_eff_max_s[tag][sel_den]=max_eff_s;
	  map_eff_max_b[tag][sel_den_b]=max_eff_b;
	  map_max_est[tag][sel_den]=max_est;
	  map_d2cut_max_est[tag][sel_den]=d2cut_max_est;
	  
	  TString draw_options;

	  if(graph_options)
	    {
	      draw_options = this->GO_->Get_draw_options().at(s);
	    }
	  else draw_options = "L";

	  if(graph_colors)
	    {
	      color = this->GO_->Get_colors().at(s);
	      roc->first->SetLineColor(color);
	      roc->second->SetLineColor(color);
	    }
	  else draw_options+= "PLC";

	  leg_entry = tag.c_str();
	  if(overwrite_leg) leg_entry = this->GO_->Get_leg_entries().at(s);
	  leg_roc->AddEntry(roc->first, leg_entry);
	  leg_est->AddEntry(roc->second, leg_entry);

	  if(d==0) draw_options+="A";
	  if(d>0) draw_options+= "same";
	  c_roc->cd();
	  roc->first->Draw(draw_options);
      
	  if(this->GO_->Get_overwrite_Y_maximum()) roc->first->SetMaximum(this->GO_->Get_maximum());
	  if(this->GO_->Get_overwrite_Y_minimum()) roc->first->SetMinimum(this->GO_->Get_minimum());
	  
	  c_est->cd();
	  roc->second->Draw(draw_options);
	  

	  //roc->second->GetYaxis()->SetLimits(1*pow(10,-5), 1);
	  roc->second->GetXaxis()->SetNdivisions(50);
	  roc->second->GetXaxis()->SetLabelSize(0.02);
	  roc->second->GetXaxis()->LabelsOption("d");

	  if(this->GO_->Get_overwrite_Y_maximum()) roc->second->SetMaximum(this->GO_->Get_maximum());
          if(this->GO_->Get_overwrite_Y_minimum()) roc->second->SetMinimum(this->GO_->Get_minimum());
	  
	  d++;	  
	  s++;	  

	}
      
      c_roc->cd();
      leg_roc->Draw();
      
      bool atlas_legend = this->GO_->Get_ATLAS_option();
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();

      c_roc->SaveAs(("ROC_wrt_" + sel_den +".pdf").c_str());

      c_est->cd();
      leg_est->Draw();
      
      if(atlas_legend) this->GO_->Draw_ATLAS_legend();                                                                                                                                                                                                              
      c_est->SaveAs(("Estimator_wrt_" + sel_den +".pdf").c_str());       
      
    }

  string previous_tag = "";
  
  string file_name = "file_" + var_name + "_maxestimator.root";
  TFile *file = new TFile(file_name.c_str(), "RECREATE");
	  

  TCanvas *can_max_est = new TCanvas();
  TLegend *lg_max_est = new TLegend();
  TMultiGraph *multigr_max_est = new TMultiGraph();

  TCanvas *can_in3sigma = new TCanvas();
  TLegend *lg_in3sigma = new TLegend();
  TMultiGraph *multigr_in3sigma = new TMultiGraph();
      
  TCanvas *can_out3sigma = new TCanvas();
  TLegend *lg_out3sigma = new TLegend();  
  TMultiGraph *multigr_out3sigma = new TMultiGraph();

  TCanvas *can_d2cut_max_est = new TCanvas();
  TLegend *lg_d2cut_max_est= new TLegend();
  TMultiGraph *multigr_d2cut_max_est = new TMultiGraph();
  

  for(auto Tag : chain_tag)
    {
      unsigned first = Tag.find("_");
      unsigned end_pos_of_delim = first + 1;
      unsigned last = Tag.find_first_of("_", end_pos_of_delim);
      string big_tag = Tag.substr(end_pos_of_delim, last - end_pos_of_delim);

      if(big_tag == previous_tag) continue;
      previous_tag = big_tag;

      
      for(auto tag : chain_tag)
	{
	  if(strstr(tag.c_str(), (big_tag).c_str())==NULL) continue;

	  TGraph *gr_in3sigma = new TGraph();
	  gr_in3sigma->SetName((tag + "_eff_in3sigma").c_str());

	  map<string,double>::iterator m;
    
	  int n_point = 0;
          m = map_eff_max_s[tag].begin();
          while(m != map_eff_max_s[tag].end())
            {
              for(int i=0; i<sels_den_s.size(); i++)
                {
                  if(strcmp(m->first.c_str(),sels_den_s.at(i).c_str())==0)
                    {
                      gr_in3sigma->SetPoint(n_point, i + 1., m->second);
                      n_point++;
                    }
                }
              m++;

            }

          lg_in3sigma->AddEntry(gr_in3sigma, tag.c_str());

          gr_in3sigma->GetXaxis()->SetAlphanumeric();

          m = map_eff_max_s[tag].begin();
          while(m != map_eff_max_s[tag].end())
            {
              for(int i=0; i<sels_den_s.size(); i++)
                {
                  if(strcmp(m->first.c_str(), sels_den_s.at(i).c_str())==0)
                    {
                      gr_in3sigma->GetXaxis()->SetBinLabel(gr_in3sigma->GetXaxis()->FindBin(i+1.), m->first.c_str());
                    }
                }
              m++;
            }

	  can_in3sigma->cd();

	  gr_in3sigma->GetXaxis()->SetLabelSize(0.02);
	  gr_in3sigma->GetXaxis()->LabelsOption("d");

 	  multigr_in3sigma->Add(gr_in3sigma);
	  
      
	  TGraph *gr_out3sigma = new TGraph();
	  gr_out3sigma->SetName((tag + "_eff_out3sigma").c_str());

    
	  n_point = 0;
          m = map_eff_max_b[tag].begin();
          while(m != map_eff_max_b[tag].end())
            {
              for(int i=0; i<sels_den_b.size(); i++)
                {
                  if(strcmp(m->first.c_str(),sels_den_b.at(i).c_str())==0)
                    {
                      gr_out3sigma->SetPoint(n_point, i + 1., m->second);
                      n_point++;
                    }
                }
              m++;

            }

          lg_out3sigma->AddEntry(gr_out3sigma, tag.c_str());

          gr_out3sigma->GetXaxis()->SetAlphanumeric();

          m = map_eff_max_b[tag].begin();
          while(m != map_eff_max_b[tag].end())
            {
              for(int i=0; i<sels_den_b.size(); i++)
                {
                  if(strcmp(m->first.c_str(), sels_den_b.at(i).c_str())==0)
                    {
                      gr_out3sigma->GetXaxis()->SetBinLabel(gr_out3sigma->GetXaxis()->FindBin(i+1.), m->first.c_str());
                    }
                }
              m++;
            }

	  can_out3sigma->cd();
	  
	  gr_out3sigma->GetXaxis()->SetLabelSize(0.02);
	  gr_out3sigma->GetXaxis()->LabelsOption("d");

	  multigr_out3sigma->Add(gr_out3sigma);


	  TGraph *gr_max_est = new TGraph();
	  gr_max_est->SetName((tag + "_eff_max_est").c_str());

	  n_point = 0;
          m = map_max_est[tag].begin();
          while(m != map_max_est[tag].end())
            {
              for(int i=0; i<sels_den_s.size(); i++)
                {
                  if(strcmp(m->first.c_str(),sels_den_s.at(i).c_str())==0)
                    {
                      gr_max_est->SetPoint(n_point, i + 1., m->second);
                      n_point++;
                    }
                }
              m++;
            }

          lg_max_est->AddEntry(gr_max_est, tag.c_str());
          gr_max_est->GetXaxis()->SetAlphanumeric();

          m = map_max_est[tag].begin();
          while(m != map_max_est[tag].end())
            {
              for(int i=0; i<sels_den_s.size(); i++)
                {
                  if(strcmp(m->first.c_str(), sels_den_s.at(i).c_str())==0)
                    {
                      gr_max_est->GetXaxis()->SetBinLabel(gr_max_est->GetXaxis()->FindBin(i+1.), m->first.c_str());
                    }
                }
              m++;
            }

	  can_max_est->cd();
	  
	  gr_max_est->GetXaxis()->SetLabelSize(0.02);
	  gr_max_est->GetXaxis()->LabelsOption("d");

	  multigr_max_est->Add(gr_max_est);

	  TGraph *gr_d2cut_max_est = new TGraph();
	  gr_d2cut_max_est->SetName((tag + "_eff_d2cut_max_est").c_str());
    
	  n_point = 0;
	  m = map_d2cut_max_est[tag].begin();
	  while(m != map_d2cut_max_est[tag].end())
	    {
	      for(int i=0; i<sels_den_s.size(); i++)
		{
		  if(strcmp(m->first.c_str(),sels_den_s.at(i).c_str())==0)
		    {
		      gr_d2cut_max_est->SetPoint(n_point, i + 1., m->second);
		      n_point++;
		    }
		}
	      m++;
		     
	    }

	  lg_d2cut_max_est->AddEntry(gr_d2cut_max_est, tag.c_str());

	  gr_d2cut_max_est->GetXaxis()->SetAlphanumeric();
	  
	  m = map_d2cut_max_est[tag].begin();
	  while(m != map_d2cut_max_est[tag].end())
	    {
	      for(int i=0; i<sels_den_s.size(); i++)
		{
		  if(strcmp(m->first.c_str(), sels_den_s.at(i).c_str())==0)
		    {
		      gr_d2cut_max_est->GetXaxis()->SetBinLabel(gr_d2cut_max_est->GetXaxis()->FindBin(i+1.), m->first.c_str());
		    }
		}
	      m++;
	    }

	  can_d2cut_max_est->cd();
	  
	  gr_d2cut_max_est->GetXaxis()->SetLabelSize(0.02);
	  gr_d2cut_max_est->GetXaxis()->LabelsOption("d");

	  multigr_d2cut_max_est->Add(gr_d2cut_max_est);


	  file->cd();
	  gr_in3sigma->Write();
	  gr_out3sigma->Write();
	  gr_max_est->Write();
	  gr_d2cut_max_est->Write();
	  file->Write();
	}
    }
  
  can_in3sigma->cd();
  multigr_in3sigma->Draw("A L PLC");
  multigr_in3sigma->SetTitle("WP on in3#sigma events, at max value of Estimator");                                                                                                                       
  multigr_in3sigma->GetYaxis()->SetTitle("#epsilon(in 3#sigma)");     
  multigr_in3sigma->GetXaxis()->SetAlphanumeric();

  for(int i=0;i<sels_den_s.size();i++)multigr_in3sigma->GetXaxis()->SetBinLabel(multigr_in3sigma->GetXaxis()->FindBin(i+1.), sels_den_s.at(i).c_str()); //now

  multigr_in3sigma->GetXaxis()->SetLabelSize(0.02);
  multigr_in3sigma->GetXaxis()->LabelsOption("d");
  lg_in3sigma->Draw();                                                                                                                                                                                    
  can_in3sigma->SaveAs(("WP_maxEstimator_for_in3sigma_" + var_name + ".pdf").c_str());      

  can_out3sigma->cd ();
  multigr_out3sigma->Draw("A L PLC");
  multigr_out3sigma->SetTitle("WP on out3#sigma events, at max value of Estimator");
  multigr_out3sigma->GetYaxis()->SetTitle("#epsilon(out 3#sigma)");
  multigr_out3sigma->GetXaxis()->SetAlphanumeric();

  for(int i=0;i<sels_den_s.size();i++)multigr_out3sigma->GetXaxis()->SetBinLabel(multigr_out3sigma->GetXaxis()->FindBin(i+1.), sels_den_s.at(i).c_str()); //now                                            

  multigr_out3sigma->GetXaxis()->SetLabelSize(0.02);
  multigr_out3sigma->GetXaxis()->LabelsOption("d");

  lg_out3sigma->Draw();                                                                                                                                                                                    
  can_out3sigma->SaveAs(("WP_maxEstimator_for_out3sigma_" + var_name + ".pdf").c_str());
  

  can_max_est->cd();
  multigr_max_est->Draw("A L PLC");
  multigr_max_est->SetTitle("Max Estimator values");
  multigr_max_est->GetYaxis()->SetTitle("Max Estimator");
  multigr_max_est->GetXaxis()->SetAlphanumeric();

  for(int i=0;i<sels_den_s.size();i++)multigr_max_est->GetXaxis()->SetBinLabel(multigr_max_est->GetXaxis()->FindBin(i+1.), sels_den_s.at(i).c_str()); //now                                            

  multigr_max_est->GetXaxis()->SetLabelSize(0.02);
  multigr_max_est->GetXaxis()->LabelsOption("d");
  lg_max_est->Draw();
  can_max_est->SaveAs(("maxEstimator_" + var_name + ".pdf").c_str());
  

  can_d2cut_max_est->cd();
  multigr_d2cut_max_est->Draw("A L PLC");
  multigr_d2cut_max_est->SetTitle("D2X cuts at max Estimator values");
  multigr_d2cut_max_est->GetYaxis()->SetTitle("D2 cuts");
  multigr_d2cut_max_est->GetXaxis()->SetAlphanumeric();

  for(int i=0;i<sels_den_s.size();i++)multigr_d2cut_max_est->GetXaxis()->SetBinLabel(multigr_d2cut_max_est->GetXaxis()->FindBin(i+1.), sels_den_s.at(i).c_str()); //now                                                 
  multigr_d2cut_max_est->GetXaxis()->SetLabelSize(0.02);
  multigr_d2cut_max_est->GetXaxis()->LabelsOption("d");

  lg_d2cut_max_est->Draw();
  can_d2cut_max_est->SaveAs(("d2cut_maxEstimator_" + var_name + ".pdf").c_str());
    
}


void Statistics::DrawEfficiencies_SignalMap(string sel_num, string sel_den, string var_name)
{
  vector<string> chain_tag =  h_Maker_->Get_chain_tags();
  gStyle->SetPalette(104);
  gStyle->SetOptStat(0);

  cout<<endl;
  cout << " CALCULATING EFFICIENCY ON VARIABLE: " << var_name << endl;
  
  TFile *file = TFile::Open(("file_"+var_name+".root").c_str());

  cout << " for selection : " << sel_num << endl;
  cout<<endl;
  cout << " with respect to selection: " << sel_den<< endl;

  vector<string> tag_mY, tag_ratio;
  for(auto Tag : chain_tag)
    {    
      unsigned first = Tag.find("_");
      unsigned end_pos_of_delim = first + 1;
      unsigned last = Tag.find_first_of("_", end_pos_of_delim);
      string big_tag = Tag.substr(end_pos_of_delim, last - end_pos_of_delim);      	
      
      if((tag_mY.size()==0) or ((tag_mY.size()>0) and (std::find(tag_mY.begin(), tag_mY.end(), big_tag) == tag_mY.end())))
	{
	  tag_mY.push_back(big_tag);
	}
      
      unsigned end = Tag.find_first_of("_", last+1);
      string tag_mX = Tag.substr(last+1 );
      tag_mX.erase(remove(tag_mX.begin(), tag_mX.end(), '_'), tag_mX.end());

      double my = std::stod(big_tag);
      double mx = std::stod(tag_mX);
      double ratio = (double)(mx/my);
      
      std::stringstream Stream;
      Stream << std::fixed << std::setprecision(2) << ratio;
      string str_ratio = Stream.str();

      if((tag_ratio.size()==0) or ((tag_ratio.size()>0) and (std::find(tag_ratio.begin(), tag_ratio.end(), str_ratio) == tag_ratio.end())))
      {
	tag_ratio.push_back(str_ratio);      
      }
    }

  multiset<std::string> g(tag_ratio.begin(), tag_ratio.end()); // """sort"""
  std::vector<std::string> tag_ratio_sorted(g.begin(), g.end());  

  multiset<std::string> d(tag_mY.begin(), tag_mY.end()); // """sort"""                                                                                                                             
  std::vector<std::string> tag_mY_sorted(d.begin(), d.end());


  TCanvas *c = new TCanvas();
  if(this->GO_->Get_logy())  c->SetLogy();
  if(this->GO_->Get_logx())  c->SetLogx();

  TString map_eff_name = "Eff_" + sel_den + "_" + var_name;
  TString map_eff_title = "Efficiency of " + sel_num + " wrt " + sel_den;
  
  Int_t nbinsX = tag_ratio_sorted.size();
  Int_t nbinsY = tag_mY_sorted.size();

  TH2D *map_eff = new TH2D(map_eff_name, map_eff_title, nbinsX, 0, nbinsX, nbinsY, 0, nbinsY);

  for(int i=0; i<nbinsY; i++)  map_eff->GetYaxis()->SetBinLabel(i+1, (tag_mY_sorted.at(i)).c_str());
  for(int i=0; i<nbinsX; i++)
    {
      if((i%2)==0) map_eff->GetXaxis()->SetBinLabel(i+1, (tag_ratio_sorted.at(i)).c_str());
      else map_eff->GetXaxis()->SetBinLabel(i+1, "");
    }
  map_eff->GetYaxis()->LabelsOption("u");
  map_eff->GetXaxis()->LabelsOption("v");
  map_eff->GetYaxis()->SetTitle("mY");
  map_eff->GetXaxis()->SetTitle("mX/mY");
  map_eff->GetYaxis()->SetLabelSize(0.06);
  map_eff->GetXaxis()->SetLabelSize(0.04);

  for(auto tag : chain_tag)
    {
      cout<< endl;
      cout << "  --> On sample:  " << tag << endl;

      unsigned first = tag.find("_");
      unsigned end_pos_of_delim = first + 1;
      unsigned last = tag.find_first_of("_", end_pos_of_delim);
      string tagY = tag.substr(end_pos_of_delim, last - end_pos_of_delim);

      string tagX = tag.substr(last+1);

      unsigned end = tag.find_last_of("_");
      tagX.erase(remove(tagX.begin(), tagX.end(), '_'), tagX.end());

      double my = std::stod(tagY);
      double mx = std::stod(tagX);
      double ratio = (double)(mx/my);

      std::stringstream Stream;
      Stream << std::fixed << std::setprecision(2) << ratio;
      string ratio_mxmy = Stream.str();

      TString name_histo_den = sel_den + "_" + tag;
      TH1D *h_den = (TH1D*) file->Get(name_histo_den);

      double yields_den = h_den->Integral(1, h_den->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_den = h_den->Integral(1, h_den->GetNbinsX()+1, "width");

      if(yields_den<=0)
	{
	  WARNING("For " + tag + " " + sel_den + " yields are not >0 .. Efficiency not calculated!!");
	  continue;
	}

      TString name_histo_num = sel_num + "_" + tag;
      TH1D *h_num = (TH1D*) file->Get(name_histo_num);

      double yields_num = h_num->Integral(1, h_num->GetNbinsX()+1);
      if (h_Maker_->Get_ScaletoBinWidth()) yields_num = h_num->Integral(1, h_num->GetNbinsX()+1, "width");

      double eff = (double)(yields_num/yields_den);
      
      int index_Y = 0;
      int index_X = 0;

      for(int iY=0; iY<nbinsY; iY++)
	{
	  if(strcmp(tagY.c_str(), (tag_mY_sorted.at(iY)).c_str())==0) index_Y = iY;
	}
      for(int iX=0; iX<nbinsX; iX++)
        {
          if(strcmp(ratio_mxmy.c_str(), (tag_ratio_sorted.at(iX)).c_str())==0) index_X = iX;
        }

      for(int x=0; x<nbinsX; x++)
	{
	  for(int y=0; y<nbinsY; y++) 
	    {
	      if((x==index_X) and (y==index_Y))
		{
		  map_eff -> SetBinContent((x+1), (y+1), eff);
		}
	    }
	}

    }

  gStyle->SetPaintTextFormat("1.3f");
  map_eff->SetMarkerSize(0.8);
  map_eff->Draw("TEXT45 Col");

  c->SaveAs(("Eff_SignalMap_" + sel_num + "_wrt_" + sel_den + ".pdf").c_str());

}
