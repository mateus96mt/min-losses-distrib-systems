#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Aug 25 23:22:02 2019

@author: gbm
"""
import numpy as np
import csv
#import os
from plotGraph import plotGraph

file   = open('teste_capacitor_1caps.csv', newline='')
reader = csv.reader(file)
    
cap1 = []
cap2 = []
cap3 = []
loss = []
tens = []

for row in reader:
    cap1.append(int(row[0]))
    cap2.append(int(row[1]))
    cap3.append(int(row[2]))
    loss.append(float(row[3]))
    tens.append(float(row[4]))
    
cap1 = np.array(cap1)
cap2 = np.array(cap2)
cap3 = np.array(cap3)
loss = np.array(loss)
tens = np.array(tens)

best = loss.argsort()

for i in range(10):
    print( "--------------------------------------" )
    print( "#" + str(i+1) + " Best Allocation" )
    print( "LOSS \t\tTENSION\t\tCAP 1 \tCAP 2 \tCAP 3" )
    print( str(loss[best[i]]) + "\t" +str(tens[best[i]]) + "\t" + "ID " + str(cap1[best[i]]) + "\tID " +  str(cap2[best[i]]) + "\tID " +  str(cap3[best[i]]) )
#    plotGraph(i+1,str(cap1[best[i]]), str(cap2[best[i]]), str(cap3[best[i]]) )
#    os.system("plotGraph.py " + str(cap1[best[i]]) + " " +  str(cap2[best[i]]) + " " +  str(cap3[best[i]]))

print( "--------------------------------------" )

