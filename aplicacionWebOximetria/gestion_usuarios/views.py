from django.shortcuts import render
from django.http import HttpResponseRedirect

from gestion_usuarios.forms import formulario
import datetime

def solicitar_nombre(request):
    # if this is a POST request we need to process the form data
    if request.method=="POST":
        mi_formulario=formulario(request.POST)
        if mi_formulario.is_valid():
            mi_formulario.save()
            print('Nuevo paciente guardado')
           # return render(request,'gestion_usuarios/gracias.html')
            return HttpResponseRedirect('/gestion/gracias.html/')
           #return HttpResponseRedirect('www.google.com')
        else:
            print('Datos de nuevo paciente no son validos')


    else:
        mi_formulario= formulario(initial={'numero_terapias': '3','frecuencia_estimulacion':'100'})
        #mi_formulario= formulario(initial={'numero_terapias': '3','frecuencia_estimulacion':'100'})
    
    #return render(request,'gestion_usuario/name.html')#,{'mi_formulario':mi_formulario})
    return render(request,'gestion_usuarios/name.html',{'mi_formulario':mi_formulario})

def gracias(request):

    return render(request,'gestion_usuarios/gracias.html')



        



# Create your views here.
