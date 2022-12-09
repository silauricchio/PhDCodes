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
  
  std::string config_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/app/config_Plotter.cfg";
  
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
  //h_maker->MakeChain("Signal_4000_250_");
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

  //CHAINS AND LEG ENTRIES ARE IN THE OPPOSITE ORDER!! 

  // *************   NORMALIZATIONS   ************* //
  //h_maker->Set_NormalizeHistos(1.); //<------- normalize all histos 
  //h_maker->Set_NormalizeHisto("mV SR_boosted_WP60_d2_1p0 Signal_2500_1600", (0.00318*175.91391)); //<------- normalize a single histo
  //h_maker->Set_NormalizeStack("mVres SR_resolved_WP60", 1.); //<------- normalize a stacks


  //***** STACK PLOTS *******// <------- here create the stack
  
  vector<TString> leg_entries = {"mY2500_mX1600"};
  vector<Color_t> colors = {kBlack};
  vector<TString> options = {"hist"};

  vector<TString> leg_entries_stack = {"Diboson", "tt", "QCD dijets"};
  vector<Color_t> colors_stack = {kOrange, kGreen+1, kCyan-1};

  go->Set_legend_position(0.65, 0.7, 0.89, 0.89);

  go->Set_maximum(1*pow(10,7));//logY                                                                                                                                                                    
  go->Set_minimum(1*pow(10,-4)); //logY           

  go->Set_logY(true);
  go->Set_lumi(139);
  go->Set_write_lumi(true);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);
  go->Set_draw_options(options);
  go->Overwrite_Selection(false, ""); //<------- write the selection you want in the canvas --->(true, string selection)

  go->Set_draw_stat_uncertainties(true); //<------- statistical uncertainties on ratio plot if MC=DATA (MC/Data=1)

  go->Set_leg_entries_stack(leg_entries_stack);
  go->Set_legend_stack(false);
  go->Set_colors_stack(colors_stack);

  h_maker->Set_ScaletoBinWidth(false, "GeV"); //<------- note: when the bin is filled with event/bin_size it happens that RAW events have no sense!
  
  //h_maker->DrawStacks();
  

  //***** HISTOS *******//   <------- here create superimposed histograms

  leg_entries = {"mY2600 mX200 r=0.077", "mY4000 mX500 r=0.125", "mY5000 m65 r=0.013", "mY5000 mX900 r=0.18", "mY5000 mX1600 r=0.32", "mY5000 mX2000 r=0.4"}; 
  colors = {kBlue, kGreen+1, kMagenta, kRed, kOrange, kCyan};
  options = {"hist", "hist", "hist", "hist", "hist", "hist"};

  //go->Set_lumi(59.1); //mc16e
  //go->Set_lumi(36.1); //mc16a
  go->Set_lumi(139); // fullstat

  go->Set_maximum(1*pow(10,5));//logY                                                                                                                                                                    
  go->Set_minimum(1*pow(10,-4)); //logY   

  go->Set_logY(true);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_legend_position(0.7, 0.6, 0.88, 0.88);
  go->Set_colors(false);
  go->Set_draw_options(false);
  //go->Set_X_axis_name("#Delta #eta");
  h_maker->Set_ScaletoBinWidth(false, "GeV");

  //h_maker->DrawHistos();

  vector<string> histos_num = {"Signal_2600_200", "Signal_4000_500", "Signal_5000_1600"};
  vector<string> histos_den = {"Bkg_QCD_dijet", "Bkg_QCD_dijet", "Bkg_QCD_dijet"};

  vector<Color_t> colors_ratios = {kGreen, kMagenta, kRed}; 
  vector<TString> options_ratios = {"E", "E", "E"};

  go->Set_maximum_ratio(5);
  go->Set_minimum_ratio(0);

  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);
  go->Set_ratio_name("signals/QCD dijet");

  //h_maker->DrawHistos(histos_num, histos_den);
  
  
  //<------- compare histograms <------- //
  vector<string> sels_to_compare = {"SR_resolved_WP60_d2_2p0", "SR_resolved_WP60_d2_1p0", "SR_resolved_WP60_d2_par"};

  vector<string> vars_to_compare = {"trackjet1_pt", "trackjet2_pt","trackjet1_eta", "trackjet2_eta"};

  vector<string> tags_to_compare = {"Signal_3400_300", "Signal_3400_1600"};

  colors = {kViolet, kGreen, kRed};
  options = {"hist", "hist", "hist"};

  leg_entries = {"D2 cut = 2", "D2 cut = 1", "D2 cut = parametric"};

  go->Set_maximum(1*pow(10,5));//logY
  go->Set_minimum(1*pow(10,-2)); //logY

  //go->Set_lumi(139); 
  go->Set_lumi(36.1);
  //go->Set_lumi(59.1);  

  go->Set_logY(true);
  go->Set_colors(colors);
  go->Set_draw_options(options);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  //go->Set_legend_position(0.6, 0.68, 0.86, 0.85); 
  go->Overwrite_Selection(false, "D2(X)>2");
  go->Set_X_axis_name("M_{X res} [GeV]"); 

  //h_maker->Set_ScaletoBinWidth(true, "GeV");
 
  //h_maker->CompareVariables(sels_to_compare, vars_to_compare, tags_to_compare); //<------- this method compares variables for a given selection
  //h_maker->CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare); //<------- this method compares selections for a given variable 
  
  vector<string> sels_num = {"SR_resolved_WP60_d2_2p0", "SR_resolved_WP60_d2_2p0"};
  vector<string> sels_den = {"SR_resolved_WP60_d2_1p0", "SR_resolved_WP60_d2_par"};

  go->Set_ratio_name("2/others");
  go->Set_maximum_ratio(1);                                                                                                                               
  go->Set_minimum_ratio(0); 

  colors_ratios = {kGreen, kRed};
  options_ratios = {"E", "E"};

  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);
  go->Set_draw_ratio_uncertainties(true);  //both num and den with errors

  //h_maker->CompareSelections(sels_to_compare, vars_to_compare, tags_to_compare, sels_num, sels_den); ///this method compares selections for a given variable with ratio
  

  //********** COMPARE HISTOS FROM FILES **************// <------- this method needs alredy existent histograms stored in root files

  string root1 = "SR_resolved_WP60_d2_2p0_Signal_2600_700";
  string file1 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/D2X_studies/v1.4_WP60_FULLSTAT/significances/all/";
  
  string root2 = "SR_resolved_WP60_d2_1p0_Signal_2600_700";
  string file2 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/D2X_studies/v1.4_WP60_FULLSTAT/significances/all/";

  string root3 = "SR_resolved_WP60_d2_par_Signal_2600_700";
  string file3 = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/D2X_studies/v1.4_WP60_FULLSTAT/significances/all/";

  vars_to_compare = {"mVres"};

  leg_entries = {"D2 cut = 2", "D2 cut = 1", "D2 cut = parametric"};
  go->Overwrite_Selection(true, "SR");  
  //go->Set_X_axis_name("M[Gev]"); 
  //go->Set_Y_axis_name("Entries/bin_size");  


  vector<string> histo_names, file_names;
  histo_names.push_back(root1);
  histo_names.push_back(root2);
  histo_names.push_back(root3);

  file_names.push_back(file1);
  file_names.push_back(file2);
  file_names.push_back(file3);

  
  colors = {kViolet, kGreen, kRed};
  options = {"hist", "hist", "hist"};
  go->Set_colors(colors);
  go->Set_draw_options(options);
  go->Set_logY(true);  
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  go->Set_legend_position(0.72, 0.6, 0.88, 0.88);                                                                                                                      
  go->Set_maximum(1*pow(10,5));
  //go->Set_minimum(1*pow(10,-8));

  //h_maker->CompareHistos_fromFile(file_names, histo_names, vars_to_compare); //<------- this method needs alredy existent histos for each var to compare

  vector<string> histo_num = {"SR_resolved_WP60_d2_2p0_Signal_2600_700", "SR_resolved_WP60_d2_2p0_Signal_2600_700"};
  vector<string> histo_den = {"SR_resolved_WP60_d2_1p0_Signal_2600_700", "SR_resolved_WP60_d2_par_Signal_2600_700" };
  
  go->Set_ratio_name("2/others");
  go->Set_maximum_ratio(5);
  go->Set_minimum_ratio(0);
  colors_ratios = {kGreen, kRed};
  options_ratios = {"E", "E"};
  go->Set_colors_ratio(colors_ratios);
  go->Set_draw_options_ratio(options_ratios);

  //h_maker->CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare); //<------- the same comparison, with ratio plot
  
  // **************    with significance bin by bin    *********** //
  //vector<string> histo_bkg = {"CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data", "CR2_boosted_rew_Data"};
  //vector<string> histo_signals = {"CR2_boosted_Signal_1500_300", "CR2_boosted_Signal_2000_300", "CR2_boosted_Signal_2600_300", "CR2_boosted_Signal_3000_300", "CR2_boosted_Signal_3400_300", "CR2_boosted_Signal_4000_300"};
  
  //go->Set_minimum_sig(0);
  //go->Set_maximum_sig(4);
  //vector<Color_t> colors_sig = {kViolet, kBlue, kRed, kRed-2, kGreen, kYellow+1};
  //vector<TString> options_sig = {"hist", "hist", "hist", "hist", "hist", "hist"};
  //go->Set_colors_sig(colors_sig);
  //go->Set_draw_options_sig(options_sig);
  
  //h_maker->CompareHistos_fromFile(file_names, histo_names, histo_num, histo_den, vars_to_compare, histo_signals, histo_bkg);
  

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //  

  //  *************   2D PLOTS *********//
 
  vector<string> var_name1 = {"EH", "EH", "EH", "EV", "EV", "EV"};
  vector<string> var_name2 = {"pTH", "phiH", "etaH", "pTV", "phiV", "etaV"};
  
  vector<string> sel_names = {"CR2_boosted_WP60", "SB2_boosted_WP60"};
  vector<string> chain_tags = {"Signal_2600_200", "Signal_4000_500"};
 
  go->Set_logY(false);
  h_maker->Set_ScaletoBinWidth(false);

  //h_maker->Draw2DHistos(var_name1, var_name2, sel_names, chain_tags, true); //<--------- histo 2D for each pair (var_name1, var_name2), for each sel and chain tag; the last bool is for tprofile

  vector<string> sel_names_num = {"CR2_boosted_WP60"};
  vector<string> sel_names_den = {"SB2_boosted_WP60"};
  
  go->Set_minimum_2D(0.);
  go->Set_maximum_2D(3.);
  //go->Set_2D_draw_option("text colz");
  //h_maker->DrawRatio2D(sel_names_num, sel_names_den, var_name1, var_name2, chain_tags); // <--------- this method requires alredy existent 2D histos to divide
  

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //                                                                                                                                       
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //       

  //**************************************//  
  //************* EFFICIENCIES ************// 
  
  //////////////
  //-Statistics::DrawEfficiencies(selections, selection_den, variable, samples) : with this method you create efficiency curve, calculated on the "variable" chosen, for each sample in "samples". The efficiencies are calculated as a function of "selections", with respect to "selection_den". All the samples curves are superimposed on the same canvas. This method creates a txt file with efficiencies information.  A root files with all the efficiency curves is also created.

  //-Statistics::DrawSignificancies(selections, variables_for_sig, samples) : with this method you create significancies curves, varying selections, for a given variable, for each signal in "samples" vector. Two canvas are created: one with S/Radq(B) significance and another with asymptotic significance.  If the flag for the significance with systematics is set to "true", also another canvas with asymptotic significance with systematics is created.   This method creates a txt file with significancies information.  A root files with all the significance curves is also created.

  //FOR SIGNIFICANCE AND EFFICIENCY STUDIES, ROOT FILES WITH HISTOS MUST ALREDY EXIST!!!!! --> TriggerEfficiency and Statistics class take histos from root files. IF FILES OR HISTOS DO NOT EXIST, THIS WILL CAUSE A SEGMENTATION VIOLATION!!

  //////////////////

  
  Statistics *stat = new Statistics(h_maker, debug_mode, go);
  
  vector<string> selections = {"SR_boosted_WP60_nod2cut","SR_boosted_WP60_d2_2p0", "SR_boosted_WP60_d2_1p0", "SR_boosted_WP60_d2_par"};
  string selection_den = "SR_boosted_WP60_nod2cut";
  string variable = "mVH";
  vector<string> samples = {"Signal_2500_900", "Signal_3800_65", "Signal_4000_65", "Signal_5000_65"};

  leg_entries = {"mY_mX_2500_900", "mY_mX_3800_65", "mY_mX_4000_65", "mY_mX_5000_65"};
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
  h_maker->Set_ScaletoBinWidth(true, "GeV"); //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!   

  //stat->DrawEfficiencies(selections, selection_den, variable, samples);
  //stat->DrawEfficiencies_fromConfig(selection_den, variable); //the same of the above method, but with selections and samples taken from config file

  string selection_num = "SR_boosted_WP60_nod2cut_in3sigma";
  selection_den = "SR_boosted_WP60_nod2cut_in3sigma";
  variable = "mV";
  //stat->DrawEfficiencies_SignalMap(selection_num, selection_den, variable); //<--------- this method calculate a grid of efficiency (selection num/selection_den) with mY vs mX/mY (truth mass); chains taken from config


  // **************************************//   
  // *************   SIGNIFICANCES   ************ //     
  
  double k = 0.5;  //to calculate significance also with systematics, with factor k
  stat->Set_apply_systematics(false, k); /////
  
  selections = {"SR_boosted_WP60", "SR_resolved_WP60"};
  samples = {"Signal_2500_900"};

  leg_entries = {"mY 2500 mX 900"};
  colors = {kBlue};

  go->Overwrite_Selection(false);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(false);
  go->Set_colors(colors);
  go->Set_draw_options(false);

  vector<string> variables_for_sig = {"mVH", "mVHres"};

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!
  h_maker->Set_ScaletoBinWidth(false);

  //stat->DrawSignificancies(selections, variables_for_sig, samples);
  //stat->DrawSignificancies_fromConfig(variables_for_sig);  //the same of the above method, but with selections and samples taken from config file


  //<---------  X sections at a given significance value <---------  //
  //<---------  Selections and variables_for_sig must have the same dimensions

  selections = {"SR_boosted_WP60_d2_1p0", "SR_resolved_WP60_d2_1p0", "SR_boosted_WP60_d2_2p0", "SR_resolved_WP60_d2_2p0"};
  
  variables_for_sig = {"mVH", "mVHres", "mVH", "mVHres"};
 
  samples = {"Signal_1500_65", "Signal_1500_110", "Signal_1500_130", "Signal_1500_160", "Signal_1500_200", "Signal_1500_250", "Signal_1500_300"};
  
  double N_sigma = 3;

  leg_entries = {"Boosted, d2X<1", "Resolved, d2X>1", "Boosted, d2X<2", "Resolved, d2X>2"};
  colors = {kRed, kRed, kBlue, kBlue};
  
  go->Set_lumi(139); 
  
  go->Set_logY(true);
  //go->Set_maximum(1);
  //go->Set_minimum(1*pow(10,-3));

  go->Set_legend_position(0.5, 0.7, 0.80, 0.89);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  go->Set_colors(colors);

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!                                                                              
  h_maker->Set_ScaletoBinWidth(true);

  //stat->XSectionsUpperLimits(selections, variables_for_sig, samples, N_sigma);

  vector<vector<string>> sels_to_combine, vars_for_sels_to_combine;

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

  samples = {"Signal_6000_400", "Signal_6000_500", "Signal_6000_1000", "Signal_6000_1200", "Signal_6000_1400", "Signal_6000_1600", "Signal_6000_1800", "Signal_6000_2000", "Signal_6000_2500", "Signal_6000_3000"}; 

  N_sigma = 3;


  leg_entries = {"Combined D2X cut=2", "Combined D2X cut=1", "Combined D2X parametric cut", "Boosted no cut"};
  colors = {kViolet, kGreen, kRed, kBlack};
  
  go->Set_lumi(139); 
  
  go->Set_logY(false);
  go->Set_maximum(0.003);
  go->Set_minimum(0.0001);

  go->Set_legend_position(0.5, 0.7, 0.8, 0.86);
  go->Set_leg_entries(leg_entries);
  go->Set_legend(true);
  go->Set_colors(colors);

  //IMPORTANT!!! -> SELECT Set_ScaletoBinWidth TRUE OF FALSE, ACCORDING TO HOW PLOTS YOU ARE GOING TO OPEN HAVE BEEN DRAWN!!                                                                              
  h_maker->Set_ScaletoBinWidth(true);

  //stat->XSectionsUpperLimits_Combined(sels_to_combine, vars_for_sels_to_combine, samples, N_sigma);


  p.Stop();
  p.Print();
}
