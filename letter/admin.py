from django.contrib import admin
from .models import Letter, Establish, Structure


class LetterAdmin(admin.ModelAdmin):
    list_display = ('subject', 'number', 'date', 'receiver', 'deadline', 'status')


class EstablishAdmin(admin.ModelAdmin):
    list_display = ('subject', 'number', 'date', 'responsible', 'deadline', 'status')


class StructureAdmin(admin.ModelAdmin):
    list_display = ('subject', 'number', 'date', 'responsible', 'deadline', 'status')


admin.site.register(Letter, LetterAdmin)
admin.site.register(Establish, EstablishAdmin)
admin.site.register(Structure, StructureAdmin)

# Register your models here.
