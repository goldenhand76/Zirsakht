# Generated by Django 3.1.2 on 2020-12-20 04:40

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('pmplan', '0007_auto_20201220_0808'),
    ]

    operations = [
        migrations.AlterField(
            model_name='checklists',
            name='status',
            field=models.BooleanField(blank=True, max_length=10, null=True, verbose_name='وضعیت'),
        ),
    ]
