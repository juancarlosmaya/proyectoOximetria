"""electroestimulador_django URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
#from django.urls import path

# archivos estaticos como .css
from django.contrib.staticfiles.urls import staticfiles_urlpatterns


# paginas de inicio y acerca de
from .views import pagina_inicio 
#from .views import acerca_de

# para importar urls de otras aplicaciones
from django.conf.urls import url,include


urlpatterns = [
    # página de administración
    url(r'^admin/', admin.site.urls),
    
    # páginas de incio
    url(r'^$',pagina_inicio,name='pagina_inicio'), 
    
    # página acerca de
    #url(r'^acerca_de/$',acerca_de),

    # pagina de revisar pacientes
    url(r'^revisar/', include('revisar_usuarios.urls')),

    # página de gestión de pacientes
    url(r'^gestion/', include('gestion_usuarios.urls')),

    # página de simulación de uso de lavadora
    url(r'^simulador/', include('simulador_oximetria.urls')),
]

urlpatterns += staticfiles_urlpatterns()