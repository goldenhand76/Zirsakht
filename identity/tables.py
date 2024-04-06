import django_tables2 as tables


class RepairTable(tables.Table):
    date = tables.Column(attrs={"td": {"align": "center", "id": "date"}})
    device = tables.Column(attrs={"td": {"align": "center"}})
    repairman = tables.Column(attrs={"td": {"align": "center"}})
    explain = tables.Column(attrs={"td": {"align": "center"}})


    class Meta:
        template_name = "django_tables2/bootstrap.html"
        sequence = ('date', 'device', 'repairman', 'explain',)