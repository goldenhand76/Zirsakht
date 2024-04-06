from django.db import models
import datetime
from persiantools.jdatetime import JalaliDate
from identity.models import Boiler, Stiff, Pump, DomesticHotWater
from django_jalali.db import models as jmodels

DURATION = (('1', 'روزانه'), ('2', 'هفتگی'), ('3', 'ماهانه'), ('4', 'سالانه'))
SEASON = (('1', 'زمستان'), ('2', 'تابستان'), ('3', 'چهارفصل'))


class Checklist(models.Model):
    date = jmodels.jDateField(verbose_name='تاریخ', auto_now_add=True)
    season = models.CharField(verbose_name='فصل',choices=SEASON, max_length=10)
    duration = models.CharField(verbose_name='مدت', choices=DURATION, max_length=10)
    status = models.BooleanField(verbose_name='وضعیت', default=False)

    class Meta:
        verbose_name_plural = "چک لیست"

    def __str__(self):
        return f'{self.id}_{self.get_duration_display()}'


class Checklists(models.Model):
    checklist = models.ForeignKey(Checklist, on_delete=models.CASCADE)
    device = models.CharField(verbose_name='تجهیز', max_length=20)
    code = models.CharField(verbose_name='کد تجهیز', max_length=10)
    location = models.CharField(verbose_name='موقعیت', max_length=20)
    duty = models.TextField(verbose_name='ملاحظات', max_length=100)
    factor = models.IntegerField(verbose_name='ضریب', )
    status = models.BooleanField(verbose_name='وضعیت', blank=True, null=True)
    explain = models.TextField(verbose_name='توضیحات', max_length=100, blank=True, null=True)

    class Meta:
        verbose_name_plural = "چک لیست ها"

    def __str__(self):
        return f'{self.id}_{self.checklist}'

# STATUS = (('1', 'روشن'), ('2', 'خاموش'))
#
#
# class BoilerDaily(models.Model):
#     date = models.DateField(auto_now_add=True)
#     checklist = models.ForeignKey(Checklist, on_delete=models.CASCADE, blank=True, null=True)
#
#     num = models.ForeignKey(Boiler, on_delete=models.CASCADE)
#
#     status = models.CharField(max_length=10, choices=STATUS, blank=True, null=True)
#     pressure = models.FloatField(blank=True, null=True)
#     Temperature = models.IntegerField(blank=True, null=True)
#
#     thermostat = models.BooleanField(blank=True, null=True)
#     exhaust = models.BooleanField(blank=True, null=True)
#     leak = models.BooleanField(blank=True, null=True)
#
#     blower = models.BooleanField(blank=True, null=True)
#     appearance = models.BooleanField(blank=True, null=True)
#
#     comment = models.TextField(max_length=50, blank=True, null=True)
#
#     def __str__(self):
#         return str(self.date)
#
#
# class StiffDaily(models.Model):
#     date = models.DateField(auto_now_add=True)
#     checklist = models.ForeignKey(Checklist, on_delete=models.CASCADE, blank=True, null=True)
#     num = models.ForeignKey(Stiff, on_delete=models.CASCADE)
#     status = models.CharField(max_length=10, choices=STATUS, blank=True, null=True)
#
#     stiff_in = models.FloatField(blank=True, null=True)
#     stiff_out = models.FloatField(blank=True, null=True)
#
#     salt_tank = models.BooleanField(blank=True, null=True)
#     revival = models.BooleanField(blank=True, null=True)
#     valves = models.BooleanField(blank=True, null=True)
#
#     appearance = models.BooleanField(blank=True, null=True)
#
#     comment = models.TextField(max_length=50, blank=True, null=True)
#
#     def __str__(self):
#         return str(self.date)
#
#
# class PumpDaily(models.Model):
#     date = models.DateField(auto_now_add=True)
#     checklist = models.ForeignKey(Checklist, on_delete=models.CASCADE, blank=True, null=True)
#     num = models.ForeignKey(Pump, on_delete=models.CASCADE)
#     status = models.CharField(max_length=10, choices=STATUS, blank=True, null=True)
#
#     input_pressure = models.FloatField(blank=True, null=True)
#     output_pressure = models.FloatField(blank=True, null=True)
#     amper = models.FloatField(blank=True, null=True)
#
#     valve = models.BooleanField(blank=True, null=True)
#     filter = models.BooleanField(blank=True, null=True)
#     coupling = models.BooleanField(blank=True, null=True)
#     leak = models.BooleanField(blank=True, null=True)
#     noise_vibration = models.BooleanField(blank=True, null=True)
#     gage = models.BooleanField(blank=True, null=True)
#
#     appearance = models.BooleanField(blank=True, null=True)
#
#     comment = models.TextField(max_length=50, blank=True, null=True)
#
#     def __str__(self):
#         return str(self.date)
#
#
# class DomesticHotWaterDaily(models.Model):
#     date = models.DateField(auto_now_add=True)
#     checklist = models.ForeignKey(Checklist, on_delete=models.CASCADE, blank=True, null=True)
#     num = models.ForeignKey(DomesticHotWater, on_delete=models.CASCADE)
#     status = models.CharField(max_length=10, choices=STATUS, blank=True, null=True)
#
#     coil_in_temp = models.IntegerField(blank=True, null=True)
#     coil_out_temp = models.IntegerField(blank=True, null=True)
#     cons_in_temp = models.IntegerField(blank=True, null=True)
#     cons_out_temp = models.IntegerField(blank=True, null=True)
#     circulate_amp = models.FloatField(blank=True, null=True)
#     valve = models.BooleanField(blank=True, null=True)
#     appearance = models.BooleanField(blank=True, null=True)
#
#     comment = models.TextField(max_length=50, blank=True, null=True)
#
#     def __str__(self):
#         return str(self.date)
