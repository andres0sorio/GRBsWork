import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

use_file = 2

use_setI = 1

model = 'ZeroPt'

xmlfi = ''

if   use_file == 1:
    xmlfi = 'model_config_SetI.xml'  #march 21 - Sarira Set I parameters
elif use_file == 2:
    xmlfi = 'model_config_SetII.xml' #march 21 - Sarira Set II parameters
elif use_file == 3:
    xmlfi = 'model_config_Earth.xml'
    model = 'EarthB'
elif use_file == 4:
    xmlfi = 'model_config_Ohlsson.xml'
    model = 'EarthB'    
else:
    print 'No configuration available'
    sys.exit()

xmax  = 1.0e14
x0    = 1.0e11
dx    = 10.0

steps = '1'

#Set I - march 21
if xmlfi == 'model_config_SetI.xml':
    angles = '33.8,8.8,45.0'
    dmasses = '0.0032,0.00008'
    steps = '1,2,3'

#Set II - march 21
if xmlfi == 'model_config_SetII.xml':
    angles = '33.8,12.0,45.0'
    dmasses = '0.0032,0.00008'
    steps = '1,2,3'

if model == 'EarthB':
    
    xmax  = 1.0e10
    x0    = 1.0e9
    dx    = 1.0e9

    dmasses = '0.0032,0.00008'
    
    if use_setI   == 1:
        angles = '33.8,8.8,45.0'  #Set I
    elif use_setI == 2:
        angles = '33.8,12.0,45.0' #Set II
    elif use_setI == 3:
        angles = '45.0,5.0,45.0'  #Ohlsson
        dmasses = '0.0032,0.0'
    else:
        print 'No configuration available'
    
    steps = '1'

while x0 < xmax:

    if model == 'EarthB':
        x1 = x0 + dx
    else:
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

