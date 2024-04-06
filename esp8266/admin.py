from django.contrib import admin
from .models import AirCondition, Motor, RoomGet, RoomPost


class RoomAdmin(admin.ModelAdmin):
    list_display = ('number', 'temp', 'time')

class AirConditionAdmin(admin.ModelAdmin):
    list_display = ('number', 'temp', 'speed', 'time')


admin.site.register(AirCondition, AirConditionAdmin)
admin.site.register(Motor)
admin.site.register(RoomPost, RoomAdmin)
admin.site.register(RoomGet)
# Register your models here.
