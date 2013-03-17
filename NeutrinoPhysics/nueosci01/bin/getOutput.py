import sys, os, stat, shutil
#--------------------------------------------------------
#--------------------------------------------------------

first_job = 184

outfile = ['output','.root']

for k in range(0,21):
    
    job = jobs( first_job + k )
    print ">get output: ", job.id
    path = job.outputdir
    output = job.outputsandbox[0]
    source = path + output
    target = './root_files/' + outfile[0] + '_' + str(k) + '_' + outfile[1]
    cmd = 'cp -v ' + source + ' ' + target
    os.system(cmd)

print "job submission done."

