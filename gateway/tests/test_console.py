import serial
from src.console import Console

def test_console_should_be_opened(mocker) -> None:
    # Given
    console = Console()
    mocker.patch.object(serial.Serial, "flushInput")
    mocker.patch.object(serial.Serial, "flushOutput")
    mock_serial_open = mocker.patch.object(serial.Serial, "open")

    # When
    device_is_opened = console.open()

    # Then
    mock_serial_open.assert_called()
    assert device_is_opened == True

def test_console_should_not_be_opened(mocker) -> None:
    # Given
    console = Console()
    mock_serial_open = mocker.patch.object(serial.Serial, "open")
    mock_serial_open.side_effect = serial.SerialException

    # When
    device_is_opened = console.open()

    # Then
    mock_serial_open.assert_called()
    assert device_is_opened == False

def test_console_should_be_closed(mocker) -> None:
    # Given
    console = Console()
    mock_serial_close = mocker.patch.object(serial.Serial, "close")

    # When
    console.close()

    # Then
    mock_serial_close.assert_called()

def test_console_should_send_message(mocker) -> None:
    # Given
    console = Console()
    mocker.patch.object(serial.Serial, "flushInput")
    mocker.patch.object(serial.Serial, "flushOutput")
    mock_serial_write = mocker.patch.object(serial.Serial, "write")
    test_message = "Hello world\n"
    expected_len = len(test_message)

    # When
    console.send(test_message)

    # Then
    assert mock_serial_write.call_count == expected_len

def test_console_should_receive_message(mocker) -> None:
    # Given
    console = Console()
    expected_message = b'Hello world\n'
    mock_serial_read_until = mocker.patch.object(serial.Serial, "read_until", return_value=expected_message)

    # When
    message = console.receive()

    # Then
    mock_serial_read_until.assert_called()
    assert expected_message == message.encode("utf-8")
