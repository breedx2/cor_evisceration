#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <set>
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
    address_space *addr         = find_ram(machine);
    Player player               = build_player(addr);
    uint8_t waveNum             = addr->read_byte(RAM_P1_WAVE);
    std::list<Point> humans     = build_humans(addr);
    std::list<Point> electrodes = build_electrodes(addr);
    std::list<Point> grunts     = build_grunts(addr);
    std::list<Point> hulks      = build_hulks(addr);
    std::list<Point> brains     = build_brains(addr);
    WaveState state(player, waveNum, humans, electrodes, grunts, hulks, brains);
    return state;
}

Player build_player(address_space *addr) {
    Point position = { addr->read_byte(RAM_PLAYER_X), addr->read_byte(RAM_PLAYER_Y) };
    uint8_t lives  = addr->read_byte(RAM_P1_LIVES);
    uint32_t score = read_score(addr);
    Player player  = { position, lives, score };
    return player;
}

//score is stored in 32 bits, each nibble is a decimal digit, resulting in
// an 8-digit max score of 99,999,999
uint32_t read_score(address_space *addr) {
    uint32_t score = addr->read_dword(RAM_P1_SCORE);
    char buff[20];
    sprintf(buff, "%X", score);
    return atoi(buff);
}

std::list<Point> build_humans(address_space *addr) {
    printf("Searching for humans...");
    return read_ptr_list(addr, RAM_HUMANS, { OBJ_TYPE_MOMMY, OBJ_TYPE_DADDY, OBJ_TYPE_MIKEY });
}

std::list<Point> build_electrodes(address_space *addr) {
    printf("Searching for electrodes...");
    return read_ptr_list(addr, RAM_ELECTRODES, { OBJ_TYPE_ELEC1 });
}

std::list<Point> build_grunts(address_space *addr) {
    printf("Searching for grunts...");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_GRUNT });
}

std::list<Point> build_hulks(address_space *addr) {
    printf("Searching for hulks...");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_HULK1, OBJ_TYPE_HULK2 });
}

std::list<Point> build_brains(address_space *addr) {
    printf("Searching for brains...");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_BRAIN });
}

std::list<Point> read_ptr_list(address_space *addr, uint16_t startPtr, std::set<uint8_t> types) {
    std::list<Point> result;
    uint16_t ptr = addr->read_word(startPtr);
    if (!ptr) {
        return result;
    }
    do {
        printf("TYPE: %X\n", addr->read_byte(ptr + 2));
        if (types.find(addr->read_byte(ptr + 2)) != types.end()) {
            Point p = { addr->read_byte(ptr + 4), addr->read_byte(ptr + 5) };
            result.push_back(p);
        }
        ptr = addr->read_word(ptr);
    } while (ptr);
    printf("\n");
    return result;
}

address_space *find_ram(running_machine &machine) {
    // first region is the maincpu
    address_space *addr = machine.memory().first_space();
    while (addr) {
        if (strcmp(addr->device().tag(), ":maincpu") == 0) {
            return addr;
        }
        addr = addr->next();
    }
    return NULL;
}

WaveState::WaveState(Player p, uint8_t waveNum, std::list<Point> humanList, std::list<Point> electrodeList,
                     std::list<Point> gruntList, std::list<Point> hulkList, std::list<Point> brainList) {
    player = p;
    wave   = waveNum;
    humans.assign(humanList.begin(), humanList.end());
    electrodes.assign(electrodeList.begin(), electrodeList.end());
    grunts.assign(gruntList.begin(), gruntList.end());
    hulks.assign(hulkList.begin(), hulkList.end());
    brains.assign(brainList.begin(), brainList.end());
}

void WaveState::debugPrint() {
    printf(" :: player :: waveNum: %d, pos(%02X,%02X), lives %d, score: %d\n",
            wave, player.pos.x, player.pos.y, player.lives, player.score);
    printPoints("humans", humans);
    printPoints("electrodes", electrodes);
    printPoints("grunts", grunts);
    printPoints("hulks", hulks);
    printPoints("brains", brains);
}

void WaveState::printPoints(const char *name, std::list<Point> points) {
    printf(" :: %s (%ld) :: ", name, points.size());
    std::list<Point>::iterator iter;
    for (iter = points.begin(); iter != points.end(); ++iter) {
        printf("(%02X, %02X) ", iter->x, iter->y);
    }
    printf("\n");
}
