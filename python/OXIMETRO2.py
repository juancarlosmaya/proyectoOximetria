#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  9 18:51:33 2020

@author: rubendiaz
"""
import time


from time import sleep
import serial
import io
import matplotlib.pyplot as plt
import numpy as np
from drawnow import *
import tkinter as Tk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from scipy.ndimage.interpolation import shift
import matplotlib.animation as animation

import csv
from numpy import genfromtxt
import tkinter.filedialog
import tkinter as Tk
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, welch, decimate
from scipy import mean

import operator
plt.rcParams.update({'figure.max_open_warning': 0})

from tkinter import W

### Diseño de filtro pasa bandas 

def butter_bandpass(lowcut, highcut, fs, order):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a


def butter_bandpass_filter(data, lowcut, highcut, fs, order):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

### Diseño de filtro pasa bajos

def butter_lowpass(highcut, fs, order):
    nyq = 0.5 * fs
    high = highcut / nyq
    b, a = butter(order, high, btype='low', )
    return b, a


def butter_lowpass_filter(data, highcut, fs, order):
    b, a = butter_lowpass(highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

### Diseño de filtro pasa altos

def butter_highpass(lowcut, fs, order):
    nyq = 0.5 * fs
    high = lowcut / nyq
    b, a = butter(order, high, btype='high' )
    return b, a

def butter_highpass_filter(data, lowcut, fs, order):
    b, a = butter_highpass(lowcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

def eliminarPicos(y):
    numeroDatos=np.size(y)
    p=np.zeros(500)
    
    
    for i in range(1,numeroDatos-1,1):
        p[i]=min(abs(y[i]-y[i-1]), abs(y[i+1]-y[i]))
    
    ubicacion=1*(p>5000)
    
    if sum(ubicacion)>0:
        #plt.plot(y)
        i = list(ubicacion).index(True)
        y[i]=y[i-1]
        #plt.plot(y)
        #plt.pause(0.1)
    return y

def find(condition):
    res, = np.nonzero(np.ravel(condition))
    return res

def freq_from_crossings(sig, fs):
    """Estimate frequency by counting zero crossings
    
    Pros: Fast, accurate (increasing with data length).  Works well for long low-noise sines, square, triangle, etc.
    
    Cons: Doesn't work if there are multiple zero crossings per cycle, low-frequency baseline shift, noise, etc.
    
    """
    # Find all indices right before a rising-edge zero crossing
    indices = find((sig[1:] >= 0) & (sig[:-1] < 0))
    
    # Naive (Measures 1000.185 Hz for 1000 Hz, for instance)
    #crossings = indices
    
    # More accurate, using linear interpolation to find intersample 
    # zero-crossings (Measures 1000.000129 Hz for 1000 Hz, for instance)
    crossings = [i - sig[i] / (sig[i+1] - sig[i]) for i in indices]
    
    # Some other interpolation based on neighboring points might be better. Spline, cubic, whatever
    
    return fs / np.average(np.diff(crossings))

def obtenerValorAcDc(senal,fs):
    tiempo=np.arange(0,500,1)/100
    #--------------- VALOR DC -------------------
    ## Filtro a 0.1 Hz 
    #plt.figure()
    #plt.plot(senal)
    
    senalDC = butter_lowpass_filter(senal, highcut=1, fs=fs, order=3)
    #plt.plot(senalDC)
    
    # obtener valorDC
    valorDC=np.mean(senalDC[200::])
    senal=senal-valorDC
    #plt.plot(senal)
    

    #plt.figure()
    #--------------- VALOR AC -------------------
    ## Filtro pasa altos
    senal=butter_highpass_filter(senal, lowcut=0.5, fs=fs, order=3)
    ##senal=eliminarPicos(senal)
    
    ## Filtro pasa banda de 0.1 Hz a 20 Hz
    
    senalImprimir = butter_bandpass_filter(senal, highcut=5, lowcut=0.4, fs=fs, order=3)
    #esto si iba# #figuraRojo.plot(tiempo,-1*senalImprimir)
    #esto si iba# #figuraRojo.set_title('Señal Led Rojo e Infrarojo',fontsize=20)
    #plt.plot(senalImprimir[50:-1])
    
    
    #senal=np.hamming(np.size(senal))*senal
    #senalFiltrada = butter_bandpass_filter(senal, highcut=10, lowcut=0.25, fs=fs, order=3)
    
    senal=np.hamming(np.size(senal))*senal
    senal=butter_highpass_filter(senal, lowcut=0.3, fs=fs, order=3)
    senalFiltrada=butter_lowpass_filter(senal, highcut=5, fs=fs, order=3)
    #plt.plot(senalFiltrada)
    
    #senalFiltrada = butter_bandpass_filter(senal, highcut=5, lowcut=0.4, fs=fs, order=3)
    
    
    
    #plt.plot(senalFiltrada)
    #Submuestreo, ahora la señal queda a 50 Hz
    #plt.plot(senalFiltrada)
    senalAC=senalFiltrada[::2]/100
    tiempoAC=tiempo[::2]
    fsAC = fs/2
    
    senalAC=senalAC[::2]
    fsAC = fsAC/2
    tiempoAC=tiempoAC[::2]
 
   
    
    # obtener y graficar la densidad espectral de potencia
    #senalAC=np.hamming(np.size(senalAC))*senalAC
    espectroSenalAC=welch(senalAC, fsAC, window='hanning', nperseg=125, nfft=None, detrend='constant', return_onesided=True, scaling='density', axis=-1)
    
    #plt.figure()
    
    #plt.plot(espectroSenalAC[0][0:50],espectroSenalAC[1][0:50])
    
    # obtener valorDC
    indexMaxAC, valueMaxAc = max(enumerate(espectroSenalAC[1]), key=operator.itemgetter(1))
    FrecuenciaMaxima=espectroSenalAC[0][indexMaxAC]
    FrecuenciaMaxima=freq_from_crossings(senalFiltrada,100)
    valorAC=valueMaxAc

    return valorDC,valorAC,FrecuenciaMaxima,senalImprimir

def analizarSerial():
    print("Analizando puerto serial")
    global ser
    global ejeyRojo
    global ejeyInfrarojo
    global sio    

    ser = serial.serial_for_url(url='COM4', baudrate=115200, timeout=0.01)
    sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    sleep(3)
    
    ser.reset_input_buffer()
    aniRojoInfrarojo = animation.FuncAnimation(figura, animate,save_count=0, init_func=init, interval=1, blit=True, cache_frame_data=False)
    

           
def analizarCsv():
    
    print("Analizando archivo csv")
    fs=25 # Frecuencia de muestreo

    ### SElECCIÓN DE DIRECTORIO
    #directorio=Tk.filedialog.askopenfilename(filetypes=(("how code files",".csv"),("all files","*.*")))
    #print("el archivo escogido es"+directorio)
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/senalPrueba.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/dc+1Hz+Hz.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/senal_simulada.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/daos400.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/datos800.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/desaturacionLeonel1.csv"
    #directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/desaturacionLeonel3.csv"
    directorio="D:\proyectoOximetria\python\desaturacionLeonel3.csv"
    print("Leyendo datos de "+directorio)
    
    ### LECTURA DE ARCHIVO
    datosLeidos=genfromtxt(directorio, delimiter=';')
    
    ### LECTURA DE SEÑALES Y CORRECCIÓN LECTURAS 'NAN'
    tiempo=datosLeidos[:,0]
    infrarojo=datosLeidos[:,2]
    while np.argwhere(np.isnan(infrarojo)).size:
        infrarojo[np.argwhere(np.isnan(infrarojo))]=infrarojo[np.argwhere(np.isnan(infrarojo))-1]
    rojo=datosLeidos[:,3]
    while np.argwhere(np.isnan(rojo)).size:
        rojo[np.argwhere(np.isnan(rojo))]=rojo[np.argwhere(np.isnan(rojo))-1]


    for x in range(0,np.size(infrarojo)-500,10):

        #x=11500  
        plt.figure()
        print("##################################################")
        print()
        print("Tomando las muestras entre " + str(x)+ " y "+  str(x+500)) 
       
        tiempo=np.arange(x,x+500)
        senalIR=infrarojo[x:x+500]
        senalRojo=rojo[x:x+500]
        
        figuraRojoInfrarojo.cla()
        [valorDCIR,valorACIR,frecuenciaCardiacaHzIR]=obtenerValorAcDc(senalIR,fs)
        [valorDCRojo,valorACRojo,frecuenciaCardiacaHzRojo]=obtenerValorAcDc(senalRojo,fs)
        canvas.draw()

        #print("el valor DC IR es:"+str(valorDCIR))
        #print("el valor AC IR es: "+str(valorACIR))
        print("la frecuencia cardíaca IR es:"+str(frecuenciaCardiacaHzIR*60))
        #print()
    
    
    
    
        #print("el valor DC Rojo es:"+str(valorDCRojo))
        #print("el valor AC Rojo es: "+str(valorACRojo))
        print("la frecuencia cardíaca Rojo es:"+str(frecuenciaCardiacaHzRojo*60))
        # en instructable: frecuenci cardíaca desde IR
        #print()
    
    
        R=(valorACRojo/valorDCRojo)/(valorACIR/valorDCIR)
        #print("el valor R es:"+str(R))
        #print()
        R=R/6
    
        spo2=110-25*R
    
        # De instructable
        #SpO2 = (-45.06*R + 30.354)*R + 94.845
        print("el valor de la SpO2 es: "+str(spo2))
        #print()
    
        if (abs(frecuenciaCardiacaHzIR-frecuenciaCardiacaHzRojo)>0.4) or  (frecuenciaCardiacaHzIR>4) or (frecuenciaCardiacaHzRojo>4) or (frecuenciaCardiacaHzIR<0.5) or (frecuenciaCardiacaHzRojo<0.5) or spo2<50 or spo2>99:
            print("ERROR, NO MOSTRAR ESTOS DATOS")
            valorFrecuencia['text'] = "--"
            valorSaturacion['text']= "--"
        else:
            valorFrecuencia['text'] = str(int(((frecuenciaCardiacaHzIR*60)+(frecuenciaCardiacaHzRojo*60))/2))
            valorSaturacion['text']= str(int(spo2))
    
    
        print("")



        

        
        # # Graficar Rojo
        # figuraRojo.cla()
        # figuraRojo.plot(tiempo,senalRojo,color="red")
        # figuraRojo.set_title('Señal Led Rojo',fontsize=20)
        # #figuraRojo.set_xlabel('Tiempo /s',fontsize=16)
        # #figuraRojo.set_ylabel('Voltaje /v',fontsize=16)
        # canvas1.draw()
        
        # # Graficar Infrarojo
        # figuraInfrarojo.cla()
        # figuraInfrarojo.plot(tiempo,senalIR)#,'ro',markersize=1, color="blue")
        # figuraInfrarojo.set_title('Señal Led Infrarojo',fontsize=20)
        # #figuraInfrarojo.set_xlabel('Tiempo /s',fontsize=16)
        # #figuraInfrarojo.set_ylabel('Voltaje /v',fontsize=16)
        # canvas2.draw()
        
        sleep(0.01*10)
        
        root.update()

def init():  # only required for blitting to give a clean slate.
    global ser
    global ejex
    global ejeyRojo
    global ejeyInfrarojo
       
    plotRojo.set_ydata(ejeyRojo)
    plotInfrarojo.set_ydata(ejeyInfrarojo)

    ser.reset_input_buffer()


    return plotRojo,plotInfrarojo


def animate(i):
    global ejeyRojo
    global ejeyInfrarojo
    global tiempo
    global senalRojo
    global senalInfrarojo
    global ser
    global sio

    
    fs=100
    
    #print(str(1/(time.time()-tiempo)))
    #tiempo=time.time()
    
    
    muestras=0
    while (ser.inWaiting()>0):
        muestras=muestras+1
        if ser.inWaiting()>500:
            ser.reset_input_buffer()
            print("el numero de bytes ahora es:"+str(ser.inWaiting()))
        try:
            ## LEER DATOS DEL PUERTO SERIAL, rojo e infrarojo se encuntran separados por una ","
            arduinoString=sio.readline() # Read the newest output from the Arduino
            #arduinoString=ser.readline() # Read the newest output from the Arduino
            #arduinoString=ser.read_until(terminator='\n')
            
            #arduinoString=arduinoString.decode('utf-8')
            dataArray = arduinoString.split(',') 
            rojo = float( dataArray[0])           
            infraRojo = float( dataArray[1])
            #print('el rojo es: ' + str(rojo))
            #print('el infrarojo es: ' + str(infraRojo))
            
            senalRojo=shift(senalRojo, -1, cval=0)
            senalRojo[499] = rojo 

            senalInfrarojo=shift(senalInfrarojo, -1, cval=0)
            senalInfrarojo[499] = infraRojo
        except:
            print("Ocurrió un error con" + str(arduinoString))
            ser.reset_input_buffer()
    
    print(str(muestras))
    

    
    
    [valorDCRojo,valorACRojo,frecuenciaCardiacaHzRojo,senalImprimirRojo]=obtenerValorAcDc(senalRojo,fs)
    [valorDCIR,valorACIR,frecuenciaCardiacaHzIR,senalImprimirIR]=obtenerValorAcDc(senalInfrarojo,fs)
    
    plotRojo.set_ydata(senalImprimirRojo)
    plotInfrarojo.set_ydata(senalImprimirIR)
    figuraRojoInfrarojo.set_ylim((-3*np.std(senalImprimirIR),3*np.std(senalImprimirIR)))
    canvas.draw()
    
    try:
        R = (valorACRojo/valorDCRojo)/(valorACIR/valorDCIR)
        R = R*4
        spo2 = 110-25*R
        valorSaturacion['text'] = str(int(spo2))
        valorFrecuencia['text']= str(int(60*frecuenciaCardiacaHzIR))
    except:
        pass
    
    
    return plotRojo,plotInfrarojo

### Interfaz gráfica

tiempo=0
root = Tk.Tk()
root.geometry("350x220")
root.title("MONITOR DE OXIMETRÍA Y FRECUENCIA CARDÍACA")
#Tk.Label(root,text="Seleccione opcion en el Menú Archivo").pack(side=Tk.TOP)

bottomframe = Tk.Frame(root)
bottomframe.pack( side = Tk.BOTTOM )

etiquetaFrecuencia = Tk.Label(bottomframe, text="Frecuencia cardíaca /BPM:", fg="green")
etiquetaFrecuencia.pack( side = Tk.LEFT)

valorFrecuencia = Tk.Label(bottomframe, text="80", fg="green")
valorFrecuencia.pack( side = Tk.LEFT)

etiquetaSaturacion = Tk.Label(bottomframe, text="Saturación de oxígeno /%:", fg="brown")
etiquetaSaturacion.pack( side = Tk.LEFT )

valorSaturacion = Tk.Label(bottomframe, text="100", fg="brown")
valorSaturacion.pack( side = Tk.RIGHT )

# Menú
bar=Tk.Menu(root)
fileMenu=Tk.Menu(bar,tearoff=0)
fileMenu.add_command(label="Analizar señal via puerto serial",command=analizarSerial)
fileMenu.add_command(label="Analizar archivo csv",command=analizarCsv)
fileMenu.add_command(label="Salir")
bar.add_cascade(label="Archivo",menu=fileMenu)
root.config(menu=bar)


### Figura Señal Rojo e Infrarojo
figura = Figure( dpi=50)
figuraRojoInfrarojo = figura.add_subplot(111,autoscale_on=True,autoscaley_on=True)
figuraRojoInfrarojo.set_title('SEÑAL',fontsize=18)
figuraRojoInfrarojo.set_xlabel('Tiempo /s',fontsize=15)
figuraRojoInfrarojo.set_ylabel('Voltaje /mV',fontsize=15)

#figuraRojoInfrarojo.set_ylim((-100,100),auto=True)

canvas = FigureCanvasTkAgg(figura, master=root)
canvas.draw()
canvas.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1,padx=1,pady=1)
canvas._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1,padx=1,pady=1)

ejex=np.arange(0,5,0.01)
ejeyRojo=np.zeros(500)
ejeyInfrarojo=np.zeros(500)

plotRojo, = figuraRojoInfrarojo.plot(ejex, ejeyRojo, 'r')
plotInfrarojo, = figuraRojoInfrarojo.plot(ejex, ejeyInfrarojo, 'y')

senalRojo=np.zeros(500)
senalInfrarojo=np.zeros(500)
figura.legend(plotRojo,plotInfrarojo,labels=('Rojo', 'Infrarojo'),loc='upper right',bbox_to_anchor=(0.9,0.88))

#### El programa queda esperando a un evento de la GUI
Tk.mainloop()


