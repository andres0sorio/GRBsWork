#!/bin/python

XMIN = 1.0e-24
XMAX = 1.0e-13
Dx = 1.0e-24

ratio = XMAX/XMIN

Ax = 1.0e-24
counter = 0

if ratio > 100:
    print 'Will use special step'
    while Ax < XMAX:
        if Ax < 1.0E-21:
            Ax += Dx*1.0e1
            counter += 1
        elif Ax >=1.0E-21 and Ax < 1.0E-18:
            Ax += (Dx*1.0e2)
            counter += 1
        else:
            Ax += (Dx*1.0e5)
            counter += 1


print counter





