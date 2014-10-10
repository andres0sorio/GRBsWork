#! /bin/tcsh -f

## AO: Oct - 2014: script exclusive for generating data for paper02

setenv EMIN  $1
setenv EMAX  $2
setenv MODEL $3
setenv STEP  $4
setenv XMLF  $5
setenv ANGS  $6
setenv DM2S  $7
setenv DCP   $8

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv CMD  "./paper01.exe --model "$MODEL" --prob Pee,aPee --erange "$EMIN","$EMAX" --step "$STEP" --modfile "$XMLF" --angles "$ANGS" --dmass2 "$DM2S" --dCP "$DCP

echo INFO:  launching command: $CMD

$CMD
echo DONE
