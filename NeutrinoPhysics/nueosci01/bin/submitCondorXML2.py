import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

use_setI = 3

model = 'EarthB'

xmlfi = 'model_config_Earth.xml'

xmax  = 1.0e14
x0    = 1.0e11
dx    = 10.0

steps = '1'

angles = '33.83,11.39,45.0'
dmasses = '0.0032,0.00008'
    
if model == 'EarthB':
    
    dmasses = '0.0032,0.00008'
    
    if use_setI   == 1:
        angles = '33.8,8.8,45.0'      # Set I
    elif use_setI == 2:
        angles = '33.8,12.0,45.0'     # Set II
    elif use_setI == 3:
        angles = '45.0,5.0,45.0'      # Ohlsson
        dmasses = '0.0032,0.0'
    elif use_setI == 4:
        angles = '33.83,11.39,45.0'   # Mena
        dmasses = '0.0024,0.00008'
    else:
        print 'No configuration available'
        sys.exit(0)
    
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
                  'paper01.exe']

script = './condor_xml0.csh'
output = 'output.root'
#--------------------------------------------------------

app = Executable( exe=File( script ) )

sp = ArgSplitter()
sp.args = arguments

mg = RootMerger()
mg.files=[output]
mg.ignorefailed = True

#Job declaration and initialisation:
myjob = Job( application = app, backend = 'Condor' )
myjob.name = 'nueosc.cmssw'
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

myjob.submit()

print "job submission done."

