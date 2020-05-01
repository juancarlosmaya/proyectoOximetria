# -*- coding: utf-8 -*-
"""
Codificación función PSD
"""

from numpy import genfromtxt
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np
from  scipy import signal
import operator


def obtenerValorAC(senal):
    senal=senal[::2]/10
    #senal=senal[::2]
    #ventana=128
    #window=signal.hamming(ventana)
    #muestra=window*senal
    #espectro=np.abs(fft(muestra))
    #espectro=espectro[0:int((np.size(muestra))/2)]
    #psd=2*espectro*espectro/(np.size(muestra)*np.sum(window*window))
    #psdPromedio=psd
    
    #overlap=20
    
    ventana=128
    overlap=int(ventana*0.6)
    
    superposicion=ventana-overlap
    psdPromedio=np.zeros(int(ventana/2))
    i=0
    for puntero in range(0,np.size(senal)-ventana,superposicion):
        i=i+1 
        window=signal.hamming(ventana)
        muestra=window*senal[puntero:puntero+ventana]
        espectro=np.abs(fft(muestra))
        espectro=espectro[0:int((np.size(muestra))/2)]
        psd=2*espectro*espectro/(np.size(muestra)*np.sum(window*window))
        #plt.plot(psd)
        psdPromedio=psdPromedio+psd
        puntero=puntero+superposicion
    plt.plot(psdPromedio/i)
    print(i)
    
    indexMaxAC, valueMaxAc = max(enumerate(psdPromedio), key=operator.itemgetter(1))
    frecuenciaMaxima=((((fs)/2)/2)/(ventana/2))*indexMaxAC
    valorAC=valueMaxAc
    return valorAC,frecuenciaMaxima
    
fs=100 # Frecuencia de muestreo

################# INFRAROJO ##################################

#directorio="D:\proyecto oximetría\python\senalOLED.csv"
directorio="D:\proyecto oximetría\python\senalIRconHamming.csv"
#directorio="D:\proyecto oximetría\python\senalIRconHammingDesaturada.csv"
print("Leyendo datos de "+directorio)

### LECTURA DE ARCHIVO
senalIR=genfromtxt(directorio, delimiter=';')
senalIR=np.concatenate((np.zeros(6),senalIR,np.zeros(6)))
#senalIR=np.concatenate((senalIR,senalIR,senalIR,senalIR))
[valorACIR,frecuenciaCardiacaHzIR]=obtenerValorAC(senalIR)
valorDCIR=122501.19449262046


################### ROJO #####################################

directorio="D:\proyecto oximetría\python\senalRojoconHamming.csv"
#directorio="D:\proyecto oximetría\python\senalRojoconHammingDesaturada.csv"
print("Leyendo datos de "+directorio)

### LECTURA DE ARCHIVO
senalRojo=genfromtxt(directorio, delimiter=';')
senalRojo=np.concatenate((np.zeros(6),senalRojo,np.zeros(6)))
#senalRojo=np.concatenate((senalRojo,senalRojo,senalRojo,senalRojo))
[valorACRojo,frecuenciaCardiacaHzRojo]=obtenerValorAC(senalRojo)
valorDCRojo=152368.66045332392

print("la frecuencia cardíaca IR es: "+str(frecuenciaCardiacaHzIR*60))
print("la frecuencia cardíaca Rojo es: "+str(frecuenciaCardiacaHzRojo*60))

R=(valorACRojo/valorDCRojo)/(valorACIR/valorDCIR)
R=R/6
spo2=110-25*R
print("el valor de la SpO2 es: "+str(spo2))


