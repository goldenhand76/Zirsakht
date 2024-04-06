from django import forms
from .models import Checklist


class ChecklistForm(forms.ModelForm):
    class Meta:
        model = Checklist
        fields = ['duration', 'status']
        widgets = {'duration': forms.HiddenInput}
        # labels = {'name': '',
        #           'number': '',
        #           'location': ''}
