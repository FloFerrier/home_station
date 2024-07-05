import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
from influxdb_client.domain.write_precision import WritePrecision

class Database:
    def __init__(self) -> None:
        self.__api: dict = {
            "url": "",
            "token": "",
            "org": "",
            "bucket": ""
        }

    def init(self, url: str = "", token: str = "", org: str = "", bucket: str = "") -> None:
        self.__api["url"] = "http://{}".format(url)
        self.__api["token"] = token
        self.__api["org"] = org
        self.__api["bucket"] = bucket
        client = influxdb_client.InfluxDBClient(url=self.__api["url"], token=self.__api["token"], org=self.__api["org"])
        self.__write_api = client.write_api(write_options=SYNCHRONOUS)

    def __data_check(self, data: dict) -> bool:
        keys = ["timestamp", "name", "location", "source", "type", "unit", "value"]
        key_found = 0
        for key in keys:
            if key in data.keys():
                key_found += 1
        return False if key_found != len(keys) else True

    def write(self, data: dict) -> bool:
        data_is_correct: bool = self.__data_check(data)
        if data_is_correct is True:
            record = influxdb_client.Point.from_dict(data, write_precision=WritePrecision.S, record_measurement_key="name", record_time_key="timestamp", record_tag_keys=["location", "source", "type", "unit"], record_field_keys=["value"])
            self.__write_api.write(bucket=self.__api["bucket"], org=self.__api["org"], record=record)
        return data_is_correct

if __name__ == "__main__":
    pass
