import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

model = 'ModelC'
xmax  = 1.0e14
x0    = 1.0e11
dx    = 10.0
steps = '1,2,3'

while x0 < xmax:
    x1 = x0*dx
    erange = str(x0)+','+str(x1)
    values = erange.split(',')
    values.append( model )
    values.append( steps )
    arguments.append( values )
    x0 = x1

print arguments

input_sandbox = ['matrix_config.xml',
                 'matrix_config.dtd',
                 'model_config.xml',
                 'model_config.dtd',
                 'paper01.exe']

script = './condor.csh'
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

