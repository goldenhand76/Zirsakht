# Generated by Django 3.1.2 on 2020-11-20 16:48

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Letter',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('date', models.DateField(blank=True, null=True)),
                ('subject', models.CharField(blank=True, max_length=50, null=True)),
                ('receiver', models.CharField(blank=True, max_length=30, null=True)),
                ('deadline', models.IntegerField(blank=True, null=True)),
                ('status', models.BooleanField(blank=True, null=True)),
            ],
        ),
    ]
