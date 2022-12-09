
list='data'

file_path="/afs/cern.ch/user/s/sauricch/work/YXH/ReaderOutput/21Oct20/SkimmedTrees/mc16e/"
histo_path='/afs/cern.ch/user/s/sauricch/work/YXH/plotter/run/Normalization_map/FULLSTAT/file_Ratio2D_fatjetH.root'
histo_name='Ratio_CR2_boosted_WP60_SB2_boosted_WP60_pTH_etaH_Data'

#map1
varX='etaH' #on x axis
varY='pTH' #on y axis

for f in $list
do 
    echo 'RUNNING REWEIGHT MACRO FOR ' $f 
 
    root -l -q  "/afs/cern.ch/user/s/sauricch/work/YXH/plotter/source/util/Reweight/Reweight2D_1map.C(0,\"$file_path\", \"$f\", \"$histo_path\", \"$histo_name\", \"$varX\", \"$varY\")"
done


