# Generated by Django 3.1.2 on 2020-11-06 08:03

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='boiler',
            name='card',
            field=models.FileField(blank=True, null=True, upload_to=''),
        ),
    ]
