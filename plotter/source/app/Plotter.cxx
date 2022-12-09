#include <vector>
#include <fstream>
#include <sstream>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <atomic>
#include "TStopwatch.h"

#include "../Plotter/ReadFiles.h"
#include "../Plotter/HistoPlotter.h"
#include "../Plotter/GraphicsOptions.h"
#include "../Plotter/Trigger.h"
#include "../Plotter/Statistics.h"


using namespace std;

void printUsage (std::ostream & os)
{
  os << "Usage: " << std::endl;
  os << "Plotter  [ options ]" << std::endl;
  os << "   Options: " << std::endl;
  os << "    " << std::setw(24) << "-h | --help : print this text" << std::endl;
  os << "    " << std::setw(24) << "-c | --config <path> : override the config file" << std::endl;
  os << "    " << std::setw(24) << "-d | --debug : Activate debug printout"  << std::endl;
}


int main(int argc, char* argv[])
{
  TStopwatch p;
  p.Start();
  
  bool override_config = false;
  bool debug_mode = false;
  
  //std::string config_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/app/config_Plotter.cfg";
  std::string config_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/app/config_Plotter_FULLSTAT_v1p9.cfg";
  
  // construct options                                                                                                                                                                                     
  static struct option long_options[] =
    {
      {"help",       no_argument,       0, 'h'},
      {"config",     required_argument, 0, 'c'},
      {"debug", no_argument, 0, 'd'},
      {0, 0, 0}
    };

  int option_index =0;
  int c = 0;

  // get arguments from command line                                                                                                                                                                       
  while ((c = getopt_long(argc, argv, "hdc:m:n:o:s:w:y:", long_options, &option_index ) ) != -1 )
    {
      switch(c)
	{
	case 0:
	  if (long_options[option_index].flag != 0)
	    break;
	case 'h':
	  printUsage(std::cout);
	  return 0;
	case 'c':
	  override_config = true;
	  config_path = std::string (optarg);
	  break;
	case 'd':
	  debug_mode = true;
	  break;
	}
    }

  ReadFiles *reader = new ReadFiles();

  reader->ReadConfig(debug_mode, config_path);
  
  if(debug_mode) 
    {
      vector<string> tag_files = reader->Get_file_tags();
      
      for(auto file : tag_files)
	{
	  cout << "File: " << file << " --> " <<reader->Get_file(file)<<endl;
	}
      
      vector<string> tag_sel = reader->Get_sel_tags();

      for(string sel : tag_sel)
	{
	  cout<<"Selection: "<< sel <<" string: " << reader->Get_sel_string(sel) << endl;
	}
      
      vector<string> vars = reader->Get_vars_name();
      for(string var : vars)
	{	
	  bool variable_binning = reader->Get_varbins(var);
	  if(!variable_binning)
	    {
	      cout<<"Variable: "<< var << " name Xaxis: " << reader->Get_vars_Xtitle(var) << " VarBins: " << reader->Get_varbins(var)<< " NBins: " << reader->Get_vars_Nbins(var) << " range: [" << reader->Get_vars_range(var).first << ", " << reader->Get_vars_range(var).second << "]" << endl;
	    }
	  else
	    {
	      cout<<"Variable: "<< var << " name Xaxis: " << reader->Get_vars_Xtitle(var) <<" VarBins: " << reader->Get_varbins(var) << " NBins: " << (reader->Get_binning(var)).size() - 1 << " variable binning = ";
	      for(int g=0; g<reader->Get_binning(var).size(); g++) cout << (reader->Get_binning(var)).at(g) << " ";
	      cout << endl;
	    }
	}
      
    }

  Graphics_options *go = new Graphics_options();
  HistoMaker *h_maker = new HistoMaker(debug_mode, reader, go);
  /*  
  h_maker->MakeChain("Signal_1500");
  h_maker->MakeChain("Signal_1800");
  h_maker->MakeChain("Signal_2000");
  h_maker->MakeChain("Signal_2300");
  h_maker->MakeChain("Signal_2500");
  h_maker->MakeChain("Signal_2600");
  h_maker->MakeChain("Signal_3000");
  h_maker->MakeChain("Signal_3400");
  h_maker->MakeChain("Signal_3600");
  h_maker->MakeChain("Signal_3800");
  h_maker->MakeChain("Signal_4000");
  h_maker->MakeChain("Signal_5000");
  h_maker->MakeChain("Signal_6000");
  */

  h_maker->MakeChain("Signal_1000_65");
  h_maker->MakeChain("Signal_1000_110_");
  h_maker->MakeChain("Signal_1000_130");
  h_maker->MakeChain("Signal_1000_160_");
  h_maker->MakeChain("Signal_1000_200_");
  h_maker->MakeChain("Signal_1000_500");
  h_maker->MakeChain("Signal_1200_65");
  h_maker->MakeChain("Signal_1200_110_");
  h_maker->MakeChain("Signal_1200_130");
  h_maker->MakeChain("Signal_1200_160_");
  h_maker->MakeChain("Signal_1200_200_");
  h_maker->MakeChain("Signal_1200_250_");
  h_maker->MakeChain("Signal_1500_65");
  h_maker->MakeChain("Signal_1500_80_");
  h_maker->MakeChain("Signal_1500_110_");
  h_maker->MakeChain("Signal_1500_130");
  h_maker->MakeChain("Signal_1500_160_");
  h_maker->MakeChain("Signal_1500_200_");
  h_maker->MakeChain("Signal_1500_250_");
  h_maker->MakeChain("Signal_1500_300");
  h_maker->MakeChain("Signal_1500_400");
  h_maker->MakeChain("Signal_1500_500");
  h_maker->MakeChain("Signal_1500_600");
  h_maker->MakeChain("Signal_1500_700");
  h_maker->MakeChain("Signal_1800_65");
  h_maker->MakeChain("Signal_1800_110_");
  h_maker->MakeChain("Signal_1800_130");
  h_maker->MakeChain("Signal_1800_160_");
  h_maker->MakeChain("Signal_1800_200_");
  h_maker->MakeChain("Signal_1800_250_");
  h_maker->MakeChain("Signal_1800_300");
  h_maker->MakeChain("Signal_1800_400");
  h_maker->MakeChain("Signal_1800_500");
  h_maker->MakeChain("Signal_1800_600");
  h_maker->MakeChain("Signal_1800_700");
  h_maker->MakeChain("Signal_1800_800");
  h_maker->MakeChain("Signal_2000_65");
  h_maker->MakeChain("Signal_2000_110_");
  h_maker->MakeChain("Signal_2000_130");
  h_maker->MakeChain("Signal_2000_160_");
  h_maker->MakeChain("Signal_2000_200_");
  h_maker->MakeChain("Signal_2000_250_");
  h_maker->MakeChain("Signal_2000_300");
  h_maker->MakeChain("Signal_2000_400");
  h_maker->MakeChain("Signal_2000_500");
  h_maker->MakeChain("Signal_2000_600");
  h_maker->MakeChain("Signal_2000_700");
  h_maker->MakeChain("Signal_2000_800");
  h_maker->MakeChain("Signal_2000_1000");
  h_maker->MakeChain("Signal_2000_1100");
  h_maker->MakeChain("Signal_2300_65");
  h_maker->MakeChain("Signal_2300_110_");
  h_maker->MakeChain("Signal_2300_130");
  h_maker->MakeChain("Signal_2300_160_");
  h_maker->MakeChain("Signal_2300_200_");
  h_maker->MakeChain("Signal_2300_250_");
  h_maker->MakeChain("Signal_2300_300");
  h_maker->MakeChain("Signal_2300_400");
  h_maker->MakeChain("Signal_2300_500");
  h_maker->MakeChain("Signal_2300_600");
  h_maker->MakeChain("Signal_2300_700");
  h_maker->MakeChain("Signal_2300_800");
  h_maker->MakeChain("Signal_2300_900");
  h_maker->MakeChain("Signal_2300_1000");
  h_maker->MakeChain("Signal_2300_1100");
  h_maker->MakeChain("Signal_2300_1200");
  h_maker->MakeChain("Signal_2500_900");
  h_maker->MakeChain("Signal_2500_1200");
  h_maker->MakeChain("Signal_2500_1600");
  h_maker->MakeChain("Signal_2500_2000");
  h_maker->MakeChain("Signal_2600_65");
  h_maker->MakeChain("Signal_2600_110_");
  h_maker->MakeChain("Signal_2600_130");
  h_maker->MakeChain("Signal_2600_160_");
  h_maker->MakeChain("Signal_2600_200_");
  h_maker->MakeChain("Signal_2600_250_");
  h_maker->MakeChain("Signal_2600_300");
  h_maker->MakeChain("Signal_2600_400");
  h_maker->MakeChain("Signal_2600_500");
  h_maker->MakeChain("Signal_2600_600");
  h_maker->MakeChain("Signal_2600_700");
  h_maker->MakeChain("Signal_2600_800");
  h_maker->MakeChain("Signal_2600_900");
  h_maker->MakeChain("Signal_2600_1000");
  h_maker->MakeChain("Signal_2600_1100"); 
  h_maker->MakeChain("Signal_2600_1200");
  h_maker->MakeChain("Signal_2600_1400");
  h_maker->MakeChain("Signal_3000_65");
  h_maker->MakeChain("Signal_3000_80_");
  h_maker->MakeChain("Signal_3000_110_");
  h_maker->MakeChain("Signal_3000_130");
  h_maker->MakeChain("Signal_3000_160_");
  h_maker->MakeChain("Signal_3000_200_");
  h_maker->MakeChain("Signal_3000_250_");
  h_maker->MakeChain("Signal_3000_300");
  h_maker->MakeChain("Signal_3000_400");
  h_maker->MakeChain("Signal_3000_500");
  h_maker->MakeChain("Signal_3000_600");
  h_maker->MakeChain("Signal_3000_700");
  h_maker->MakeChain("Signal_3000_800");
  h_maker->MakeChain("Signal_3000_900");
  h_maker->MakeChain("Signal_3000_1100");
  h_maker->MakeChain("Signal_3000_1200");
  h_maker->MakeChain("Signal_3000_1400");
  h_maker->MakeChain("Signal_3000_1600");
  h_maker->MakeChain("Signal_3400_65");
  h_maker->MakeChain("Signal_3400_110_");
  h_maker->MakeChain("Signal_3400_130");
  h_maker->MakeChain("Signal_3400_160_");
  h_maker->MakeChain("Signal_3400_200_");
  h_maker->MakeChain("Signal_3400_250_");
  h_maker->MakeChain("Signal_3400_300");
  h_maker->MakeChain("Signal_3400_400");
  h_maker->MakeChain("Signal_3400_500");
  h_maker->MakeChain("Signal_3400_600");
  h_maker->MakeChain("Signal_3400_700");
  h_maker->MakeChain("Signal_3400_800");
  h_maker->MakeChain("Signal_3400_900");
  h_maker->MakeChain("Signal_3400_1000");
  h_maker->MakeChain("Signal_3400_1200");
  h_maker->MakeChain("Signal_3400_1400");
  h_maker->MakeChain("Signal_3400_1600");
  h_maker->MakeChain("Signal_3600_900");
  h_maker->MakeChain("Signal_3600_1000");
  h_maker->MakeChain("Signal_3600_1100");
  h_maker->MakeChain("Signal_3600_1200");
  h_maker->MakeChain("Signal_3600_1400");
  h_maker->MakeChain("Signal_3600_1600");
  h_maker->MakeChain("Signal_3600_1800");
  h_maker->MakeChain("Signal_3800_65");
  h_maker->MakeChain("Signal_3800_110_");
  h_maker->MakeChain("Signal_3800_130");
  h_maker->MakeChain("Signal_3800_160_");
  h_maker->MakeChain("Signal_3800_200_");
  h_maker->MakeChain("Signal_3800_250_");
  h_maker->MakeChain("Signal_3800_300");
  h_maker->MakeChain("Signal_3800_400");
  h_maker->MakeChain("Signal_3800_500");
  h_maker->MakeChain("Signal_3800_600");
  h_maker->MakeChain("Signal_3800_700");
  h_maker->MakeChain("Signal_3800_800");
  h_maker->MakeChain("Signal_3800_900");
  h_maker->MakeChain("Signal_3800_1000");
  h_maker->MakeChain("Signal_3800_1100");
  h_maker->MakeChain("Signal_3800_1200");
  h_maker->MakeChain("Signal_3800_1400");
  h_maker->MakeChain("Signal_3800_1600");
  h_maker->MakeChain("Signal_3800_1800");
  h_maker->MakeChain("Signal_3800_2000");
  h_maker->MakeChain("Signal_4000_65");
  h_maker->MakeChain("Signal_4000_110_");
  h_maker->MakeChain("Signal_4000_130");
  h_maker->MakeChain("Signal_4000_160_");
  h_maker->MakeChain("Signal_4000_200_");
  h_maker->MakeChain("Signal_4000_250_");
  h_maker->MakeChain("Signal_4000_300");
  h_maker->MakeChain("Signal_4000_400");
  h_maker->MakeChain("Signal_4000_500");
  h_maker->MakeChain("Signal_4000_600");
  h_maker->MakeChain("Signal_4000_700");
  h_maker->MakeChain("Signal_4000_800");
  h_maker->MakeChain("Signal_4000_900");
  h_maker->MakeChain("Signal_4000_1000");
  h_maker->MakeChain("Signal_4000_1100");
  h_maker->MakeChain("Signal_4000_1200");
  h_maker->MakeChain("Signal_4000_1400");
  h_maker->MakeChain("Signal_4000_1600");
  h_maker->MakeChain("Signal_4000_1800");
  h_maker->MakeChain("Signal_4000_2000");
  h_maker->MakeChain("Signal_4000_2200");
  h_maker->MakeChain("Signal_5000_65");
  h_maker->MakeChain("Signal_5000_200_");
  h_maker->MakeChain("Signal_5000_400");
  h_maker->MakeChain("Signal_5000_600");
  h_maker->MakeChain("Signal_5000_800");
  h_maker->MakeChain("Signal_5000_900");
  h_maker->MakeChain("Signal_5000_1000");
  h_maker->MakeChain("Signal_5000_1200");
  h_maker->MakeChain("Signal_5000_1400");
  h_maker->MakeChain("Signal_5000_1600");
  h_maker->MakeChain("Signal_5000_1800");
  h_maker->MakeChain("Signal_5000_2000");
  h_maker->MakeChain("Signal_5000_2500");
  h_maker->MakeChain("Signal_6000_400");
  h_maker->MakeChain("Signal_6000_500");
  h_maker->MakeChain("Signal_6000_1000");
  h_maker->MakeChain("Signal_6000_1200");
  h_maker->MakeChain("Signal_6000_1400");
  h_maker->MakeChain("Signal_6000_1600");
  h_maker->MakeChain("Signal_6000_1800");
  h_maker->MakeChain("Signal_6000_2000");
  h_maker->MakeChain("Signal_6000_2500");
  h_maker->MakeChain("Signal_6000_3000");

  h_maker->MakeChain("Bkg_tt");
  h_maker->MakeChain("Bkg_Vjets");
  h_maker->MakeChain("Bkg_QCD_dijet");
  h_maker->MakeChain("Data");

  //IT IS IMPORTANT TO KNOW THAT CHAINS AND LEG ENTRIES ARE IN THE OPPOSITE ORDER!!

  h_maker->Set_NormalizeHistos(1.);
  //h_maker->Set_NormalizeHisto("mVHres SR_resolved_WP60_d2_1p0 Signal_2500_1600", (0.00318*175.91391));
  //h_maker->Set_NormalizeHisto("mVres SR_resolved_WP60_d2_1p0 Signal_2500_1600", (0.00318*175.91391));
  //h_maker->Set_NormalizeHisto("mVH SR_boosted_WP60_nod2cut Signal_2500_1600", (0.00318*158.74191));
  //h_maker->Set_NormalizeHisto("mV SR_boosted_WP60_nod2cut Signal_2500_1600", (0.00318*158.74191));
  //h_maker->Set_NormalizeHisto("mVH SR_boosted_WP60_d2_1p0 Signal_2500_1600", (0.00318*175.91391));
  //h_maker->Set_NormalizeHisto("mV SR_boosted_WP60_d2_1p0 Signal_2500_1600", (0.00318*175.91391));
  //h_maker->Set_NormalizeStack("mVres SR_resolved_WP60", 1.);


  //***** STACK PLOTS *******//
  
  vector<TString> leg_entries = {"mY2500_mX1600"};
  vector<Color_t> colors = {kBlack};
  vector<TString> options = {"hist"};

  vector<TString> leg_entries_stack = {"Diboson", "tt", "QCD dijets"};
  vector<Color_t> colors_stack = {kOrange, kGreen+1, kCyan-1};

  //go->Set_legend_position(0.65, 0.7, 0.89, 0.89);

  //go->Set_maximum(1*pow(10,5));//logY                                                                                                                                                                    
  //go->Set_minimum(1*pow(10,-4)); //logY           

  go->Set_logY(true);
  go->Set_lumi(139);
  go->Set_write_lumi(true);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  //go->Set_colors(colors);
  //go->Set_draw_options(options);
  go->Overwrite_Selection(false, "");

  go->Set_draw_stat_uncertainties(true);

  go->Set_leg_entries_stack(leg_entries_stack);
  go->Set_legend_stack(false);
  go->Set_colors_stack(colors_stack);

  h_maker->Set_ScaletoBinWidth(false, "GeV"); // when the bin is filled with event/bin_size it happens that RAW events have no sense!
  
  //h_maker->DrawStacks();

  //********* DATA vs BKG (with DNN) **********//
  leg_entries = {""};
  colors = {kBlack};
  options = {"hist"};
  //go->Set_legend_position(0.65, 0.7, 0.89, 0.89);

  //go->Set_maximum(1*pow(10,6));//logY                                                                                                                                                                    
  //go->Set_minimum(1*pow(10,-4)); //logY                                                                                                                                                                   

  go->Set_logY(true);

  //go->Set_lumi(59.1); //mc16e
  //go->Set_lumi(43.8); //mc16d   
  //go->Set_lumi(36.1); //mc16a
  go->Set_lumi(139); // fullstat
  //go->Set_lumi(80); //16a+16d

  go->Set_write_lumi(true);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);
  //go->Set_draw_options(options);
  go->Overwrite_Selection(false, "");

  h_maker->Set_ScaletoBinWidth(false, "GeV");   

  bool Draw_observed_sig = true;
  //h_maker->DrawDataDNNBkgPlots(Draw_observed_sig);
  
  
  //***** HISTOS *******//   
  leg_entries = {"Data 2015-18"};

  colors = {kBlack};
  options = {"hist"};

  //go->Set_lumi(59.1); //mc16e
  //go->Set_lumi(43.8); //mc16d   
  //go->Set_lumi(36.1); //mc16a
  go->Set_lumi(139); // fullstat
  //go->Set_write_lumi(false);

  //go->Set_maximum(200); //logY 
  //go->Set_maximum(10);//logY                                                                                                                                                                    
  //go->Set_minimum(1*pow(10,-4)); //logY   

  go->Set_logY(true);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  //go->Set_legend_position(0.6, 0.58, 0.88, 0.88);
  go->Set_colors(colors);
  go->Set_draw_options(options);
  //go->Set_X_axis_name("#Delta #eta");
  h_maker->Set_ScaletoBinWidth(false, "GeV");
  go->Overwrite_Selection(true, "LSB0 VR");

  //h_maker->DrawHistos();

  vector<string> histos_num = {"HSB1_boosted_WP60_mH165_200_Data"};
  vector<string> histos_den = {"HSB0_boosted_WP60_mH165_200_Data"};

  vector<Color_t> colors_ratios = {kBlue}; 
  vector<TString> options_ratios = {"E"};

  //go->Set_maximum_ratio(5);
  //go->Set_minimum_ratio(0);

  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);
  go->Set_ratio_name("tagged/untagged");

  //h_maker->DrawHistos(histos_num, histos_den);
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~//

  vector<string> sels_to_compare = {
    "tagged_LSB_val", "untagged_LSB_val", "untagged_LSB_val_NominalRew"
    //"tagged_HSB_train", "untagged_HSB_train", "untagged_HSB_train_NominalRew"  
    //"new_tagged_HSB_train", "tagged_HSB_train"
    //"new_untagged_HSB_train", "untagged_HSB_train" 
  };      

  vector<string> vars_to_compare = {
    //"d2TracksV", 
    "XCand_AnomScore"
    //"NNScore_nominal"
    //"trackjet1_pt", "trackjet2_pt", "trackjet1_eta", "trackjet2_eta", "trackjet1_m", "trackjet2_m", 
    //"deta_trackjets", "deltaR_trackjets", "trackjets_pt_balance",
    //"pTH", "etaH", "EH", "mVH",
    //"mV"
    //, "pTV","etaV", "EV", "phiV", 
    //"deta_fatjets", "deltaR_fatjets",
    //"ntrkVVJJ_H",
    
    //"mVres", "mVHres",
    //"ntrkVVJJ_H",     
    //"SmallJet1_X_pT", "SmallJet1_X_eta", "SmallJet1_X_E", "SmallJet1_X_M", 
    //"SmallJet2_X_pT", "SmallJet2_X_eta", "SmallJet2_X_E", "SmallJet2_X_M",
    //"deta_smalljets", "deltaR_smalljets", 
    //"deltaY_smalljets", "smalljets_ptbalance", 				    
    };
  
  vector<string> tags_to_compare = {"Data", 
				    //"Signal_3000_300", 
				    //"Signal_4000_600"
  };
 
  colors = {kBlack, 
	    kOrange,
	    kBlue
  };

  options = {"P", "hist", "E"};
  //options = {"hist", "hist"};
  leg_entries = {"LSB1 (65<mH<75 GeV)", "LSB0 (65<mH<75 GeV)", "reweighted LSB0 (65<mH<75 GeV)"};
  //leg_entries = {"HSB1 (145<mH<175 GeV)", "HSB0 (145<mH<175 GeV)", "reweighted HSB0 (145<mH<175 GeV)"};    
  //go->Overwrite_Selection(true, "Data 2015-18, Merged selection");

  go->Set_maximum(1*pow(10, 2));//logY
  //go->Set_minimum(1*pow(10,-6)); //logY

  //go->Set_maximum(0.4);//linearY
  //go->Set_minimum(0); //linearY
  
  go->Set_lumi(139); 
  //go->Set_lumi(36.1);
  //go->Set_lumi(59.1);  

  go->Set_ATLAS_draw(true);
  go->Set_logY(true);
  go->Set_colors(colors);
  go->Set_draw_options(options);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  go->Set_legend_position(0.53, 0.75, 0.79, 0.93); 
  //go->Set_X_axis_name("M [GeV]"); 

  h_maker->Set_ScaletoBinWidth(false, "");
 
  //h_maker->CompareVariables(sels_to_compare, vars_to_compare, tags_to_compare); //this method compares variables for a given selection
  //h_maker->CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare); ///this method compares selections for a given variable 

  vector<string> sels_num = {
    "tagged_LSB_val", "tagged_LSB_val",
    //"tagged_HSB_train", "tagged_HSB_train", 
   //"new_tagged_HSB_train"
   //"new_untagged_HSB_train" 
  };
  vector<string> sels_den = {
    "untagged_LSB_val","untagged_LSB_val_NominalRew" 
    //"untagged_HSB_train","untagged_HSB_train_NominalRew"  
    //"tagged_HSB_train"
    //"untagged_HSB_train"
  };

  go->Set_ratio_name("X_{bb}-tag/X_{bb}-untag");
  //go->Set_ratio_name("2.44/2.46");
  
  go->Set_maximum_ratio(2);                                                                                                                               
  go->Set_minimum_ratio(0); 
  
  colors_ratios = {//kBlack, 
    kOrange, kBlue
  };
  options_ratios = {"E2", "E2"};

  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);
  go->Set_draw_ratio_uncertainties(true);  //both num and den with errors

  h_maker->CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare, sels_num, sels_den); ///this method compares selections for a given variable with ratio
  

  //********** COMPARE HISTOS FROM FILES **************//

  string root1 = "ratio_tagged_HSB_val_untagged_HSB_val_Data";                                                                                                            
  string file1 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/last/";    
  
  string root2 = "ratio_tagged_HSB_train_untagged_HSB_train_Data";
  string file2 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/last/";
  
  string root3 = "ratio_tagged_LSB_val_untagged_LSB_val_Data";
  string file3 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/last/";

  string root4 = "ratio_tagged_LSB_val2_untagged_LSB_val2_Data";
  string file4 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/last/";

  //string root1 = "ratio_HSB1_WP60_mH145_175_resolved_rew_HSB0_WP60_mH145_175_resolved_Data";                                                                                                            
  //string file1 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/rew_different_SBs/comparisons/";    

  //string root2 = "ratio_HSB1_WP60_mH175_200_resolved_rew_HSB0_WP60_mH175_200_resolved_Data";
  //string file2 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/rew_different_SBs/comparisons/";
  
  //string root3 = "ratio_LSB1_WP60_mH65_75_resolved_rew_LSB0_WP60_mH65_75_resolved_Data";
  //string file3 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/rew_different_SBs/comparisons/";

  //string root4 = "ratio_LSB1_WP60_mH50_65_resolved_rew_LSB0_WP60_mH50_65_resolved_Data";
  //string file4 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/rew_different_SBs/comparisons/";

  

  leg_entries = {"HSB, 175<mH<200", "HSB, 145<mH<175", "LSB, 65<mH<75", "LSB, 50<mH<65"};
  //leg_entries = {"HSB, 175<mH<200"};
  //leg_entries = {"LSB, 65<mH<75"};
  //leg_entries = {"LSB, 50<mH<65"};

  go->Overwrite_Selection(true, "Data");  
  //go->Overwrite_Selection(true, "merged");  
  //go->Overwrite_Selection(true, "resolved");
  //go->Set_X_axis_name("M[Gev]"); 
  //go->Set_Y_axis_name("tagged/rew_untagged");  

 
  vector<string> histo_names, file_names;
  histo_names.push_back(root1);
  histo_names.push_back(root2);
  histo_names.push_back(root3);
  histo_names.push_back(root4);
  
  file_names.push_back(file1);
  file_names.push_back(file2);
  file_names.push_back(file3);
  file_names.push_back(file4);
  
  vars_to_compare = {"trackjet1_pt", "trackjet2_pt", "trackjet1_eta", "trackjet2_eta", "trackjet1_m", "trackjet2_m",  "pTH", "etaH", "EH", "phiH", "mV", "pTV","etaV", "EV", "phiV",  "mVH", "ntrkVVJJ_H",		     "mVres", "mVHres",
		     "SmallJet1_X_pT", "SmallJet1_X_eta", "SmallJet1_X_E", "SmallJet1_X_M",
		     "SmallJet2_X_pT", "SmallJet2_X_eta", "SmallJet2_X_E", "SmallJet2_X_M",
};
  //vars_to_compare = {"mVHres"};

  go->Set_maximum(2);//linearY                                                                                                                                                                          
  go->Set_minimum(0); //linearY       

  colors = {kYellow+1, kGreen, kBlue, kRed};
  //colors = {kAzure};
  //colors = {kOrange};
  options = {"E2", "E2", "E2", "E2"};

  go->Set_colors(colors);
  go->Set_draw_options(options);
  go->Set_logY(false);  
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  //go->Set_legend_position(0.5, 0.6, 0.8, 0.88);                                                                                                                      
  //go->Set_maximum(2);
  //go->Set_minimum(0);

  //h_maker->CompareHistos_fromFile(file_names, histo_names, vars_to_compare); //this method needs alredy existent histos for each var to compare

  go->Set_maximum_ratio(1.5);
  go->Set_minimum_ratio(0.5);
  double value = 1.;
  //h_maker->CompareRatios_fromFile(file_names, histo_names, value, vars_to_compare); // bottom plots with bin-by-bin pulls with respect to a given value "value"
    

  vector<string> histo_num = {"ratio_tagged_HSB_val_untagged_HSB_val_Data", "ratio_tagged_HSB_val_untagged_HSB_val_Data", "ratio_tagged_HSB_val_untagged_HSB_val_Data"};
  vector<string> histo_den = {"ratio_tagged_HSB_train_untagged_HSB_train_Data", "ratio_tagged_LSB_val_untagged_LSB_val_Data", "ratio_tagged_LSB_val2_untagged_LSB_val2_Data"};
  
  go->Set_ratio_name("175-200/others");
  go->Set_maximum_ratio(1.5);
  go->Set_minimum_ratio(0.5);
  colors_ratios = {kGreen, kBlue, kRed};
  options_ratios = {"E","E","E"};
  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);

  //h_maker->CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare); 
  
  //with significance bin by bin
  //vector<string> histo_bkg = {"CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data"};
  //vector<string> histo_signals = {"CR2_boosted_Signal_1500_300", "CR2_boosted_Signal_2000_300", "CR2_boosted_Signal_2600_300", "CR2_boosted_Signal_3000_300", "CR2_boosted_Signal_3400_300", "CR2_boosted_Signal_4000_300"};
  
  //go->Set_minimum_sig(0);
  //go->Set_maximum_sig(4);
  //vector<Color_t> colors_sig = {kViolet, kBlue, kRed, kRed-2, kGreen, kYellow+1};
  //vector<TString> options_sig = {"hist", "hist", "hist", "hist", "hist", "hist"};
  //go->Set_colors_sig(colors_sig);
  //go->Set_draw_options_sig(options_sig);
  
  //h_maker->CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare, histo_signals, histo_bkg);
  
  //********** NON-CLOSURE plots *********//  
  
  // This method produces the non closure plots with stat and sistematic error, in a given selection (on the corresponding chosen variable) 

  vector<string> sels_name = {"LSB1_full_merged", "LSB1_full_resolved"};
  vector<string> vars_name = {"mVH", "mVHres"};  // vars_name and sels_name must be of the same dimension

  string nominal_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/NonClosure/Nominal_histos/LSB_full/";
  string noboot_nominal_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/NonClosure/Nominal_histos_noboot/LSB_full/";
  string var_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/NonClosure/Var_histos/LSB_full/";
  
  string chain = "Data";


  go->Set_lumi(139);
  go->Set_write_lumi(true);

  go->Overwrite_Selection(false, "");

  go->Set_legend_position(0.63, 0.6, 0.95, 0.95); //legend in the upper pad                                                                                                                                                        
  go->Set_maximum(10); //limits in the upper pad                                                                                                                                                         
  go->Set_minimum(0.00001); 

  go->Set_logY(true); //for the upper pad y axisx
  
  go->Set_maximum_ratio(1.4);  //limits in the ratio pad
  go->Set_minimum_ratio(0.6);
  
  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!                                                                             
  h_maker->Set_ScaletoBinWidth(false, "GeV");

  //h_maker->CreateNonClosurePlots(sels_name, vars_name, nominal_path, var_path, noboot_nominal_path, chain);


  //********** 2D PLOTS *********//
 
  vector<string> var_name1 = {"SmallJet1_X_m_pT", "SmallJet2_X_m_pT", "SmallJet1_X_m_pT", "SmallJet2_X_m_pT", "SmallJet1_X_M", "SmallJet2_X_M"};
  vector<string> var_name2 = {"SmallJet1_X_pT", "SmallJet2_X_pT", "SmallJet1_X_M", "SmallJet2_X_M", "SmallJet1_X_pT", "SmallJet2_X_pT"};

  vector<string> sel_names = {"Resolved_presel_final"};  
  vector<string> chain_tags = {"Signal_2000_200_", "Signal_2500_1600", "Signal_2600_600", "Signal_3000_1400"};

  go->Set_logY(false);
  go->Set_logX(true);
  h_maker->Set_ScaletoBinWidth(false);
  go->Set_Palette(kCividis);
  go->Set_2D_draw_option("colz");  
  //h_maker->Draw2DHistos(var_name1, var_name2, sel_names, chain_tags, false); // the last bool is for tprofile

  vector<string> sel_names_num = {"CR2_boosted_WP60"};
  vector<string> sel_names_den = {"SB2_boosted_WP60"};
  
  go->Set_minimum_2D(0.);
  //go->Set_maximum_2D(3.);
  //go->Set_2D_draw_option("text colz");
  //h_maker->DrawRatio2D(sel_names_num, sel_names_den, var_name1, var_name2, chain_tags); // this method requires alredy existent histogram to divide
  
  //h_maker->DrawPulls_2D(var_name1, var_name2, sel_names_num, sel_names_den,  chain_tags, 1);
  
  double significance = 3;

  //h_maker->CalculateMaxSignificance(var_name_cut, var_name_bins, signal_tags, bkg_tags, significance);


  
  //!!!!FOR TRIGGER, SIGNIFICANCE AND EFFICIENCY STUDIES, ROOT FILES WITH HISTOS MUST ALREDY EXIST!!!!!
  
  //************* TRIGGER EFFICIENCIES ************//
  
  TriggerEfficiency *tr_eff = new TriggerEfficiency(h_maker, debug_mode, go);
  //le efficienze vengono calcolate a fissata variabile, tutte le selezioni al num e per ciascuna selezione tutti i tags 
  vector<string> chain_tags_for_trigger = {"Signal_2600_160_","Signal_3800_200_"};
  vector<string> var_names_for_trigger = {"pTH", "phiH", "etaH", "deltaR_fatjets"};
  vector<string> sel_names_num_for_trigger = {"SR_boosted_WP60"};
  vector<string> sel_names_den_for_trigger = {"HiggsSB"};
  vector<Color_t> colors_for_eff = {kRed, kBlue};
  leg_entries = {""};

  //go->Set_maximum(1);
  //go->Set_minimum(0.0);
  go->Set_colors(colors_for_eff);
  go->Set_leg_entries(leg_entries);
  //go->Set_legend_position();
  go->Set_legend(false);

  //tr_eff->Draw_Efficiency_Curves(sel_names_num_for_trigger , sel_names_den_for_trigger , var_names_for_trigger , chain_tags_for_trigger);

  chain_tags_for_trigger = {"Data"};
  vector<string> var_names1 = {"trackjet1_pt", "trackjet2_pt"};
  vector<string> var_names2 = {"trackjet1_eta", "trackjet2_eta"}; 
  //tr_eff->Draw_Efficiency_Maps(sel_names_num_for_trigger , sel_names_den_for_trigger, var_names1, var_names2, chain_tags_for_trigger);
  

  
  //************* EFFICIENCIES ************// 
  
  //////////////
  //-Statistics::DrawEfficiencies(selections, selection_den, variable, samples) : with this method you create efficiency curve, calculated on the "variable" chosen, for each sample in "samples". The efficiencies are calculated as a function of "selections", with respect to "selection_den". All the samples curves are superimposed on the same canvas. This method creates a txt file with efficiencies information.  A root files with all the efficiency curves is also created.

  //-Statistics::DrawSignificancies(selections, variables_for_sig, samples) : with this method you create significancies curves, varying selections, for a given variable, for each signal in "samples" vector. Two canvas are created: one with S/Radq(B) significance and another with asymptotic significance.  If the flag for the significance with systematics is set to "true", also another canvas with asymptotic significance with systematics is created.   This method creates a txt file with significancies information.  A root files with all the significance curves is also created.

  //FOR TRIGGER, SIGNIFICANCE AND EFFICIENCY STUDIES, ROOT FILES WITH HISTOS MUST ALREDY EXIST!!!!! --> TriggerEfficiency and Statistics class take histos from root files. IF FILES OR HISTOS DO NOT EXIST, THIS WILL CAUSE A SEGMENTATION VIOLATION!!

  //////////////////

  
  Statistics *stat = new Statistics(h_maker, debug_mode, go);
  
  vector<string> selections = {"preSel", "LSB", "HiggsMassWindow", "HSB"};
  string selection_den = "preSel";
  string variable = "nLargeRJets";
  vector<string> samples = {"Data", "Bkg_QCD_dijet", "Bkg_tt", "Bkg_Vjets"};

  leg_entries = {"Signal_1500_65", "Signal_2000_65", "Signal_2600_65", "Signal_3400_65", "Signal_4000_65", "Signal_5000_65"};
  colors = {kBlue-1, kViolet};
  //options = {};

  go->Set_logY(false);
  //go->Set_maximum(1.5);
  //go->Set_minimum(0);

  go->Overwrite_Selection(false);
  go->Set_leg_entries(leg_entries);
  go->Set_legend_position(0.7, 0.3, 0.89, 0.6);  
  go->Set_legend(false);
  go->Set_colors(false);
  go->Set_draw_options(false);
  h_maker->Set_ScaletoBinWidth(false, "GeV");

  //stat->DrawEfficiencies(selections, selection_den, variable, samples);
  //stat->DrawEfficiencies_fromConfig(selection_den, variable);

  string selection_num = "HSB0_boosted_WP60_less2trkjets";
  selection_den = "HSB0_boosted_WP60";
  variable = "mH";
  //stat->DrawEfficiencies_SignalMap(selection_num, selection_den, variable); //this method calculate a grid of efficiency (selection num/selection_den) with mY vs mX/mY (truth mass)

  //vector<string> selections_den = {"in3sigma_mXpTX_0p05", "in3sigma_mXpTX_0p05_0p10", "in3sigma_mXpTX_0p10_0p15", "in3sigma_mXpTX_0p15_0p20", "in3sigma_mXpTX_0p20_0p25", "in3sigma_mXpTX_0p25_0p30", "in3sigma_mXpTX_0p30_0p35", "in3sigma_mXpTX_0p35_0p40", "in3sigma_mXpTX_0p40_0p45", "in3sigma_mXpTX_0p45_0p50", "in3sigma_mXpTX_0p50_0p55", "in3sigma_mXpTX_0p55"};
  vector<string> selections_den = {"out3sigma_mXpTX_0p05", "out3sigma_mXpTX_0p05_0p10", "out3sigma_mXpTX_0p10_0p15", "out3sigma_mXpTX_0p15_0p20", "out3sigma_mXpTX_0p20_0p25", "out3sigma_mXpTX_0p25_0p30", "out3sigma_mXpTX_0p30_0p35", "out3sigma_mXpTX_0p35_0p40", "out3sigma_mXpTX_0p40_0p45", "out3sigma_mXpTX_0p45_0p50", "out3sigma_mXpTX_0p50_0p55", "out3sigma_mXpTX_0p55"};         
  
  double WP = 0.6;
  //stat->Cuts_studies(selections_den, variable, WP); //this method calculate the cut to apply on variable at a specified WP, varying cuts on variables and calculating efficiency wrt each element of selections_den

  go->Set_logY(false);
  //go->Set_maximum(10);
  //go->Set_minimum(1*pow(10,-2));

  string selection_signal_substring = "in3sigma";
  string selection_background_substring = "out3sigma";

  selections_den = {"in3sigma_mXpTX_0p05", "in3sigma_mXpTX_0p05_0p10", "in3sigma_mXpTX_0p10_0p15", "in3sigma_mXpTX_0p15_0p20", "in3sigma_mXpTX_0p20_0p25", "in3sigma_mXpTX_0p25_0p30", "in3sigma_mXpTX_0p30_0p35", "in3sigma_mXpTX_0p35_0p40", "in3sigma_mXpTX_0p40_0p45", "in3sigma_mXpTX_0p45_0p50", "in3sigma_mXpTX_0p50_0p55", "in3sigma_mXpTX_0p55"};
  
  variable = "mV";

  //stat->Cuts_studies_withROC(selections_den, selection_signal_substring, selection_background_substring, variable);
  // <----- this method calculate the cut to apply on variable in order to maximize estimator eff(signal)*(1-eff(bkg)), where selections to define signal is sel_den+selection_signal_substring (and the same for bkg). varying cut on variable and calculating efficiencies wrt each sel_den in selections_den also the ROC curve is drawn.

  //************* SIGNIFICANCES ************//     
  
  double k = 0.5;  //to calculate significance also with systematics, with factor k
  stat->Set_apply_systematics(false, k); /////
  
  //selections = {"SR_boosted_WP60", "SR_resolved_WP60"};
  selections = {"SR_resolved_WP60_d2_par"};  
  samples = {"Signal_4000_800"};
  //samples = {"Signal_2500_900", "Signal_2500_1200", "Signal_2500_1600", "Signal_2500_2000", "Signal_5000_65", "Signal_5000_900", "Signal_5000_1600", "Signal_5000_2000"};

  leg_entries = {"YXH_qqqq_m65", "YXH_qqqq_Xm200"};
  colors = {kBlue, kViolet};

  go->Overwrite_Selection(false);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);
  go->Set_draw_options(false);

  //vector<string> variables_for_sig = {"mVH", "mVHres"};
  vector<string> variables_for_sig = {"mVHres"};

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!
  h_maker->Set_ScaletoBinWidth(false);

  //stat->DrawSignificancies(selections, variables_for_sig, samples);
  //stat->DrawSignificancies_fromConfig(variables_for_sig);  
  //stat->CombineSignificancies(path1, path2);

  selections = {"SR_resolved_WP60_d2_par"};
  
  variables_for_sig = {"mVHres"}; //selections and variables_for_sig must have the same dimensions
 
  samples = {"Signal_4000_800"};
  //samples = {"Signal_1500_65", "Signal_1500_110_", "Signal_1500_130", "Signal_1500_160_", "Signal_1500_200_", "Signal_1500_250_", "Signal_1500_300"};
  //samples = {"Signal_1800_65", "Signal_1800_110", "Signal_1800_130", "Signal_1800_160", "Signal_1800_200", "Signal_1800_250", "Signal_1800_300"};                                                        
  //samples = {"Signal_2000_65", "Signal_2000_110", "Signal_2000_130", "Signal_2000_160", "Signal_2000_200", "Signal_2000_250", "Signal_2000_300", "Signal_2000_500"};                                     
  //samples = {"Signal_2300_65", "Signal_2300_110", "Signal_2300_130", "Signal_2300_160", "Signal_2300_200", "Signal_2300_250", "Signal_2300_300", "Signal_2300_500"};                                     
  //samples = {"Signal_2500_900", "Signal_2500_1200", "Signal_2500_1600", "Signal_2500_2000"};                                                                                                             
  //samples = {"Signal_2600_65", "Signal_2600_110", "Signal_2600_130", "Signal_2600_160", "Signal_2600_200", "Signal_2600_250", "Signal_2600_300", "Signal_2600_500"};                                     
  //samples = {"Signal_3000_65", "Signal_3000_80", "Signal_3000_110", "Signal_3000_130", "Signal_3000_160", "Signal_3000_200", "Signal_3000_250", "Signal_3000_300", "Signal_3000_400", "Signal_3000_500",   "Signal_3000_600", "Signal_3000_700"};                                                                                                                                                                   
