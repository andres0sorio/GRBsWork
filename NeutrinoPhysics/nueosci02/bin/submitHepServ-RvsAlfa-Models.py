import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs alpha variation
# All star models
#--------------------------------------------------------

datasets = []
arguments = []

infile = open('all_datasets_models.dat','r')

for line in infile:
    datasets.append(line[:-1])
infile.close()

for data in datasets:
        values = []
        values.append( data )
        arguments.append( values )

input_sandbox = ['config.xml',
                 'config.dtd',
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
myjob.name = 'nueosc.StdPicture'
myjob.splitter = sp
myjob.merger = mg

# myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

print "job submission done."

