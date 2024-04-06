import requests
import json

url = 'http://127.0.0.1:8000/%2Fmonitor/add'
data = {'number': '1', 'temp': '23'}
x = requests.post(url, json.dumps(data))

'chown www-data:www-data /var/www/Zirsakht/Zirsakht'
'chown www-data:www-data /var/www/Zirsakht/Zirsakht/db.sqlite3'