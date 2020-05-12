from django.conf.urls import url
from .views import simulador


urlpatterns = [
   
    # páginas de incio
    url(r'^$',simulador,name='simulador'), 
    

]