#!/usr/bin/env python

import serial
import time
import json

class Console:
    def __init__(self, baudrate=115200, port="/dev/ttyACM0"):
        self.serial = serial.Serial()
        self.serial.baudrate = baudrate
        self.serial.port = port
        self.serial.timeout = None

    def open(self):
        ret = True
        try:
            self.serial.open()
        except serial.SerialException:
            ret = False
        self.serial.flushInput()
        self.serial.flushOutput()
        return ret

    def send(self, message=""):
        self.serial.flushInput()
        self.serial.flushOutput()
        message += "\n"

        for character in message:
            self.serial.write(character.encode("utf-8"))
            time.sleep(0.1)  # in seconds
        print("[Console:send] \"{}\"".format(message.rstrip()))

    def receive(self):
        raw_message = self.serial.read_until()
        message = raw_message.decode("utf-8")
        print("[Console:receive] \"{}\"".format(message.rstrip()))
        return message

    def __del__(self):
        self.serial.close()
        del self.serial
class Device:
    def __init__(self):
        self.console = Console(baudrate=115200, port="/dev/ttyACM0")
        self.console.open()

    def runCommand(self, command=""):
        self.console.send(command)
        response = self.console.receive()
        return json.loads(response)

    def __del__(self):
        del self.console

def main() -> None:
    device = Device()

    cmdResult = device.runCommand("sensor_selfTest")
    if cmdResult["code"] == "SUCCESS":
        print("Success to run self-test")
    else:
        print("Fail to run self-test...")
        exit(0)

    cmdResult = device.runCommand("sensor_getData")
    if cmdResult["code"] == "SUCCESS":
        print("Success to get temperature")
    else:
        print("Fail to get temperature : {}".format(cmdResult["message"]))

    cmdResult = device.runCommand("sensor_getData")
    if cmdResult["code"] == "SUCCESS":
        print("Success to get data")
        print("{}".format(cmdResult["response"]))
    else:
        print("Fail to get data : {}".format(cmdResult["message"]))

if __name__ == "__main__":
    main()
