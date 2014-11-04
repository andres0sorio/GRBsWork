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
first_job  = int(job_range[0])
last_job   = int(job_range[1])

variations = {'0.0'   :'Q13-1',
	      '5.74'  :'Q13-2',
	      '8.13'  :'Q13-3',
	      '9.97'  :'Q13-4',
	      '11.54' :'Q13-5',
	      '12.92' :'Q13-6',
	      '13.56' :'Q13-7' }

outfile = open("Summary-Q13-Var.csv" , "w")

for job in jobs.select( first_job, last_job ):

	list_of_args = []
	
	arguments = job.splitter.args
	max_args = len(arguments)
	model = arguments[max_args-1][2]
	max_energy = arguments[max_args-1][1].replace('+','')
	config = arguments[max_args-1][4].split('.')[0].split('_')[-1]
	phase = 'dCP' + arguments[max_args-1][7]
        theta13 = arguments[max_args-1][5].split(',')[1]
	var = variations[theta13]
	dmasses = arguments[max_args-1][6]

	list_of_args.append(model)
	list_of_args.append(config)
	list_of_args.append(angles)
	list_of_args.append(dmasses)
	list_of_args.append(var)
	list_of_args.append(phase)
	
	row = ','.join(list_of_args)

	#outfile.write( row + '\n' )

	print row


#logfile.close()

print "Done"

