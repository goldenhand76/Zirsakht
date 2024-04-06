from django.shortcuts import render
from .models import Boiler, Pump, PumpLinear, DomesticHotWater, Stiff, ExpTank, Chiller, AirConditioner, Repair, RepairMan, Generator, CoolingTower
from .forms import RepairForm
from django.http import JsonResponse, HttpResponse
from django.core import serializers
from persiantools.jdatetime import JalaliDate
from django.contrib.auth.decorators import login_required
from django.contrib import messages


@login_required
def identity_card(request):
    from pmplan.models import Checklist
    for i in Checklist.objects.all().order_by('date'):
        if i.status is not True:
            messages.warning(request, message=f'چک لیست {i} تکمیل نشده است')
    return render(request, 'identity/identity.html', {'topic': 'کارت شناسایی تجهیزات'})


@login_required
def add_repair(request):
    repairman = RepairMan.objects.get(id=request.POST.get('repair_man'))
    date = str(request.POST.get('repair_date')).split('/')
    day = int(date[2])
    month = int(date[1])
    year = int(date[0])
    date = JalaliDate(year, month, day).to_gregorian()
    Repair(date=date, repairman=repairman, device=request.POST.get('repair_device'), explain=request.POST.get('repair_explain')).save()

    return HttpResponse("OK")


@login_required
def edit_repair(request, pk):
    repair = Repair.objects.get(id=pk)
    if request.method != 'POST':
        form = RepairForm(instance=repair)
    else:
        form = RepairForm(instance=repair, data=request.POST)
        if form.is_valid():
            form.save()
    return render(request, 'identity/edit_repair.html', context={'repair': repair, 'form': form})


@login_required
def sort_repair(request):
    if request.method == 'POST':
        device = request.POST.get('device')
        field = request.POST.get('field')
        ascending = request.POST.get('ascending')
        if ascending == 'true':
            repairs = Repair.objects.filter(device=device).order_by(f'{field}')
        else:
            repairs = Repair.objects.filter(device=device).order_by(f'-{field}')
        repairs = serializers.serialize('json', repairs, )

    return JsonResponse({'repairs': repairs}, status=200)


@login_required
def boiler_page(request):
    if request.method == 'POST':
        boiler = Boiler.objects.get(id=request.POST.get('boiler'))
        repairs = Repair.objects.filter(device=boiler).order_by('-date')
        boiler = serializers.serialize('json', [boiler, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse({'boiler': boiler, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'بویلر', 'boilers': Boiler.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/boiler.html', context=context)


@login_required
def pump_page(request):
    if request.method == 'POST':
        pump = Pump.objects.get(id=request.POST.get('pump'))
        repairs = Repair.objects.filter(device=pump).order_by('-date')
        pump = serializers.serialize('json', [pump, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'pump': pump, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'پمپ', 'pumps': Pump.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/pump.html', context=context)


@login_required
def pump_linear_page(request):
    if request.method == 'POST':
        pump = PumpLinear.objects.get(id=request.POST.get('pump'))
        repairs = Repair.objects.filter(device=pump).order_by('-date')
        pump = serializers.serialize('json', [pump, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'pump': pump, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'پمپ', 'pumps': PumpLinear.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/pumplinear.html', context=context)


@login_required
def domestic_hot_water_page(request):
    if request.method == 'POST':
        domestic_hot_water = DomesticHotWater.objects.get(id=request.POST.get('domestic_hot_water'))
        repairs = Repair.objects.filter(device=domestic_hot_water).order_by('-date')
        domestic_hot_water = serializers.serialize('json', [domestic_hot_water, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'domestic_hot_water': domestic_hot_water, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'منبع آب گرم', 'domestic_hot_waters': DomesticHotWater.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/domestic_hot_water.html', context=context)


@login_required
def stiff_page(request):
    if request.method == 'POST':
        water_softener = Stiff.objects.get(id=request.POST.get('water_softener'))
        repairs = Repair.objects.filter(device=water_softener).order_by('-date')
        water_softener = serializers.serialize('json', [water_softener, ])
        repairs = serializers.serialize('json', repairs, )
        return JsonResponse(data={'water_softener': water_softener, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'سختی گیر', 'water_softeners': Stiff.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/water_softener.html', context=context)


@login_required
def expansion_page(request):
    if request.method == 'POST':
        expansion_tank = ExpTank.objects.get(id=request.POST.get('expansion_tank'))
        repairs = Repair.objects.filter(device=expansion_tank).order_by('-date')
        expansion_tank = serializers.serialize('json', [expansion_tank, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'expansion_tank': expansion_tank, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'منبع انبساط', 'expansion_tanks': ExpTank.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/expansion_tank.html', context=context)


@login_required
def chiller_page(request):
    if request.method == 'POST':
        chiller = Chiller.objects.get(id=request.POST.get('chiller'))
        repairs = Repair.objects.filter(device=chiller).order_by('-date')
        chiller = serializers.serialize('json', [chiller, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'chiller': chiller, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'چیلر', 'chillers': Chiller.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/chiller.html', context=context)


@login_required
def cooling_tower_page(request):
    if request.method == 'POST':
        cooling_tower = CoolingTower.objects.get(id=request.POST.get('cooling_tower'))
        repairs = Repair.objects.filter(device=cooling_tower).order_by('-date')
        cooling_tower = serializers.serialize('json', [cooling_tower, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'cooling_tower': cooling_tower, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'برج خنک کن', 'cooling_towers': CoolingTower.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/cooling_tower.html', context=context)


@login_required
def air_conditioner_page(request):
    if request.method == 'POST':
        air_conditioner = AirConditioner.objects.get(id=request.POST.get('air_conditioner'))
        repairs = Repair.objects.filter(device=air_conditioner).order_by('-date')
        air_conditioner = serializers.serialize('json', [air_conditioner, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'air_conditioner': air_conditioner, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'هواساز', 'air_conditioners': AirConditioner.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/air_conditioner.html', context=context)


@login_required
def generator_page(request):
    if request.method == 'POST':
        generator = Generator.objects.get(id=request.POST.get('generator'))
        repairs = Repair.objects.filter(device=generator).order_by('-date')
        generator = serializers.serialize('json', [generator, ])
        repairs = serializers.serialize('json', repairs, )

        return JsonResponse(data={'generator': generator, 'repairs': repairs}, status=200)
    else:
        context = {'topic': 'هواساز', 'generators': Generator.objects.all(), 'repairmans': RepairMan.objects.all()}
        return render(request, 'identity/generator.html', context=context)


@login_required
def fancoil_page(request):
    pass