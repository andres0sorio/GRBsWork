#!/usr/bin/python
import os, sys
#--------------------------------------------------------


#--------------------------------------------------------

LOCATION = os.getcwd()

models = ['ModelA','ModelB','ModelC']
dcp    = ['dCP0','dCP180']
vars   = ['Var1','Var2','Var3']
config = ['SetI','SetII']
energy = '1e17'

source = LOCATION + '/root_files/'

if not os.path.isdir(source):
	print 'cannot continue - no root files'
	sys.exit(1)

arguments = []
datasets = []

outfile = open('all_datasets_models.dat','w')

for model in models:
	for cfg in config:
		for phase in dcp:
		
			dataset_name = 'dataset_' + model + '_' + cfg + '_' + phase + '_RvsAlfa.dat'
			dataset = open (dataset_name,'w')
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
					

for dataset in datasets:
	
	if sys.version_info < (2,7):
		dataset.seek(-1,2)
	else:
		dataset.seek(-1, os.SEEK_END)
		
	dataset.truncate()
	dataset.close()


