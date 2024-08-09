*** settings ***
Library    SerialLibrary   encoding=ascii
Library    String
Library    JSONLibrary
Suite Setup       Open Serial Port
Suite Teardown    Close Serial Port

*** Variables ***
${SERIAL_PORT}    /tmp/console
${SERIAL_BAUDRATE}    115200
${PROTOCOL_JSON_SCHEMA} =    Convert String to JSON    {"type": "object", "properties": {"code": {"type": "string"}, "message": {"type": "string"}, "response": {"type": "string"}}}

*** test cases ***
Shell should not know a random command
    [Setup]    Clean Serial Port
    Run Shell Command    try
    ${response} =    Read Until
    ${json_obj} =	Convert String to JSON    ${response}
    Validate Json By Schema    ${PROTOCOL_JSON_SCHEMA}    ${json_obj}
    Should Be Equal As Strings    FAILURE    ${json_obj["code"]}
    Should Be Equal As Strings    Command unknown    ${json_obj["message"]}
    Should Be Equal As Strings    Tap help to display all available command.    ${json_obj["response"]}
    [Teardown]    Clean Serial Port

Shell should display all available commands
    [Setup]    Clean Serial Port
    Run Shell Command    help
    ${response} =    Read Until
    ${json_obj} =      Convert String to JSON    ${response}
    Validate Json By Schema    ${PROTOCOL_JSON_SCHEMA}    ${json_obj}
    Should Be Equal As Strings    SUCCESS    ${json_obj["code"]}
    # No need to check response due to human readable information
    [Teardown]    Clean Serial Port

Shell should run a self-test for sensor
    [Setup]    Clean Serial Port
    Run Shell Command    sensor_selfTest
    ${response} =    Read Until
    ${json_obj} =      Convert String to JSON    ${response}
    Validate Json By Schema    ${PROTOCOL_JSON_SCHEMA}    ${json_obj}
    Should Be Equal As Strings    FAILURE    ${json_obj["code"]}
    Should Be Equal As Strings    Sensor not found    ${json_obj["message"]}
    [Teardown]    Clean Serial Port

Shell should get data from sensor
    [Setup]    Clean Serial Port
    Run Shell Command    sensor_getData
    ${response} =    Read Until
    ${json_obj} =      Convert String to JSON    ${response}
    Validate Json By Schema    ${PROTOCOL_JSON_SCHEMA}    ${json_obj}
    Should Be Equal As Strings    FAILURE    ${json_obj["code"]}
    #Should Be Equal As Strings    Sensor not found    ${json_obj["message"]}

*** Keywords ***
Open Serial Port
    Add Port    ${SERIAL_PORT}
    ...        baudrate=${SERIAL_BAUDRATE}
    ...        bytesize=8
    ...        parity=N
    ...        stopbits=1
    ...        timeout=5

Clean Serial Port
    Flush Port
    Reset Input Buffer
    Reset Output Buffer

Close Serial Port
    Delete All Ports

Run Shell Command
    [Arguments]    ${command}
    Write Data    ${command}\r\n

