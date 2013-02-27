import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

method = "--method 1 "
opt1   = "--par1 1.e-4 "
opt2   = "--par2 1.e-3 "
opt3   = "--subint 10000 "
opt4   = "--point "

arguments = []

x0 = 1000.0
dx = 10

#--------------------------------------------------------

base_script = "runincmssw.sh"
tmp_script = "tmp_exec_script.sh"
shutil.copy( base_script, tmp_script )
exec_script = open( tmp_script, 'a')
cmd = './validate.exe ' + method + opt1 + opt2 + opt3 + opt4 + "$1"
exec_script.writelines( cmd + '\n\n')
exec_script.close()
os.system( 'chmod +x ' + tmp_script )

#--------------------------------------------------------

for i in range(0,8):
    options = str(x0)
    arguments.append( options.split() )
    x0 *= dx

#--------------------------------------------------------

app = Executable()
app.exe = File( tmp_script ) 
app.args = []

sp = ArgSplitter()
sp.args = arguments

mg = TextMerger()
mg.files=['stdout']
mg.ignorefailed = True

#Job declarion and initialisation:
myjob = Job( application = app, backend = 'Local' )
myjob.name = 'validate.M1.cmssw'
myjob.splitter = sp
myjob.merger = mg
myjob.inputsandbox = [ File('validate.exe' ),
                       File('config.xml'),
                       File('config.dtd'),
                       File('XSec_neut.dat'),
                       File('XSec_anti.dat') ]

myjob.submit()

os.remove( tmp_script )

print "job submission done."

