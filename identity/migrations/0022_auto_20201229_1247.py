# Generated by Django 3.1.2 on 2020-12-29 09:17

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0021_auto_20201229_1246'),
    ]

    operations = [
        migrations.AlterField(
            model_name='repair',
            name='device',
            field=models.CharField(choices=[('B001', 'B001'), ('B002', 'B002'), ('B003', 'B003'), ('B004', 'B004'), ('B101', 'B101'), ('B102', 'B102'), ('P001', 'P001'), ('P002', 'P002'), ('P003', 'P003'), ('P004', 'P004'), ('P005', 'P005'), ('P006', 'P006'), ('P007', 'P007'), ('P008', 'P008'), ('P009', 'P009'), ('P010', 'P010'), ('P011', 'P011'), ('P012', 'P012'), ('P013', 'P013'), ('P014', 'P014'), ('P015', 'P015'), ('P101', 'P101'), ('P102', 'P102'), ('P103', 'P103'), ('D001', 'D001'), ('D002', 'D002'), ('D101', 'D101'), ('D102', 'D102'), ('S001', 'S001')], max_length=30),
        ),
    ]
