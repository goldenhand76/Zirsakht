from django.shortcuts import render, redirect
from .models import AirCondition, Motor, RoomGet, RoomPost
from django.http import JsonResponse, HttpResponse
from .forms import AirConditionForm, RoomForm
import json
from django.core import serializers
from django.utils import timezone
from django.contrib.auth.decorators import login_required
from django.contrib import messages
from datetime import datetime, timedelta
from Zirsakht.influx import connect_database


def motor(request):
    m = Motor.objects.get(id=1)
    try:
        temp = AirCondition.objects.last().temp
        if m.auto is True:
            if m.start_time < timezone.now().time() < m.stop_time:
                if temp < m.min_temp:
                    m.status = False
                if temp > m.max_temp:
                    m.status = True
            else:
                m.status = False
            m.save()
    except AttributeError:
        pass
    status = m.status
    auto = m.auto
    return JsonResponse({'status': status, 'auto': auto})


@login_required
def change(request):
    if request.method == 'POST':
        m = Motor.objects.get(id=1)
        if request.POST.get("status") == 'false':
            m.status = False
        else:
            m.status = True
        if request.POST.get("auto") == 'false':
            m.auto = False
        else:
            m.auto = True
        m.save()
        status = m.status
        auto = m.auto
        return JsonResponse({'status': status, 'auto': auto})
    m = Motor.objects.get(id=1)
    return JsonResponse(data={m})


@login_required
def aircondition_1(request):
    numbers = []
    temps = []
    speeds = []
    angles = []
    dates = []
    if request.method == 'POST':
        for i in reversed(AirCondition.objects.filter(number=1).order_by('-time')[:300]):
            temps.append(i.temp)
            speeds.append(i.speed)
            angles.append(i.angle)
            dates.append(i.time)
        string_dates = [f'{j.hour}:{j.minute}' for j in dates]
        return JsonResponse(data={'number': numbers,
                                  'temps': temps,
                                  'speeds': speeds,
                                  'angles': angles,
                                  'dates': string_dates,})
    else:
        try:
            last_update = AirCondition.objects.last().time
        except AttributeError:
            last_update = timezone.now()
        now = timezone.now()
        last_update = now - last_update

        from pmplan.models import Checklist, Checklists
        for i in Checklist.objects.all().order_by('date'):
            if i.status is not True:
                messages.warning(request, message=f'چک لیست {i} تکمیل نشده است')

        return render(request, 'esp8266/monitor.html', {'topic': 'مانیتورینگ', 'last_update': last_update})


def room_get(request):
    if request.method == 'POST':
        room = RoomGet.objects.get(number=request.POST.get('room'))
        room = serializers.serialize('json', [room, ])
        return JsonResponse(data={'room': room}, status=200)


def room_post(request):
    if request.method == 'POST':
        room = RoomGet.objects.get(number=request.POST.get('room'))
        room.min_temp = int(request.POST.get('range')[0:2])
        room.max_temp = int(request.POST.get('range')[3:5])
        room.start = datetime.strptime(request.POST.get('start'), "%H:%M").time()
        room.stop = datetime.strptime(request.POST.get('stop'), "%H:%M").time()
        room.save()
        return HttpResponse("Successful", 200)


def room_subscribe(request):
    room = RoomGet.objects.get(id=1)
    try:
        influxDB = connect_database()
        query = influxDB.query(f"SELECT temp FROM FC WHERE room='611' ORDER BY time DESC LIMIT 1")
        for l in query:
            for d in l:
                temp = d["temp"]
        if room.auto is True:
            if room.start < timezone.now().time() < room.stop:
                if temp < room.min_temp:
                    room.status = False
                if temp > room.max_temp:
                    room.status = True
            else:
                room.status = False
            room.save()
    except AttributeError:
        pass
    number = room.number
    status = room.status
    auto = room.auto
    min_temp = room.min_temp
    max_temp = room.max_temp
    start = room.start
    stop = room.stop
    t = timezone.now().time()
    return JsonResponse({'number': number,
                         'time': t,
                         'status': status,
                         'auto': auto,
                         'min_temp': min_temp,
                         'max_temp': max_temp,
                         'start': start,
                         'stop': stop},)


def room_map(request):
    return render(request, 'esp8266/room_map.html')
