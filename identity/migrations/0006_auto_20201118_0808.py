# Generated by Django 3.1.2 on 2020-11-18 04:38

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0005_pump'),
    ]

    operations = [
        migrations.AlterField(
            model_name='boiler',
            name='card',
            field=models.ImageField(blank=True, null=True, upload_to='media/identity/boiler'),
        ),
    ]
