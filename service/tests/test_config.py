import io
import typing
import yaml

from src.config import config_read

def test_config_should_failed_due_to_wrong_filename(mocker) -> None:
    mock_builtins_open = mocker.patch("builtins.open", side_effect=FileNotFoundError)

    config = config_read("incorrect_filename")

    mock_builtins_open.assert_called_with("incorrect_filename")

def test_config_should_be_read_with_success(mocker) -> None:
    def fake_open(path: str, *args, **kwargs) -> typing.TextIO:
        return io.StringIO(yaml.dump(
            {"openweathermap": {
                "url": "openweathermap-url",
                "city": "openweathermap-city",
                "key": "openweathermap-key",
                },
             "influxdb": {
                "url": "influxdb-url",
                "token": "influxdb-token",
                "org": "influxdb-org",
                "bucket": "influxdb-bucket",
                },
            }))
    mock_builtins_open = mocker.patch("builtins.open", new=fake_open)

    config = config_read("filename")

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
