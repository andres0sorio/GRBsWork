import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs Sin(theta_13)
# StdPicture
#--------------------------------------------------------

arguments = []

config='config_Esmaili.xml'
matrix_config='matrix_config_Esmaili.xml'

## dCP = 0
deltavalues = str(0) + ',' + matrix_config + ',' + config
arguments.append( deltavalues.split(',') )

## dCP = 180
deltavalues = str(180) + ',' + matrix_config + ',' + config
arguments.append( deltavalues.split(',') )

print arguments

input_sandbox = ['matrix_config.dtd',
                 'config.dtd',
                 matrix_config,
		 config, 
                 'paper02.exe',
                 '../../nueoscilib/lib/libnueosci.so',
                 '../data/XSec_neut.dat',
                 '../data/XSec_anti.dat',
                 '../data/pshadow-at-180.dat']

script = './local-RvsSinQ13.csh'
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

