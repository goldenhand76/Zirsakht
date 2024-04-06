from django.urls import path
from . import views
from django.views.decorators.csrf import csrf_exempt
from django.conf.urls.static import static
from django.conf import settings

app_name = 'identity'

urlpatterns = [
    path('identity_card/', csrf_exempt(views.identity_card), name='identity_card'),
    path('add_repair/', csrf_exempt(views.add_repair), name='add_repair'),
    path('sort_repair/', csrf_exempt(views.sort_repair), name='sort_repair'),
    path('edit_repair/<int:pk>/', csrf_exempt(views.edit_repair), name='edit_repair'),
    path('boiler/', csrf_exempt(views.boiler_page), name='boiler_page'),
    path('pump/', csrf_exempt(views.pump_page), name='pump_page'),
    path('pump_linear/', csrf_exempt(views.pump_linear_page), name='pump_linear_page'),
    path('domestic-hot-water/', csrf_exempt(views.domestic_hot_water_page), name='domestic_hot_water_page'),
    path('water-softener/', csrf_exempt(views.stiff_page), name='water_softener_page'),
    path('expansion-tank/', csrf_exempt(views.expansion_page), name='expansion_page'),
    path('chiller/', csrf_exempt(views.chiller_page), name='chiller_page'),
    path('cooling_tower/', csrf_exempt(views.cooling_tower_page), name='cooling_tower_page'),
    path('air-conditioner/', csrf_exempt(views.air_conditioner_page), name='air_conditioner_page'),
    path('generator/', csrf_exempt(views.generator_page), name='generator_page'),
    path('fancoil/', csrf_exempt(views.fancoil_page), name='fancoil_page')

]

if settings.DEBUG:
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)