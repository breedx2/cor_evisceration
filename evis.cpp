#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../devices/machine/nvram.h"
#include "../emu/debug/debugcon.h"
#include "../emu/debug/debugcmd.h"

int mini_printf(running_machine &machine, char *buffer, const char *format, int params, UINT64 *param);

void execute_evis_init(running_machine &machine, int ref, int params, const char **param) {
    printf("evisceration initialized.\n");
}

void execute_evis_print(running_machine &machine, int ref, int params, const char **param) {
    UINT64 values[MAX_COMMAND_PARAMS];
    char buffer[1024];
    int i;

    /* validate the other parameters */
    for (i = 1; i < params; i++) {
        if (!debug_command_parameter_number(machine, param[i], &values[i])) {
            return;
        }
    }

    /* then do a printf */
    mini_printf(machine, buffer, param[0], params - 1, &values[1]);
    printf("%s", buffer);
}
