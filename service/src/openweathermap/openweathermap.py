import requests

class OpenWeatherMap:
    def __init__(self) -> None:
        self.__api: dict = {
            "url": "",
            "key": "",
        }
        self.attr: dict = {
            "error": {
                "code": 0,
                "message": "",
            },
            "location": {
                "city": "",
                "latitude": 0.0,
                "longitude": 0.0,
            },
            "data": {
                "temperature": 0.0,
                "humidity": 0,
                "pressure": 0,
            },
            "timestamp": 0,
        }

    def requests_get(self, url: str = "") -> dict:
        return requests.get(url).json()

    def init(self, api_url: str = "", api_key: str = "", city_name: str = "") -> dict:
        self.__api["url"] = api_url
        self.__api["key"] = api_key
        self.attr["location"]["city"] = city_name
        self.attr["error"]["code"] = 200
        self.attr["error"]["message"] = "No error"
        url = "https://{}/geo/1.0/direct?q={}&limit=1&appid={}".format(self.__api["url"], self.attr["location"]["city"], self.__api["key"])
        data = self.requests_get(url)
        if "cod" in data:
            self.attr["error"]["code"] = data["cod"]
            self.attr["error"]["message"] = data["message"]
        else:
            self.attr["location"]["latitude"] = data[0]["lat"]
            self.attr["location"]["longitude"] = data[0]["lon"]
        return self.attr

    def weather_get(self) -> dict:
        url = "https://{}/data/2.5/weather?lat={}&lon={}&appid={}&units=metric".format(self.__api["url"], self.attr["location"]["latitude"], self.attr["location"]["longitude"], self.__api["key"])
        data = self.requests_get(url)
        self.attr["error"]["code"] = data["cod"]
        self.attr["error"]["message"] = data["msg"] if "message" in data else "No error"
        self.attr["timestamp"] = data["dt"]
        self.attr["data"]["temperature"] = data["main"]["temp"]
        self.attr["data"]["pressure"] = data["main"]["pressure"]
        self.attr["data"]["humidity"] = data["main"]["humidity"]
        return self.attr

if __name__ == "__main__":
    pass
