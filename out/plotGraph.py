#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 11 21:03:38 2018
@author: gabrielbmiranda
"""
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd

#import sys
#from antlr4 import *
#if __name__ == '__main__':
#    input1 = FileStream(sys.argv[1])
#    input2 = FileStream(sys.argv[2])
#    input3 = FileStream(sys.argv[3])
def plotGraph(bestpos,input1,input2,input3):
    folder = str()
    if(input3 == '0'):
        if(input2 == '0'):
            folder = 'caps1/'
        else:
            folder = 'caps2/'
    else:
        folder = 'caps3/'
    input_e = folder + 'graph_cap' + str(input1) + '-' + str(input2) + '-' + str(input3) + '_e_tree.csv'
    input_v = folder + 'graph_cap' + str(input1) + '-' + str(input2) + '-' + str(input3) + '_v_tree.csv'

    dfe = pd.read_csv( input_e , names = ['no1', 'no2', 'perda', 'fluxo' ])
    dfv = pd.read_csv( input_v , names = ['no' , 'capacitor' , 'potencia'])
    
    D = nx.DiGraph()
        
    for i in range(dfv.shape[0]):
        D.add_node(dfv.astype(int).at[dfv.shape[0]-(i+1),'no'], size = dfv.astype(float).at[dfv.shape[0]-(i+1),'potencia'] )
        
    for i in range(dfe.shape[0]):
        D.add_edge(dfe.astype(int).at[i,'no1'], dfe.astype(int).at[i,'no2'], loss=10e3*dfe.astype(float).at[i,'perda'], cor='k')
    
    
    #myCM = plt.cm.inferno 
    #myCM = plt.cm.ocean 
    #myCM = plt.cm.gnuplot
    #myCM = plt.cm.gnuplot2
    #myCM = plt.cm.magma
    #myCM = plt.cm.plasma
    myCM = plt.cm.rainbow
    #myCM = plt.cm.coolwarm
    #myCM = plt.cm.Spectral
    #myCM = plt.cm.viridis
    #myCM = plt.cm.brg
    #myCM = plt.cm.cool
    #myCM = plt.cm.jet
    
    D.nodes.data('cor', default='b') 
    
    edges   =  D.edges()
    colors  = [D[u][v]['cor' ] for u,v in edges]
    loss    = [D[u][v]['loss'] for u,v in edges]
    color_v = ['r' if c > 0 else 'g' for c in dfv['capacitor']]
    color_v[-1] = 'b' # Root
    color_v.reverse()
    
    pos = nx.kamada_kawai_layout(D)
#    pos = nx.planar_layout(D)
    
    nx.draw(D, pos, edges=edges, edge_color=colors , node_color=color_v , cmap=myCM, with_labels=True , width=loss)
    
    #label='Perda Ativa: 196.093754(kw), Tensao Minima: 0.935283(pu)'
    plt.draw()
    #savename = "graph_" + str(theName) + ".eps"
    plt.savefig("network_cap_" + str(bestpos) + "_" + str(input1) + "-" + str(input2) + "-" + str(input3) + "-" + ".eps")
    plt.show()