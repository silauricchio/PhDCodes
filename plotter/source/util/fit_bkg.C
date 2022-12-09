#include "TColor.h"
#include "TStyle.h"

void fit_bkg_old(string path, string root_file_name, string histo_name1, string histo_name2)
{

  string file_path = path + "/" + root_file_name;
  TFile *file = new TFile(file_path.c_str(), "READ");
  TH1D *h1 = (TH1D*)file->Get(histo_name1.c_str());
  TH1D *h2 = (TH1D*)file->Get(histo_name2.c_str());

  TH1D *sum = (TH1D*)h2->Clone();
  sum -> Add(h1);

  
  sum->SetMinimum(0.05);
  sum->SetMaximum(0.07);
  sum->GetYaxis()->SetTitle("Tagged/Rew Untagged");
  sum->GetYaxis()->SetNdivisions(3);
  sum->GetYaxis()->SetTitleSize(0.04);
  sum->GetYaxis()->SetTitleOffset(0.9);
  sum->GetYaxis()->SetLabelSize(0.03);

  sum->GetXaxis()->SetLabelSize(0.03);
  sum->GetXaxis()->SetTitle("M_H [GeV]");
  sum->GetXaxis()->SetTitleSize(0.04);
  sum->GetXaxis()->SetTitleOffset(0.9);


  TCanvas *c = new TCanvas();
  sum->Draw("E");
  sum->SetFillStyle(1001);
  sum->SetFillColorAlpha(kBlue,0.1);  


  TF1 *my_pol0 = new TF1("my_pol0", "pol0");
  TF1 *my_pol1 = new TF1("my_pol1", "pol1");
  TF1 *my_pol2 = new TF1("my_pol2", "pol2");
  TF1 *my_pol3 = new TF1("my_pol3", "pol3");
  TF1 *my_pol4 = new TF1("my_pol4", "pol4");
  
  
  my_pol0->SetLineColor(kGreen);  
  //sum->Fit("my_pol0", "");

  my_pol1->SetLineColor(kBlack);
  sum->Fit("my_pol1", "");

  my_pol2->SetLineColor(kRed);  
  //sum->Fit("my_pol2", "+");
  my_pol3->SetLineColor(kViolet);
  //sum->Fit("my_pol3", "+");

  //my_pol4->SetLineColor(kBlue);  
  //sum->Fit("my_pol4", "+");

  //my_pol1->SetRange(75,200);
  //my_pol2->SetRange(50, 75);
  //TF1 *my_func = new TF1("my_func", "my_pol2+my_pol1", 50, 200);

  //my_func->SetLineColor(kBlack);
  //sum->Fit("my_func", "R+");

  double bin1 = sum->GetBinCenter(1);
  double SF_LSB_noval = my_pol1->Eval(bin1);
  cout<< "bin 1 center = " << bin1 << "function = " << SF_LSB_noval<<endl;
  
  double bin2 = sum->GetBinCenter(2);
  double SF_LSB_val = my_pol1->Eval(bin2);
  cout<< "bin 2 center = " << bin2 << "function = " << SF_LSB_val << endl;

  double bin4 = sum->GetBinCenter(4);
  double SF_HSB_val = my_pol1->Eval(bin4);
  cout<< "bin 4 center = " << bin4 << "function = " << SF_HSB_val<<endl;

  double bin5 = sum->GetBinCenter(5);
  double SF_HSB_noval = my_pol1->Eval(bin5);
  cout << "bin 5 center = " << bin5 << "function = " << SF_HSB_noval<<endl;

  cout << "bin 1 entries " << sum->GetBinContent(1) << " +- " << sum->GetBinError(1) << endl;
  cout << "bin 2 entries " << sum->GetBinContent(2) << " +- " << sum->GetBinError(2) <<endl;
  cout << "bin 4 entries " << sum->GetBinContent(4) << " +- " << sum->GetBinError(3) <<endl;
  cout << "bin 5 entries " << sum->GetBinContent(5) << " +- " << sum->GetBinError(5) <<endl;
}


void fit_bkg(string path, string root_file_name, string histo_name)
{

  string file_path = path + "/" + root_file_name;
  TFile *file = new TFile(file_path.c_str(), "READ");
  TH1D *h1 = (TH1D*)file->Get(histo_name.c_str());

  
  h1->SetMinimum(0.05);
  h1->SetMaximum(0.07);
  h1->GetYaxis()->SetTitle("Tagged/Rew Untagged");
  h1->GetYaxis()->SetNdivisions(3);
  h1->GetYaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleOffset(0.9);
  h1->GetYaxis()->SetLabelSize(0.03);

  h1->GetXaxis()->SetLabelSize(0.03);
  h1->GetXaxis()->SetTitle("M_H [GeV]");
  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetXaxis()->SetTitleOffset(0.9);


  TCanvas *c = new TCanvas();
  h1->Draw("E");
  h1->SetFillStyle(1001);
  h1->SetFillColorAlpha(kBlue,0.1);  


  TF1 *my_pol0 = new TF1("my_pol0", "pol0");
  TF1 *my_pol1 = new TF1("my_pol1", "pol1");
  TF1 *my_pol2 = new TF1("my_pol2", "pol2");
  TF1 *my_pol3 = new TF1("my_pol3", "pol3");
  TF1 *my_pol4 = new TF1("my_pol4", "pol4");
  
  
  my_pol0->SetLineColor(kGreen);  
  //h1->Fit("my_pol0", "");

  my_pol1->SetLineColor(kBlack);
  h1->Fit("my_pol1", "");

  my_pol2->SetLineColor(kRed);  
  //h1->Fit("my_pol2", "+");
  my_pol3->SetLineColor(kViolet);
  //h1->Fit("my_pol3", "+");

  //my_pol4->SetLineColor(kBlue);  
  //h1->Fit("my_pol4", "+");

  //my_pol1->SetRange(75,200);
  //my_pol2->SetRange(50, 75);
  //TF1 *my_func = new TF1("my_func", "my_pol2+my_pol1", 50, 200);

  //my_func->SetLineColor(kBlack);
  //h1->Fit("my_func", "R+");

  double bin1 = h1->GetBinCenter(1);
  double SF_LSB_noval = my_pol1->Eval(bin1);
  cout<< "bin 1 center = " << bin1 << "function = " << SF_LSB_noval<<endl;
  
  double bin2 = h1->GetBinCenter(2);
  double SF_LSB_val = my_pol1->Eval(bin2);
  cout<< "bin 2 center = " << bin2 << "function = " << SF_LSB_val << endl;

  double bin4 = h1->GetBinCenter(4);
  double SF_HSB_val = my_pol1->Eval(bin4);
  cout<< "bin 4 center = " << bin4 << "function = " << SF_HSB_val<<endl;

  double bin5 = h1->GetBinCenter(5);
  double SF_HSB_noval = my_pol1->Eval(bin5);
  cout << "bin 5 center = " << bin5 << "function = " << SF_HSB_noval<<endl;

  cout << "bin 1 entries " << h1->GetBinContent(1) << " +- " << h1->GetBinError(1) << endl;
  cout << "bin 2 entries " << h1->GetBinContent(2) << " +- " << h1->GetBinError(2) <<endl;
  cout << "bin 4 entries " << h1->GetBinContent(4) << " +- " << h1->GetBinError(4) <<endl;
  cout << "bin 5 entries " << h1->GetBinContent(5) << " +- " << h1->GetBinError(5) <<endl;
}
