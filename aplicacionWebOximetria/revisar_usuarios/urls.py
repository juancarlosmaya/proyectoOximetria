
from django.conf.urls import url
from .views import revisar_usuarios

urlpatterns = [
    url(r'^$',revisar_usuarios),
    
]
