#! /bin/tcsh -f

setenv DCP $1
setenv MIXPARS $2

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv CMD  "./paper02.exe --steps 1 --dCP "$DCP" --neuosc "$MIXPARS

echo INFO:  launching command: $CMD

$CMD
echo DONE
