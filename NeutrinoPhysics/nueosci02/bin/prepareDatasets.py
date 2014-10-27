#!/usr/bin/python
import os, sys
#-------------------------------------------------------------------------
from optparse import OptionParser
#-------------------------------------------------------------------------
parser = OptionParser()
parser.add_option("-c", type = "string", dest="config",
                  help="new directory" )

(options, args) = parser.parse_args()

if options.config is None:
        parser.error("please provide a configuration set (SetI or SetII)")
#-------------------------------------------------------------------------

LOCATION = os.getcwd()

cfg    = options.config

models = ['ModelA','ModelB','ModelC']
dcp    = ['dCP0','dCP180']
vars   = ['Var1','Var2','Var3']
energy = '1e17'

source = LOCATION + '/root_files/'

if not os.path.isdir(source):
	print 'cannot continue - no root files'
	sys.exit(1)

arguments  = []
datasets   = []

outfile = open('all_datasets_models_' + cfg + '.dat','w')

for model in models:
	for phase in dcp:
		dataset_name = 'dataset_' + model + '_' + cfg + '_' + phase + '_RvsAlfa.dat'
		dataset = open (dataset_name,'r+')
		datasets.append(dataset)
		
		outfile.write(dataset_name + '\n')

		for var in vars:
			arguments = ['output']
			arguments.append(model)
			arguments.append(cfg)
			arguments.append(energy)
			arguments.append(var)
			arguments.append(phase)
			file = source + model + '/' + '_'.join(arguments) + '.root'
			if os.path.isfile(file):
				dataset.write(file + '\n')
			else:
				print 'Error: file not found: ', file
				
for dataset in datasets:

	dataset.seek(0)
	first_char = dataset.read(1)
	
	if not first_char:
		print 'Warning - dataset is EMPTY:', dataset.name
		dataset.close()
	else:
		if sys.version_info < (2,7):
			dataset.seek(-1,2)
		else:
			dataset.seek(-1, os.SEEK_END)
		
		dataset.truncate()
		dataset.close()

