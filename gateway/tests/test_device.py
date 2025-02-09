from src.device import Device
from src.console import Console
import json

def test_device_should_be_initialized(mocker) -> None:
    # Given
    mock_console_open = mocker.patch.object(Console, "open")

    # When
    device = Device()

    # Then
    mock_console_open.assert_called()
    device is not None

def test_device_should_run_command(mocker) -> None:
    # Given
    mock_console_open = mocker.patch.object(Console, "open")
    device = Device()
    mock_console_send = mocker.patch.object(Console, "send")
    expected_string = "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"\"}}"
    mock_console_receive = mocker.patch.object(Console, "receive", return_value=expected_string)

    # When
    response = device.runCommand("help")

    # Then
    mock_console_send.assert_called_with("help")
    mock_console_receive.assert_called()
    assert response["request"]["code"] == "SUCCESS"
    assert response["request"]["message"] == ""
