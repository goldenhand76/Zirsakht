# Generated by Django 3.1.2 on 2021-01-28 14:52

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('esp8266', '0008_auto_20210128_1642'),
    ]

    operations = [
        migrations.AddField(
            model_name='aircondition',
            name='angle',
            field=models.IntegerField(default=0),
        ),
        migrations.AddField(
            model_name='aircondition',
            name='speed',
            field=models.IntegerField(default=10),
        ),
    ]
