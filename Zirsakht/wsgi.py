"""
WSGI config for Zirsakht project.

It exposes the WSGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/3.1/howto/deployment/wsgi/
"""

import os
import sys
from django.core.wsgi import get_wsgi_application

BASE_PATH = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

sys.path.append(BASE_PATH)
sys.path.append(BASE_PATH + '/venv/lib/python3.6/site-packages')

# sys.path.append('/var/www/Zirsakht/Zirsakht')
# sys.path.append('/var/www/Zirsakht/Zirsakht/venv/lib/python3.8/site-packages')

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'Zirsakht.settings')

application = get_wsgi_application()
