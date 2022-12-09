void mergeHistos() 
{
  
  TString basic_path = "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/D2X_studies/v1.4_WP60_FULLSTAT/V_masses/efficiencies/";
  
  TString variable = "mV";
  
  vector<TString> selections = {"SR_boosted_WP60_nod2cut", "SR_boosted_WP60_nod2cut_in3sigma", "SR_boosted_WP60_nod2cut_out3sigma"};

  //vector<TString> selections = {"SR_boosted_WP60_nod2cut", "SR_boosted_WP60_nod2cut_in3sigma",  "SR_boosted_WP60_mXpTX_0p05_in3sigma", "SR_boosted_WP60_mXpTX_0p05_0p10_in3sigma", "SR_boosted_WP60_mXpTX_0p10_0p15_in3sigma", "SR_boosted_WP60_mXpTX_0p15_0p20_in3sigma", "SR_boosted_WP60_mXpTX_0p20_0p25_in3sigma", "SR_boosted_WP60_mXpTX_0p25_0p30_in3sigma", "SR_boosted_WP60_mXpTX_0p30_0p35_in3sigma", "SR_boosted_WP60_mXpTX_0p35_0p40_in3sigma", "SR_boosted_WP60_mXpTX_0p40_0p45_in3sigma", "SR_boosted_WP60_mXpTX_0p45_0p50_in3sigma", "SR_boosted_WP60_mXpTX_0p50_0p55_in3sigma", "SR_boosted_WP60_mXpTX_0p55_in3sigma",  "SR_boosted_WP60_nod2cut_out3sigma",  "SR_boosted_WP60_mXpTX_0p05_out3sigma", "SR_boosted_WP60_mXpTX_0p05_0p10_out3sigma", "SR_boosted_WP60_mXpTX_0p10_0p15_out3sigma", "SR_boosted_WP60_mXpTX_0p15_0p20_out3sigma", "SR_boosted_WP60_mXpTX_0p20_0p25_out3sigma", "SR_boosted_WP60_mXpTX_0p25_0p30_out3sigma", "SR_boosted_WP60_mXpTX_0p30_0p35_out3sigma", "SR_boosted_WP60_mXpTX_0p35_0p40_out3sigma", "SR_boosted_WP60_mXpTX_0p40_0p45_out3sigma", "SR_boosted_WP60_mXpTX_0p45_0p50_out3sigma", "SR_boosted_WP60_mXpTX_0p50_0p55_out3sigma", "SR_boosted_WP60_mXpTX_0p55_out3sigma"};
   
  /*    
  vector<TString> selections = {
    "in3sigma_mXpTX_0p05", "in3sigma_mXpTX_0p05_d2_0p2", "in3sigma_mXpTX_0p05_d2_0p4", "in3sigma_mXpTX_0p05_d2_0p6", "in3sigma_mXpTX_0p05_d2_0p8", "in3sigma_mXpTX_0p05_d2_1p0", "in3sigma_mXpTX_0p05_d2_1p2", "in3sigma_mXpTX_0p05_d2_1p4", "in3sigma_mXpTX_0p05_d2_1p6", "in3sigma_mXpTX_0p05_d2_1p8", "in3sigma_mXpTX_0p05_d2_2p0", "in3sigma_mXpTX_0p05_d2_2p2", "in3sigma_mXpTX_0p05_d2_2p4", "in3sigma_mXpTX_0p05_d2_2p6", "in3sigma_mXpTX_0p05_d2_2p8", "in3sigma_mXpTX_0p05_d2_3p0", "in3sigma_mXpTX_0p05_d2_3p2", "in3sigma_mXpTX_0p05_d2_3p4", "in3sigma_mXpTX_0p05_d2_3p6", "in3sigma_mXpTX_0p05_d2_3p8", "in3sigma_mXpTX_0p05_d2_4p0", "in3sigma_mXpTX_0p05_d2_4p2", "in3sigma_mXpTX_0p05_d2_4p4", "in3sigma_mXpTX_0p05_d2_4p6", "in3sigma_mXpTX_0p05_d2_4p8", "in3sigma_mXpTX_0p05_d2_5p0", 

    "in3sigma_mXpTX_0p05_0p10", "in3sigma_mXpTX_0p05_0p10_d2_0p2", "in3sigma_mXpTX_0p05_0p10_d2_0p4", "in3sigma_mXpTX_0p05_0p10_d2_0p6", "in3sigma_mXpTX_0p05_0p10_d2_0p8", "in3sigma_mXpTX_0p05_0p10_d2_1p0", "in3sigma_mXpTX_0p05_0p10_d2_1p2", "in3sigma_mXpTX_0p05_0p10_d2_1p4", "in3sigma_mXpTX_0p05_0p10_d2_1p6", "in3sigma_mXpTX_0p05_0p10_d2_1p8", "in3sigma_mXpTX_0p05_0p10_d2_2p0", "in3sigma_mXpTX_0p05_0p10_d2_2p2", "in3sigma_mXpTX_0p05_0p10_d2_2p4", "in3sigma_mXpTX_0p05_0p10_d2_2p6", "in3sigma_mXpTX_0p05_0p10_d2_2p8", "in3sigma_mXpTX_0p05_0p10_d2_3p0", "in3sigma_mXpTX_0p05_0p10_d2_3p2", "in3sigma_mXpTX_0p05_0p10_d2_3p4", "in3sigma_mXpTX_0p05_0p10_d2_3p6", "in3sigma_mXpTX_0p05_0p10_d2_3p8", "in3sigma_mXpTX_0p05_0p10_d2_4p0", "in3sigma_mXpTX_0p05_0p10_d2_4p2", "in3sigma_mXpTX_0p05_0p10_d2_4p4", "in3sigma_mXpTX_0p05_0p10_d2_4p6", "in3sigma_mXpTX_0p05_0p10_d2_4p8", "in3sigma_mXpTX_0p05_0p10_d2_5p0", 

    "in3sigma_mXpTX_0p10_0p15", "in3sigma_mXpTX_0p10_0p15_d2_0p2", "in3sigma_mXpTX_0p10_0p15_d2_0p4", "in3sigma_mXpTX_0p10_0p15_d2_0p6", "in3sigma_mXpTX_0p10_0p15_d2_0p8", "in3sigma_mXpTX_0p10_0p15_d2_1p0", "in3sigma_mXpTX_0p10_0p15_d2_1p2", "in3sigma_mXpTX_0p10_0p15_d2_1p4", "in3sigma_mXpTX_0p10_0p15_d2_1p6", "in3sigma_mXpTX_0p10_0p15_d2_1p8", "in3sigma_mXpTX_0p10_0p15_d2_2p0", "in3sigma_mXpTX_0p10_0p15_d2_2p2", "in3sigma_mXpTX_0p10_0p15_d2_2p4", "in3sigma_mXpTX_0p10_0p15_d2_2p6", "in3sigma_mXpTX_0p10_0p15_d2_2p8", "in3sigma_mXpTX_0p10_0p15_d2_3p0", "in3sigma_mXpTX_0p10_0p15_d2_3p2", "in3sigma_mXpTX_0p10_0p15_d2_3p4", "in3sigma_mXpTX_0p10_0p15_d2_3p6", "in3sigma_mXpTX_0p10_0p15_d2_3p8", "in3sigma_mXpTX_0p10_0p15_d2_4p0", "in3sigma_mXpTX_0p10_0p15_d2_4p2", "in3sigma_mXpTX_0p10_0p15_d2_4p4", "in3sigma_mXpTX_0p10_0p15_d2_4p6", "in3sigma_mXpTX_0p10_0p15_d2_4p8", "in3sigma_mXpTX_0p10_0p15_d2_5p0", 

    "in3sigma_mXpTX_0p15_0p20", "in3sigma_mXpTX_0p15_0p20_d2_0p2", "in3sigma_mXpTX_0p15_0p20_d2_0p4", "in3sigma_mXpTX_0p15_0p20_d2_0p6", "in3sigma_mXpTX_0p15_0p20_d2_0p8", "in3sigma_mXpTX_0p15_0p20_d2_1p0", "in3sigma_mXpTX_0p15_0p20_d2_1p2", "in3sigma_mXpTX_0p15_0p20_d2_1p4", "in3sigma_mXpTX_0p15_0p20_d2_1p6", "in3sigma_mXpTX_0p15_0p20_d2_1p8", "in3sigma_mXpTX_0p15_0p20_d2_2p0", "in3sigma_mXpTX_0p15_0p20_d2_2p2", "in3sigma_mXpTX_0p15_0p20_d2_2p4", "in3sigma_mXpTX_0p15_0p20_d2_2p6", "in3sigma_mXpTX_0p15_0p20_d2_2p8", "in3sigma_mXpTX_0p15_0p20_d2_3p0", "in3sigma_mXpTX_0p15_0p20_d2_3p2", "in3sigma_mXpTX_0p15_0p20_d2_3p4", "in3sigma_mXpTX_0p15_0p20_d2_3p6", "in3sigma_mXpTX_0p15_0p20_d2_3p8", "in3sigma_mXpTX_0p15_0p20_d2_4p0", "in3sigma_mXpTX_0p15_0p20_d2_4p2", "in3sigma_mXpTX_0p15_0p20_d2_4p4", "in3sigma_mXpTX_0p15_0p20_d2_4p6", "in3sigma_mXpTX_0p15_0p20_d2_4p8", "in3sigma_mXpTX_0p15_0p20_d2_5p0", 

    "in3sigma_mXpTX_0p20_0p25", "in3sigma_mXpTX_0p20_0p25_d2_0p2", "in3sigma_mXpTX_0p20_0p25_d2_0p4", "in3sigma_mXpTX_0p20_0p25_d2_0p6", "in3sigma_mXpTX_0p20_0p25_d2_0p8", "in3sigma_mXpTX_0p20_0p25_d2_1p0", "in3sigma_mXpTX_0p20_0p25_d2_1p2", "in3sigma_mXpTX_0p20_0p25_d2_1p4", "in3sigma_mXpTX_0p20_0p25_d2_1p6", "in3sigma_mXpTX_0p20_0p25_d2_1p8", "in3sigma_mXpTX_0p20_0p25_d2_2p0", "in3sigma_mXpTX_0p20_0p25_d2_2p2", "in3sigma_mXpTX_0p20_0p25_d2_2p4", "in3sigma_mXpTX_0p20_0p25_d2_2p6", "in3sigma_mXpTX_0p20_0p25_d2_2p8", "in3sigma_mXpTX_0p20_0p25_d2_3p0", "in3sigma_mXpTX_0p20_0p25_d2_3p2", "in3sigma_mXpTX_0p20_0p25_d2_3p4", "in3sigma_mXpTX_0p20_0p25_d2_3p6", "in3sigma_mXpTX_0p20_0p25_d2_3p8", "in3sigma_mXpTX_0p20_0p25_d2_4p0", "in3sigma_mXpTX_0p20_0p25_d2_4p2", "in3sigma_mXpTX_0p20_0p25_d2_4p4", "in3sigma_mXpTX_0p20_0p25_d2_4p6", "in3sigma_mXpTX_0p20_0p25_d2_4p8", "in3sigma_mXpTX_0p20_0p25_d2_5p0", 

        "in3sigma_mXpTX_0p25_0p30", "in3sigma_mXpTX_0p25_0p30_d2_0p2", "in3sigma_mXpTX_0p25_0p30_d2_0p4", "in3sigma_mXpTX_0p25_0p30_d2_0p6", "in3sigma_mXpTX_0p25_0p30_d2_0p8", "in3sigma_mXpTX_0p25_0p30_d2_1p0", "in3sigma_mXpTX_0p25_0p30_d2_1p2", "in3sigma_mXpTX_0p25_0p30_d2_1p4", "in3sigma_mXpTX_0p25_0p30_d2_1p6", "in3sigma_mXpTX_0p25_0p30_d2_1p8", "in3sigma_mXpTX_0p25_0p30_d2_2p0", "in3sigma_mXpTX_0p25_0p30_d2_2p2", "in3sigma_mXpTX_0p25_0p30_d2_2p4", "in3sigma_mXpTX_0p25_0p30_d2_2p6", "in3sigma_mXpTX_0p25_0p30_d2_2p8", "in3sigma_mXpTX_0p25_0p30_d2_3p0", "in3sigma_mXpTX_0p25_0p30_d2_3p2", "in3sigma_mXpTX_0p25_0p30_d2_3p4", "in3sigma_mXpTX_0p25_0p30_d2_3p6", "in3sigma_mXpTX_0p25_0p30_d2_3p8", "in3sigma_mXpTX_0p25_0p30_d2_4p0", "in3sigma_mXpTX_0p25_0p30_d2_4p2", "in3sigma_mXpTX_0p25_0p30_d2_4p4", "in3sigma_mXpTX_0p25_0p30_d2_4p6", "in3sigma_mXpTX_0p25_0p30_d2_4p8", "in3sigma_mXpTX_0p25_0p30_d2_5p0", 

    "in3sigma_mXpTX_0p30_0p35", "in3sigma_mXpTX_0p30_0p35_d2_0p2", "in3sigma_mXpTX_0p30_0p35_d2_0p4", "in3sigma_mXpTX_0p30_0p35_d2_0p6", "in3sigma_mXpTX_0p30_0p35_d2_0p8", "in3sigma_mXpTX_0p30_0p35_d2_1p0", "in3sigma_mXpTX_0p30_0p35_d2_1p2", "in3sigma_mXpTX_0p30_0p35_d2_1p4", "in3sigma_mXpTX_0p30_0p35_d2_1p6", "in3sigma_mXpTX_0p30_0p35_d2_1p8", "in3sigma_mXpTX_0p30_0p35_d2_2p0", "in3sigma_mXpTX_0p30_0p35_d2_2p2", "in3sigma_mXpTX_0p30_0p35_d2_2p4", "in3sigma_mXpTX_0p30_0p35_d2_2p6", "in3sigma_mXpTX_0p30_0p35_d2_2p8", "in3sigma_mXpTX_0p30_0p35_d2_3p0", "in3sigma_mXpTX_0p30_0p35_d2_3p2", "in3sigma_mXpTX_0p30_0p35_d2_3p4", "in3sigma_mXpTX_0p30_0p35_d2_3p6", "in3sigma_mXpTX_0p30_0p35_d2_3p8", "in3sigma_mXpTX_0p30_0p35_d2_4p0", "in3sigma_mXpTX_0p30_0p35_d2_4p2", "in3sigma_mXpTX_0p30_0p35_d2_4p4", "in3sigma_mXpTX_0p30_0p35_d2_4p6", "in3sigma_mXpTX_0p30_0p35_d2_4p8", "in3sigma_mXpTX_0p30_0p35_d2_5p0", 

    "in3sigma_mXpTX_0p35_0p40", "in3sigma_mXpTX_0p35_0p40_d2_0p2", "in3sigma_mXpTX_0p35_0p40_d2_0p4", "in3sigma_mXpTX_0p35_0p40_d2_0p6", "in3sigma_mXpTX_0p35_0p40_d2_0p8", "in3sigma_mXpTX_0p35_0p40_d2_1p0", "in3sigma_mXpTX_0p35_0p40_d2_1p2", "in3sigma_mXpTX_0p35_0p40_d2_1p4", "in3sigma_mXpTX_0p35_0p40_d2_1p6", "in3sigma_mXpTX_0p35_0p40_d2_1p8", "in3sigma_mXpTX_0p35_0p40_d2_2p0", "in3sigma_mXpTX_0p35_0p40_d2_2p2", "in3sigma_mXpTX_0p35_0p40_d2_2p4", "in3sigma_mXpTX_0p35_0p40_d2_2p6", "in3sigma_mXpTX_0p35_0p40_d2_2p8", "in3sigma_mXpTX_0p35_0p40_d2_3p0", "in3sigma_mXpTX_0p35_0p40_d2_3p2", "in3sigma_mXpTX_0p35_0p40_d2_3p4", "in3sigma_mXpTX_0p35_0p40_d2_3p6", "in3sigma_mXpTX_0p35_0p40_d2_3p8", "in3sigma_mXpTX_0p35_0p40_d2_4p0", "in3sigma_mXpTX_0p35_0p40_d2_4p2", "in3sigma_mXpTX_0p35_0p40_d2_4p4", "in3sigma_mXpTX_0p35_0p40_d2_4p6", "in3sigma_mXpTX_0p35_0p40_d2_4p8", "in3sigma_mXpTX_0p35_0p40_d2_5p0", 

    "in3sigma_mXpTX_0p40_0p45", "in3sigma_mXpTX_0p40_0p45_d2_0p2", "in3sigma_mXpTX_0p40_0p45_d2_0p4", "in3sigma_mXpTX_0p40_0p45_d2_0p6", "in3sigma_mXpTX_0p40_0p45_d2_0p8", "in3sigma_mXpTX_0p40_0p45_d2_1p0", "in3sigma_mXpTX_0p40_0p45_d2_1p2", "in3sigma_mXpTX_0p40_0p45_d2_1p4", "in3sigma_mXpTX_0p40_0p45_d2_1p6", "in3sigma_mXpTX_0p40_0p45_d2_1p8", "in3sigma_mXpTX_0p40_0p45_d2_2p0", "in3sigma_mXpTX_0p40_0p45_d2_2p2", "in3sigma_mXpTX_0p40_0p45_d2_2p4", "in3sigma_mXpTX_0p40_0p45_d2_2p6", "in3sigma_mXpTX_0p40_0p45_d2_2p8", "in3sigma_mXpTX_0p40_0p45_d2_3p0", "in3sigma_mXpTX_0p40_0p45_d2_3p2", "in3sigma_mXpTX_0p40_0p45_d2_3p4", "in3sigma_mXpTX_0p40_0p45_d2_3p6", "in3sigma_mXpTX_0p40_0p45_d2_3p8", "in3sigma_mXpTX_0p40_0p45_d2_4p0", "in3sigma_mXpTX_0p40_0p45_d2_4p2", "in3sigma_mXpTX_0p40_0p45_d2_4p4", "in3sigma_mXpTX_0p40_0p45_d2_4p6", "in3sigma_mXpTX_0p40_0p45_d2_4p8", "in3sigma_mXpTX_0p40_0p45_d2_5p0", 

    "in3sigma_mXpTX_0p45_0p50", "in3sigma_mXpTX_0p45_0p50_d2_0p2", "in3sigma_mXpTX_0p45_0p50_d2_0p4", "in3sigma_mXpTX_0p45_0p50_d2_0p6", "in3sigma_mXpTX_0p45_0p50_d2_0p8", "in3sigma_mXpTX_0p45_0p50_d2_1p0", "in3sigma_mXpTX_0p45_0p50_d2_1p2", "in3sigma_mXpTX_0p45_0p50_d2_1p4", "in3sigma_mXpTX_0p45_0p50_d2_1p6", "in3sigma_mXpTX_0p45_0p50_d2_1p8", "in3sigma_mXpTX_0p45_0p50_d2_2p0", "in3sigma_mXpTX_0p45_0p50_d2_2p2", "in3sigma_mXpTX_0p45_0p50_d2_2p4", "in3sigma_mXpTX_0p45_0p50_d2_2p6", "in3sigma_mXpTX_0p45_0p50_d2_2p8", "in3sigma_mXpTX_0p45_0p50_d2_3p0", "in3sigma_mXpTX_0p45_0p50_d2_3p2", "in3sigma_mXpTX_0p45_0p50_d2_3p4", "in3sigma_mXpTX_0p45_0p50_d2_3p6", "in3sigma_mXpTX_0p45_0p50_d2_3p8", "in3sigma_mXpTX_0p45_0p50_d2_4p0", "in3sigma_mXpTX_0p45_0p50_d2_4p2", "in3sigma_mXpTX_0p45_0p50_d2_4p4", "in3sigma_mXpTX_0p45_0p50_d2_4p6", "in3sigma_mXpTX_0p45_0p50_d2_4p8", "in3sigma_mXpTX_0p45_0p50_d2_5p0", 

    "in3sigma_mXpTX_0p50_0p55", "in3sigma_mXpTX_0p50_0p55_d2_0p2", "in3sigma_mXpTX_0p50_0p55_d2_0p4", "in3sigma_mXpTX_0p50_0p55_d2_0p6", "in3sigma_mXpTX_0p50_0p55_d2_0p8", "in3sigma_mXpTX_0p50_0p55_d2_1p0", "in3sigma_mXpTX_0p50_0p55_d2_1p2", "in3sigma_mXpTX_0p50_0p55_d2_1p4", "in3sigma_mXpTX_0p50_0p55_d2_1p6", "in3sigma_mXpTX_0p50_0p55_d2_1p8", "in3sigma_mXpTX_0p50_0p55_d2_2p0", "in3sigma_mXpTX_0p50_0p55_d2_2p2", "in3sigma_mXpTX_0p50_0p55_d2_2p4", "in3sigma_mXpTX_0p50_0p55_d2_2p6", "in3sigma_mXpTX_0p50_0p55_d2_2p8", "in3sigma_mXpTX_0p50_0p55_d2_3p0", "in3sigma_mXpTX_0p50_0p55_d2_3p2", "in3sigma_mXpTX_0p50_0p55_d2_3p4", "in3sigma_mXpTX_0p50_0p55_d2_3p6", "in3sigma_mXpTX_0p50_0p55_d2_3p8", "in3sigma_mXpTX_0p50_0p55_d2_4p0", "in3sigma_mXpTX_0p50_0p55_d2_4p2", "in3sigma_mXpTX_0p50_0p55_d2_4p4", "in3sigma_mXpTX_0p50_0p55_d2_4p6", "in3sigma_mXpTX_0p50_0p55_d2_4p8", "in3sigma_mXpTX_0p50_0p55_d2_5p0", 

    "in3sigma_mXpTX_0p55", "in3sigma_mXpTX_0p55_d2_0p2", "in3sigma_mXpTX_0p55_d2_0p4", "in3sigma_mXpTX_0p55_d2_0p6", "in3sigma_mXpTX_0p55_d2_0p8", "in3sigma_mXpTX_0p55_d2_1p0", "in3sigma_mXpTX_0p55_d2_1p2", "in3sigma_mXpTX_0p55_d2_1p4", "in3sigma_mXpTX_0p55_d2_1p6", "in3sigma_mXpTX_0p55_d2_1p8", "in3sigma_mXpTX_0p55_d2_2p0", "in3sigma_mXpTX_0p55_d2_2p2", "in3sigma_mXpTX_0p55_d2_2p4", "in3sigma_mXpTX_0p55_d2_2p6", "in3sigma_mXpTX_0p55_d2_2p8", "in3sigma_mXpTX_0p55_d2_3p0", "in3sigma_mXpTX_0p55_d2_3p2", "in3sigma_mXpTX_0p55_d2_3p4", "in3sigma_mXpTX_0p55_d2_3p6", "in3sigma_mXpTX_0p55_d2_3p8", "in3sigma_mXpTX_0p55_d2_4p0", "in3sigma_mXpTX_0p55_d2_4p2", "in3sigma_mXpTX_0p55_d2_4p4", "in3sigma_mXpTX_0p55_d2_4p6", "in3sigma_mXpTX_0p55_d2_4p8", "in3sigma_mXpTX_0p55_d2_5p0"
};
  */  
  vector<TString> merged_tags = {"Signal_1500", "Signal_1800",  "Signal_2000", "Signal_2300", "Signal_2500", "Signal_2600", "Signal_3000", "Signal_3400", "Signal_3600", "Signal_3800", "Signal_4000", "Signal_5000", "Signal_6000"};


  TString name_file = "file_" + variable + ".root";
  
  TString path_file = basic_path + name_file;

  TString new_path = basic_path + "merged_mY_signals/" + name_file;

  TFile *new_file = TFile::Open(new_path, "RECREATE");

  TFile *original_file = TFile::Open(path_file);

  vector<TString> names_histos;
  
  for(auto sel : selections)
    {
      for(auto tag : merged_tags)
	{
	  TString subname = sel + "_" + tag;
	  cout << "creating hadd: " << subname << endl;

	  TH1D *histo;
	  int h=0;
	  
	  original_file->cd();
	  TObject *obj;
	  TKey *key;
	  TIter next(original_file->GetListOfKeys());

	  while ((key = (TKey *) next()))
	    {
	      obj = original_file->Get(key->GetName());
	  
	      if(!obj->InheritsFrom(TH1::Class())) continue;

	      if(strstr(((std::string)key->GetName()).c_str(), subname)==NULL)continue;
	      cout << " --> adding histo " << (std::string)key->GetName() << endl;

	      if(h==0)
		{
		  histo = (TH1D*) obj->Clone();
		  histo->SetName(subname);
		}
	      else histo->Add((TH1D*)obj->Clone());

	      h++;
	    }
	  new_file->cd();
	  histo->Write();
	}
    }
  original_file->Close();
  new_file->Close();
}
 
