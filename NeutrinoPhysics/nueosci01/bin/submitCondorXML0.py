import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

#xmlfi = 'model_config_OLD.xml' #march 21
#xmlfi = 'model_config_SetI.xml' #march 21 - Sarira SetI parameters

xmlfi = 'model_config_SetII.xml' #march 21 - Sarira SetII parameters

model = 'ModelA'

xmax  = 1.0e14
x0    = 1.0e11
dx    = 10.0

steps = '1'

#ONLY OLD Parameters - march 21
#angles = '33.8,8.0,45.0'
#dmasses = '0.0032,0.00008'

#Set I - march 21
#angles = '33.8,8.8,45.0'
#dmasses = '0.0032,0.00008'

#Set II - march 21
angles = '33.8,12.0,45.0'
dmasses = '0.0032,0.00008'

if model == 'EarthB':
    xmax  = 1.0e10
    x0    = 1.0e9
    dx    = 1.0e9

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

