from django.db import models
from django.utils import timezone
import datetime

# Create your models here.
class AirCondition(models.Model):
    number = models.IntegerField()
    temp = models.IntegerField()
    pressure = models.IntegerField(default=90000)
    speed = models.IntegerField(default=10)
    angle = models.IntegerField(default=0)
    time = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"AirCondition_temp_{self.number}"


class Motor(models.Model):
    number = models.IntegerField()
    status = models.BooleanField()
    auto = models.BooleanField(default=False)
    min_temp = models.IntegerField(default=10)
    max_temp = models.IntegerField(default=25)
    start_time = models.TimeField(default=datetime.time(7, 30, 0))
    stop_time = models.TimeField(default=datetime.time(4, 30, 0))


class RoomPost(models.Model):
    number = models.IntegerField()
    temp = models.IntegerField()
    time = models.DateTimeField(auto_now_add=True)
    def __str__(self):
        return f"room_{self.number}"


class RoomGet(models.Model):
    number = models.IntegerField()
    description = models.CharField(max_length=50, default='')
    min_temp = models.IntegerField()
    max_temp = models.IntegerField()
    start = models.TimeField()
    stop = models.TimeField()
    status = models.BooleanField()
    auto = models.BooleanField(default=False)
    def __str__(self):
        return f"room_{self.number}"