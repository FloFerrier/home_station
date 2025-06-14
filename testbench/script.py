#!/usr/bin/env python

import sys
sys.path.append("../gateway")

from src.device import Device

def main() -> None:
    device = Device()

    response = device.runCommand("sensor_selfTest\n")
    if response["request"]["code"] == "SUCCESS":
        print("Success to run self-test")
    else:
        print("Fail to run self-test...")
        exit(0)

    response = device.runCommand("sensor_getData\n")
    if response["request"]["code"] == "SUCCESS":
        print("Success to get temperature")
    else:
        print("Fail to get temperature : {}".format(response["request"]["message"]))

    response = device.runCommand("sensor_getData\n")
    if response["request"]["code"] == "SUCCESS":
        print("Success to get data")
        print("{}".format(response["data"]))
    else:
        print("Fail to get data : {}".format(response["request"]["message"]))

if __name__ == "__main__":
    main()
