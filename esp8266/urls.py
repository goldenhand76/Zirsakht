from django.urls import path
from django.conf.urls import url
from . import views
from django.views.decorators.csrf import csrf_exempt

app_name = 'esp8266'
urlpatterns = [
    path('monitor/', csrf_exempt(views.aircondition_1), name='aircondition_1'),
    path('monitor/motor', csrf_exempt(views.motor), name='motor'),
    path('monitor/change', csrf_exempt(views.change), name='change'),

    path('monitor/room', csrf_exempt(views.room_map), name='room_map'),
    path('monitor/room/get', csrf_exempt(views.room_get), name='room_get'),
    path('monitor/room/post', csrf_exempt(views.room_post), name='room_post'),
    path('monitor/room/subscribe', csrf_exempt(views.room_subscribe), name='room_subscribe'),
]
