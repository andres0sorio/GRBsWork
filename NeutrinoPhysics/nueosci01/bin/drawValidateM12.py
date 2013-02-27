#!/usr/bin/python
# ...
# Andres Osorio - aosorio@uniandes.edu.co
# ...

import os,sys
import string

from optparse import OptionParser
#-----------------------------------------------------
#-----------------------------------------------------
parser = OptionParser()
parser.add_option("-f", type = "string", dest="infiles",
                  help="Input files", metavar="INPUT" )

(options, args) = parser.parse_args()

if options.infiles is None:
        parser.error("please provide input data ")
	
#-----------------------------------------------------
#-----------------------------------------------------

sys.argv = sys.argv[:1] + ['-b'] + sys.argv[1:]

import ROOT
from ROOT import TCanvas, TPad, TFormula, TF1, TPaveLabel, TH1F, TGraph, TFile, TLatex, TLegend, TPaveStats
from ROOT import TStyle
from ROOT import gROOT, gBenchmark, gDirectory
from array import array
import math

gROOT.Reset()
gBenchmark.Start( 'drawValidation' )

#....................................................................................
tdrStyle = TStyle("tdrStyle","Style for P-TDR")

tdrStyle.SetCanvasBorderMode(0)
tdrStyle.SetCanvasColor(10)
tdrStyle.SetTitleBorderSize(0)
tdrStyle.SetTitleFillColor(10)
tdrStyle.SetTitleFontSize(0.04)
tdrStyle.SetGridColor(16)
tdrStyle.SetGridStyle(3)
tdrStyle.SetGridWidth(1)
tdrStyle.SetLabelColor(1, "XYZ")
tdrStyle.SetLabelFont(42, "XYZ")
tdrStyle.SetLabelOffset(0.007, "XYZ")
tdrStyle.SetLabelSize(0.04, "XYZ")

tdrStyle.cd()
#....................................................................................

graphs = []
infiles = options.infiles.split(',')

for file in infiles:
	k = 0
	g1 = TGraph()
	g1.SetMarkerStyle(20)
	g1.SetMarkerSize(0.8)
	data = open(file,'r')
	for line in data.readlines():
		info = line.split()
		x1 = float ( info[1] )
		y1 = float ( info[3] )
		g1.SetPoint( k, math.log10(x1), y1 )
		k += 1
	graphs.append(g1)
	data.close()

canvas = TCanvas()
canvas.SetGridy()
canvas.cd()
isfirst = True

for graph in graphs:
	if isfirst:
		graph.Draw("AP")
		isfirst = False
	else:
		graph.Draw("P")

canvas.Print("integrationMethods.ps")

