# Generated by Django 3.1.2 on 2020-12-26 00:29

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('pmplan', '0010_auto_20201222_0349'),
    ]

    operations = [
        migrations.AlterField(
            model_name='checklists',
            name='status',
            field=models.BooleanField(blank=True, null=True, verbose_name='وضعیت'),
        ),
    ]
