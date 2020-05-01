# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from time import sleep
import serial
import matplotlib.pyplot as plt
import numpy as np
from drawnow import *
import tkinter as Tk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from scipy.ndimage.interpolation import shift

import csv
from numpy import genfromtxt
import tkinter.filedialog
import tkinter as Tk
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, welch, decimate
from scipy import mean
import matplotlib.animation as animation


import operator


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


def obtenerValorAcDc(senal,fs):
    #--------------- VALOR DC -------------------
    ## Filtro a 0.1 Hz 
    #plt.figure()
    #plt.plot(senal)
    
    senalDC = butter_lowpass_filter(senal, highcut=0.5, fs=fs, order=3)
    #plt.plot(senalDC)
    
    # obtener valorDC
    valorDC=np.mean(senalDC[100::])
    senal=senal-valorDC
    #plt.plot(senal)
    
    
    #--------------- VALOR AC -------------------
    ## Filtro pasa banda de 0.1 Hz a 20 Hz 
    #senal=np.hamming(np.size(senal))*senal
    senalFiltrada = butter_bandpass_filter(senal, highcut=10, lowcut=0.25, fs=fs, order=3)
    #plt.plot(senalFiltrada)
    #Submuestreo, ahora la señal queda a 50 Hz
    senalAC=senalFiltrada[::2]/100
    tiempoAC=tiempo[::2]
    fsAC = fs/2
    
    senalAC=senalAC[::2]
    fsAC = fsAC/2
    tiempoAC=tiempoAC[::2]
    
    #plt.plot(senalAC)
    
    # obtener y graficar la densidad espectral de potencia
    senalAC=np.hamming(np.size(senalAC))*senalAC
    espectroSenalAC=welch(senalAC, fsAC, window='hanning', nperseg=125, nfft=None, detrend='constant', return_onesided=True, scaling='density', axis=-1)
    
    #plt.figure()
    
    #plt.plot(espectroSenalAC[0][0:50],espectroSenalAC[1][0:50])
    
    # obtener valorDC
    indexMaxAC, valueMaxAc = max(enumerate(espectroSenalAC[1]), key=operator.itemgetter(1))
    FrecuenciaMaxima=espectroSenalAC[0][indexMaxAC]
    valorAC=valueMaxAc

    return valorDC,valorAC,FrecuenciaMaxima



# Adquirir y procesar señales
def analizarSerial():
    print("hola")

    
    
    # Apertura comunicación serial
    ser = serial.Serial('/dev/cu.usbserial-0001', 115200) # Establish the connection on a specific port
    #ser.open()
    
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    
    #ser.write(str(chr(counter))) # Convert the decimal number to ASCII then send it to the Arduino
    
    # Retardo para que lleguen el mensaje de inicialización de ESP32
    sleep(3)
    
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
   
    # Se lee el mensaje de inicio de ESP32, varias líneas
    arduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()

    # Configuracíon de ejes de gráficas
    ejex=np.arange(0,5,0.01)
    ejeyRojo=np.zeros(500)
    ejeyInfrarojo=np.zeros(500)
    
    # Se graficarán 1000 muestras de cada señal, rojo e infrarojo
    for x in range(0, 1000, 1):
        
       
        while (ser.inWaiting()==0): #Wait here until there is data
            pass #do nothing
        

        
        
        ## LEER DATOS DEL PUERTO SERIAL, rojo e infrarojo se encuntran separados por una ","
        arduinoString=ser.readline() # Read the newest output from the Arduino
        arduinoString=arduinoString.decode('utf-8')
        print(arduinoString)
        dataArray = arduinoString.split(',') 
        rojo = float( dataArray[0])            
        infraRojo = float( dataArray[1])
        
        
        ejeyRojo=shift(ejeyRojo, -1, cval=0)
        ejeyRojo[499]=rojo
        
        ejeyInfrarojo=shift(ejeyInfrarojo, -1, cval=0)
        ejeyInfrarojo[499]=infraRojo
        
        
        # Graficar Rojo
        figuraRojo.cla()
        figuraRojo.plot(ejex,ejeyRojo,color="red")
        figuraRojo.set_title('Señal Led Rojo',fontsize=20)
        figuraRojo.set_xlabel('Tiempo /s',fontsize=16)
        figuraRojo.set_ylabel('Voltaje /v',fontsize=16)
        canvas1.draw()
        
        # Graficar Infrarojo
        figuraInfrarojo.cla()
        figuraInfrarojo.plot(ejex,ejeyInfrarojo)#,'ro',markersize=1, color="blue")
        figuraInfrarojo.set_title('Señal Led Infrarojo',fontsize=20)
        figuraInfrarojo.set_xlabel('Tiempo /s',fontsize=16)
        figuraInfrarojo.set_ylabel('Voltaje /v',fontsize=16)
        canvas2.draw()
        
        
        root.update()
        sleep(0.01)  #equivale a 250Hz
    
