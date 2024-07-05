from database import Database

def test_init_success(mocker):
    database = Database()
    assert database is not None

    database.init("arg1", "arg2", "arg3", "arg4")

def test_write_fail(mocker):
    database = Database()
    assert database is not None

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

def test_write_success(mocker):
    database = Database()
    assert database is not None

    test_data = {
        "timestamp": 0,
        "name": "test-name",
        "location": "test-loc",
        "source": "test-src",
        "type": "test-type",
        "unit": "test-unit",
        "value": 0,
    }
    data_is_correct = database.write(test_data)
    assert data_is_correct is True
