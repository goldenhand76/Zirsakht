# Generated by Django 3.1.2 on 2021-02-15 09:06

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('identity', '0028_coolingtower'),
    ]

    operations = [
        migrations.AlterField(
            model_name='domestichotwater',
            name='device',
            field=models.CharField(default='منبع کویل دار', max_length=20),
        ),
        migrations.AlterField(
            model_name='repair',
            name='device',
            field=models.CharField(choices=[('B001', 'B001'), ('B002', 'B002'), ('B003', 'B003'), ('B004', 'B004'), ('B101', 'B101'), ('B102', 'B102'), ('P001', 'P001'), ('P002', 'P002'), ('P003', 'P003'), ('P004', 'P004'), ('P005', 'P005'), ('P006', 'P006'), ('P007', 'P007'), ('P008', 'P008'), ('P009', 'P009'), ('P010', 'P010'), ('P011', 'P011'), ('P012', 'P012'), ('P013', 'P013'), ('P014', 'P014'), ('P015', 'P015'), ('P101', 'P101'), ('P102', 'P102'), ('P103', 'P103'), ('LP001', 'LP001'), ('LP002', 'LP002'), ('LP003', 'LP003'), ('D001', 'D001'), ('D002', 'D002'), ('D101', 'D101'), ('D102', 'D102'), ('S001', 'S001'), ('E001', 'E001'), ('E002', 'E002'), ('E101', 'E101'), ('CH001', 'CH001'), ('CH002', 'CH002'), ('CH003', 'CH003'), ('CT001', 'CT001'), ('CT002', 'CT002'), ('CT003', 'CT003'), ('A001', 'A001'), ('A002', 'A002'), ('A003', 'A003'), ('A004', 'A004'), ('A005', 'A005'), ('A006', 'A006'), ('A007', 'A007'), ('A008', 'A008'), ('A009-1', 'A009-1'), ('A009-2', 'A009-2'), ('A101', 'A101'), ('A102', 'A102')], max_length=30),
        ),
    ]
