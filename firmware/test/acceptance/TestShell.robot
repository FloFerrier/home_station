*** settings ***
Library    SerialLibrary   encoding=ascii
Library    String
Suite Setup       Open Serial Port
Suite Teardown    Close Serial Port

*** Variables ***
${SERIAL_PORT}    /tmp/console
${SERIAL_BAUDRATE}    115200

*** test cases ***
Shell should not know a random command
    [Setup]    Clean Serial Port
    Run Shell Command    try
    Check Expected Response    > Unknown command
    Check Expected Response    Tap "help" command : Display all commands available
    [Teardown]    Clean Serial Port

Shell should display all available commands
    [Setup]    Clean Serial Port
    Run Shell Command    help
    Check Expected Response    (help)> List of available command :
    Check Expected Response    sensor_selfTest : Performing a sensor self-test
    Check Expected Response    sensor_getData : Request a sensor to get data
    [Teardown]    Clean Serial Port

Shell should run a self-test for sensor
    [Setup]    Clean Serial Port
    Run Shell Command    sensor_selfTest
    Check Expected Response    (sensor)> Self-test failed ... 3
    [Teardown]    Clean Serial Port

Shell should get data from sensor
    [Setup]    Clean Serial Port
    Run Shell Command    sensor_getData
    Check Expected Response    (sensor)> Self-test failed ... 3
    [Teardown]    Clean Serial Port

*** Keywords ***
Open Serial Port
    Add Port    ${SERIAL_PORT}
    ...        baudrate=${SERIAL_BAUDRATE}
    ...        bytesize=8
    ...        parity=N
    ...        stopbits=1
    ...        timeout=0.5

Clean Serial Port
    Flush Port
    Reset Input Buffer
    Reset Output Buffer

Close Serial Port
    Delete All Ports

Run Shell Command
    [Arguments]    ${command}
    Write Data    ${command}\r\n

Check Expected Response
    [Arguments]    ${expected}
    ${response} =    Read Until
    ${response} =  Remove String    ${response}    \r\n
    ${response} =  Remove String    ${response}    \t
    Should Be Equal As Strings    ${expected}    ${response}
