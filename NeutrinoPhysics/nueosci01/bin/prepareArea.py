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

target = ['results']

for tg in target:
	if not os.path.isdir(tg):
		os.mkdir( tg )

path =options.path

types = ['probs','flux','ratio']
objects = ['ModelA','ModelB','ModelC','Vacuum','ZeroPt','EarthA','EarthB','Lambdas']
file_ext = ['eps','png','pdf']

for type in types:
    for obj in objects:
	    for ext in file_ext:
		    base = target[0] + '/' + path + '/'
		    new_path = base + type + '/' + obj + '/' + ext
		    if not os.path.isdir(new_path):
			    os.makedirs( new_path )

sym_link = target[0] + '/' + path

cmd = 'rm paper01-plots; ln -s ' + sym_link + ' paper01-plots'

os.system(cmd)

