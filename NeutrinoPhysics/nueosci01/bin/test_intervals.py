#!/bin/python

XMIN = 1.0e-22
XMAX = 1.0e-12
Dx = 1.0e-23

ratio = XMAX/XMIN

Ax = 1.0e-22
counter = 0

if ratio > 100:
    print 'Will use special step'
    while Ax < XMAX:
        if Ax < 1.0E-19:
            Ax += Dx*1.0e2
            counter += 1
        elif Ax >=1.0E-19 and Ax < 1.0E-15:
            Ax += (Dx*1.0e4)
            counter += 1
        else:
            Ax += (Dx*1.0e8)
            counter += 1


print counter





