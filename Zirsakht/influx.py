from django.conf import settings
from influxdb import InfluxDBClient
from django import template

register = template.Library()


def connect_database():
    client = InfluxDBClient(
        host=settings.INFLUXDB_HOST,
        port=settings.INFLUXDB_PORT,
        username=settings.INFLUXDB_USERNAME,
        password=settings.INFLUXDB_PASSWORD,
        database=settings.INFLUXDB_DATABASE,
        timeout=settings.INFLUXDB_TIMEOUT,
    )
    return client