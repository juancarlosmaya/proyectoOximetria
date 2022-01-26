# -*- coding: utf-8 -*-
"""
Editor de Spyder

Este es un archivo temporal.
"""
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "Times New Roman"
x1=[88,90,91,92,93,94,95,96]
y1=[90,91,90,91,96,94,93,96]
x2=[89,90,91,92,93,94,95,96]
y2=[89,91,93,92,93,94,95,97]
x=np.concatenate((x1,x2))
y=np.concatenate((y1,y2))
grafico=plt.plot(x,y,'o',color='blue')
plt.xlabel("SPO2 leida por oxímetro marca XYZ /%",**{'fontname':'Times New Roman'})
plt.ylabel("SPO2 leida por sistema embebido propuesto /%",**{'fontname':'Times New Roman'})
print(np.corrcoef(x, y))