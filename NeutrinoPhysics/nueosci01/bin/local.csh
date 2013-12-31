#! /bin/tcsh -f

setenv EMIN $1
setenv EMAX $2






set CURRENTDIR=${PWD}

setenv LD_LIBRARY_PATH ${CURRENTDIR}:${LD_LIBRARY_PATH}





setenv CMD  "./paper01.exe --model ModelA --prob Pee,Pem,Pet --erange "$EMIN","$EMAX


echo INFO:  launching command: $CMD

$CMD
echo DONE
