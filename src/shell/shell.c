/**
 * @file
 */

#include "shell.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#define EMBEDDED_CLI_IMPL
#include "ble.h"
#include "console.h"
#include "embedded_cli.h"
#include "led.h"

#ifndef TEST
#define FOREVER() (1u)
#define STATIC static
#else
int loopCnt;
#define FOREVER() (loopCnt--)
#define STATIC
#endif  // TEST

static void writeChar(EmbeddedCli *embeddedCli, char c) {
    (void)embeddedCli;
    (void)console_send("%c", c);
}

static void setLed(EmbeddedCli *cli, char *args, void *context) {
    (void)cli;
    (void)context;

    const char *arg1 = embeddedCliGetToken(args, 1);
    if (strcmp(arg1, "on") == 0) {
        led_setState(LED_ID_RED, LED_STATE_ON);
    } else if (strcmp(arg1, "off") == 0) {
        led_setState(LED_ID_RED, LED_STATE_OFF);
    } else {
        console_send("Incorrect arg ... \"on\" or \"off\" available !\r\n",
                     arg1);
    }
}

static void rn4871(EmbeddedCli *cli, char *args, void *context) {
    (void)cli;
    (void)context;

    const char *arg1 = embeddedCliGetToken(args, 1);
    if (strcmp(arg1, "$$$") == 0) {
        ble_sendCmdMode();
    } else if (strcmp(arg1, "factory_reset") == 0) {
        ble_sendFactoryReset();
    } else if (strcmp(arg1, "reboot") == 0) {
        ble_sendReboot();
    } else if (strcmp(arg1, "version") == 0) {
        ble_sendVersion();
    } else if (strcmp(arg1, "fake_packet") == 0) {
        ble_sendFakePacket();
    } else if (strcmp(arg1, "advertising") == 0) {
        ble_sendAdvertising();
    } else if (strcmp(arg1, "reset_services") == 0) {
        ble_sendResetServices();
    } else {
        console_send("Incorrect arg ...\r\n");
    }
}

static void printStackUsage(TaskHandle_t task, const char *name,
                            UBaseType_t stackSize) {
    UBaseType_t highWater = uxTaskGetStackHighWaterMark(task);
    UBaseType_t used = stackSize - highWater;
    UBaseType_t percent = (used * 100) / stackSize;
    console_send("%-12s Stack: %4u Used: %3u%% Free: %u\r\n", name, stackSize,
                 percent, highWater);
}

static void stack(EmbeddedCli *cli, char *args, void *context) {
    (void)cli;
    (void)args;
    (void)context;

    console_send("=== Stack Usage ===\r\n");
    printStackUsage(NULL, "ble", 256);
    printStackUsage(NULL, "fsm", 256);
    printStackUsage(NULL, "sampling", 256);
    printStackUsage(NULL, "shell", 256);
}

void shell_task(void *params) {
    (void)params;

    EmbeddedCliConfig *config = embeddedCliDefaultConfig();
    config->maxBindingCount = 16;
    EmbeddedCli *cli = embeddedCliNew(config);
    cli->writeChar = writeChar;
    CliCommandBinding setLed_binding = {.name = "set-led",
                                        .help = "Set led on/off",
                                        .tokenizeArgs = true,
                                        .context = NULL,
                                        .binding = setLed};
    embeddedCliAddBinding(cli, setLed_binding);
    CliCommandBinding rn4871_binding = {.name = "rn4871",
                                        .help = "RN4871 CLI",
                                        .tokenizeArgs = true,
                                        .context = NULL,
                                        .binding = rn4871};
    embeddedCliAddBinding(cli, rn4871_binding);
    CliCommandBinding stack_binding = {.name = "stack",
                                       .help = "Show stack usage",
                                       .tokenizeArgs = false,
                                       .context = NULL,
                                       .binding = stack};
    embeddedCliAddBinding(cli, stack_binding);

    do {
        char character = 0;
        bool reception_is_success = console_receive(&character);
        if (reception_is_success == true) {
            embeddedCliReceiveChar(cli, character);
        }
        embeddedCliProcess(cli);
    } while (FOREVER());
}