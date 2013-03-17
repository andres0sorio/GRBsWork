import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

model = 'ModelA'
steps = '1'

input_sandbox = ['matrix_config.xml',
                 'matrix_config.dtd',
                 'model_config.dtd',
                 'paper01.exe']

script = './condor_var.csh'
output = 'output.root'
#--------------------------------------------------------

var_file_prefix = 'model_config_var_'

for k in range(0,21):
    varxml = var_file_prefix + str(k) + '.xml'

    xmax  = 1.0e14
    x0    = 1.0e11
    dx    = 10.0
    arguments = []
    
    while x0 < xmax:
        x1 = x0*dx
        erange = str(x0)+','+str(x1)
        values = erange.split(',')
        values.append( model )
        values.append( varxml )
        values.append( steps )
        arguments.append( values )
        x0 = x1

    print arguments

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

    myjob.inputsandbox.append( File ( varxml ) )
    
    myjob.submit()

print "job submission done."

