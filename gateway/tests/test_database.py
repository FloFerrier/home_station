from src.database import Database

from influxdb_client.rest import ApiException
from influxdb_client.client.write_api import SYNCHRONOUS

def test_database_should_be_instantiate(mocker) -> None:

    database = Database()

    assert database is not None

def test_database_should_be_initialized(mocker):
    database = Database()

    database.init("arg1", "arg2", "arg3", "arg4")

def test_database_should_instantiate_influxdb_client(mocker) -> None:
    database = Database()
    mock_influxdb_client = mocker.patch("src.database.influxdb_client.InfluxDBClient")

    database.init(url="arg1", token="arg2", org="arg3", bucket="arg4")

    mock_influxdb_client.assert_called_once_with(url="http://arg1", token="arg2", org="arg3")

def test_database_should_get_write_api_from_influxdb_client(mocker) -> None:
    database = Database()
    mock_influxdb_client_write_api = mocker.patch("src.database.influxdb_client.InfluxDBClient.write_api")

    database.init("arg1", "arg2", "arg3", "arg4")

    mock_influxdb_client_write_api.assert_called_once_with(write_options=SYNCHRONOUS)

def test_database_should_failed_to_write_due_to_wrong_data(mocker) -> None:
    database = Database()

    test_data = {
        "timestamp": 0,
        "name": "test-name",
        "location": "test-loc",
        "source": "test-src",
        "type": "test-type",
        "unit": "test-unit",
    }
    data_is_correct = database.write(test_data)
    assert data_is_correct is False

def test_database_should_failed_to_write_due_to_influxdb_api(mocker) -> None:
    database = Database()
    database.init(url="arg1", token="arg2", org="arg3", bucket="arg4")
    test_data = {
        "timestamp": 0,
        "name": "test-name",
        "location": "test-loc",
        "source": "test-src",
        "type": "test-type",
        "unit": "test-unit",
        "value": 0,
    }
    mock_database_write_api = mocker.patch.object(database, "write_api")
    mock_database_write_api.write.side_effect=ApiException("unauthorized")

    data_is_correct = database.write(test_data)

    assert data_is_correct is False

def test_database_should_succeed_to_write_data(mocker) -> None:
    database = Database()
    database.init(url="arg1", token="arg2", org="arg3", bucket="arg4")
    test_data = {
        "timestamp": 0,
        "name": "test-name",
        "location": "test-loc",
        "source": "test-src",
        "type": "test-type",
        "unit": "test-unit",
        "value": 0,
    }
    mock_database_write_api = mocker.patch.object(database, "write_api")

    data_is_correct = database.write(test_data)

    mock_database_write_api.write.assert_called_with(bucket="arg4", org="arg3", record=mocker.ANY)
    assert data_is_correct is True
