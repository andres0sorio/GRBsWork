import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs alpha variation
# StdPicture
#--------------------------------------------------------

dcp = ''
xmlfi = ''
arguments = []

use_file = 1 

if   use_file == 1:
    xmlfi = 'matrix_config_SetI.xml'
elif use_file == 2:
    xmlfi = 'matrix_config_SetII.xml'
else:
    print 'No configuration available'
    sys.exit()

dcp_vars = ['0','180']

dmass_vars = ['0.0014','0.006','0.0032']

config = xmlfi.split('_')[2]

for dcp in dcp_vars:
    for dmass in dmass_vars:
        dmasses = dmass + ',' + '0.00008'
        values = []
        values.append( xmlfi )
        values.append( dmasses )
        values.append( dcp )
        arguments.append( values )

input_sandbox = ['matrix_config_SetI.xml',
                 'matrix_config_SetII.xml',
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

script = './local-RvsAlfa.csh'
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
myjob.name = 'RvsAlfa.StdPicture.' + config
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

print "job submission done."

