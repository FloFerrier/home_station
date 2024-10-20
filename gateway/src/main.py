from config import config_read
from openweathermap import OpenWeatherMap
from database import Database
import os

def main() -> None:
    dir_path = os.path.dirname(os.path.realpath(__file__))
    config = config_read("{}/config.yaml".format(dir_path))

    weather = OpenWeatherMap()
    database = Database()

    response = weather.init(config["openweathermap"]["url"], config["openweathermap"]["key"], config["openweathermap"]["city"])
    if response["error"]["code"] != 200:
        print("Fail to init OpenWeatherMap [{}]:\"{}\"".format(response["error"]["code"], response["error"]["message"]))
        exit(0)

    database.init(config["influxdb"]["url"], config["influxdb"]["token"], config["influxdb"]["org"], config["influxdb"]["bucket"])

    response = weather.weather_get()
    if response["error"]["code"] != 200:
        print("Fail to get data OpenWeatherMap [{}]:\"{}\"".format(response["error"]["code"], response["error"]["message"]))
        exit(0)

    measurements = [
        {
            "timestamp": int(response["timestamp"]),
            "name": "home",
            "location": "outside",
            "source": "openweathermap-api",
            "type": "temperature",
            "unit": "degree celsius",
            "value": int(response["data"]["temperature"]),
        },
        {
            "timestamp": int(response["timestamp"]),
            "name": "home",
            "location": "outside",
            "source": "openweathermap-api",
            "type": "pressure",
            "unit": "hPa",
            "value": int(response["data"]["pressure"]),
        },
        {
            "timestamp": int(response["timestamp"]),
            "name": "home",
            "location": "outside",
            "source": "openweathermap-api",
            "type": "humidity",
            "unit": "%",
            "value": int(response["data"]["humidity"]),
        }
    ]

    for data in measurements:
        print(data)
        response = database.write(data)
        if response is not True:
            print("Fail to write data")

if __name__ == "__main__":
    main()