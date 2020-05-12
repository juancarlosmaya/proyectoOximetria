
from django.conf.urls import url
from .views import solicitar_nombre
from .views import gracias

urlpatterns = [
    url(r'^$',solicitar_nombre,name='gestion'),
    url('gracias',gracias),
   
    
]
