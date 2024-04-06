from django.db import models
import datetime
# Create your models here.


class Letter(models.Model):
    date = models.DateField(blank=True, null=True)
    number = models.CharField(max_length=10, blank=True, null=True)
    subject = models.CharField(max_length=50, blank=True, null=True)
    receiver = models.CharField(max_length=30, blank=True, null=True)
    deadline = models.IntegerField(blank=True, null=True)
    text = models.TextField(blank=True, null=True)
    status = models.BooleanField(blank=True, null=True)


class Establish(models.Model):
    date = models.DateField(blank=True, null=True)
    number = models.CharField(max_length=10, blank=True, null=True)
    subject = models.CharField(max_length=50, blank=True, null=True)
    responsible = models.CharField(max_length=30, blank=True, null=True)
    deadline = models.IntegerField(blank=True, null=True)
    text = models.TextField(blank=True, null=True)
    status = models.BooleanField(blank=True, null=True)


class Structure(models.Model):
    date = models.DateField(blank=True, null=True)
    number = models.CharField(max_length=10, blank=True, null=True)
    subject = models.CharField(max_length=50, blank=True, null=True)
    responsible = models.CharField(max_length=30, blank=True, null=True)
    deadline = models.IntegerField(blank=True, null=True)
    text = models.TextField(blank=True, null=True)
    status = models.BooleanField(blank=True, null=True)
