# Generated by Django 3.1.2 on 2021-01-28 13:12

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('esp8266', '0007_auto_20201222_0427'),
    ]

    operations = [
        migrations.AlterField(
            model_name='aircondition',
            name='pressure',
            field=models.IntegerField(default=90000),
        ),
        migrations.AlterField(
            model_name='aircondition',
            name='temp',
            field=models.IntegerField(),
        ),
    ]
