#ifndef GraphicsOptions_h
#define GraphicsOptions_h

#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TFile.h"
#include <iostream>
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TColor.h"
#include "TMarker.h"
#include "TLatex.h"
#include "TROOT.h"

using namespace std;

class Graphics_options
{

 public:
  Graphics_options(){};
  ~Graphics_options();
  
 private:
  
  int options_stats_ = 0;
  
  bool draw_stat_uncertainties_ = false;
  bool draw_ratio_uncertainties_ = false;

  bool logXscale_ = false;
  bool logYscale_ = false;
  
  TString Y_name_;
  TString X_name_;

  TString ratio_name_;
  TString sig_name_;  

  TString histo_name_;

  double Y_minimum_ = 0.;
  double Y_maximum_ = 0.;

  double Y_minimum_ratio_ = 0.;
  double Y_maximum_ratio_ = 2.;

  double Y_minimum_sig_ = 0.;
  double Y_maximum_sig_ = 0.;

  double Y_minimum_2D_ = 0.;
  double Y_maximum_2D_ = 0.;


  EColorPalette histo_palette_ = kRainBow;
  TString draw2D_option_ = "colz";

  bool overwrite_selection_ = false;
  bool overwrite_Y_maximum_ = false;  
  bool overwrite_Y_minimum_ = false;
  bool overwrite_Y_maximum_ratio_ = false;
  bool overwrite_Y_minimum_ratio_ = false;
  bool overwrite_Y_maximum_sig_ = false;
  bool overwrite_Y_minimum_sig_ = false;
  bool overwrite_Y_minimum_2D_ = false;
  bool overwrite_Y_maximum_2D_ = false;
  bool overwrite_colors_ = false;
  bool overwrite_colors_ratio_ = false;
  bool overwrite_colors_sig_ = false;
  bool overwrite_colors_stack_ = false;
  bool overwrite_draw_opts_ = false;
  bool overwrite_draw_opts_ratio_ = false;
  bool overwrite_draw_opts_sig_ = false;
  bool overwrite_markers_style_ = false;
  bool overwrite_markers_style_ratio_ = false;
  bool overwrite_markers_style_sig_ = false;
  bool overwrite_legend_ = false;
  bool overwrite_legend_stack_ = false;
  bool overwrite_X_name_ = false;
  bool overwrite_Y_name_ = false;
  bool overwrite_ratio_name_ = false;
  bool overwrite_sig_name_ = false;
  bool overwrite_histo_name_ = false;
  bool overwrite_histo_palette_ = false;

  vector<Color_t> colors_;
  vector<Color_t> colors_ratio_;
  vector<Color_t> colors_sig_;
  vector<Color_t> colors_stack_;

  vector<int> markers_;
  vector<int> markers_ratio_;
  vector<int> markers_sig_;

  vector<TString> drawing_options_;
  vector<TString> drawing_options_ratio_;
  vector<TString> drawing_options_sig_;

  bool ATLAS_legend_ = true;
  bool write_Lumi_ = false;

  double Lumi_;
  string Selection_="";
  string New_Selection_ = "";

  double leg_Xmin_ = 0.69;
  double leg_Ymin_ = 0.75;
  double leg_Xmax_ = 0.88;
  double leg_Ymax_ = 0.89;
 
  vector<TString> leg_entries_;
  vector<TString> leg_entries_stack_;  
  

 public:

  bool write_selection_ = false;

  void Set_draw_stat_uncertainties(bool set)
  {
    draw_stat_uncertainties_ = set;
  }

  bool Get_draw_stat_uncertainties()
  {
    bool set = draw_stat_uncertainties_;
    return set;
  }

  void Set_draw_ratio_uncertainties(bool set)
  {
    draw_ratio_uncertainties_ = set;
  }

  bool Get_draw_ratio_uncertainties()
  {
    bool set = draw_ratio_uncertainties_;
    return set;
  }

  void Set_Palette(EColorPalette palette)
  {
    this->histo_palette_ = palette;
  }

  void Draw_Options()
  {
    gStyle->SetOptStat(options_stats_);
    gStyle->SetPalette(histo_palette_);
  }

  void Set_options_stats(double opt)
  {
    options_stats_ = opt;
  }

  void Set_logY(bool log)
  {
    logYscale_ = log;
  }

  void Set_logX(bool log)
  {
    logXscale_ = log;
  }

  bool Get_logy()
  {
    bool log =  logYscale_;
    return log;
  }

  bool Get_logx()
  {
    bool log =logXscale_;
    return log;
  }

   
  void Set_ATLAS_draw(bool set)
  {
    ATLAS_legend_ = set;
  }

  bool Get_ATLAS_option()
  {
    bool opt = ATLAS_legend_;
    return opt;
  }

