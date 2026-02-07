*** settings ***
Library    SerialLibrary   encoding=ascii
Library    String
Suite Setup       Open Serial Port
Suite Teardown    Close Serial Port

*** Variables ***
${SERIAL_PORT}        COM5
${SERIAL_BAUDRATE}    115200

*** test cases ***
Shell should display all available commands
    [Setup]    Clean Serial Port
    Run Shell Command    help
    ${response} =    Read Until
    Should Not Be Empty    ${response}
    Log To Console    \n${response}
    # No need to check response due to human readable information
    [Teardown]    Clean Serial Port

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
