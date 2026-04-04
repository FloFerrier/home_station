/**
 * @file
 */

#include "shell.h"

#include <ctype.h>
#include <string.h>

#define EMBEDDED_CLI_IMPL
#include "embedded_cli.h"

#include "led.h"
#include "console.h"

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
    (void) cli;
    (void) context;

    const char *arg1 = embeddedCliGetToken(args, 1);
    if (strcmp(arg1, "on") == 0) {
        led_setState(LED_ID_RED, LED_STATE_ON);
    }
    else if (strcmp(arg1, "off") == 0) {
        led_setState(LED_ID_RED, LED_STATE_OFF);
    }
    else {
        console_send("Incorrect arg ... \"on\" or \"off\" available !\r\n", arg1);
    }
}

void shell_task(void *params) {
    (void)params;

    EmbeddedCliConfig *config = embeddedCliDefaultConfig();
    config->maxBindingCount = 16;
    EmbeddedCli *cli = embeddedCliNew(config);
    cli->writeChar = writeChar;
    CliCommandBinding setLed_binding = {
            .name = "set-led",
            .help = "Set led on/off",
            .tokenizeArgs = true,
            .context = NULL,
            .binding = setLed
    };
    embeddedCliAddBinding(cli, setLed_binding);

    do {
        char character = 0;
        bool reception_is_success = console_receive(&character);
        if (reception_is_success == true) {
            embeddedCliReceiveChar(cli, character);
        }
        embeddedCliProcess(cli);
    } while (FOREVER());
}