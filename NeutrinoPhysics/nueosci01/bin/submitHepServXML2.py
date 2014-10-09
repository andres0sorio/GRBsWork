import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

use_file = 1
use_mass = 1

dmass_vars = ['0.0014','0.006','0.0032']

dmass32 = dmass_vars[use_mass-1]

all_models = 'ModelA,ModelB,ModelC'

xmlfi = ''

if   use_file == 1:
    xmlfi = 'model_config_SetI.xml'  #march 21 - Sarira Set I parameters
elif use_file == 2:
    xmlfi = 'model_config_SetII.xml' #march 21 - Sarira Set II parameters
else:
    print 'No configuration available'
    sys.exit()

xmax  = 1.0e17
x0    = 1.0e11
dx    = 10.0

steps = '1'

#Set I - march 21
if xmlfi == 'model_config_SetI.xml':
    angles = '33.8,8.8,45.0'
    dmasses = dmass32 + ',' + '0.00008'
    steps = '1,2,3'

#Set II - march 21
if xmlfi == 'model_config_SetII.xml':
    angles = '33.8,12.0,45.0'
    dmasses = dmass32 + ',' + '0.00008'
    steps = '1,2,3'

# Submit for all three Models

script = './local_xml2.csh'

output = 'output.root'

models = all_models.split(',')

for model in models:

    x0  = 1.0e11
    arguments = []
        
    while x0 < xmax:

        x1 = x0*dx
        erange = str(x0)+','+str(x1)
        values = erange.split(',')
        values.append( model )
        values.append( steps )
        values.append( xmlfi )
        values.append( angles )
        values.append( dmasses )
        arguments.append( values )
        x0 = x1

    print arguments

    input_sandbox = [ xmlfi,
                      'matrix_config.xml',
                      'matrix_config.dtd',
                      'model_config.dtd',
                      'paper01.exe',
                      '../../nueoscilib/lib/libnueosci.so']

    #--------------------------------------------------------

    app = Executable( exe=File( script ) )
    sp = ArgSplitter()
    sp.args = arguments

    mg = RootMerger()
    mg.files=[output]
    mg.ignorefailed = True

    #Job declaration and initialisation:
    myjob = Job( application = app, backend = 'Local' )
    myjob.name = 'nueosc.'+model
    myjob.splitter = sp
    myjob.merger = mg

    myjob.outputsandbox = [output]

    for input in input_sandbox:
        myjob.inputsandbox.append( File ( input ) )

    #myjob.submit()

    print myjob.name + " submission done."

