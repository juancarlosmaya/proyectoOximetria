from .models import usuario
from django import forms
import datetime

class formulario(forms.ModelForm):
    class Meta:
        model = usuario
        fields= ['nombre', 'cedula', 'fecha_nacimiento', 'oximetria', 'frecuenciaCardiaca']
        # initials ={
        #     'fecha_nacimiento':datetime.date(1980, 5, 17),
        # }
        widgets = {
             'fecha_nacimiento': forms.DateInput(format=('%m/%d/%Y'), attrs={'class':'form-control', 'placeholder':'Select a date', 'type':'date'}),
        }
        
        



#from django import forms

#class formulario(forms.Form):
#    nombre_nuevo_paciente                   = forms.CharField(label='Nombre', max_length=20)
#    fecha_naciemiento                      = forms.DateField(widget = forms.SelectDateWidget)
#    edad_nuevo_paciente                     = forms.DecimalField(label='Edad',decimal_places=0,max_digits=3)
#    frecuencia_estimulacion_nuevo_paciente  = forms.DecimalField(label='Frecuencia de estimulación',decimal_places=1,max_digits=3)
#    numero_terapias_nuevo_paciente          = forms.DecimalField(label='Número de terapias',decimal_places=0,max_digits=3)
    #terapias_realizadas_nuevo_paciente      = forms.DecimalField(decimal_places=0,max_digits=3,max_value=10, min_value=0)
