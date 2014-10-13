import sys, os, stat, shutil
#----------------------------------------------------------
# submitting to HEP Server / Ganga / R vs Sin2Q13 variation
# All star models
#----------------------------------------------------------

datafile = 'all_datasets_q13_models_SetI.dat'

config = ''
datasets = []
arguments = []

infile = open(datafile,'r')

for line in infile:
    datasets.append(line[:-1])
infile.close()

if len(datasets) != 0:
    config = datasets[0].split('_')[2]

alfa_var = ['1.8','2.0','2.2']

for alfa in alfa_var:
    values = []
    for data in datasets:
        values = []
        values.append( data )
        values.append( alfa )
        arguments.append( values )

input_sandbox = ['config.xml',
                 'config.dtd',
 		 'matrix_config.dtd',
		 'matrix_config.xml',
                 'paper02.exe',
                 '../../nueoscilib/lib/libnueosci.so',
                 '../data/XSec_neut.dat',
                 '../data/XSec_anti.dat',
                 '../data/pshadow-at-180.dat']

for data in datasets:
    input_sandbox.append( data )

script = './local-RvsQ13-Models.csh'

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
myjob.name = 'Paper2.RvsQ13.' + config
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

print "job submission done."

