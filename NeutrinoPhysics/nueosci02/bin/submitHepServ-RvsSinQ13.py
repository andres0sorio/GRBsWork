import sys, os, stat, shutil
#--------------------------------------------------------
# submitting to HEP Server / Ganga / R vs Sin(theta_13)
# StdPicture
#--------------------------------------------------------

use_file = 1
dcp_vars = ['0','180']

#--------------------------------------------------------

cfg    = 'config.xml'
xmlfi  = 'matrix_config.xml'

if   use_file == 1:
    xmlfi = 'matrix_config_SetI.xml'
elif use_file == 2:
    xmlfi = 'matrix_config_SetII.xml'
elif use_file == 3:
    xmlfi = 'matrix_config_Esmaili.xml'
    cfg   = 'config_Esmaili.xml'
else:
    print 'No configuration available'
    sys.exit()

#--------------------------------------------------------

arguments = []

for dcp in dcp_vars:
    values = ''
    values = dcp + ',' + xmlfi + ',' + cfg
    arguments.append( values.split(',') )

config = xmlfi.split('_')[2].split('.')[0]

input_sandbox = ['matrix_config.dtd',
                 'config.dtd',
                 xmlfi,
		 cfg, 
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
myjob.name = 'Paper02.RvsQ13.Std.' + config
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

print "job submission done."

