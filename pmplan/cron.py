from pmplan.models import Checklist, Checklists
from identity.models import Boiler, Stiff, Pump, DomesticHotWater, AirConditioner, ExpTank, Chiller, CoolingTower
import pandas as pd
import os
from Zirsakht import settings
from .python_code.xlsx_to_csv import read_excel

DURATION = {1: 'روزانه', 2: 'هفتگی', 3: 'ماهانه', 4: 'سالانه'}
SEASON = {1: 'زمستان', 2: 'تابستان', 3: 'چهارفصل'}


def create_checklist(season, duration):
    Checklist(status=False, duration=duration, season=season).save()
    checklist_item = Checklist.objects.latest('id')
    BASE_DIR = os.path.dirname(os.path.abspath(__file__))
    df = read_excel(BASE_DIR + '/python_code/تعمیرات نگهداری نهایی.xlsx', DURATION[duration], SEASON[season])

    for code in Boiler.objects.filter(status=True):
        for index in df.loc[df['device'] == 'دیگ و مشعل'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in Stiff.objects.filter(status=True):
        for index in df.loc[df['device'] == 'سختی گیر'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in Pump.objects.filter(status=True):
        for index in df.loc[df['device'] == 'پمپ'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in DomesticHotWater.objects.filter(status=True):
        for index in df.loc[df['device'] == 'منبع کویلی'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in AirConditioner.objects.filter(status=True):
        for index in df.loc[df['device'] == 'هواساز'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in ExpTank.objects.filter(status=True):
        for index in df.loc[df['device'] == 'منبع انبساط'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in Chiller.objects.filter(status=True):
        for index in df.loc[df['device'] == 'چیلر'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    for code in CoolingTower.objects.filter(status=True):
        for index in df.loc[df['device'] == 'برج خنک کن'].index:
            Checklists(checklist=checklist_item, device=df['device'][index], code=code, location=code.location, duty=df['duty'][index], factor=df['factor'][index], status=None).save()
    pass
