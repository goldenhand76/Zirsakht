# Generated by Django 3.1.2 on 2020-12-02 04:53

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0013_auto_20201202_0731'),
    ]

    operations = [
        migrations.AlterField(
            model_name='repairman',
            name='field',
            field=models.CharField(blank=True, choices=[('برق', 'برق'), ('مکانیک', 'مکانیک')], max_length=10, null=True),
        ),
    ]