def analizarCsv():
    
    plt.cla()

    fs=100 # Frecuencia de muestreo

    directorio="/Users/rubendiaz/Desktop/proyecto oximetría/python/datos800.csv"

    datosLeidos=genfromtxt(directorio, delimiter=';')

    

    m=0  
    print("##################################################")
    print()
    print("Tomando las muestras entre " + str(m)+ " y "+  str(m+5000)) 
    print()
    tiempo=datosLeidos[:,0][m:m+5000]
    senalIR=datosLeidos[:,2][m:m+5000]
    #plt.plot(senalIR)
    senalRojo=datosLeidos[:,3][m:m+5000]
    #plt.plot(senalRojo)
    #plt.figure()
    
    # Configuracíon de ejes de gráficas
    ejex=np.arange(0,40,0.04)
    ejeyRojo=np.zeros(500)
    ejeyInfrarojo=np.zeros(500)

    ejeyRojo2=np.zeros(1000)
    ejeyInfrarojo2=np.zeros(1000)    
    # Se graficarán 1000 muestras de cada señal, rojo e infrarojo
    for x in range(0, 4000, 1):
 
        rojo = senalRojo[x]          
        infraRojo = senalIR[x]
        
        
        ejeyRojo=shift(ejeyRojo, -1, cval=0)
        ejeyRojo[499]=rojo
        
        ejeyInfrarojo=shift(ejeyInfrarojo, -1, cval=0)
        ejeyInfrarojo[499]=infraRojo
        
        
        
        senal=ejeyRojo
        ###################### ROJO ###########################################
        #--------------- VALOR DC -------------------
        ## Filtro a 0.1 Hz 
        #plt.figure()
        #plt.plot(senal)
    
        senalDC = butter_lowpass_filter(senal, highcut=0.5, fs=fs, order=3)
        #plt.plot(senalDC)
    
        # obtener valorDC
        valorDC=np.mean(senalDC[100::])
        senal=senal-valorDC
        #plt.plot(senal)
    
    
        #--------------- VALOR AC -------------------
        ## Filtro pasa banda de 0.1 Hz a 20 Hz 
        #senal=np.hamming(np.size(senal))*senal
        senalFiltrada = butter_bandpass_filter(senal, highcut=10, lowcut=0.25, fs=fs, order=3)
        #plt.plot(senalFiltrada)
        #Submuestreo, ahora la señal queda a 50 Hz
        senalAC=senalFiltrada[::2]/100
        tiempoAC=tiempo[::2]
        fsAC = fs/2
        
        senalAC=senalAC[::2]
        fsAC = fsAC/2
        tiempoAC=tiempoAC[::2]
        
        #plt.plot(senalAC)
        
        # obtener y graficar la densidad espectral de potencia
        espectroSenalAC=welch(senalAC, fsAC, window='hanning', nperseg=125, nfft=None, detrend='constant', return_onesided=True, scaling='density', axis=-1)
        
        #plt.figure()
        
        #plt.plot(espectroSenalAC[0][0:50],espectroSenalAC[1][0:50])
        
        # obtener valorDC
        indexMaxAC, valueMaxAc = max(enumerate(espectroSenalAC[1]), key=operator.itemgetter(1))
        FrecuenciaMaxima=espectroSenalAC[0][indexMaxAC]
        valorAC=valueMaxAc
        
        if abs(senalAC[124])<20:
            ejeyRojo2=shift(ejeyRojo2, -1, cval=0)
            ejeyRojo2[-1]=senalAC[124]
        

        
        
        # Graficar Rojo
        figuraRojo.cla()
        figuraRojo.plot(ejex,ejeyRojo2,color="red")
        figuraRojo.set_title('Señal Led Rojo',fontsize=20)
        #figuraRojo.set_xlabel('Tiempo /s',fontsize=16)
        #figuraRojo.set_ylabel('Voltaje /v',fontsize=16)
        canvas1.draw()
        #######################################################################
        
        
        senal=ejeyInfrarojo
        ###################### Infrarojo ###########################################
        #--------------- VALOR DC -------------------
        ## Filtro a 0.1 Hz 
        #plt.figure()
        #plt.plot(senal)
    
        senalDC = butter_lowpass_filter(senal, highcut=0.5, fs=fs, order=3)
        #plt.plot(senalDC)
    
        # obtener valorDC
        valorDC=np.mean(senalDC[100::])
        senal=senal-valorDC
        #plt.plot(senal)
    
    
        #--------------- VALOR AC -------------------
        ## Filtro pasa banda de 0.1 Hz a 20 Hz 
        #senal=np.hamming(np.size(senal))*senal
        senalFiltrada = butter_bandpass_filter(senal, highcut=10, lowcut=0.25, fs=fs, order=3)
        #plt.plot(senalFiltrada)
        #Submuestreo, ahora la señal queda a 50 Hz
        senalAC=senalFiltrada[::2]/100
        tiempoAC=tiempo[::2]
        fsAC = fs/2
        
        senalAC=senalAC[::2]
        fsAC = fsAC/2
        tiempoAC=tiempoAC[::2]
        
        #plt.plot(senalAC)
        
        # obtener y graficar la densidad espectral de potencia
        #senalAC=np.hamming(np.size(senalAC))*senalAC
        espectroSenalAC=welch(senalAC, fsAC, window='hanning', nperseg=125, nfft=None, detrend='constant', return_onesided=True, scaling='density', axis=-1)
        
        #plt.figure()
        
        #plt.plot(espectroSenalAC[0][0:50],espectroSenalAC[1][0:50])
        
        # obtener valorDC
        indexMaxAC, valueMaxAc = max(enumerate(espectroSenalAC[1]), key=operator.itemgetter(1))
        FrecuenciaMaxima=espectroSenalAC[0][indexMaxAC]
        valorAC=valueMaxAc
        
        if abs(senalAC[124])<20:
            ejeyInfrarojo2=shift(ejeyInfrarojo2, -1, cval=0)
            ejeyInfrarojo2[-1]=senalAC[124]
        
        # Graficar Infrarojo
        figuraInfrarojo.cla()
        figuraInfrarojo.plot(ejex,ejeyInfrarojo2)#,'ro',markersize=1, color="blue")
        figuraInfrarojo.set_title('Señal Led Infrarojo',fontsize=20)
        #figuraInfrarojo.set_xlabel('Tiempo /s',fontsize=16)
        #figuraInfrarojo.set_ylabel('Voltaje /v',fontsize=16)
        canvas2.draw()
        #######################################################################
        
        
        
        
        
        
        
        
        
        
        
        

        

        
        
        root.update()
        #sleep(0.01)  #equivale a 250Hz
    
    
    
    # [valorDCIR,valorACIR,frecuenciaCardiacaHzIR]=obtenerValorAcDc(senalIR,fs)
    
    # print("el valor DC IR es:"+str(valorDCIR))
    # print("el valor AC IR es: "+str(valorACIR))
    # print("la frecuencia cardíaca IR es:"+str(frecuenciaCardiacaHzIR*60))
    # print()
    
    # [valorDCRojo,valorACRojo,frecuenciaCardiacaHzRojo]=obtenerValorAcDc(senalRojo,fs)
    
    # print("el valor DC Rojo es:"+str(valorDCRojo))
    # print("el valor AC Rojo es: "+str(valorACRojo))
    # print("la frecuencia cardíaca Rojo es:"+str(frecuenciaCardiacaHzRojo*60))
    # print()
    
    # R=(valorACRojo/valorDCRojo)/(valorACIR/valorDCIR)
    # print("el valor R es:"+str(R))
    # print()
    # R=R/6
    # spo2=110-25*R
    # print("el valor de la SpO2 es:"+str(spo2))
    # print()
    
