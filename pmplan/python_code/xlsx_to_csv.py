import pandas as pd
import xlrd
import os


def read_excel(file, dur, sea):
    xlsx = file
    wb = xlrd.open_workbook(xlsx)
    sheet = wb.sheet_by_index(0)

    season = []
    device = []
    duration = []
    duty = []
    factor = []
    for i in range(1, sheet.nrows):
        device.append(sheet.cell_value(i, 0))
        season.append(sheet.cell_value(i, 1))
        duration.append(sheet.cell_value(i, 2))
        duty.append(sheet.cell_value(i, 3))
        factor.append(sheet.cell_value(i, 4))
    df = pd.DataFrame({'device': device, 'season': season, 'duration': duration, 'duty': duty, 'factor': factor})
    print(df['duration'])

    return df.loc[df['duration'] == dur].loc[df['season'] == sea]
