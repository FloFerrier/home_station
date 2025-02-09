from src.console import Console
import json

class Device:
    def __init__(self) -> None:
        self.__console = Console(baudrate=115200, port="/dev/ttyACM0")
        self.__console.open()

    def __del__(self) -> None:
        del self.__console

    def runCommand(self, command="") -> dict:
        self.__console.send(command)
        response = self.__console.receive()
        return json.loads(response)

if __name__ == "__main__":
    pass
