from django.urls import path
from . import views
from django.views.decorators.csrf import csrf_exempt

app_name = 'letter'
urlpatterns = [
    path('letter/', views.letter, name='index'),
    path('sort_letter/', csrf_exempt(views.sort_letter), name='sort_letter'),
    path('add_letter/', csrf_exempt(views.add_letter), name='add_letter'),
    path('add_structure/', csrf_exempt(views.add_structure), name='add_structure'),
    path('add_establish/', csrf_exempt(views.add_establish), name='add_establish'),

]
