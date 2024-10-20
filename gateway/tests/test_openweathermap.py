from src.openweathermap import OpenWeatherMap

def test_openweathermap_should_failed_due_to_invalid_url(mocker) -> None:
    openweathermap = OpenWeatherMap()
    response_mock = {
        "cod": 400,
        "message": "Invalid format",
        "parameters": [
            "date"
        ]
    }
    mock_openweathermap_requests_get = mocker.patch.object(openweathermap, "requests_get", return_value=response_mock)

    response = openweathermap.init(api_url="my-url", api_key="my-key", city_name="my-city")

    mock_openweathermap_requests_get.assert_called_once_with("https://my-url/geo/1.0/direct?q=my-city&limit=1&appid=my-key")
    assert response is not None
    assert response["error"]["code"] is 400
    assert response["error"]["message"] is "Invalid format"

def test_openweathermap_should_be_initialized(mocker) -> None:
    openweathermap = OpenWeatherMap()
    # Response exemple from OpenWeatherMap API
    response_mock = [
        {
            "name":"London",
            "local_names":{},
            "lat":51.5073219,
            "lon":-0.1276474,
            "country":"GB",
            "state":"England"
        },
        {
            "name":"City of London",
            "local_names":{},
            "lat":51.5156177,
            "lon":-0.0919983,
            "country":"GB",
            "state":"England"
        },
        {
            "name":"London",
            "local_names":{},
            "lat":42.9832406,
            "lon":-81.243372,
            "country":"CA",
            "state":"Ontario"
        },
        {
            "name":"Chelsea",
            "local_names":{},
            "lat":51.4875167,
            "lon":-0.1687007,
            "country":"GB",
            "state":"England"
        },
        {
            "name":"London",
            "lat":37.1289771,
            "lon":-84.0832646,
            "country":"US",
            "state":"Kentucky"
        }
    ]
    mock_openweathermap_requests_get = mocker.patch.object(openweathermap, "requests_get", return_value=response_mock)

    response = openweathermap.init(api_url="my-url", api_key="my-key", city_name="my-city")

    mock_openweathermap_requests_get.assert_called_once_with("https://my-url/geo/1.0/direct?q=my-city&limit=1&appid=my-key")
    response is not None
    assert response["error"]["code"] == 200
    assert response["error"]["message"] == "No error"
    assert response["location"]["city"] == "my-city"
    assert response["location"]["latitude"] == 51.5073219
    assert response["location"]["longitude"] == -0.1276474

def test_openweathermap_should_succeed_to_get_data(mocker) -> None:
    openweathermap = OpenWeatherMap()
    openweathermap.attr["location"]["latitude"] = 1
    openweathermap.attr["location"]["longitude"] = 2
    # Response exemple from OpenWeatherMap API
    response_mock = {
        "coord": {
            "lon": 10.99,
            "lat": 44.34
        },
        "weather": [
            {
            "id": 501,
            "main": "Rain",
            "description": "moderate rain",
            "icon": "10d"
            }
        ],
        "base": "stations",
        "main": {
            "temp": 298.48,
            "feels_like": 298.74,
            "temp_min": 297.56,
            "temp_max": 300.05,
            "pressure": 1015,
            "humidity": 64,
            "sea_level": 1015,
            "grnd_level": 933
        },
        "visibility": 10000,
        "wind": {
            "speed": 0.62,
            "deg": 349,
            "gust": 1.18
        },
        "rain": {
            "1h": 3.16
        },
        "clouds": {
            "all": 100
        },
        "dt": 1661870592,
        "sys": {
            "type": 2,
            "id": 2075663,
            "country": "IT",
            "sunrise": 1661834187,
            "sunset": 1661882248
        },
        "timezone": 7200,
        "id": 3163858,
        "name": "Zocca",
        "cod": 200
    }
    mock_openweathermap_requests_get = mocker.patch.object(openweathermap, "requests_get", return_value=response_mock)

    response = openweathermap.weather_get()

    mock_openweathermap_requests_get.assert_called_once_with("https:///data/2.5/weather?lat=1&lon=2&appid=&units=metric")
    assert response is not None
    assert response["error"]["code"] == 200
    assert response["error"]["message"] == "No error"
    assert response["timestamp"] == 1661870592
    assert response["data"]["temperature"] == 298.48
    assert response["data"]["pressure"] == 1015
    assert response["data"]["humidity"] == 64
