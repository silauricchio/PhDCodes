#include <boost/algorithm/string/replace.hpp>

void fit_Gauss(TString file_name)
{
  TFile *file = TFile::Open(file_name);

  std::map<string, std::pair<double, double>> map_fit;

  TObject *obj;
  TKey *key;
  TIter next( file->GetListOfKeys());
  while ((key = (TKey *) next())) 
    {
      obj = file->Get(key->GetName()); 
      //printf(" found object:%s\n",key->GetName());

      if(!obj->InheritsFrom(TH1::Class())) continue;

      if(strstr(((std::string)key->GetName()).c_str(), "Signal")==NULL)continue;

      TH1D *histo = (TH1D*) obj->Clone();

      std::string sample_name = histo->GetName();

      std::size_t pos = sample_name.find("Signal");   

      std::string name_str = sample_name.substr(pos); 

      cout << "Sample: " << sample_name << endl;
 
      unsigned first = name_str.find_first_of("_");
      unsigned second =  name_str.find_first_of("_", first+1);

      std::string massX_str = name_str.substr(second+1);
      
      size_t found = massX_str.find("_");
      if(found != string::npos) boost::replace_all(massX_str, "_", "");

      double mass = std::stod(massX_str); 
      
      cout << "mass " << mass << endl;
      
      double sigma = sqrt(mass);
      double min = mass - 6*sigma;
      double max = mass + 10*sigma;

      //TF1 *Gaus = new TF1("Gaus", "gaus", min, max);
      
      
      TF1 *CB = new TF1("CB", "[4]*ROOT::Math::crystalball_function(x, [0], [1], [2], [3])",min, max);

      CB->SetParameter(0, 1);
      CB->SetParameter(1, 1);
      CB->SetParameter(2, sigma);
      CB->SetParameter(3, mass);
      CB->SetParameter(4, 1);
      
      //CB->SetParLimits(3,0,4000); 
      //CB->SetParLimits(2,0,1000);

     

      TCanvas *c = new TCanvas();

      //c->SetLogy();
      
      histo->SetMaximum(0.5);
      //histo->SetMinimum(1*pow(10, -4));

      histo->SetLineColor(kBlack);

      histo->Draw("hist");
      histo->Fit("CB");
     
      double fit_mass = CB->GetParameter(3);
      double fit_sigma = CB->GetParameter(2);
     
      //cout << "fit_mass --> " << fit_mass << endl;
      //cout << "fit_sigma --> " << fit_sigma << endl;
      //cout <<endl;
      //cout <<endl;

      map_fit[name_str] = std::make_pair(fit_mass, fit_sigma);

      TLatex text;
      text.SetTextSize(0.03);
      text.DrawLatexNDC(0.55,0.7,Form("fitted mass = %lf",fit_mass));
      text.DrawLatexNDC(0.55,0.75,Form("fitted #sigma = %lf",fit_sigma));
     
      
      CB ->SetLineColor(kRed);
      CB ->Draw("same");

      std::string pdf_name = sample_name + "_fitted.pdf";
      
      c->SaveAs(pdf_name.c_str());

      c->Destructor();
    }
  file->Close();

  std::map<string, pair<double, double>>::iterator m;

  m = map_fit.begin();

  cout<< endl;
  cout<<endl;
  cout << "~~~~~~~~~~~ fit results ~~~~~~~~~~~" << endl;
  cout<<endl;

  while(m != map_fit.end())
    {
      cout << m->first << " mass fit : " << m->second.first << " sigma fit: " << m->second.second << endl; 
      m++;
    }        
}

