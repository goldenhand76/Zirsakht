from django.db import models
from datetime import datetime
from django.utils import timezone
# Create your models here.


class Boiler(models.Model):
    device = models.CharField(default='دیگ و مشعل', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    heat_capacity = models.IntegerField()
    pressure = models.IntegerField()
    consumption = models.IntegerField()
    water_capacity = models.IntegerField()
    weight = models.IntegerField()
    flange_size = models.IntegerField()

    isolator_material = models.CharField(max_length=30, blank=True, null=True)
    isolator_density = models.IntegerField(blank=True, null=True)
    isolator_thickness = models.IntegerField(blank=True, null=True)

    door_material = models.CharField(max_length=30, blank=True, null=True)

    card = models.ImageField(upload_to= 'media/identity/boiler', blank=True, null=True,)

    def __str__(self):
        return f'{self.code}'


class Pump(models.Model):
    device = models.CharField(default='پمپ', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    input = models.CharField(max_length=10)
    output = models.CharField(max_length=10)

    motor_power = models.FloatField()
    motor_speed = models.IntegerField()

    helical_power = models.CharField(max_length=10)
    helical_speed = models.CharField(max_length=10)
    helical_flow_rate = models.CharField(max_length=10)
    helical_height = models.CharField(max_length=10)

    input_valve_size = models.CharField(max_length=10)
    output_valve_size = models.CharField(max_length=10)

    card = models.ImageField(upload_to='media/identity/pump', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class PumpLinear(models.Model):
    device = models.CharField(default='پمپ خطی', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    input = models.CharField(max_length=10)
    output = models.CharField(max_length=10)

    motor_power = models.FloatField()
    motor_speed = models.IntegerField()

    input_valve_size = models.CharField(max_length=10)
    output_valve_size = models.CharField(max_length=10)

    card = models.ImageField(upload_to='media/identity/pumplinear', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class DomesticHotWater(models.Model):
    device = models.CharField(default='منبع کویل دار', max_length=20)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    capacity = models.IntegerField(blank=True, null=True)
    tank_height = models.IntegerField(blank=True, null=True)
    tank_diameter = models.IntegerField(blank=True, null=True)

    coil_length = models.IntegerField(blank=True, null=True)
    coil_diameter = models.IntegerField(blank=True, null=True)
    coil_surface = models.FloatField(blank=True, null=True)

    card = models.ImageField(upload_to='media/identity/DomesticHotWater', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class Stiff(models.Model):
    device = models.CharField(default='سختی گیر', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    capacity = models.IntegerField()
    resin = models.FloatField()
    flow_rate = models.IntegerField()
    backwash_flow_rate = models.IntegerField()
    valve_size = models.CharField(max_length=10)
    tank_size = models.CharField(max_length=10)
    salt_amount = models.CharField(max_length=10)

    card = models.ImageField(upload_to='media/identity/WaterSoftener', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class ExpTank(models.Model):
    device = models.CharField(default='منبع انبساط', max_length=15)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    capacity = models.IntegerField()
    pressure = models.IntegerField()
    diameter = models.IntegerField()
    height = models.IntegerField()
    valve_size = models.CharField(max_length=10)
    weight = models.IntegerField()

    card = models.ImageField(upload_to='media/identity/ExpansionTank', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class Chiller(models.Model):
    device = models.CharField(default='چیلر', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    cool_capacity = models.IntegerField()
    cool_water_flow_rate = models.FloatField()

    comp_model = models.CharField(max_length=20)
    comp_power = models.FloatField()
    comp_num = models.IntegerField()

    cond_model = models.CharField(max_length=20)
    cond_flow_rate = models.CharField(max_length=10)
    cond_pressure_drop = models.FloatField()

    cooler_model = models.CharField(max_length=20)
    cooler_pressure_drop = models.FloatField()

    card = models.ImageField(upload_to='media/identity/Chiller', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class CoolingTower(models.Model):
    device = models.CharField(default='برج خنک کن', max_length=15)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    motor_power = models.IntegerField()
    motor_speed = models.IntegerField()

    fan = models.CharField(max_length=20)
    built = models.CharField(max_length=20)

    flow = models.FloatField()
    temp_input = models.IntegerField()
    temp_output = models.IntegerField()

    card = models.ImageField(upload_to='media/identity/CoolingTower', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class AirConditioner(models.Model):
    device = models.CharField(default='هواساز', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    motor_quantity = models.IntegerField()
    motor_power = models.FloatField()

    fan_quantity = models.IntegerField()
    fan_speed = models.IntegerField()
    fan_size = models.CharField(max_length=20)

    coil_cool = models.IntegerField()
    coil_warm = models.IntegerField()
    coil_pre_warm = models.IntegerField()

    flow_rate = models.IntegerField()
    TSP = models.FloatField()
    ESP = models.FloatField()

    card = models.ImageField(upload_to='media/identity/AirConditioner', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


class Generator(models.Model):
    device = models.CharField(default='ژنراتور', max_length=10)
    status = models.BooleanField(default=True)

    code = models.CharField(max_length=10)
    location = models.CharField(max_length=30)
    producer = models.CharField(max_length=30)

    _type = models.CharField(max_length=30)
    serial = models.CharField(max_length=30)
    model = models.CharField(max_length=30)
    since = models.CharField(max_length=4)

    power = models.IntegerField()
    rpm = models.FloatField()

    faze = models.IntegerField()
    frequency = models.IntegerField()

    card = models.ImageField(upload_to='media/identity/Generator', blank=True, null=True)

    def __str__(self):
        return f'{self.code}'


FIELDS = (('برق', 'برق'), ('مکانیک', 'مکانیک'))


class RepairMan(models.Model):
    name = models.CharField(max_length=30, blank=True, null=True)
    field = models.CharField(choices=FIELDS, max_length=10, blank=True, null=True)
    phone = models.CharField(max_length=20, blank=True, null=True)
    since = models.DateField(blank=True, null=True)
    until = models.DateField(blank=True, null=True)

    def __str__(self):
        return f'{self.name}'


REPAIRMANS = [(str(i), str(i)) for i in RepairMan.objects.all()]

devices = [(str(i), str(i)) for i in Boiler.objects.all()] + \
          [(str(i), str(i)) for i in Pump.objects.all()] + \
            [(str(i), str(i)) for i in PumpLinear.objects.all()] + \
          [(str(i), str(i)) for i in DomesticHotWater.objects.all()] + \
          [(str(i), str(i)) for i in Stiff.objects.all()] + \
            [(str(i), str(i)) for i in ExpTank.objects.all()] + \
          [(str(i), str(i)) for i in Chiller.objects.all()] + \
          [(str(i), str(i)) for i in CoolingTower.objects.all()] + \
          [(str(i), str(i)) for i in AirConditioner.objects.all()] + \
            [(str(i), str(i)) for i in Generator.objects.all()]


class Repair(models.Model):
    date = models.DateField(default=timezone.now)
    device = models.CharField(choices=tuple(devices), max_length=30)
    repairman = models.CharField(choices=REPAIRMANS, max_length=30, blank=True, null=True)
    explain = models.CharField(max_length=200, blank=True, null=True)