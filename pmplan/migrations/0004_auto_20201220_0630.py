# Generated by Django 3.1.2 on 2020-12-20 03:00

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('pmplan', '0003_auto_20201220_0404'),
    ]

    operations = [
        migrations.AlterField(
            model_name='checklists',
            name='device',
            field=models.CharField(max_length=20, verbose_name='تجهیز'),
        ),
        migrations.AlterField(
            model_name='checklists',
            name='explain',
            field=models.TextField(blank=True, max_length=100, null=True, verbose_name='توضیحات'),
        ),
    ]
