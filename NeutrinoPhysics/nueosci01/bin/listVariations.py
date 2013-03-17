#!/usr/bin/python
import os, sys
from optparse import OptionParser

#--------------------------------------------------------
parser = OptionParser()
parser.add_option("-p", type = "string", dest="path",
                  help="new directory" )

(options, args) = parser.parse_args()

if options.path is None:
        parser.error("please give a path")
#--------------------------------------------------------

outfile = ['output','.root']

for k in range(0,21):
	target = './' + options.path + '/' + outfile[0] + '_' + str(k) + '_' + outfile[1]
	print target

