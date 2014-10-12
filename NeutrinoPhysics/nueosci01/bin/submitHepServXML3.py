import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

use_file = 1

dcp_vars   = ['0','180']
theta_vars = ['0.0','5.74','8.13','9.97','11.54','12.92','13.56']
all_models = ['ModelA','ModelB','ModelC']

xmlfi = ''

if   use_file == 1:
    xmlfi = 'model_config_SetI.xml'  #march 21 - Sarira Set I parameters
elif use_file == 2:
    xmlfi = 'model_config_SetII.xml' #march 21 - Sarira Set II parameters
else:
    print 'No configuration available'
    sys.exit()

#--------------------------------------------------------

xmax   = 1.0e17
x0     = 1.0e11
dx     = 10.0

steps  = '1,2,3'
dmass  = '0.0032,0.00008'
script = './local_xml3.csh'
output = 'output.root'

#--------------------------------------------------------

for model in all_models:

    for dcp in dcp_vars:

        for theta13 in theta_vars:

            x0  = 1.0e11
            arguments = []
            angles = '33.8,' + theta13 + ',45.0'
            
            while x0 < xmax:

                x1 = x0*dx
                erange = str(x0)+','+str(x1)
                values = erange.split(',')
                values.append( model )
                values.append( steps )
                values.append( xmlfi )
                values.append( angles )
                values.append( dmass )
                values.append( dcp )
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
            myjob.name = 'Paper2.' + model
            myjob.splitter = sp
            myjob.merger = mg

            myjob.outputsandbox = [output]

            for input in input_sandbox:
                myjob.inputsandbox.append( File ( input ) )

            print myjob.name + " submission done."

