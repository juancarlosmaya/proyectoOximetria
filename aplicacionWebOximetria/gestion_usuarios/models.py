from django.db import models

# Create your models here.

class usuario(models.Model):
    nombre                  = models.CharField(max_length=20)
    cedula                  = models.DecimalField(decimal_places=0,max_digits=12)
    fecha_nacimiento        = models.DateField()    
    oximetria               = models.DecimalField(decimal_places=0,max_digits=3)
    frecuenciaCardiaca      = models.DecimalField(decimal_places=0,max_digits=3)
    
    
    # funcion que devuelve una cadena de texto asociada al objeto
    def __str__(self):
        return self.nombre
    
    #def terapias_pendientes(self):
    #    return self.numero_terapias-self.terapias_realizadas