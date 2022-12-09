//std C++ libraries                                                                                                                                                     
#include <string>
#include <fstream>
#include <getopt.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <TObjArray.h>

#include "../util/Debug.h"
#include "../Plotter/ReadFiles.h"


using namespace std;


ReadFiles::ReadFiles(){}

ReadFiles::~ReadFiles(){}

void ReadFiles::ReadConfig(bool debug_mode, string config_path)
{

  INFO(" Reading config file: " << config_path.c_str());

  bool correctly_read = false;
  bool correctly_read_files = false;
  bool correctly_read_sel = false;
  bool correctly_read_var = false;
  bool correctly_read_binning = false;
  
  string name_file;// Input Root File to be accessed = {"WqqZll_Sh221.root"};
  string tag1; // First Argument for tag in ntuple (Bkg/Data/Signal)
  string tag2; // Second Argument for tag in ntuple (Zjets, top, etc..)
  string tag3; // Third Argument for tag in ntuple (ZeeL, ttbar, etc..)
  string tag; //tag1+tag2+tag3 


  string Variable; //Variables to draw
  string name_Xaxis;
  int nbins;
  double A;
  double B;

  map<string, string> map_files; //files root in a map, identified by tag                                                                                                                                  
  map<string, string> map_selections; //selections in a map, identified by tag
  map<string, string> map_Xaxis_titles; //maps with Xaxis titles for the variables to plot, identified by the variable name
  map<string, int> map_Nbins; //maps with Nbins for the variables to plot, identified by the variable name  
  map<string, pair<double, double>> map_ranges;//maps with ranges for the variables to plot, identified by the variable name
  map<string, bool> map_varbins; //maps with the bool isVariableBin,  identified by the variable name  
  map<string, vector<Double_t>> map_binning; //maps with variable binnings

  ifstream config_file (config_path.c_str());
  string line;
  string word;

  // Open and read the config file                                                                                                                                      
  if (config_file.is_open())
    {
      INFO(" Succesfully Access config file ");
      
      while ( ! config_file.eof())
	{
	  getline(config_file, line);
	  istringstream iss(line);
	  vector<string> results(istream_iterator<string>{iss},istream_iterator<string>());	
	  

	  if (debug_mode) cout << " Howmany word   == " << results.size() << endl;

	  char IsOn[] = "ON"; 

	  if (results.size()>0 and (strstr(results[0].c_str(),"#")==NULL) and (strstr(results[1].c_str(),"*")==NULL) and results.size() == 5 and (strstr(results[0].c_str(),"binning")==NULL))
	    {
	
	      correctly_read_files = true;
		      
	      if (debug_mode)
		{
		  cout << " Found Process   == " << results[0].c_str() << endl;
		  cout << " Found RootFile  == " << results[1].c_str() << endl;
		  cout << " Found Bkg/Data  == " << results[2].c_str() << endl;
		  cout << " Found Category  == " << results[3].c_str() << endl;
		  cout << " Found Status    == " << results[4].c_str() << endl;
		}
	      
	      if ( !strcmp(results[4].c_str(), IsOn) )
		{
	
		  cout<<" Load the sample --> " << results[0].c_str()<<endl;
		  name_file = results[1].c_str();
		  tag1 = results[2].c_str(); //Bkg-Data
		  tag2 = results[3].c_str(); //Diboson, Zjets, Top, etc...
		  tag3 = results[0].c_str(); //Process Name --> ZeeL, Singletop, ttbar, etc..
		  tag = tag1 + " " + tag2 + " " + tag3;

		  map_files[tag] = name_file;

		}

	      else
		{ 
		  if (debug_mode) cout << " Skipping sample --> " << results[0].c_str() << endl;
		}
	    }
	  
	  else if (results.size()>0 and (strstr(results[0].c_str(),"#")==NULL) and (strstr(results[1].c_str(),"*")!=NULL) and (strstr(results[0].c_str(),"binning")==NULL))
	    {

	      correctly_read_sel = true;
	      cout << " Read the selection --> " << results[0].c_str() << endl;
	       
	      string space = " ";
	      string string_sel; // Selection to apply at ntuples

	      for(int i=1; i<results.size(); i++)  string_sel += (space + results[i].c_str());
	
	      map_selections[results[0].c_str()] = string_sel;


	      if (debug_mode)
                {
                  cout << " Found selection == " << results[0].c_str();
		  cout << " with string selection == " << string_sel << endl;
		}

	    }
	  else if(results.size()>0 and (strstr(results[0].c_str(),"#")==NULL) and (strstr(results[1].c_str(),"*")==NULL) and results.size()==6 and (strstr(results[0].c_str(),"binning")==NULL))
	    {

	      if (debug_mode)
                {
		  cout << " Found variable == " << results[0].c_str();
		  cout << " with axis title == " << results[1].c_str();
		  cout << " with nbins == " << results[2].c_str();
		  cout << " with range == " << results[3].c_str() << " " <<results[4].c_str();
		  cout << " with var bin set to == " << results[5].c_str() << endl;
		}

	      correctly_read_var = true;
	      Variable = results[0].c_str();
	      name_Xaxis = results[1].c_str();
	      nbins = std::stoi(results[2].c_str());
	      A = std::stod(results[3].c_str());
	      B = std::stod(results[4].c_str());

	      map_Xaxis_titles[Variable] = name_Xaxis;
	      map_Nbins[Variable] = nbins;
	      map_ranges[Variable] = make_pair(A, B);
	      (!strcmp(results[5].c_str(),IsOn)) ? map_varbins[Variable]=true : map_varbins[Variable]=false;
	      
	    } 

	  else if(results.size()>0 and (strstr(results[0].c_str(),"#")==NULL) and (strstr(results[1].c_str(),"*")==NULL) and (strstr(results[0].c_str(),"binning")!=NULL))
	    {
	      correctly_read_binning = true;
	      Variable = results[1].c_str();

	      int size = (results.size()-2) -1; // nbins
	      vector<Double_t> binning;
	      for(int i=2; i<results.size(); i++)  binning.push_back(atof(results[i].c_str()));
              map_binning[Variable] = binning;

	      if(debug_mode) 
		{
		  cout << " Found variable binning for " << results[1].c_str();
		  cout << " Binning = ( ";
		  for(int f=0; f<results.size()-2; f++) cout << binning.at(f) << " ";
		  cout << " )" << endl;
		}
	    }
	}
      
      correctly_read =  correctly_read_files and correctly_read_sel and correctly_read_var;
      config_file.close();
      
      if(correctly_read) //here the config files setting are stored
	{  
	  this->Set_map_files(map_files);
	  this->Set_map_selections(map_selections);
	  this->Set_map_Xaxis_title(map_Xaxis_titles);
	  this->Set_map_Nbins(map_Nbins);
	  this->Set_map_ranges(map_ranges);
	  this->Set_map_varbins(map_varbins);
	  if(correctly_read_binning) this->Set_map_binning(map_binning);
	}
      
      if(!correctly_read)
	{
	  ERROR(" The config file is not written properly");
	  exit (EXIT_FAILURE);
	}

    }
  else
    {
      ERROR(" Unable to open config file: " << config_path );
      exit (EXIT_FAILURE);
    }
    
}