  void Draw_ATLAS_legend()
  {
    gROOT->SetStyle("ATLAS");
    gROOT->ForceStyle();
    TLatex l; 
    l.SetTextFont(72);
    l.SetTextSize(0.04);
    l.DrawLatexNDC(0.2, 0.89, "ATLAS");  
    l.SetTextFont(42);
    l.DrawLatexNDC(0.3, 0.89, "Internal");
    if(!write_Lumi_) l.DrawLatexNDC(0.2, 0.842, TString::Format("#scale[0.8]{#sqrt{s} = 13 TeV}" ));    
    //if(write_Lumi_) l.DrawLatexNDC(0.2, 0.844, TString::Format("#scale[0.8]{#sqrt{s} = 13 TeV,  #int Ldt = %.1f fb^{-1}}", Lumi_));  
    if(write_Lumi_) l.DrawLatexNDC(0.2, 0.842, TString::Format("#scale[0.8]{#sqrt{s} = 13 TeV,  %.1f fb^{-1}}", Lumi_));
    if(write_selection_) 
      {
	TString selection = Selection_.c_str() ;
	l.DrawLatexNDC(0.2, 0.79, "#scale[0.9]{" + selection +"}"); 
      } 
  }
  
  void Overwrite_Selection(bool set, string sel="")
  {
    overwrite_selection_ = set;
    New_Selection_ = sel;
  }

  void Set_selection(string sel)
  {
    write_selection_ = true;
    Selection_ = sel;    
  }
  
  string Get_selection()
  {
    string sel = Selection_;
    return sel;
  }

  bool Get_overwrite_selection()
  {
    bool sel =  overwrite_selection_;
    return sel;
  }
  string Get_new_selection()
  {
    string sel = New_Selection_;
    return sel;
  }

  void Set_X_axis_name(TString name)
  {
    overwrite_X_name_ = true;
    X_name_ = name;
  }
  
  void Set_Y_axis_name(TString name)
  {
    overwrite_Y_name_ =true;
    Y_name_ = name;
  }
  
  TString Get_X_axis_name()
  {
    TString name = X_name_;
    return name;
  }

  TString Get_Y_axis_name()
  {
    TString name = Y_name_;
    return name;
  }

  void Set_ratio_name(TString name)
  {
    overwrite_ratio_name_ =true;
    ratio_name_ = name;
  }
  void Set_sig_name(TString name)
  {
    overwrite_sig_name_ =true;
    sig_name_ = name;
  }

  TString Get_ratio_name()
  {
    TString name = ratio_name_;
    return name;
  }
  TString Get_sig_name()
  {
    TString name = sig_name_;
    return name;
  }

  bool Get_overwrite_x_name()
  {
    bool ov = overwrite_X_name_;
    return ov;
  }
  bool Get_overwrite_y_name()
  {
    bool ov = overwrite_Y_name_;
    return ov;
  }
  bool Get_overwrite_ratio_name()
  {
    bool ov = overwrite_ratio_name_;
    return ov;
  }
  bool Get_overwrite_sig_name()
  {
    bool ov = overwrite_sig_name_;
    return ov;
  }
  
  void Set_maximum(double max)
  {
    Y_maximum_ = max;
    overwrite_Y_maximum_ = true;    
  }
  void Set_minimum(double min)
  {
    Y_minimum_ = min;
    overwrite_Y_minimum_ = true;
  }

  double Get_maximum()
  {
    double max = Y_maximum_;
    return max;
  }

  double Get_minimum()
  {
    double min = Y_minimum_;
    return min;
  }

  void Set_maximum_2D(double max)
  {
    Y_maximum_2D_ = max;
    overwrite_Y_maximum_2D_ = true;
  }

  void Set_minimum_2D(double max)
  {
    Y_minimum_2D_ = max;
    overwrite_Y_minimum_2D_ = true;
  }

  double Get_maximum_2D()
  {
    double max = Y_maximum_2D_;
    return max;
  }

  double Get_minimum_2D()
  {
    double min = Y_minimum_2D_;
    return min;
  }

  

  void Set_maximum_ratio(double max)
  {
    Y_maximum_ratio_ = max;
    overwrite_Y_maximum_ratio_ = true;
  }

  void Set_maximum_sig(double max)
  {
    Y_maximum_sig_ = max;
    overwrite_Y_maximum_sig_ = true;
  }

  void Set_minimum_ratio(double min)
  {
    Y_minimum_ratio_ = min;
    overwrite_Y_minimum_ratio_ = true;
  }
  void Set_minimum_sig(double min)
  {
    Y_minimum_sig_ = min;
    overwrite_Y_minimum_sig_ = true;
  }

  double Get_maximum_ratio()
  {
    double max = Y_maximum_ratio_;
    return max;
  }
  double Get_maximum_sig()
  {
    double max = Y_maximum_sig_;
    return max;
  }

