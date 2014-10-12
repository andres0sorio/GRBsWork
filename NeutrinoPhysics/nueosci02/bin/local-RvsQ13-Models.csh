#! /bin/tcsh -f

setenv DATA $1
setenv ALFA $2

set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}

setenv PAPER_DATA ${PWD}

setenv CMD  "./paper02.exe --steps 6 --input "$DATA" --alfa "$ALFA

echo INFO:  launching command: $CMD

$CMD
echo DONE

