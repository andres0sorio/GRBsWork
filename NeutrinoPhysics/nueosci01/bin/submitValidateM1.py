import sys, os
#--------------------------------------------------------
#--------------------------------------------------------

method = "--method 1 "
opt1   = "--par1 1.e-3 "
opt2   = "--par2 1.e-1 "
opt3   = "--subint 1000000 "
opt4   = "--point "

arguments = []

x0 = 1000.0
dx = 10

for i in range(0,6):
    options = method + opt1 + opt2 + opt3 + opt4 + str(x0)
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
myjob.name = 'validate.M1'
myjob.splitter = sp
myjob.merger = mg
myjob.inputsandbox = [ File('config.xml'), File('config.dtd'), File('XSec_neut.dat'), File('XSec_anti.dat') ]

myjob.submit()

print "job submission done."

