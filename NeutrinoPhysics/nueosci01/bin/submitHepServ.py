import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

xmax = 1.0e14
x0   = 1.0e11
dx   = 10.0

while x0 < xmax:
    x1 = x0*dx
    erange = str(x0)+','+str(x1)
    arguments.append( erange.split(',') )
    x0 = x1

print arguments

input_sandbox = ['matrix_config.xml',
                 'matrix_config.dtd',
                 'model_config.xml',
                 'model_config.dtd',
                 'paper01.exe',
                 '../../nueoscilib/lib/libnueosci.so']

script = './local.csh'
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
myjob.name = 'nueosc.cmssw'
myjob.splitter = sp
myjob.merger = mg

myjob.outputsandbox = [output]

for input in input_sandbox:
    myjob.inputsandbox.append( File ( input ) )

myjob.submit()

print "job submission done."

