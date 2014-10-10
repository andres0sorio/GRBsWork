#! /bin/tcsh -f

setenv MIXPARS $1
setenv DM2S    $2
setenv DCP     $3

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv PAPER_DATA ${PWD}

setenv CMD  "./paper02.exe --steps 1 --neuosc "$MIXPARS" --dmass2 "$DM2S" --dCP "$DCP

echo INFO:  launching command: $CMD

$CMD
echo DONE
