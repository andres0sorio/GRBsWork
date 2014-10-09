import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs alpha variation
# StdPicture
#--------------------------------------------------------

arguments = []

## dCP = 0
deltavalues = str(0)+','+str("matrix_config_SetI.xml")
arguments.append( deltavalues.split(',') )

## dCP = 180

deltavalues = str(180)+','+str("matrix_config_SetI.xml")
arguments.append( deltavalues.split(',') )

print arguments

input_sandbox = ['matrix_config_SetI.xml',
                 'matrix_config.dtd',
                 'model_config_Earth.xml',
                 'model_config.dtd',
                 'config.xml',
                 'config.dtd',
                 'paper02.exe',
                 '../../nueoscilib/lib/libnueosci.so',
                 '../data/XSec_neut.dat',
                 '../data/XSec_anti.dat',
                 '../data/pshadow-at-180.dat']

script = './local.csh'
output = 'detection.root'
#--------------------------------------------------------

app = Executable( exe=File( script ) )

sp = ArgSplitter()
sp.args = arguments

mg = RootMerger()
mg.files=[output]
mg.ignorefailed = True

#Job declaration and initialisation:
myjob = Job( application = app, backend = 'Local' )
myjob.name = 'nueosc.paper02'
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

myjob.submit()

print "job submission done."

