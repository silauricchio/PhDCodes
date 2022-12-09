
list='data'

file_path="/afs/cern.ch/user/s/sauricch/work/YXH/ReaderOutput/21Oct20/SkimmedTrees/mc16a/"
histo_path='/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/Normalization_map/FULLSTAT/file_Ratio2D_fatjetH.root'

histo_name='Ratio_CR2_boosted_WP60_SB2_boosted_WP60_pTH_etaH_Data' 
var='trackjet1_pt'


for f in $list
do 
    echo 'RUNNING REWEIGHT MACRO FOR ' $f 

    root -l -q  "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/util/Reweight/Reweight1D.C(0,\"$file_path\", \"$f\", \"$histo_path\", \"$histo_name\", \"$var\")" 

done


