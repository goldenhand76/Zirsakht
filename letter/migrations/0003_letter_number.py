# Generated by Django 3.1.2 on 2020-11-22 00:47

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('letter', '0002_letter_text'),
    ]

    operations = [
        migrations.AddField(
            model_name='letter',
            name='number',
            field=models.CharField(blank=True, max_length=10, null=True),
        ),
    ]
