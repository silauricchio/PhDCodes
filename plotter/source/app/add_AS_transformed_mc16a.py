from ROOT import *
import numpy as np
import glob

ROOT.EnableImplicitMT()
source = '/eos/atlas/atlascerngroupdisk/phys-hdbs/dbl/YXH/superNtuples_v1.9/Skimmed/mc16a/' 
save_dir = '/eos/user/s/sauricch/ReaderOutput/SuperNtuples_v1.9_skimmed/mc16a/'

#--------------------------
def skimming(df):
    return df.Filter("HCand_XbbScore > (-2)")

#---------------------------
def add_variables(df, jet1_factor, jet2_factor):
    """
    Adds transformed anomaly score and series of other useful variables
    """

    df = df.Define("jet1_AnomScoreTransf", "1-(jet1_AnomScore*{jet1_factor})".format(jet1_factor = jet1_factor))
    df = df.Define("jet2_AnomScoreTransf", "1-(jet2_AnomScore*{jet2_factor})".format(jet2_factor = jet2_factor))
    df = df.Define("XCand_AnomScore", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? jet2_AnomScoreTransf : jet1_AnomScoreTransf") ###These!
    df = df.Define("HCand_AnomScore", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? jet1_AnomScoreTransf : jet2_AnomScoreTransf")
    #df = df.Define("XTau32", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet2_Tau32 : Jet1_Tau32")
    #df = df.Define("HTau32", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet1_Tau32 : Jet2_Tau32")
    #df = df.Define("XSplit12", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet2_Split12 : Jet1_Split12")
    #df = df.Define("HSplit12", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet1_Split12 : Jet2_Split12")
    #df = df.Define("XSplit23", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet2_Split23 : Jet1_Split23")
    #df = df.Define("HSplit23", "(Jet1_XbbScore_V3 > Jet2_XbbScore_V3) ? Jet1_Split23 : Jet2_Split23")

    return(df)

#---------------------------
def get_factors(bckg_df):
    """
    Get jet factors (mean of the background anomaly score) in transforming each jet's anomaly score
    """
    jet1_factor = 0.5/bckg_df.Filter("jet1_AnomScore < 50").Mean("jet1_AnomScore").GetValue()
    jet2_factor = 0.5/bckg_df.Filter("jet2_AnomScore < 50").Mean("jet2_AnomScore").GetValue()
    return jet1_factor, jet2_factor

#===========================
def main():
    print("Making data dataframes...")
    #data = ROOT.RDataFrame("Nominal", glob.glob(source+"*merged_top2_00*.root", recursive=True)) #Gabriel
    data = ROOT.RDataFrame("Nominal", source + 'data_AS_NNscores_Bootstrap.root')
    print("Getting jet factors...")
    jf1, jf2 = get_factors(data)

    print("Adding new variables and saving...")
    f_name = 'data_XHAS_NNscores_Bootstrap.root'
    print(f_name)

    df = add_variables(data, jf1, jf2)
    #df = skimming(df) #Gabriel

    df.Snapshot("Nominal", save_dir + f_name)

#---
if __name__ == "__main__":
    main()