  double Get_minimum_ratio()
  {
    double min = Y_minimum_ratio_;
    return min;
  }
  double Get_minimum_sig()
  {
    double min = Y_minimum_sig_;
    return min;
  }
  
  void Set_colors(bool set)
  {
    overwrite_colors_ = set;
  }

  void Set_colors(vector<Color_t> col)
  {
    overwrite_colors_ = true;
    colors_ = col;
  }
  
  vector<Color_t> Get_colors()
    {
      vector<Color_t> col = colors_;
      return col;
    }

  void Set_colors_ratio(bool set)
  {
    overwrite_colors_ratio_ = set;
  }
  void Set_colors_ratio(vector<Color_t> col)
  {
    overwrite_colors_ratio_ = true;
    colors_ratio_ = col;
  }
  void Set_colors_sig(vector<Color_t> col)
  {
    overwrite_colors_sig_ = true;
    colors_sig_ = col;
  }
  
  vector<Color_t> Get_colors_ratio()
    {
      vector<Color_t> col = colors_ratio_;
      return col;
    }

  vector<Color_t> Get_colors_sig()
    {
      vector<Color_t> col = colors_sig_;
      return col;
    }

  void Set_colors_stack(vector<Color_t> col)
  {
    overwrite_colors_stack_ = true;
    colors_stack_ = col;
  }

  void Set_colors_stack(bool set)
  {
    overwrite_colors_stack_ = set;
  }

  vector<Color_t> Get_colors_stack()
    {
      vector<Color_t> col = colors_stack_;
      return col;
    }


  void Set_draw_options(bool set)
  {
    overwrite_draw_opts_ = set;
  }

  void Set_draw_options(vector<TString> options)
  {
    overwrite_draw_opts_ = true;
    drawing_options_ = options;
  }

  vector<TString> Get_draw_options()
    {
      vector<TString> options = drawing_options_;
      return options;
    }

  void Set_draw_options_ratio(vector<TString> options)
  {
    overwrite_draw_opts_ratio_ = true;
    drawing_options_ratio_ = options;
  }
  void Set_draw_options_sig(vector<TString> options)
  {
    overwrite_draw_opts_sig_ = true;
    drawing_options_sig_ = options;
  }

  vector<TString> Get_draw_options_ratio()
    {
      vector<TString> options = drawing_options_ratio_;
      return options;
    }
  
  vector<TString> Get_draw_options_sig()
    {
      vector<TString> options = drawing_options_sig_;
      return options;
    }

  void Set_markers(vector<int> marks)
  {
    overwrite_markers_style_ = true;
    markers_ = marks;
  }
  vector<int> Get_markers()
    {
      vector<int> marks = markers_;
      return marks;
    }
  
  void Set_markers_ratio(vector<int> marks)
  {
    overwrite_markers_style_ratio_ = true;
    markers_ratio_ = marks;
  }

  void Set_markers_sig(vector<int> marks)
  {
    overwrite_markers_style_sig_ = true;
    markers_sig_ = marks;
  }

  vector<int> Get_markers_ratio()
    {
      vector<int> marks = markers_ratio_;
      return marks;
    }

  vector<int> Get_markers_sig()
    {
      vector<int> marks = markers_sig_;
      return marks;
    }

  bool Get_overwrite_Markers()
  {
    bool overwrite_markers = overwrite_markers_style_;
    return overwrite_markers;
  }

  
  bool Get_overwrite_Markers_ratio()
  {
    bool overwrite_markers = overwrite_markers_style_ratio_;
    return overwrite_markers;
  }

  bool Get_overwrite_Markers_sig()
  {
    bool overwrite_markers = overwrite_markers_style_sig_;
    return overwrite_markers;
  }


  bool Get_overwrite_Colors()
  {
    bool overwrite_col =  overwrite_colors_;
    return overwrite_col;
  }

  bool Get_overwrite_Colors_ratio()
  {
    bool overwrite_col =  overwrite_colors_ratio_;
    return overwrite_col;
  }
  
  bool Get_overwrite_Colors_sig()
  {
    bool overwrite_col =  overwrite_colors_sig_;
    return overwrite_col;
  }

  bool Get_overwrite_Colors_stack()
  {
    bool overwrite_col =  overwrite_colors_stack_;
    return overwrite_col;
  }


  bool Get_overwrite_Draw_Options()
  {
    bool overwrite_draw = overwrite_draw_opts_;
    return overwrite_draw;
  }

  bool Get_overwrite_Draw_Options_ratio()
  {
    bool overwrite_draw = overwrite_draw_opts_ratio_;
    return overwrite_draw;
  }
  
  bool Get_overwrite_Draw_Options_sig()
  {
    bool overwrite_draw = overwrite_draw_opts_sig_;
    return overwrite_draw;
  }