def probando():
    print("probando")
    


    
#----------------- CONFIGURACIÓN INTERFAZ GRÁFICA

root = Tk.Tk()
root.title("Clasificador Señal Ecg Normal y Fibrilacion Auricula AF")
Tk.Label(root,text="Seleccione opcion en el Menú Archivo").pack(side=Tk.TOP)

# Figura Rojo
figura1 = Figure(figsize=(12, 10 ), dpi=60)
figuraRojo = figura1.add_subplot(211)
figuraRojo.set_title('Señal Led Rojo',fontsize=12)
figuraRojo.set_xlabel('Tiempo /s',fontsize=8)
figuraRojo.set_ylabel('Voltaje /v',fontsize=8)


# Figura Infrarojo
#figura2 = Figure(figsize=(5, 3), dpi=60)
figuraInfrarojo = figura1.add_subplot(212)
figuraInfrarojo.set_title('Señal Led Infrarojo',fontsize=12)
figuraInfrarojo.set_xlabel('Tiempo /s',fontsize=8)
figuraInfrarojo.set_ylabel('Voltaje /v',fontsize=8)

canvas1 = FigureCanvasTkAgg(figura1, master=root)
canvas1.draw()
canvas1.get_tk_widget().pack(side=Tk.TOP, fill=Tk.BOTH, expand=1,padx=1,pady=1)
canvas1._tkcanvas.pack(side=Tk.TOP, fill=Tk.BOTH, expand=1,padx=1,pady=1)


