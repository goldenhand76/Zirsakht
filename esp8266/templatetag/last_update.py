from django import template
register = template.Library()


@register.simple_tag
def last(object):
    if object.seconds < 60:
        return f'{int(object.seconds)} ثانیه قبل'
    elif object.seconds < 3600:
        return f'{int(object.seconds / 60)} دقیقه قبل'
    else:
        return f'{int(object.seconds / 3600)} ساعت قبل'