from django import template
from persiantools.jdatetime import JalaliDate
from pmplan.models import Checklists
register = template.Library()


@register.simple_tag
def get_verbose_field_name(object, field_name):
    return object._meta.get_field(field_name).verbose_name.title()


@register.simple_tag
def georgian_to_jalali(object):
    if object.date is not None:
        return JalaliDate.to_jalali(object.date.year, object.date.month, object.date.day).strftime("%Y/%m/%d")


@register.simple_tag
def ltr(object):
    out = str(object).split('-')
    out = '/'.join(out)
    return out


@register.simple_tag
def point(object):
    df = Checklists.objects.filter(checklist=object)
    points = 0
    all = 0
    for i in df:
        if i.status is True:
            points += i.factor
        all += i.factor

    try:
        return int(points * 100 / all)
    except ZeroDivisionError:
        return 0


@register.simple_tag
def percent(object):
    df = Checklists.objects.filter(checklist=object)
    percents = 0
    all = 0
    for i in df:
        if i.status is not None:
            percents += 1
        all += 1
    try:
        return int(percents * 100 / all)
    except ZeroDivisionError:
        return 0