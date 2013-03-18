#!/usr/bin/python
import os, sys
from optparse import OptionParser

#--------------------------------------------------------

parser = OptionParser()

parser.add_option("-f", type = "string", dest="infile",
                  help="input file" )

parser.add_option("-p", type = "string", dest="parameter",
                  help="parameter to change" )

parser.add_option("-r", type = "string", dest="range",
                  help="range for this parameter" )

parser.add_option("-n", type = "string", dest="nvar",
                  help="number of variations" )

(options, args) = parser.parse_args()

if options.infile is None:
        parser.error("please give 1 cent, try -h")

if options.parameter is None:
        parser.error("please give 2 cents, try -h")

if options.range is None:
        parser.error("please give 3 cents, try -h")

if options.nvar is None:
        parser.error("please give 4 cents, try -h")

#--------------------------------------------------------

infile  = open( options.infile, 'r' )
param   = options.parameter
range   = options.range.split(',')
nvar    = float( options.nvar )

outfile_prefix = options.infile.split('.')[0]

print param, range, nvar, outfile_prefix

vmin = float( range[0] )
vmax = float( range[1] )

delta = (vmax - vmin) / nvar

k = 0

var = vmin

while var <= vmax:

	outfile_name = outfile_prefix + '_' + str(k) + '.xml'
	outfile = open( outfile_name,'w')

	for line in infile.readlines():
		new_line = line.replace('#####'+param+'#####', str(var) )
                outfile.writelines( new_line )
		
	k += 1
	var += delta
	outfile.close()
	infile.seek(0)

