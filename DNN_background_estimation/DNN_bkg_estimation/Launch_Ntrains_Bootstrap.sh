#!/bin/bash --posix
for i in {1..101}
do
    python /afs/cern.ch/user/s/sauricch/work/MachineLearning_HEP/YXH/DirectImportanceEstimation/codes/NN_train_test_FULLSTAT_Bootstrap.py $i
done
