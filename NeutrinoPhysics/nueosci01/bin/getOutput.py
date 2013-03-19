#!/usr/bin/python

import sys, os, stat, shutil
from optparse import OptionParser
#--------------------------------------------------------
parser = OptionParser()

parser.add_option("-r", type = "string", dest="range",
                  help="range for this parameter" )

(options, args) = parser.parse_args()

if options.range is None:
        parser.error("please give 5 cents, try -h")
#--------------------------------------------------------

job_range  = options.range.split(',')

first_job = int(job_range[0])
last_job = int(job_range[1])

outfile = ['output','.root']

k = 0

for job in jobs.select( first_job, last_job ):
	print ">get output: ", job.id
	path = job.outputdir
	output = job.outputsandbox[0]
	source = path + output
	target = './root_files/' + outfile[0] + '_' + str(k) + '_' + outfile[1]
	cmd = 'cp -v ' + source + ' ' + target
	os.system(cmd)
	k+=1
	
print "Total files transfered: ", (k-1)

