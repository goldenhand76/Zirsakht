from django.contrib import admin
from .models import Boiler, Stiff, Pump, PumpLinear, DomesticHotWater, ExpTank, Chiller, AirConditioner, Generator, Repair, RepairMan, CoolingTower


class RepairAdmin(admin.ModelAdmin):
    list_display = ('explain', 'device', 'repairman', 'date')


class RepairManAdmin(admin.ModelAdmin):
    list_display = ('name', 'field', 'phone')


admin.site.register(Boiler)
admin.site.register(Pump)
admin.site.register(PumpLinear)
admin.site.register(DomesticHotWater)
admin.site.register(Stiff)
admin.site.register(ExpTank)
admin.site.register(Chiller)
admin.site.register(CoolingTower)
admin.site.register(AirConditioner)
admin.site.register(Generator)
admin.site.register(Repair, RepairAdmin)
admin.site.register(RepairMan, RepairManAdmin)


# Register your models here.
