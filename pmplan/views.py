from django.core.paginator import Paginator
from django.shortcuts import render, redirect
from django.forms import modelformset_factory
from django import forms
from .models import Checklist, Checklists
from .forms import ChecklistForm
from .tables import PMTable
from django_tables2 import SingleTableView
from django.core import serializers
from django.http import JsonResponse, HttpResponse
from django.contrib.auth.decorators import login_required
from django.contrib import messages
from django.template import loader


@login_required
def pm_page(request):
    return render(request, 'pmplan/pmplan_page.html', {'topic': 'رنامه تعمیر و نگهداری'})


@login_required
def checklists(request):
    if request.method == 'POST':
        data = Checklist.objects.get(id=request.POST.get('id'))
        data = serializers.serialize('json', [data, ])
        return JsonResponse({'data': data}, status=200)

    else:
        plan_list = Checklist.objects.all()
        serialized_data = serializers.serialize('json', plan_list)
        context = {'data': [serialized_data,], 'topic': 'لیست تعمیرات و نگهداری پیشگیرانه'}

        for i in plan_list.order_by('date'):
            if i.status is not True:
                messages.warning(request, message=f'چک لیست {i} تکمیل نشده است')

        return JsonResponse(data=context, status=200)


@login_required
def checklist(request, object_id):
    checklist_item = Checklist.objects.get(id=object_id)

    checklist_formset = modelformset_factory(Checklists,
                                             fields='__all__',
                                             extra=0,
                                             widgets={'device': forms.HiddenInput,
                                                      'code': forms.HiddenInput,
                                                      'location': forms.HiddenInput,
                                                      'duty': forms.HiddenInput,
                                                      'factor': forms.HiddenInput,
                                                      'status': forms.RadioSelect(choices=((True, ''), (False, '')), attrs={'class': 'form-checkbox'}),
                                                      'explain': forms.TextInput(attrs={'class': 'form-text'}),
                                                      'checklist': forms.HiddenInput})

    if request.method == 'POST':
        checklist_forms = checklist_formset(request.POST, queryset=Checklists.objects.filter(checklist=checklist_item),
                                            prefix='PM')  # .errors
        print(checklist_forms.errors)
        checklist_form = ChecklistForm(instance=checklist_item, data=request.POST)

        if checklist_form.is_valid():
            checklist_form.save()

        if checklist_forms.is_valid():
            for form in checklist_forms:
                form.save()

            return redirect('pmplan:checklists')

    else:
        checklist_forms = checklist_formset(queryset=Checklists.objects.filter(checklist=checklist_item), prefix='PM')

        checklist_form = ChecklistForm(instance=checklist_item)

    topic = str(checklist_item.date) + '  /  ' + str(checklist_item.get_duration_display()) + '  /  ' + str(checklist_item.get_season_display())
    return render(request, 'pmplan/boiler.html', context={'checklist': checklist_form,
                                                          'checklists': checklist_forms,
                                                          'topic': topic })
