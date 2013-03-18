#!/usr/bin/python

import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

arguments = []

model = 'ModelA'
steps = '1'

input_sandbox = ['matrix_config.xml',
                 'matrix_config.dtd',
                 'model_config.dtd',
                 'model_config.xml',
                 'paper01.exe']

script = './condor_var1.csh'
output = 'output.root'
#--------------------------------------------------------

dq = .25
theta_13 = 8.0

while theta_13 <= 13.0:

    xmax  = 1.0e14
    x0    = 1.0e11
    dx    = 10.0
    arguments = []
    
    angles= '33.83,'+ str(theta_13) + ',45.0'
    
    while x0 < xmax:
        x1 = x0*dx
        erange = str(x0)+','+str(x1)
        values = erange.split(',')
        values.append( model )
        values.append( steps )
        values.append( angles )
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

    myjob.submit()

    theta_13 += dq

print "job submission done."