# Menú
bar=Tk.Menu(root)
fileMenu=Tk.Menu(bar,tearoff=0)
fileMenu.add_command(label="Analizar señal via puerto serial",command=analizarSerial)
fileMenu.add_command(label="Analizar archivo csv",command=analizarCsv)
fileMenu.add_command(label="Analizar archivo csv",command=probando)
fileMenu.add_command(label="Salir")
bar.add_cascade(label="Archivo",menu=fileMenu)
root.config(menu=bar)


## de probando
    




def init():  # only required for blitting to give a clean slate.
    print("hola")
    global ser
    
    #ser.open()
    
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    
    sleep(3)
    
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
   
    # Se lee el mensaje de inicio de ESP32, varias líneas
    arduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()
    rduinoString=ser.readline()

    plotRojo.set_ydata([np.nan] * len(ejeyRojo))
    plotInfrarojo.set_ydata([np.nan] * len(ejeyInfrarojo))
    return plotRojo, plotInfrarojo


def animate(i):
    
    global ejeyRojo
    global ejeyInfrarojo
    global ser
        
    while (ser.inWaiting()==0): #Wait here until there is data
        pass #do nothing
    

    
    
    ## LEER DATOS DEL PUERTO SERIAL, rojo e infrarojo se encuntran separados por una ","
    arduinoString=ser.readline() # Read the newest output from the Arduino
    arduinoString=arduinoString.decode('utf-8')
    print(arduinoString)
    dataArray = arduinoString.split(',') 
    rojo = float( dataArray[0])            
    infraRojo = float( dataArray[1])
    print("rojo"+str(rojo))
    print("infrarojo"+ str(infraRojo))
    
    
    ejeyRojo=shift(ejeyRojo, -1, cval=0)
    ejeyRojo[499]=rojo
    
    
    
    ejeyInfrarojo=shift(ejeyInfrarojo, -1, cval=0)
    ejeyInfrarojo[499]=infraRojo
    
    print(ejeyInfrarojo)
    
    plotRojo.set_ydata(ejeyRojo)
    plotInfrarojo.set_ydata(ejeyInfrarojo)  # update the data.
    
    return plotRojo,plotInfrarojo


ejex=np.arange(0,5,0.01)
ejeyInfrarojo=np.zeros(500)
ejeyRojo=np.zeros(500)

plotRojo, = figuraRojo.plot(ejex, 10*np.sin(ejeyInfrarojo))
figuraRojo.set_ylim((-1,123500))

plotInfrarojo, = figuraInfrarojo.plot(ejex, 10*np.sin(ejeyInfrarojo))
figuraInfrarojo.set_ylim((-100,123500))


#figuraInfrarojo.autoscale()

ser = serial.Serial('/dev/cu.usbserial-0001', 115200)

aniRojoInfrarojo = animation.FuncAnimation(
    figura1, animate, init_func=init, interval=10, blit=True, save_count=50)




## fin de probando


def on_closing():
    global ser
    ser.close()
    print("cerrando")
    root.destroy()
    

root.protocol("WM_DELETE_WINDOW", on_closing)

# El programa queda esperando a un evento de la GUI
Tk.mainloop()
                