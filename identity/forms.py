from .models import Repair
from django import forms


class RepairForm(forms.ModelForm):
    class Meta:
        model = Repair
        fields = ['device', 'explain', 'repairman', 'date']
        widgets = {'device': forms.HiddenInput,
                   'explain': forms.Textarea(attrs={'class': 'explain'}),
                   'repairman': forms.Select(attrs={'class': 'repairman'}),
                   'date': forms.HiddenInput}
