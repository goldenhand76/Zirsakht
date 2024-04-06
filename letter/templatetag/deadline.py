from django import template
from persiantools.jdatetime import JalaliDate
from datetime import timedelta
from datetime import datetime

register = template.Library()


@register.simple_tag
def jalali_date(date):
    return JalaliDate.to_jalali(date.year, date.month, date.day)


@register.simple_tag
def deadline(letter):
    if letter.status is not None:
        return 'انجام شد'
    else:
        if ((letter.date + timedelta(days=letter.deadline)) - datetime.now().date()).days + 1 > 0:
            return str(((letter.date + timedelta(days=letter.deadline)) - datetime.now().date()).days + 1) +  ' روز مانده'
        else:
            return 'پایان مهلت تعیین شده'


@register.simple_tag
def status(status):
    if status is None:
        return 'static/assets/img/136-1364020_transparent-checkmark-png-transparent-background-linkedin-icon-white.png'
    else:
        return 'static/assets/img/356-3568543_check-icon-green-tick-hd-png-download.png'


@register.simple_tag
def color(letter):
    if letter.status is not None:
        return 'green'
    else:
        if ((letter.date + timedelta(days=letter.deadline)) - datetime.now().date()).days + 1 > 1:
            return 'green'
        else:
            return 'red'