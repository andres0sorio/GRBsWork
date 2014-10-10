#! /bin/tcsh -f

setenv DATA $1

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv PAPER_DATA ${PWD}

setenv CMD  "./paper02.exe --steps 2 --input "$DATA"

echo INFO:  launching command: $CMD

$CMD
echo DONE
