#!/bin/bash
#..................................................
#
#.. Andres Osorio - aosorio@uniandes.edu.co        
#
#..................................................
# 1. setup CMSSW

#export VO_CMS_SW_DIR=/opt/exp_soft/CMS
export SCRAM_ARCH=slc5_amd64_gcc434
source $VO_CMS_SW_DIR/cmsset_default.sh
scram project CMSSW CMSSW_4_2_5
cd CMSSW_4_2_5
eval `scram runtime -sh`
cd ../

#..................................................
# 2. RUN

