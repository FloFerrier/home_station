import serial

class TestClass:
    @classmethod
    def setup_class(cls):
        cls.console = serial.Serial(port="/tmp/console", baudrate=115200)

    @classmethod
    def teardown_class(cls):
        cls.console.close()

    def test_shell(self):
        console = self.console
        console.write(b'Hello world!\r\n')
        response = console.readline()
        assert response == b'> Unknown command\r\n'