  void Set_ATLAS_legend(bool leg)
  {
    ATLAS_legend_ = leg;
  }

  void Set_write_lumi(bool lumi)
  {
    write_Lumi_ = lumi;
  }
  
  void Set_lumi(double lumi)
  {
    Lumi_ = lumi;
  }

  double Get_lumi()
  {
    double lumi = Lumi_;
    return lumi;
  }
 
  void Set_legend(bool set)
  {
    overwrite_legend_ = set;
  }
  
  bool Get_overwrite_Legend()
  {
    bool ov_leg = overwrite_legend_;
    return ov_leg;
  }

  void Set_legend_stack(bool set)
  {
    overwrite_legend_stack_ = set;
  }

  bool Get_overwrite_Legend_stack()
  {
    bool ov_leg = overwrite_legend_stack_;
    return ov_leg;
  }

  void Set_legend_position(double xmin, double ymin, double xmax, double ymax)
  {
    leg_Xmin_ = xmin;
    leg_Ymin_ = ymin;
    leg_Xmax_ = xmax;
    leg_Ymax_ = ymax;
  }

  double Get_leg_xmin()
  {
    double xmin = leg_Xmin_;
    return xmin;
  }
  double Get_leg_xmax()
  {
    double xmax= leg_Xmax_;
    return xmax;
  }
  double Get_leg_ymin()
  {
    double ymin= leg_Ymin_;
    return ymin;
  }
  double Get_leg_ymax()
  {
    double ymax= leg_Ymax_;
    return ymax;
  }

  void Set_leg_entries(vector<TString> entries)
  {
    leg_entries_ = entries;
  }
  vector<TString> Get_leg_entries()
    {
      vector<TString> entries =  leg_entries_;
      return entries;
    }

  void Set_leg_entries_stack(vector<TString> entries)
  {
    leg_entries_stack_ = entries;
  }
  vector<TString> Get_leg_entries_stack()
    {
      vector<TString> entries =  leg_entries_stack_;
      return entries;
    }

  void Set_overwrite_Y_maximum(bool set)
  {
    overwrite_Y_maximum_ = set;
  }

  bool Get_overwrite_Y_maximum()
  {
    bool ov = overwrite_Y_maximum_;
    return ov;
  }

  void Set_overwrite_Y_minimum(bool set)
  {
    overwrite_Y_minimum_ = set;
  }

  bool Get_overwrite_Y_minimum()
  {
    bool ov = overwrite_Y_minimum_;
    return ov;
  }
  
  void Set_overwrite_Y_maximum_2D(bool set)
  {
    overwrite_Y_maximum_2D_ = set;
  }

  bool Get_overwrite_Y_maximum_2D()
  {
    bool ov = overwrite_Y_maximum_2D_;
    return ov;
  }

  void Set_overwrite_Y_minimum_2D(bool set)
  {
    overwrite_Y_minimum_2D_ = set;
  }

  bool Get_overwrite_Y_minimum_2D()
  {
    bool ov = overwrite_Y_minimum_2D_;
    return ov;
  }


  void Set_overwrite_Y_maximum_ratio(bool set)
  {
    overwrite_Y_maximum_ratio_ = set;
  }
  void Set_overwrite_Y_maximum_sig(bool set)
  {
    overwrite_Y_maximum_sig_ = set;
  }

  bool Get_overwrite_Y_maximum_ratio()
  {
    bool ov = overwrite_Y_maximum_ratio_;
    return ov;
  }
  bool Get_overwrite_Y_maximum_sig()
  {
    bool ov = overwrite_Y_maximum_sig_;
    return ov;
  }

  void Set_overwrite_Y_minimum_ratio(bool set)
  {
    overwrite_Y_minimum_ratio_ = set;
  }

  void Set_overwrite_Y_minimum_sig(bool set)
  {
    overwrite_Y_minimum_sig_ = set;
  }

  bool Get_overwrite_Y_minimum_ratio()
  {
    bool ov = overwrite_Y_minimum_ratio_;
    return ov;
  }

  bool Get_overwrite_Y_minimum_sig()
  {
    bool ov = overwrite_Y_minimum_sig_;
    return ov;
  }

  void Set_histo_name(TString name)
  {
    overwrite_histo_name_ = true;
    histo_name_ = name;
  }
  
  TString Get_histo_name()
  {
    TString name = histo_name_;
    return name;
  }

  bool Get_overwrite_histo_name()
  {
    bool ov = overwrite_histo_name_;
    return ov;
  }

  bool Get_overwrite_2D_draw_option()
  {
    return draw2D_option_;
  }

  void Set_2D_draw_option(string opt)
  {
    draw2D_option_ = opt;
  }

  TString Get_2D_draw_option()
  {
    TString opt = draw2D_option_;
    return opt;
  }

};

#endif
