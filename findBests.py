#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Aug 25 23:22:02 2019

@author: gbm
"""
import numpy as np
import csv
import os
import sys
from plotGraph import plotGraph
import pandas as pd
import xlsxwriter

file   = open( str(sys.argv[1]) , newline='')
#print(sys.argv[1])
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

if(str(cap3[best[0]]) == '0'):
    if(str(cap2[best[0]]) == '0'):
        save_folder = '1Caps_best/'
    else:
        save_folder = '2Caps_best/'
else:
    save_folder = '3Caps_best/'
    
writer = pd.ExcelWriter(str(sys.argv[1])[:-25] + save_folder + 'Bests.xlsx', engine='xlsxwriter')

for i in range(10):
    print( "--------------------------------------" )
    print( "#" + str(i+1) + " Best Allocation" )
    print( "LOSS \t\tTENSION\t\tCAP 1 \tCAP 2 \tCAP 3" )
    print( str(loss[best[i]]) + "\t" +str(tens[best[i]]) + "\t" + "ID " + str(cap1[best[i]]) + "\tID " +  str(cap2[best[i]]) + "\tID " +  str(cap3[best[i]]) )
    plotGraph( str(sys.argv[1])[:-25], i+1,str(cap1[best[i]]), str(cap2[best[i]]), str(cap3[best[i]]), writer )
    os.system("plotGraph.py " + str(cap1[best[i]]) + " " +  str(cap2[best[i]]) + " " +  str(cap3[best[i]]))
    
writer.save()
print( "--------------------------------------" )

