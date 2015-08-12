#!/usr/bin/python
import sys, os, stat, shutil, re
from optparse import OptionParser

#--------------------------------------------------------
parser = OptionParser()
#
parser.add_option("-i", type = "string", dest="infile",
                  help="input file" )
#
(options, args) = parser.parse_args()
#
if options.infile is None:
        parser.error("provide the input file, try -h")
#--------------------------------------------------------

prefix = '/home/aosorio/Share/'

input = options.infile

dCP   = input.split('.')[0].split('_')[2]

output_fname = prefix + 'mathematica-input-' + dCP + '.dat'

output = open( output_fname , 'w')

matrix_snapshot  = open( input , 'r')

energy = 0
nline  = 0

max_energy = 1.0e+16

for line in matrix_snapshot:
	
	if re.match("\*", line):
		energy = float(line[1:-1])
		nline = 0
		
		if energy >= max_energy:
			break
		else:
			output.write( str(energy) + '\n' )
	else:
		info = line.split()
		#print info
		if len( info ) > 1:
			a_matrix = info[1][5:]
			tmp = a_matrix.replace('(','')
			tmp = tmp.replace(')','').replace('e','*^')
			elements_matrix = tmp.split(',')
			if len( elements_matrix) == 9:
				nline += 1
				pos = nline % 10
				#print elements_matrix, pos
				mathematica_matrix = " ".join(elements_matrix)
				if pos == 2 or pos == 3 or pos == 4 or pos == 5 or pos == 6:
					output.write( mathematica_matrix + '\n' )
				
		if len( info ) == 1:
			detTab = info[0].split("=")[1]
			output.write( detTab + '\n')


matrix_snapshot.close()

output.close()

