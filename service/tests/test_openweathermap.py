from openweathermap import OpenWeatherMap

def test_init_fail(mocker):
    test_data = OpenWeatherMap()
    assert test_data is not None

    response_mock = {
        "cod": 400,
        "message": "Invalid format",
        "parameters": [
            "date"
        ]
    }
    mocker.patch.object(OpenWeatherMap, "requests_get", return_value=response_mock)
    response = test_data.init("arg1", "arg2", "arg3")
    OpenWeatherMap.requests_get.assert_called_once_with("https://arg1/geo/1.0/direct?q=arg3&limit=1&appid=arg2")
    assert response is not None
    assert response["error"]["code"] is 400
    assert response["error"]["message"] is "Invalid format"

def test_init_success(mocker):
    test_data = OpenWeatherMap()
    assert test_data is not None

    # Response exemple from OpenWeatherMap API
    response_mock = [
        {
            "name":"London",
            "local_names":{
                "ms":"London",
                "gu":"લંડન",
                "is":"London",
                "wa":"Londe",
                "mg":"Lôndôna",
                "gl":"Londres",
                "om":"Landan",
                "ku":"London",
                "tw":"London",
                "mk":"Лондон",
                "ee":"London",
                "fj":"Lodoni",
                "gd":"Lunnainn",
                "ky":"Лондон",
                "yo":"Lọndọnu",
                "zu":"ILondon",
                "bg":"Лондон",
                "tk":"London",
                "co":"Londra",
                "sh":"London",
                "de":"London",
                "kl":"London",
                "bi":"London",
                "km":"ឡុងដ៍",
                "lt":"Londonas",
                "fi":"Lontoo",
                "fy":"Londen",
                "ba":"Лондон",
                "sc":"Londra",
                "feature_name":"London",
                "ja":"ロンドン",
                "am":"ለንደን",
                "sk":"Londýn",
                "mr":"लंडन",
                "es":"Londres",
                "sq":"Londra",
                "te":"లండన్",
                "br":"Londrez",
                "uz":"London",
                "da":"London",
                "sw":"London",
                "fa":"لندن",
                "sr":"Лондон",
                "cu":"Лондонъ",
                "ln":"Lóndɛlɛ",
                "na":"London",
                "wo":"Londar",
                "ig":"London",
                "to":"Lonitoni",
                "ta":"இலண்டன்",
                "mt":"Londra",
                "ar":"لندن",
                "su":"London",
                "ab":"Лондон",
                "ps":"لندن",
                "bm":"London",
                "mi":"Rānana",
                "kn":"ಲಂಡನ್",
                "kv":"Лондон",
                "os":"Лондон",
                "bn":"লন্ডন",
                "li":"Londe",
                "vi":"Luân Đôn",
                "zh":"伦敦",
                "eo":"Londono",
                "ha":"Landan",
                "tt":"Лондон",
                "lb":"London",
                "ce":"Лондон",
                "hu":"London",
                "it":"Londra",
                "tl":"Londres",
                "pl":"Londyn",
                "sm":"Lonetona",
                "en":"London",
                "vo":"London",
                "el":"Λονδίνο",
                "sn":"London",
                "fr":"Londres",
                "cs":"Londýn",
                "io":"London",
                "hi":"लंदन",
                "et":"London",
                "pa":"ਲੰਡਨ",
                "av":"Лондон",
                "ko":"런던",
                "bh":"लंदन",
                "yi":"לאנדאן",
                "sa":"लन्डन्",
                "sl":"London",
                "hr":"London",
                "si":"ලන්ඩන්",
                "so":"London",
                "gn":"Lóndyre",
                "ay":"London",
                "se":"London",
                "sd":"لنڊن",
                "af":"Londen",
                "ga":"Londain",
                "or":"ଲଣ୍ଡନ",
                "ia":"London",
                "ie":"London",
                "ug":"لوندۇن",
                "nl":"Londen",
                "gv":"Lunnin",
                "qu":"London",
                "be":"Лондан",
                "an":"Londres",
                "fo":"London",
                "hy":"Լոնդոն",
                "nv":"Tooh Dineʼé Bikin Haalʼá",
                "bo":"ལོན་ཊོན།",
                "ascii":"London",
                "id":"London",
                "lv":"Londona",
                "ca":"Londres",
                "no":"London",
                "nn":"London",
                "ml":"ലണ്ടൻ",
                "my":"လန်ဒန်မြို့",
                "ne":"लन्डन",
                "he":"לונדון",
                "cy":"Llundain",
                "lo":"ລອນດອນ",
                "jv":"London",
                "sv":"London",
                "mn":"Лондон",
                "tg":"Лондон",
                "kw":"Loundres",
                "cv":"Лондон",
                "az":"London",
                "oc":"Londres",
                "th":"ลอนดอน",
                "ru":"Лондон",
                "ny":"London",
                "bs":"London",
                "st":"London",
                "ro":"Londra",
                "rm":"Londra",
                "ff":"London",
                "kk":"Лондон",
                "uk":"Лондон",
                "pt":"Londres",
                "tr":"Londra",
                "eu":"Londres",
                "ht":"Lonn",
                "ka":"ლონდონი",
                "ur":"علاقہ لندن"
            },
            "lat":51.5073219,
            "lon":-0.1276474,
            "country":"GB",
            "state":"England"
        },
        {
            "name":"City of London",
            "local_names":{
                "es":"City de Londres",
                "ru":"Сити",
                "ur":"لندن شہر",
                "zh":"倫敦市",
                "en":"City of London",
                "pt":"Cidade de Londres",
                "fr":"Cité de Londres",
                "uk":"Лондонське Сіті",
                "he":"הסיטי של לונדון",
                "hi":"सिटी ऑफ़ लंदन",
                "ko":"시티 오브 런던",
                "lt":"Londono Sitis"
            },
            "lat":51.5156177,
            "lon":-0.0919983,
            "country":"GB",
            "state":"England"
        },
        {
            "name":"London",
            "local_names":{
                "el":"Λόντον",
                "fr":"London",
                "oj":"Baketigweyaang",
                "en":"London",
                "bn":"লন্ডন",
                "be":"Лондан",
                "ko":"런던",
                "he":"לונדון",
                "ru":"Лондон",
                "lt":"Londonas",
                "hy":"Լոնտոն",
                "ga":"Londain",
                "ja":"ロンドン",
                "yi":"לאנדאן",
                "cr":"ᓬᐊᐣᑕᐣ",
                "iu":"ᓚᓐᑕᓐ",
                "ar":"لندن",
                "lv":"Landona",
                "fa":"لندن",
                "ug":"لوندۇن",
                "th":"ลอนดอน",
                "ka":"ლონდონი"
            },
            "lat":42.9832406,
            "lon":-81.243372,
            "country":"CA",
            "state":"Ontario"
        },
        {
            "name":"Chelsea",
            "local_names":{
                "id":"Chelsea, London",
                "uk":"Челсі",
                "hi":"चेल्सी, लंदन",
                "ga":"Chelsea",
                "es":"Chelsea",
                "de":"Chelsea",
                "af":"Chelsea, Londen",
                "vi":"Chelsea, Luân Đôn",
                "pl":"Chelsea",
                "pt":"Chelsea",
                "da":"Chelsea",
                "ko":"첼시",
                "sv":"Chelsea, London",
                "nl":"Chelsea",
                "az":"Çelsi",
                "it":"Chelsea",
                "hu":"Chelsea",
                "no":"Chelsea",
                "fr":"Chelsea",
                "he":"צ'לסי",
                "eu":"Chelsea",
                "ru":"Челси",
                "ar":"تشيلسي",
                "en":"Chelsea",
                "el":"Τσέλσι",
                "tr":"Chelsea, Londra",
                "zh":"車路士",
                "sh":"Chelsea, London",
                "ja":"チェルシー",
                "ur":"چیلسی، لندن",
                "sk":"Chelsea",
                "fa":"چلسی",
                "et":"Chelsea"
            },
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
    mocker.patch.object(OpenWeatherMap, "requests_get", return_value=response_mock)
    response = test_data.init("arg1", "arg2", "arg3")
    OpenWeatherMap.requests_get.assert_called_once_with("https://arg1/geo/1.0/direct?q=arg3&limit=1&appid=arg2")
    response is not None
    assert response["error"]["code"] == 200
    assert response["error"]["message"] == "No error"
    assert response["location"]["city"] == "arg3"
    assert response["location"]["latitude"] == 51.5073219
    assert response["location"]["longitude"] == -0.1276474

def test_weather_get_success(mocker):
    test_data = OpenWeatherMap()
    assert test_data is not None

    test_data.attr["location"]["latitude"] = 1
    test_data.attr["location"]["longitude"] = 2

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
    mocker.patch.object(OpenWeatherMap, "requests_get", return_value=response_mock)
    response = test_data.weather_get()
    OpenWeatherMap.requests_get.assert_called_once_with("https:///data/2.5/weather?lat=1&lon=2&appid=&units=metric")
    assert response is not None
    assert response["error"]["code"] == 200
    assert response["error"]["message"] == "No error"
    assert response["timestamp"] == 1661870592
    assert response["data"]["temperature"] == 298.48
    assert response["data"]["pressure"] == 1015
    assert response["data"]["humidity"] == 64
