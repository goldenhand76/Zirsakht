from django.urls import path
from . import views
from django.views.decorators.csrf import csrf_exempt

app_name = 'pmplan'
urlpatterns = [
    path('pmplan/', csrf_exempt(views.pm_page), name='pm_page'),
    path('checklists/', csrf_exempt(views.checklists), name='checklists'),
    path('checklists/<int:object_id>/edit/', views.checklist, name='checklist_edit'),
]