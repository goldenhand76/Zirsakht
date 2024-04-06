from django.contrib import admin
from .models import Checklist, Checklists


class ChecklistAdmin(admin.ModelAdmin):
    list_display = ('date', 'duration', 'status')


class ChecklistsAdmin(admin.ModelAdmin):
    list_display = ('device', 'code', 'duty', 'location', 'status')


admin.site.register(Checklists, ChecklistsAdmin)
admin.site.register(Checklist, ChecklistAdmin)

# Register your models here.
