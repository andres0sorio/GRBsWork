#!/usr/bin/python
import sys, os, stat, shutil, re
from optparse import OptionParser

#--------------------------------------------------------
#parser = OptionParser()
#
#parser.add_option("-r", type = "string", dest="range",
#                  help="range for this parameter" )
#
#(options, args) = parser.parse_args()
#
#if options.range is None:
#        parser.error("please give 5 cents, try -h")
#--------------------------------------------------------

prefix = './root_files/'

matrix_snapshot  = open( 'matrix_snapshot.log' , 'r')

energy = 0

max_energy = 1.2e+14

for line in matrix_snapshot:
	if re.match("\*", line):
		energy = float(line[1:-1])
		if energy >= max_energy:
			break
	else:
		info = line.split()
		if len( info ) > 1:
			a_matrix = info[1][5:]
			tmp = a_matrix.replace('(','{')
			mathematica_matrix = tmp.replace(')','}')
			if len(mathematica_matrix.split(',')) == 9:
				print mathematica_matrix
			


matrix_snapshot.close()
		
	
		
