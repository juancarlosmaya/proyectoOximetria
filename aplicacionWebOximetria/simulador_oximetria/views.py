from gestion_usuarios.models import usuario

from django.shortcuts import render



# Create your views here.

def simulador(request):
    if request.method=="POST":
        usuarios=usuario.objects.get(id=1)
        usuarios.oximetria=usuarios.oximetria-1
        usuarios.save()
        return render(request,'simulador_oximetria/simulador.html',{'usuarios':usuarios})
    else:
        usuarios=usuario.objects.get(id=1)
        return render(request,'simulador_oximetria/simulador.html',{'usuarios':usuarios})

