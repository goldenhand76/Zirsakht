# Generated by Django 3.1.2 on 2020-12-07 04:37

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0018_airconditioner'),
    ]

    operations = [
        migrations.AlterField(
            model_name='repair',
            name='date',
            field=models.DateField(default=django.utils.timezone.now),
        ),
    ]
