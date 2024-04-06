from django.shortcuts import render
from django.template import loader
from django.http import JsonResponse, HttpResponse
from django.contrib.auth.decorators import login_required
from django import template
from esp8266.models import AirCondition, Motor, RoomGet, RoomPost
from django.utils import timezone
from datetime import datetime, timedelta
from Zirsakht.influx import connect_database
import json
import os

@login_required
def index(request):
    context = {}
    context['segment'] = 'index'
    html_template = loader.get_template('Dashboard/templates/index.html')
    return HttpResponse(html_template.render(context, request))


@login_required
def airconditioner(request):
    device = request.POST.get('device')
    if request.method == 'POST':
        motor_objects = Motor.objects.all()
        db = connect_database()
        data = db.query("SELECT * FROM angle,speed,temp WHERE TIME > NOW() - 2h")

        temp_points = []
        speed_points = []
        angle_points = []

        temp_data = list(data.get_points(measurement='temp', tags={'device': device}))
        speed_data = list(data.get_points(measurement='speed', tags={'device': device}))
        angle_data = list(data.get_points(measurement='angle', tags={'device': device}))
        motor_data = list(data.get_points(measurement='motor', tags={'device': device}))

        for point in temp_data:
            temp_points.append({'x': point['time'], 'y': point['status']})
        for point in speed_data:
            speed_points.append({'x': point['time'], 'y': point['status']})
        for point in angle_data:
            angle_points.append({'x': point['time'], 'y': point['status']})

        return Json(Response(data={'temp_data': temp_points,
                                  'speed_data': speed_points,
                                  'angle_data': angle_points,}))


@login_required
def motors(request):
    if request.method == 'GET':
        motors = Motor.objects.all()
        numbers = []
        statuses = []
        autos = []
        temps = []
        dates = []
        influxDB = connect_database()
        data = influxDB.query(f"SELECT * FROM angle,speed,temp,motor WHERE TIME > NOW() - 2h")
        for motor in motors:
            temp_data = list(data.get_points(measurement='temp', tags={'device': f'AC{motor.number}'}))
            motor_data = list(data.get_points(measurement='motor', tags={'device': f'AC{motor.number}'}))
            temps.append(temp_data[-1]['status'])
            if motor_data[-1]['status'] == 0:
                statuses.append("خاموش")
            else:
                statuses.append("روشن")
            last = temp_data[-1]['time']
            numbers.append(str(motor.number))
            autos.append(str(motor.auto))
            try:
                last_update = datetime.strptime(last, "%Y-%m-%dT%H:%M:%SZ") + timedelta(hours=4, minutes=30)
            except AttributeError:
                last_update = timezone.now()
            dates.append(last_update)
        string_dates = [f'{t.hour}:{t.minute}:{t.second}' for t in dates]
        return JsonResponse(data={'motors': numbers,
                                  'statuses': statuses,
                                  'autos': autos,
                                  'temps': temps,
                                  'times': string_dates})


@login_required
def weather(request):
    BASE_DIR = os.path.dirname(os.path.abspath(os.path.abspath(__file__)))
    if request.method == 'GET':
        with open(BASE_DIR + '/Weather/five_day.json') as json_file:
            data = json.load(json_file)
            five_day_dates = [datetime.strptime(i["Date"], '%Y-%m-%dT%H:%M:%S+04:30') for i in data['DailyForecasts']]
            five_day_string_dates = [f'{i.month}/{i.day}' for i in five_day_dates]
            five_day_min_temps = [i['Temperature']['Minimum']['Value'] for i in data['DailyForecasts']]
            five_day_max_temps = [i['Temperature']['Maximum']['Value'] for i in data['DailyForecasts']]
        with open(BASE_DIR + '/Weather/twelve_hour.json') as json_file:
            data = json.load(json_file)
            half_day_dates = [datetime.strptime(i["DateTime"], '%Y-%m-%dT%H:%M:%S+04:30') for i in data]
            half_day_string_dates = [f'{i.hour}:{i.minute}' for i in half_day_dates]
            half_day_temps = [i["Temperature"]['Value'] for i in data]
    return JsonResponse(data={'half_day_dates': half_day_string_dates,
                              'half_day_temps': half_day_temps,
                              'five_day_dates': five_day_string_dates,
                              'five_day_min_temps': five_day_min_temps,
                              'five_day_max_temps': five_day_max_temps,})


def rooms(request):
    if request.method == 'POST':
        rooms = RoomGet.objects.all()
        numbers = []
        descriptions = []
        colors = []
        temps = []
        dates = []
        influxDB = connect_database()
        data = influxDB.query(f"SELECT * FROM temp, set_temp WHERE TIME > NOW() - 2h")
        for room in rooms:
            temp_data =list(data.get_points(measurement='temp', tags={'device': f'ROOM{room.number}'}))
            numbers.append(str(room.number))
            descriptions.append(str(room.description))
            temps.append(temp_data[-1]['status'])
            colors.append("00FF00")
            last = temp_data[-1]['time']

            try:
                last_update = datetime.strptime(last, "%Y-%m-%dT%H:%M:%SZ") + timedelta(hours=4, minutes=30)
            except AttributeError:
                last_update = timezone.now()

            dates.append(last_update)

        string_times = [f'{t.hour}:{t.minute}:{t.second}' for t in dates]
        data = {'rooms': numbers,
                'descriptions': descriptions,
                'colors': colors,
                'temps': temps,
                'times': string_times}
        return JsonResponse(data=data)


def room_chart(request):
    room = request.POST.get('room')
    if request.method == 'POST':
        db = connect_database()
        data = db.query(f"SELECT * FROM temp, set_temp WHERE TIME > NOW() - 2h")
        temp_points = []
        set_points = []

        temp_data = list(data.get_points(measurement='temp', tags={'device': f"ROOM{room}"}))
        set_data = list(data.get_points(measurement='set_temp', tags={'device': f"ROOM{room}"}))

        for point in temp_data:
            temp_points.append({'x': point['time'], 'y': point['status']})
        for point in set_data:
            set_points.append({'x': point['time'], 'y': point['status']})

        print(set_data)
        return JsonResponse(data={'temp_data': temp_points,
                                  'set_data': set_points})


@login_required
def pages(request):
    context = {}
    try:
        load_template = request.path.split('/')[-1]
        context['segment'] = load_template

        html_template = loader.get_template('Dashboard/templates/' + load_template)
        return HttpResponse(html_template.render(context, request))

    except template.TemplateDoesNotExist:

        html_template = loader.get_template('Dashboard/templates/page-404.html')
        return HttpResponse(html_template.render(context, request))

    except:
        html_template = loader.get_template('Dashboard/templates/page-500.html')
        return HttpResponse(html_template.render(context, request))
