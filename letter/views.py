from django.shortcuts import render
from .models import Letter, Structure, Establish
from django.core import serializers
from django.http import JsonResponse, HttpResponse, HttpResponseRedirect
from django.template import loader, RequestContext
from persiantools.jdatetime import JalaliDate
from django.contrib.auth.decorators import login_required
from django.contrib import messages

@login_required
def letter(request):
    from pmplan.models import Checklist, Checklists
    for i in Checklist.objects.all().order_by('date'):
        if i.status is not True:
            messages.warning(request, message=f'چک لیست {i} تکمیل نشده است')
    return render(request, 'home/index.html', {'topic': 'پنل کاربری',
                                               'letters': Letter.objects.filter(status = None),
                                               'structures': Structure.objects.all().order_by('-number'),
                                               'establishes': Establish.objects.all().order_by('-number')})


@login_required
def sort_letter(request):
    if request.method == 'POST':
        field = request.POST.get('field')
        ascending = request.POST.get('ascending')
        if ascending == 'true':
            letters = Letter.objects.all().order_by(f'{field}')
        else:
            letters = Letter.objects.all().order_by(f'-{field}')

        data = loader.render_to_string(template_name='home/index.html', context={'topic': 'پنل کاربری',
                                               'letters': letters,
                                               'structures': Structure.objects.all(),
                                               'establishes': Establish.objects.all()})
        return JsonResponse(data, safe=False)


@login_required
def add_letter(request):
    subject = request.POST.get('subject')
    receiver = request.POST.get('receiver')
    number = request.POST.get('number')

    date = str(request.POST.get('date')).split('/')
    day = int(date[2])
    month = int(date[1])
    year = int(date[0])
    date = JalaliDate(year, month, day).to_gregorian()

    deadline = int(request.POST.get('deadline'))
    text = request.POST.get('text')
    Letter(date=date, subject=subject, receiver=receiver, number=number, deadline=deadline, text=text).save()

    return HttpResponse('Ok')


@login_required
def add_structure(request):
    subject = request.POST.get('subject')
    responsible = request.POST.get('responsible')
    number = request.POST.get('number')

    date = str(request.POST.get('date')).split('/')
    day = int(date[2])
    month = int(date[1])
    year = int(date[0])
    date = JalaliDate(year, month, day).to_gregorian()

    deadline = int(request.POST.get('deadline'))
    text = request.POST.get('text')
    Structure(date=date, subject=subject, responsible=responsible, number=number, deadline=deadline, text=text).save()

    return HttpResponse('Ok')


@login_required
def add_establish(request):
    subject = request.POST.get('subject')
    responsible = request.POST.get('responsible')
    number = request.POST.get('number')

    date = str(request.POST.get('date')).split('/')
    day = int(date[2])
    month = int(date[1])
    year = int(date[0])
    date = JalaliDate(year, month, day).to_gregorian()

    deadline = int(request.POST.get('deadline'))
    text = request.POST.get('text')
    Establish(date=date, subject=subject, responsible=responsible, number=number, deadline=deadline, text=text).save()

    return HttpResponse('Ok')
