#!/usr/bin/env python

import serial
import time

class Console:
    def __init__(self, baudrate=115200, port="/dev/ttyACM0"):
        self.serial = serial.Serial()
        self.serial.baudrate = baudrate
        self.serial.port = port
        self.serial.timeout = 1

    def open(self):
        ret = True
        try:
            self.serial.open()
        except serial.SerialException:
            ret = False
        self.serial.flushInput()
        self.serial.flushOutput()
        return ret

    def close(self):
        self.serial.close()

    def send(self, command, args=None):
        self.serial.flushInput()
        self.serial.flushOutput()
        message = str()
        if args == None:
            message = command
        else:
            message = command + " " + str(args)
        message += "\n"

        for character in message:
            self.serial.write(character.encode("utf-8"))
            time.sleep(0.1)  # in seconds

    def receive(self):
        bytes_available = self.serial.in_waiting
        raw_message = self.serial.read(bytes_available)
        message = raw_message.decode("utf-8")
        return message

def main() -> None:
    print("Testbench")
    console = Console()
    console.open()

    console.send("")
    data = console.receive()
    print(data)

    console.send("help")
    data = console.receive()
    print(data)

    #console.send("sensor_selfTest")
    #data = console.receive()
    #print(data)

    console.close()

if __name__ == "__main__":
    main()
