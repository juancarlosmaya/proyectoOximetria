from rest_framework import serializers
from gestion_usuarios.models import usuario

class Usuario_Serializador(serializers.ModelSerializer):
    class Meta:
        model= usuario
        fields=('id','nombre','cedula','saldo','fecha_nacimiento')
