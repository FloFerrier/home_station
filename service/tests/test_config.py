import os

from src.config import config_read

def test_Config():
    test_filename = os. getcwd() + "/tests/test_config.yaml"
    config = config_read(test_filename)
    assert config is not None
    assert "openweathermap" in config is not None
    assert config["openweathermap"]["url"] == "openweathermap-url"
    assert config["openweathermap"]["city"] == "openweathermap-city"
    assert config["openweathermap"]["key"] == "openweathermap-key"
    assert "influxdb" in config is not None
    assert config["influxdb"]["url"] == "influxdb-url"
    assert config["influxdb"]["token"] == "influxdb-token"
    assert config["influxdb"]["org"] == "influxdb-org"
    assert config["influxdb"]["bucket"] == "influxdb-bucket"
