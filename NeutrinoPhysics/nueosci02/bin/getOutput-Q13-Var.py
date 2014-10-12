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

variations = {'0.0'   :'Q13-1',
	      '5.74'  :'Q13-2',
	      '8.13'  :'Q13-3',
	      '9.97'  :'Q13-4',
	      '11.54' :'Q13-5',
	      '12.92' :'Q13-6',
	      '13.56' :'Q13-7' }

logfile = open('getOutput_' + job_range[0] + '_' + job_range[1] + '.log', 'w')

outfile = ['output','.root']

for job in jobs.select( first_job, last_job ):

	print ">get output: ", job.id

	arguments = job.splitter.args
	max_args = len(arguments)
	model = arguments[max_args-1][2]
	max_energy = arguments[max_args-1][1].replace('+','')
	config = arguments[max_args-1][4].split('.')[0].split('_')[-1]
	phase = 'dCP' + arguments[max_args-1][7]
        theta13 = arguments[max_args-1][5].split(',')[1]
	var = variations[theta13]
	
	print model,config,max_energy,var,phase

	path = job.outputdir
	output = job.outputsandbox[0]
	source = path + output

	all = [outfile[0], model, config, max_energy, var, phase]
	       
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

