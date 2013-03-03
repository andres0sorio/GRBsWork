#! /bin/tcsh -f

setenv MYDIR /uscms_data/d2/aosorio/scratch0/CMSSW_5_3_4/run
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod
pushd $MYDIR
cmsenv
setenv GSLPATH `scram tool tag gsl GSL_BASE`
setenv BOOSTPATH `scram tool tag boost BOOST_BASE`

popd

setenv EMIN $1
setenv EMAX $2

setenv CMD  "./paper01.exe --model ModelA --prob Pee,Pem,Pet --erange "$EMIN","$EMAX

echo INFO:  launching command: $CMD

$CMD
echo DONE
