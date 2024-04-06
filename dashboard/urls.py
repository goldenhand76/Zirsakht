from django.urls import path, re_path
from django.views.decorators.csrf import csrf_exempt
from . import views

app_name = 'dashboard'
urlpatterns = [
    path('', views.index, name='home'),
    path('airconditioner/', csrf_exempt(views.airconditioner), name='airconditioner'),
    path('room-chart/', csrf_exempt(views.room_chart), name='room_chart'),
    path('motor/', csrf_exempt(views.motors), name='motor'),
    path('room/', csrf_exempt(views.rooms), name='room'),
    path('weather/', csrf_exempt(views.weather), name='weather'),
    # re_path(r'^.*\.*', views.pages, name='pages'),
]
