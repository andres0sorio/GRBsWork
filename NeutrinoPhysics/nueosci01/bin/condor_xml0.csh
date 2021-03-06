#! /bin/tcsh -f

setenv MYDIR /uscms_data/d2/aosorio/scratch2/CMSSW_5_3_5/run
unsetenv CMS_PATH
source /uscmst1/prod/sw/cms/cshrc prod
pushd $MYDIR
cmsenv
setenv GSLPATH `scram tool tag gsl GSL_BASE`
setenv BOOSTPATH `scram tool tag boost BOOST_BASE`

popd

setenv EMIN  $1
setenv EMAX  $2
setenv MODEL $3
setenv STEP  $4
setenv XMLF  $5
setenv ANGS  $6
setenv DM2S  $7

setenv CMD  "./paper01.exe --model "$MODEL" --prob Pee,aPee,Pem,aPem,Pet,aPet,Pmt,aPmt --erange "$EMIN","$EMAX" --step "$STEP" --modfile "$XMLF" --angles "$ANGS" --dmass2 "$DM2S

echo INFO:  launching command: $CMD

$CMD
echo DONE
