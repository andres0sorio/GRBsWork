import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

#DM32 = '0.0014'
#DM32 = '0.0060'

DM32 = '0.0032'

use_file = 1

model = 'ModelA'

xmlfi = ''

if   use_file == 1:
    xmlfi = 'model_config_SetI.xml'  #march 21 - Sarira Set I parameters
elif use_file == 2:
    xmlfi = 'model_config_SetII.xml' #march 21 - Sarira Set II parameters
else:
    print 'No configuration available'
    sys.exit()

xmax  = 1.0e12
x0    = 1.0e11
dx    = 10.0

steps = '1'

#Set I - march 21
if xmlfi == 'model_config_SetI.xml':
    angles = '33.8,8.8,45.0'
    dmasses = DM32 + ',0.00008'
    steps = '1,2,3'

#Set II - march 21
if xmlfi == 'model_config_SetII.xml':
    angles = '33.8,12.0,45.0'
    dmasses = DM32 + ',0.00008'
    steps = '1,2,3'

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

script = './local_xml0.csh'
output = 'output.root'
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

myjob.submit()

print "job submission done."

