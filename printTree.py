#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Aug 25 23:22:02 2019

@author: gbm
"""
import numpy as np
import os
import sys
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd

save_folder =  str(sys.argv[1]) 
dfe = pd.read_csv( save_folder + "/original_e_tree.csv" , names = ['no1', 'no2', 'activeLoss', 'activeFlow', 'reactiveLoss', 'reactiveFlow', 'resistance', 'reactance' ])
dfv = pd.read_csv( save_folder + "/original_v_tree.csv" , names = ['no' , 'capacitor' , 'activePower', 'reactivePower','voltage'])

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

#D.nodes.data('cor', default='b') 

edges      =  D.edges()
weights    = [ D[u][v]['reactiveFlow']  for u,v in edges ]
#weights    = [ np.sqrt( D[u][v]['reactiveFlow']**2 + D[u][v]['activeFlow']**2 )  for u,v in edges ]
#weights    = [ D[u][v]['reactiveFlow']/D.node[v]['voltage']  for u,v in edges ]
#weights    = [ D.node[v]['reactivePower']/D.node[v]['voltage']  for u,v in edges ]

print("Vertex | # Caps Allocated")
print("-------------------------")
color_v = ['r' if c > 0 else 'w' for c in dfv['capacitor']]
for i in range(dfv.shape[0]):
    if dfv.at[i,'capacitor'] > 0:
        print( dfv.at[i,'no'].astype(str) + "     |  " + dfv.at[i,'capacitor'].astype(str) )
color_v.reverse()
#color_v[-1] = 'b' # Root

myCM = plt.cm.rainbow
#myCM = plt.cm.coolwarm
#myCM = plt.cm.Spectral

pos = nx.nx_agraph.graphviz_layout(D, prog='dot')
#for k in pos: 
#    pos[k] = (pos[k][0], 1.1*pos[k][1]) # aumentar posicao em y para tentar melhorar apresentacao dos dados
#pos = nx.kamada_kawai_layout(D)
#pos = nx.planar_layout(D)

savename = save_folder + "/Originals/Original_Tree(Q)"

nx.write_gexf(D, savename + ".gexf")
nx.drawing.nx_agraph.write_dot( D,  savename + ".dot")

nx.draw(D, pos, edges=edges, node_size=400 , edge_color = weights, width=2, edge_cmap = myCM, node_shape = '_', node_color=color_v , with_labels=True, label = 'Capacitor Allocation' )

sm = plt.cm.ScalarMappable(cmap=myCM, norm=plt.Normalize(vmin = min(weights), vmax=max(weights)))
sm._A = []
plt.colorbar(sm)
plt.style.use('PlotStyle.mplstyle')
plt.savefig( savename + ".eps")
#plt.show()
plt.clf()
    

