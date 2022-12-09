
list='data'

file_path="/afs/cern.ch/user/s/sauricch/work/YXH/ReaderOutput/21Oct20/SkimmedTrees/mc16a/"
histo_path='/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/Normalization_map/FULLSTAT/file_Ratio2D_fatjetH.root'
histo_name1='Ratio_CR2_boosted_2trkJets_SB2_boosted_2trkJets_trackjet1_pt_trackjet1_eta_Data'
histo_name2='Ratio_CR2_boosted_2trkJets_SB2_boosted_2trkJets_trackjet2_pt_trackjet2_eta_Data'

#map1
var1X='trackjet1_eta' #on x axis
var1Y='trackjet1_pt' #on y axis
#map2
var2X='trackjet2_eta' #on x axis                                                                                                                                                                          
var2Y='trackjet2_pt' #on y axis

for f in $list
do 
    echo 'RUNNING REWEIGHT MACRO FOR ' $f 
 
    root -l -q  "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/util/Reweight/Reweight2D.C(0,\"$file_path\", \"$f\", \"$histo_path\", \"$histo_name1\", \"$histo_name2\", \"$var1X\", \"$var1Y\", \"$var2X\", \"$var2Y\")"
done


