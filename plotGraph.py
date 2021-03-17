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

fodase1 = []
fodase2 = []
fodase3 = []

def plotGraph(foldername='', bestpos=0,input1='0',input2='0',input3='0', writer = None):
    if(input3 == '0'):
        if(input2 == '0'):
#            if(input1 == '0'):
#                folder = ''
#                save_folder = ''
#            else: 
            folder = 'caps1/'
            save_folder = '1Caps_best/'
        else:
            folder = 'caps2/'
            save_folder = '2Caps_best/'
    else:
        folder = 'caps3/'
        save_folder = '3Caps_best/'
    
    folder = foldername + folder
    save_folder = foldername + save_folder
#    print(save_folder)
    
    input_e = folder + 'graph_cap' + str(input1) + '-' + str(input2) + '-' + str(input3) + '_e_tree.csv'
    input_v = folder + 'graph_cap' + str(input1) + '-' + str(input2) + '-' + str(input3) + '_v_tree.csv'
#    print(input_e)
#    global fodase1, fodase2
    dfe = pd.read_csv( input_e , names = ['no1', 'no2', 'activeLoss', 'activeFlow', 'reactiveLoss', 'reactiveFlow', 'resistance', 'reactance' ])
#    fodase1 = dfe
    dfv = pd.read_csv( input_v , names = ['no' , 'capacitor' , 'activePower', 'reactivePower','voltage'])
    
    dfe.to_excel(writer, sheet_name = 'edges_best-' + str(bestpos) + '_-_' + str(input1) + '-' + str(input2) + '-' + str(input3), index = False)
    dfv.to_excel(writer, sheet_name = 'nodes_best-' + str(bestpos) + '_-_' + str(input1) + '-' + str(input2) + '-' + str(input3), index = False)
    
    D = nx.DiGraph()
    
    for i in range(dfv.shape[0]):
        D.add_node(dfv.at[dfv.shape[0]-(i+1),'no'].astype(int),  
                          capacitor     = dfv.at[dfv.shape[0]-(i+1),'capacitor'].astype(int),
                          activePower   = dfv.at[dfv.shape[0]-(i+1),'activePower'].astype(float),
                          reactivePower = dfv.at[dfv.shape[0]-(i+1),'reactivePower'].astype(float),
                          voltage       = dfv.at[dfv.shape[0]-(i+1),'voltage'].astype(float) )
    
    for i in range(dfe.shape[0]):
        D.add_edge(dfe.at[i,'no1'].astype(int), dfe.at[i,'no2'].astype(int),
                   activeLoss   = dfe.at[i,'activeLoss'  ].astype(float), 
                   activeFlow   = dfe.at[i,'activeFlow'  ].astype(float),
                   reactiveLoss = dfe.at[i,'reactiveLoss'].astype(float),
                   reactiveFlow = dfe.at[i,'reactiveFlow'].astype(float),
                   resistance   = dfe.at[i,'resistance'  ].astype(float),
                   reactance    = dfe.at[i,'reactance'   ].astype(float), cor='k')

#    D.nodes.data('cor', default='b') 
    
    edges      =  D.edges()
    weights    = [ np.sqrt( D[u][v]['reactiveFlow']**2 + D[u][v]['activeFlow']**2 )  for u,v in edges ]
    
    color_v = ['r' if c > 0 else 'w' for c in dfv['capacitor']]
    color_v.reverse()
#    color_v[-1] = 'b' # Root
    
    myCM = plt.cm.rainbow
    #myCM = plt.cm.coolwarm
#    myCM = plt.cm.Spectral
    
    pos = nx.nx_agraph.graphviz_layout(D, prog='dot')
#    for k in pos: 
#        pos[k] = (pos[k][0], 1.1*pos[k][1]) # aumentar posicao em y para tentar melhorar apresentacao dos dados
#    pos = nx.kamada_kawai_layout(D)
#    pos = nx.planar_layout(D)
    
    savename = save_folder + "network_cap_" + str(bestpos) + "_" + str(input1) + "-" + str(input2) + "-" + str(input3)
    
    #label='activeLoss Ativa: 196.093754(kw), Tensao Minima: 0.935283(pu)'
    nx.write_gexf(D, savename + ".gexf")
    nx.drawing.nx_agraph.write_dot( D,  savename + ".dot")
    
    nx.draw(D, pos, edges=edges, node_size=400 , edge_color = weights, width=2, edge_cmap = myCM, node_shape = '_', node_color=color_v , with_labels=True, label = 'Capacitor Allocation' )
    
    sm = plt.cm.ScalarMappable(cmap=myCM, norm=plt.Normalize(vmin = min(weights), vmax=max(weights)))
    sm._A = []
    plt.colorbar(sm)
    plt.style.use('PlotStyle.mplstyle')
    plt.savefig( savename + ".eps")
#    plt.show()
    plt.clf()