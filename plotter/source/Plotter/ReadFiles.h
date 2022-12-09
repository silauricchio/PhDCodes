#ifndef ReadFiles_h
#define ReadFiles_h

//Standard Libraries                                                                                                                                                    
#include <iostream>
#include <fstream>
#include <map>
#include <TObjArray.h>

#include "../util/Debug.h"

using namespace std;

class ReadFiles
{

 public:
  ReadFiles();
  ~ReadFiles();

 private:
  map<string, string> map_files_;
  map<string, string> map_selections_;
  map<string, string> map_Xaxis_title_;
  map<string, int> map_Nbins_;
  map<string, pair<double, double>> map_ranges_;
  map<string, bool> map_varbins_;
  map<string, vector<Double_t>> map_binning_;
  
  void Set_map_files(map<string, string> &MapFiles){map_files_= MapFiles;}
  void Set_map_selections(map<string, string> &MapSelections){map_selections_= MapSelections;}
  void Set_map_Xaxis_title(map<string, string> &MapXaxisTitle){map_Xaxis_title_=MapXaxisTitle;}
  void Set_map_Nbins(map<string, int> &MapNBins){map_Nbins_=MapNBins;}
  void Set_map_ranges(map<string,pair<double, double>> &MapRanges){map_ranges_=MapRanges;}
  void Set_map_varbins(map<string, bool> &MapVarbins){map_varbins_=MapVarbins;}
  void Set_map_binning(map<string, vector<Double_t>> &MapBinning){map_binning_=MapBinning;}
  
 public:
  void ReadConfig(bool debug_mode, string config_path);
  
  vector<string> Get_file_tags()
    {
      vector<string> tags;
      map<string,string>::iterator r = this->map_files_.begin();
      while(r != this->map_files_.end())
	{
	  tags.push_back(r->first);
	  r++;
	}
      return tags;
    }
  
  string Get_file(string tag)
  {
    string file = map_files_[tag];
    if(file.size()==0)
      {
	ERROR("Impossible to find file with tag: " << tag);
	exit(EXIT_FAILURE);
      }
    return file;
  }
  
  vector<string> Get_sel_tags()
    {
      vector<string> tags;
      map<string,string>::iterator s = this->map_selections_.begin();
      while(s != this->map_selections_.end())
	{
	  tags.push_back(s->first);
	  s++;
	}
      return tags;
      
    }
  string Get_sel_string(string tag)
  {
    if( map_selections_.find(tag) == map_selections_.end())
      { 
	ERROR("String with tag: " << tag << " not found in map!!");
        exit(EXIT_FAILURE);
      }

    string sel = map_selections_[tag];
    return sel;
  }
  
  void Set_sel_string(string tag, string selection)
  {
    if( map_selections_.find(tag) == map_selections_.end())
      {
	ERROR("String with tag: " << tag << " not found in map!!");
	exit(EXIT_FAILURE);
      }
    map_selections_[tag] = selection;
  }
  
  vector<string> Get_vars_name()
    {
      vector<string> tags;
      map<string,string>::iterator s = this->map_Xaxis_title_.begin();
      while(s != this->map_Xaxis_title_.end())
	{
	  tags.push_back(s->first);
	  s++;
	}
      return tags;
      
    }
  string Get_vars_Xtitle(string tag)
  {
    string title = map_Xaxis_title_[tag];
    if(title.size()==0)
      {
	ERROR("Impossible to find variable with tag: " << tag);
	exit(EXIT_FAILURE);
      }
    return title;
  }
  
  int Get_vars_Nbins(string tag)
  {
    int bins = 0;
    bins = map_Nbins_[tag];
    if(bins==0)
      {
	ERROR("Impossible to find variable with tag: " << tag);
	exit(EXIT_FAILURE);
      }
    return bins;
  }
  
  vector<Double_t> Get_binning(string tag)
    {
      vector<Double_t> binning; 
      
      if(map_binning_.find(tag)!=map_binning_.end())
	{
	  binning = map_binning_[tag];
	}
      else 
	{
	  ERROR("map with variable binning, for the variable " << tag << " does not exist !!");
	  exit(EXIT_FAILURE);
	}
      return binning;
    }
  
  pair<double, double> Get_vars_range(string tag)
  {
    pair<double, double> range = make_pair(-99., -99.);
    range = map_ranges_[tag];
    if(range.first==-99. or range.second==-99.)
      {
	ERROR("Impossible to find variable with tag: " << tag);
	exit(EXIT_FAILURE);
      }
    return range;
  }
  
  bool Get_varbins(string tag)
  {
    bool varbin = map_varbins_[tag];
    return varbin;
  }
  
};



#endif
