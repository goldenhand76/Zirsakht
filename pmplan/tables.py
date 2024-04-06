import django_tables2 as tables
from django import forms


class PMTable(tables.Table):
    device = tables.Column(attrs={"td": {"align": "center"}, "th": {"align": "center"}})
    code = tables.Column(attrs={"td": {"align": "center"}})
    location = tables.Column(attrs={"td": {"align": "center"}})
    duty = tables.Column(attrs={"td": {"align": "right"}, "th": {"align": "right"}})
    # factor = tables.Column(attrs={"td": {"align": "center"}})
    status = tables.Column(attrs={"td": {"align": "center"}})

    class Meta:
        template_name = "django_tables2/semantic.html"
        sequence = ('status', 'location', 'duty', 'code', 'device')


