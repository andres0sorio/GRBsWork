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

prefix = './root_files/'

if not os.path.isdir(prefix):
	os.makedirs( prefix )

job_range  = options.range.split(',')

first_job = int(job_range[0])
last_job = int(job_range[1])

logfile = open('getOutput_' + job_range[0] + '_' + job_range[1] + '.log', 'w')

outfile = ['detection','.root']

study = "RvsAlfa"

for job in jobs.select( first_job, last_job ):

	print ">get output: ", job.id

	model = job.name.split('.')[1]

	arguments = job.splitter.args
	max_args = len(arguments)
	config = arguments[max_args-1][0].split('.')[0].split('_')[-1]

	print model,config,study

	path = job.outputdir
	output = job.outputsandbox[0]
	source = path + output

	all = [outfile[0], model, config, study]
	       
        filename = '_'.join(all) + outfile[1]

	if not os.path.isdir(prefix + model ):
		os.makedirs( prefix + model )
	
	target = prefix + model + '/' + filename	
	cmd = 'cp -v ' + source + ' ' + target
	logfile.write(cmd + '\n')
	os.system(cmd)
	print cmd

logfile.close()

print "Done"

