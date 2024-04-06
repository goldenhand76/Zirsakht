import json
import requests
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
print(BASE_DIR)
one_day_url = 'http://dataservice.accuweather.com/forecasts/v1/daily/1day/209737?apikey=lGvM9tW0ZGQ00l9jNzNKzB7P1UFhxpTD&details=true&metric=true'
one_day = requests.get(one_day_url)
JSON = json.loads(one_day.text)
print(JSON)
with open(f'{BASE_DIR}/one_day.json', 'w') as json_file:
    json.dump(JSON, json_file)

five_day_url = 'http://dataservice.accuweather.com/forecasts/v1/daily/5day/209737?apikey=lGvM9tW0ZGQ00l9jNzNKzB7P1UFhxpTD&details=true&metric=true'
five_day = requests.get(five_day_url)
JSON = json.loads(five_day.text)
print(JSON)
with open(f'{BASE_DIR}/five_day.json', 'w') as json_file:
    json.dump(JSON, json_file)

twelve_hour_url = 'http://dataservice.accuweather.com/forecasts/v1/hourly/12hour/209737?apikey=lGvM9tW0ZGQ00l9jNzNKzB7P1UFhxpTD&details=true&metric=true'
twelve_hour = requests.get(twelve_hour_url)
JSON = json.loads(twelve_hour.text)
print(JSON)
with open(f'{BASE_DIR}/twelve_hour.json', 'w') as json_file:
    json.dump(JSON, json_file)


with open(f'{BASE_DIR}/one_day.json') as json_file:
    data = json.load(json_file)
    print(data['DailyForecasts'])
    print([i['Date'] for i in data['DailyForecasts']][0])
    print([i['Temperature']['Minimum']['Value'] for i in data['DailyForecasts']][0])
    print([i['Temperature']['Maximum']['Value'] for i in data['DailyForecasts']][0])
    print([i['RealFeelTemperature']['Minimum']['Value'] for i in data['DailyForecasts']][0])
    print([i['RealFeelTemperature']['Maximum']['Value'] for i in data['DailyForecasts']][0])
    print([f"https://www.accuweather.com/images/weathericons/{i['Day']['Icon']}.svg" for i in data['DailyForecasts']][0])
    print([i['Day']['RainProbability'] for i in data['DailyForecasts']][0])
    print([i['Day']['SnowProbability'] for i in data['DailyForecasts']][0])
    print([i['Day']['IceProbability'] for i in data['DailyForecasts']][0])
    print([i['Day']['Wind']['Speed']["Value"] for i in data['DailyForecasts']][0])

with open(f'{BASE_DIR}/five_day.json') as json_file:
    data = json.load(json_file)
    print(data['DailyForecasts'])
    print([i['Date'] for i in data['DailyForecasts']])
    print([i['Temperature']['Minimum']['Value'] for i in data['DailyForecasts']])
    print([i['Temperature']['Maximum']['Value'] for i in data['DailyForecasts']])
    print([i['RealFeelTemperature']['Minimum']['Value'] for i in data['DailyForecasts']])
    print([i['RealFeelTemperature']['Maximum']['Value'] for i in data['DailyForecasts']])
    print([f"https://www.accuweather.com/images/weathericons/{i['Day']['Icon']}.svg" for i in data['DailyForecasts']])
    print([i['Day']['RainProbability'] for i in data['DailyForecasts']])
    print([i['Day']['SnowProbability'] for i in data['DailyForecasts']])
    print([i['Day']['IceProbability'] for i in data['DailyForecasts']])
    print([i['Day']['Wind']['Speed']["Value"] for i in data['DailyForecasts']])

with open(f'{BASE_DIR}/twelve_hour.json') as json_file:
    data = json.load(json_file)
    print(data)
    print([i["DateTime"] for i in data])
    print([i["Temperature"]['Value'] for i in data])
    print([i["RealFeelTemperature"]['Value'] for i in data])
    print([f"https://www.accuweather.com/images/weathericons/{i['WeatherIcon']}.svg" for i in data])
    print([i["Wind"]["Speed"]['Value'] for i in data])
    print([i["RainProbability"] for i in data])
    print([i["SnowProbability"] for i in data])
    print([i["IceProbability"] for i in data])