//samples = {"Signal_3400_65", "Signal_3400_110", "Signal_3400_130", "Signal_3400_160", "Signal_3400_200", "Signal_3400_250", "Signal_3400_300", "Signal_3400_400", "Signal_3400_500", "Signal_3400_600","Signal_3400_700", "Signal_3400_800"};                                                                                                                                                                     
//samples = {"Signal_3800_65", "Signal_3800_110", "Signal_3800_130", "Signal_3800_160", "Signal_3800_200", "Signal_3800_250", "Signal_3800_300", "Signal_3800_400", "Signal_3800_500", "Signal_3800_600","Signal_3800_700", "Signal_3800_800",  "Signal_3800_900"};                                                                                                                                                
//samples = {"Signal_4000_65", "Signal_4000_110", "Signal_4000_130", "Signal_4000_160", "Signal_4000_200", "Signal_4000_250", "Signal_4000_300", "Signal_4000_400", "Signal_4000_500", "Signal_4000_600","Signal_4000_700", "Signal_4000_800"}; 
  
  double N_sigma = 3;

  leg_entries = {"Boosted", "Resolved"};
  colors = {kRed, kBlue};
  
  go->Set_lumi(139); 
  
  go->Set_logY(true);
  //go->Set_maximum(1);
  //go->Set_minimum(1*pow(10,-3));

  go->Set_legend_position(0.5, 0.7, 0.80, 0.89);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!                                                                              
  h_maker->Set_ScaletoBinWidth(false);

  //stat->XSectionsUpperLimits(selections, variables_for_sig, samples, N_sigma);

  vector<vector<string>> sels_to_combine, vars_for_sels_to_combine;

  selections = {"SR_resolved_WP60_d2_par"};
  sels_to_combine.push_back(selections);                                                                                                                                                                   
  variables_for_sig = {"mVHres"};                                                                                                                                                                   
  vars_for_sels_to_combine.push_back(variables_for_sig);  

  /*
  selections = {"SR_boosted_WP60_d2_2p0", "SR_resolved_WP60_d2_2p0"};
  sels_to_combine.push_back(selections);
  variables_for_sig = {"mVH", "mVHres"};
  vars_for_sels_to_combine.push_back(variables_for_sig);
  
  selections = {"SR_boosted_WP60_d2_1p0", "SR_resolved_WP60_d2_1p0"};
  sels_to_combine.push_back(selections);
  variables_for_sig = {"mVH", "mVHres"};
  vars_for_sels_to_combine.push_back(variables_for_sig);

  selections = {"SR_boosted_WP60_d2_par", "SR_resolved_WP60_d2_par"};
  sels_to_combine.push_back(selections);
  variables_for_sig = {"mVH", "mVHres"};
  vars_for_sels_to_combine.push_back(variables_for_sig);
  
  selections = {"SR_boosted_WP60_nod2cut"};
  sels_to_combine.push_back(selections);
  variables_for_sig = {"mVH"};
  vars_for_sels_to_combine.push_back(variables_for_sig);
  */
  //samples = {"Signal_1500_65", "Signal_1500_80_", "Signal_1500_110_", "Signal_1500_130", "Signal_1500_160_", "Signal_1500_200_", "Signal_1500_250_", "Signal_1500_300", "Signal_1500_400", "Signal_1500_500", "Signal_1500_600"};
  //samples = {"Signal_1800_65", "Signal_1800_110_", "Signal_1800_130", "Signal_1800_160_", "Signal_1800_200_", "Signal_1800_250_", "Signal_1800_300", "Signal_1800_400", "Signal_1800_500", "Signal_1800_600", "Signal_1800_700"};
  //samples = {"Signal_2000_65", "Signal_2000_110_", "Signal_2000_130", "Signal_2000_160_", "Signal_2000_200_", "Signal_2000_250_", "Signal_2000_300", "Signal_2000_400", "Signal_2000_500", "Signal_2000_600", "Signal_2000_700", "Signal_2000_800", "Signal_2000_1000", "Signal_2000_1100"};
  //samples = {"Signal_2300_65", "Signal_2300_110_", "Signal_2300_130", "Signal_2300_160_", "Signal_2300_200_", "Signal_2300_250_", "Signal_2300_300", "Signal_2300_400", "Signal_2300_500", "Signal_2300_600",  "Signal_2300_700", "Signal_2300_800", "Signal_2300_900", "Signal_2300_1000", "Signal_2300_1100", "Signal_2300_1200"};
  //samples = {"Signal_2500_900", "Signal_2500_1200", "Signal_2500_1600", "Signal_2500_2000"};
  //samples = {"Signal_2600_65", "Signal_2600_110_", "Signal_2600_130", "Signal_2600_160_", "Signal_2600_200_", "Signal_2600_250_", "Signal_2600_300", "Signal_2600_400", "Signal_2600_500", "Signal_2600_600", "Signal_2600_700", "Signal_2600_800", "Signal_2600_900", "Signal_2600_1000", "Signal_2600_1100", "Signal_2600_1200", "Signal_2600_1400"};
  //samples = {"Signal_3000_65", "Signal_3000_80_", "Signal_3000_110_", "Signal_3000_130", "Signal_3000_160_", "Signal_3000_200_", "Signal_3000_250_", "Signal_3000_300", "Signal_3000_400", "Signal_3000_500", "Signal_3000_600", "Signal_3000_700", "Signal_3000_800", "Signal_3000_900", "Signal_3000_1100", "Signal_3000_1200", "Signal_3000_1400", "Signal_3000_1600"};
  //samples = {"Signal_3400_65", "Signal_3400_110_", "Signal_3400_130", "Signal_3400_160_", "Signal_3400_200_", "Signal_3400_250_", "Signal_3400_300", "Signal_3400_400", "Signal_3400_500", "Signal_3400_600", "Signal_3400_700", "Signal_3400_800", "Signal_3400_900", "Signal_3400_1000", "Signal_3400_1200", "Signal_3400_1400", "Signal_3400_1600"};  
  //samples = {"Signal_3600_900", "Signal_3600_1000", "Signal_3600_1100", "Signal_3600_1200", "Signal_3600_1400", "Signal_3600_1600", "Signal_3600_1800"};
  //samples = {"Signal_3800_65", "Signal_3800_110_", "Signal_3800_130", "Signal_3800_160_", "Signal_3800_200_", "Signal_3800_250_", "Signal_3800_300", "Signal_3800_400", "Signal_3800_500", "Signal_3800_600",  "Signal_3800_700", "Signal_3800_800",  "Signal_3800_900", "Signal_3800_1000", "Signal_3800_1100", "Signal_3800_1200", "Signal_3800_1400", "Signal_3800_1600", "Signal_3800_1800", "Signal_3800_2000"};   
  //samples = {"Signal_4000_65", "Signal_4000_110_", "Signal_4000_130", "Signal_4000_160_", "Signal_4000_200_", "Signal_4000_300", "Signal_4000_400", "Signal_4000_500", "Signal_4000_600",  "Signal_4000_700", "Signal_4000_800", "Signal_4000_900", "Signal_4000_1000", "Signal_4000_1100", "Signal_4000_1200", "Signal_4000_1400", "Signal_4000_1600", "Signal_4000_1800", "Signal_4000_2000", "Signal_4000_2200"};  
  //samples = {"Signal_5000_65", "Signal_5000_200_", "Signal_5000_400", "Signal_5000_600", "Signal_5000_800", "Signal_5000_900", "Signal_5000_1000", "Signal_5000_1200", "Signal_5000_1400", "Signal_5000_1600", "Signal_5000_1800", "Signal_5000_2000", "Signal_5000_2500"};
  //samples = {"Signal_6000_400", "Signal_6000_500", "Signal_6000_1000", "Signal_6000_1200", "Signal_6000_1400", "Signal_6000_1600", "Signal_6000_1800", "Signal_6000_2000", "Signal_6000_2500", "Signal_6000_3000"}; 

  N_sigma = 3;


  leg_entries = {"Combined D2X cut=2", "Combined D2X cut=1", "Combined D2X parametric cut", "Boosted no cut"};
  colors = {kViolet, kGreen, kRed, kBlack};
  
  go->Set_lumi(139); 
  
  go->Set_logY(false);
  //go->Set_maximum(0.003);
  //go->Set_minimum(0.0001);

  go->Set_legend_position(0.5, 0.7, 0.8, 0.86);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!                                                                              
  h_maker->Set_ScaletoBinWidth(false);

  //stat->XSectionsUpperLimits_Combined(sels_to_combine, vars_for_sels_to_combine, samples, N_sigma);


  p.Stop();
  p.Print();
}
