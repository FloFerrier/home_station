import serial
import time

class Console:
    def __init__(self, baudrate: int=115200, port: str="/dev/ttyACM0") -> None:
        self.__serial = serial.Serial()
        self.__serial.port = port
        self.__serial.baudrate = baudrate
        self.__serial.timeout = None

    def __del__(self) -> None:
        del self.__serial

    def open(self) -> bool:
        device_is_opened = False

        try:
            self.__serial.open()
        except serial.SerialException as e:
            print(e)
        else:
            device_is_opened = True
            self.__serial.flushInput()
            self.__serial.flushOutput()

        return device_is_opened

    def close(self) -> None:
        self.__serial.close()

    def send(self, message: str="") -> None:
        self.__serial.flushInput()
        self.__serial.flushOutput()

        for character in message:
            self.__serial.write(character.encode("utf-8"))
            time.sleep(0.1)  # in seconds

        print("[Console:send] \"{}\"".format(message.rstrip()))

    def receive(self) -> str:
        raw_message = self.__serial.read_until()
        message = raw_message.decode("utf-8")

        print("[Console:receive] \"{}\"".format(message.rstrip()))
        return message

if __name__ == "__main__":
    pass
