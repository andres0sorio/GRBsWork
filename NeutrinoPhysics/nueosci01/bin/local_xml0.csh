#! /bin/tcsh -f

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
