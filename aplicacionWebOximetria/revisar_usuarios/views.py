from gestion_usuarios.models import usuario

from django.shortcuts import render
#from django.http import HttpResponse

def revisar_usuarios(request):
    usuarios=usuario.objects.all()  # lee todos los usuarios en la base de datos

    if request.method=="POST":
        nombre_seleccionado=request.POST.get("nombre_seleccionado")
        usuario_seleccionado = usuario.objects.get(nombre=nombre_seleccionado)
        print(usuario_seleccionado.nombre)
        return render(request,'revisar_usuarios/revisar_usuario.html',{'usuarios':usuarios,'usuario_seleccionado':usuario_seleccionado})
    #return HttpResponse("<h1>Esta es la pagina de inicio</h1><h1>Hay dos opciones</h1> <p>Revisar usuarios</p> <p>Administrar usuarios</p> ")
    return render(request,'revisar_usuarios/revisar_usuario.html',{'usuarios':usuarios})




