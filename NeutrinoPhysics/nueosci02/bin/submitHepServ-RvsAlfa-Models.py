import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs alpha variation
# All star models
#--------------------------------------------------------

datafile = 'all_datasets_models_SetI.dat'

config = ''
datasets = []
arguments = []

infile = open(datafile,'r')

for line in infile:
    datasets.append(line[:-1])
infile.close()

if len(datasets) != 0:
    config = datasets[0].split('_')[2]



for data in datasets:
        values = []
        inputdata = data.split('/')[2]
        values.append( inputdata )
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

script = './local-RvsAlfa-Models.csh'

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
myjob.name = 'Paper2.RvsAlfa.' + config
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

print "job submission done."

