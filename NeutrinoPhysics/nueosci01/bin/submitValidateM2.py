import sys, os
#--------------------------------------------------------
#--------------------------------------------------------

method = "--method 2 "
opt1   = "--par1 1e-4 "
opt2   = "--subint 1000000 "
opt3   = "--point "

arguments = []

x0 = 100000.0
dx = 10

for i in range(0,6):
    options = method + opt1 + opt2 + opt3 + str(x0)
    arguments.append( options.split() )
    x0 *= dx

#--------------------------------------------------------

app = Executable()
app.exe = File( 'validate.exe' )
app.args = []

sp = ArgSplitter()
sp.args = arguments

mg = TextMerger()
mg.files=['stdout']
mg.ignorefailed = True

#Job declarion and initialisation:
myjob = Job( application = app, backend = 'Local' )
myjob.name = 'validate.M2'
myjob.splitter = sp
myjob.merger = mg
myjob.inputsandbox = [ File('config.xml'), File('config.dtd'), File('XSec_neut.dat'), File('XSec_anti.dat') ]

myjob.submit()

print "job submission done."

