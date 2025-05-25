#include "commands.h"

#include "sensor.h"
#include "protocol.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifndef TEST
#include "FreeRTOS.h"
#include "task.h"
#define STATIC static
#else
#include "mock_freertos.h"
#define STATIC
#endif // TEST

#define COMMAND_STRING_LEN_MAX (255u)

typedef struct {
  const char *name;
  void (*handler)(char *response, const uint32_t response_len_max);
  const char *desc;
} command_s;

STATIC void command_unknown(char *response, const uint32_t response_len_max);
STATIC void command_help(char *response, const uint32_t response_len_max);
STATIC void command_reboot(char *response, const uint32_t response_len_max);
STATIC void command_sensorSelfTest(char *response, const uint32_t response_len_max);
STATIC void command_sensorGetData(char *response, const uint32_t response_len_max);
STATIC void command_systemGetInfos(char *response, const uint32_t response_len_max);

STATIC const command_s command_list[] = {
    [COMMAND_UNKNOWN] = {"", command_unknown, ""},
    [COMMAND_HELP] = {"help", command_help, "Display all commands available"},
    [COMMAND_REBOOT] = {"reboot", command_reboot, "Performing a system reboot"},
    [COMMAND_SENSOR_SELF_TEST] = {"sensor_selfTest", command_sensorSelfTest, "Performing a sensor self-test"},
    [COMMAND_SENSOR_GET_DATA] = {"sensor_getData", command_sensorGetData, "Request a sensor to get data"},
    [COMMAND_SYSTEM_GET_INFOS] = {"system_getInfos", command_systemGetInfos, "Get system informations"},
};

STATIC void command_unknown(char *response, const uint32_t response_len_max) {
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_FAILURE,
            .message = "Command unknown, tap \"help\" to display all available command.",
        }
    };
    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_help(char *response, const uint32_t response_len_max) {
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        },
    };
    (void)snprintf(protocol.request.message, response_len_max, "\"%s\": %s, \"%s\": %s, \"%s\": %s",
        command_list[COMMAND_REBOOT].name, command_list[COMMAND_REBOOT].desc, command_list[COMMAND_SENSOR_SELF_TEST].name,
        command_list[COMMAND_SENSOR_SELF_TEST].desc, command_list[COMMAND_SENSOR_GET_DATA].name, command_list[COMMAND_SENSOR_GET_DATA].desc);
    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_reboot(char *response, const uint32_t response_len_max) {
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        },
    };
    (void)snprintf(protocol.request.message, response_len_max, "%s", command_list[COMMAND_REBOOT].desc);
    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_sensorSelfTest(char *response, const uint32_t response_len_max) {
    int8_t result = sensor_selfTest();
    protocol_s protocol = {0};
    protocol.request.code = (result == SENSOR_OK) ? PROTOCOL_REQUEST_CODE_SUCCESS : PROTOCOL_REQUEST_CODE_FAILURE,
    (void)snprintf(protocol.request.message, response_len_max, "%s", sensor_returnCodeAsString(result));

    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_sensorGetData(char *response, const uint32_t response_len_max) {
    sensor_data_s sensor_data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t sensor_data_number = 0;
    int8_t result = sensor_getData(sensor_data, &sensor_data_number);
    protocol_s protocol = {0};
    /* Get only the first data from sensor */
    if(result == SENSOR_OK) {
        protocol.data_nb = 3;
        /* Temperature field */
        protocol.data[0].field = PROTOCOL_DATA_FIELD_TEMPERATURE;
        protocol.data[0].value = sensor_data[0].temperature_in_deg;
        protocol.data[0].unit = PROTOCOL_DATA_UNIT_DEGREE_CELSIUS;
        /* Humidity field */
        protocol.data[1].field = PROTOCOL_DATA_FIELD_HUMIDITY;
        protocol.data[1].value = sensor_data[0].humidity_in_per100;
        protocol.data[1].unit = PROTOCOL_DATA_UNIT_PERCENTAGE;
        /* Pressure field */
        protocol.data[2].field = PROTOCOL_DATA_FIELD_PRESSURE;
        protocol.data[2].value = sensor_data[0].pressure_in_pascal;
        protocol.data[2].unit = PROTOCOL_DATA_UNIT_PASCAL;
    }
    protocol.request.code = (result == SENSOR_OK) ? PROTOCOL_REQUEST_CODE_SUCCESS : PROTOCOL_REQUEST_CODE_FAILURE;
    (void)snprintf(protocol.request.message, response_len_max, "%s", sensor_returnCodeAsString(result));

    protocol_serialize(protocol, response_len_max, response);
}

STATIC void command_systemGetInfos(char *response, const uint32_t response_len_max) {
    (void) response_len_max;
    vTaskGetRunTimeStats(response);
}

command_index_e command_getIndex(const char *cmd) {
    if(cmd == NULL) {
        return COMMAND_UNKNOWN;
    }
    for(uint32_t index=0; index < sizeof(command_list)/sizeof(command_s); index++) {
        uint32_t cmp = strncmp(cmd, command_list[index].name, COMMAND_STRING_LEN_MAX);
        if(cmp == 0) {
            return index;
        }
    }
    return COMMAND_UNKNOWN;
}

void command_execute(command_index_e command_index, const uint32_t response_len_max, char *response) {
    if((response == NULL) || (response_len_max <= 0)) {
        return;
    }

    if(command_index < COMMAND_INDEX_MAX) {
        command_list[command_index].handler(response, response_len_max);
    }
}
