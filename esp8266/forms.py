from django import forms
from .models import AirCondition, RoomGet, RoomPost


class AirConditionForm(forms.ModelForm):
    class Meta:
        model = AirCondition
        fields = ['number', 'temp', 'pressure', 'speed', 'angle']


class RoomForm(forms.ModelForm):
    class Meta:
        model = RoomPost
        fields = ["number", "temp"]
        widgets = {
            "number": forms.NumberInput(attrs={
                "id": "number",
                "required": True,
            }),
            "temp": forms.NumberInput(attrs={
                "id": "temp",
                "required": True,
            })
        }
