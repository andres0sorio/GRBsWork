#!/bin/tcsh -f

setenv LOCAL_DCP $1
setenv LOCAL_MIXPARS $2
setenv LOCAL_CONFIG $3

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv PAPER_DATA ${PWD}

setenv CMD  "./paper02.exe --steps=4 --dCP="$LOCAL_DCP" --neuosc="$LOCAL_MIXPARS" --config="$LOCAL_CONFIG

echo INFO:  launching command: $CMD

$CMD
echo DONE
