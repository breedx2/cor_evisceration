#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include "../devices/machine/nvram.h"
#include "../emu/debug/debugcon.h"
#include "../emu/debug/debugcmd.h"
#include "evis.h"

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

    WaveState state = build_wave(machine);
    state.debugPrint();
}

WaveState build_wave(running_machine &machine) {
    address_space *addr = find_ram(machine);
    Player player = build_player(addr);
    uint8_t waveNum = addr->read_byte(RAM_P1_WAVE);
    WaveState state(player, waveNum);
    return state;
}

Player build_player(address_space *addr) {
    Point position = { addr->read_byte(RAM_PLAYER_X), addr->read_byte(RAM_PLAYER_Y) };
    uint8_t lives = addr->read_byte(RAM_P1_LIVES);
    uint32_t score = read_score(addr);
    Player player = { position, lives, score };
    return player;
}

//score is stored in 32 bits, each nibble is a decimal digit, resulting in
// an 8-digit max score of 99,999,999 
uint32_t read_score(address_space *addr){
    uint32_t score = addr->read_dword(RAM_P1_SCORE);
    char buff[20];
    sprintf(buff, "%X", score);
    return atoi(buff);
}

address_space *find_ram(running_machine &machine){
    // first region is the maincpu
    address_space *addr = machine.memory().first_space();
    while(addr){
        if(strcmp(addr->device().tag(), ":maincpu") == 0){
            return addr;
        }
        addr = addr->next();
    }
    return NULL;
}

WaveState::WaveState(Player p, uint8_t waveNum) {
    player = p;
    wave = waveNum;
}

void WaveState::debugPrint() {
    printf(" :: player :: waveNum: %d, pos(%02X,%02X), lives %d, score: %d\n", wave, player.pos.x, player.pos.y, player.lives, player.score);
}
