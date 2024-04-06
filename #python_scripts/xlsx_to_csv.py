import pandas as pd
import xlrd

xlsx = '225.xlsx'
wb = xlrd.open_workbook(xlsx)
sheet = wb.sheet_by_name('اطلاعات انشعابات شرکتها')

name = []

electric_power_coefficient = []
electric_branch = []
electric_voltag = []
electric_power = []

water_dia = []
water_allow = []

gas_model = []
gas_capacity = []

for i in range(2, sheet.nrows):
    name.append(str(sheet.cell_value(i, 0)))

    try:
        electric_power_coefficient.append(float(sheet.cell_value(i, 9)))
    except ValueError:
        electric_power_coefficient.append(0)

    electric_branch.append(str(sheet.cell_value(i, 10)))

    try:
        if sheet.cell_value(i, 11).split()[1] == 'kv':
            electric_voltag.append(int(sheet.cell_value(i, 11).split()[0]) * 1000)
        else:
            electric_voltag.append(int(sheet.cell_value(i, 11).split()[0]))
    except IndexError:
        electric_voltag.append(0)

    try:
        electric_power.append(int(sheet.cell_value(i, 12)))
    except ValueError:
        electric_power.append(0)

    water_dia.append(str(sheet.cell_value(i, 15)))

    try:
        water_allow.append(int(sheet.cell_value(i, 16)))
    except ValueError:
        water_allow.append(0)

    gas_model.append(str(sheet.cell_value(i, 20)))

    try:
        gas_capacity.append(int(sheet.cell_value(i, 19)))
    except ValueError:
        gas_capacity.append(0)


print(len(name), '  ', len(gas_capacity))
output = pd.DataFrame({'name': name,
                       'electric_power_coefficient': electric_power_coefficient,
                       'electric_branch': electric_branch,
                       'electric_voltag': electric_voltag,
                       'electric_power': electric_power,
                       'water_dia': water_dia,
                       'water_allow': water_allow,
                       'gas_model': gas_model,
                       'gas_capacity': gas_capacity,})
output.to_csv('225.csv')