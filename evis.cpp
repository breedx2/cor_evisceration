#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include "../devices/machine/nvram.h"
#include "../emu/debug/debugcon.h"
#include "../emu/debug/debugcmd.h"
#include "sender.h"
#include "evis.h"

#define DEBUG_PRINT false

bool game_started = false;

Sender sender(SEND_HOST, SEND_PORT);

int mini_printf(running_machine &machine, char *buffer, const char *format, int params, UINT64 *param);
void debug_printf(const char *format, ...);

void evis_init(running_machine &machine, int ref, int params, const char **param) {
    sender.start();
    printf("evisceration initialized.\n");
}

// send the full screen state to the remote
void evis_state(running_machine &machine, int ref, int params, const char **param) {
    if (!game_started) return;
    WaveState state = build_wave(machine);
    state.debugPrint();
    sender.sendState(state);
}

void evis_game_booted(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(GAME_BOOTED);
}

void evis_game_start(running_machine &machine, int ref, int params, const char **param) {
    game_started = true;
    sender.sendMessage(GAME_STARTED);
}

void evis_coin(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(COIN_INSERTED);
}

void evis_enforcer_shot(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(ENFORCER_SHOT);
}

void evis_electrode_killed(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(ELECTRODE_KILLED);
}

void evis_grunt_killed(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(GRUNT_KILLED);
}

void evis_game_over(running_machine &machine, int ref, int params, const char **param) {
    game_started = false;
    sender.sendMessage(GAME_OVER);
}

void evis_tank_spawn(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(TANK_SPAWN);
}

void evis_savior_died(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(SAVIOR_DIED);
}

void evis_hulk_shot(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(HULK_SHOT);
}

void evis_human_saved(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(HUMAN_SAVED);
}

void evis_human_killed(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(HUMAN_KILLED);
}

void evis_human_progged(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(HUMAN_PROGGED);
}

void evis_brain_killed(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(BRAIN_KILLED);
}

void evis_spheroid_killed(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(SPHEROID_KILLED);
}

void evis_enforcer_spawn(running_machine &machine, int ref, int params, const char **param) {
    sender.sendMessage(ENFORCER_SPAWN);
}

void evis_wave(running_machine &machine, int ref, int params, const char **param) {
    char buffer[1024];
    if (!game_started) return;
    expand_param(machine, params, param, buffer);
    std::string message = std::string(WAVE)
        .append(std::to_string(atoi(buffer)+1))
        .append("\n");
    sender.sendMessage(message);
}

void expand_param(running_machine &machine, int params, const char **param, char *outBuf){
    UINT64 values[MAX_COMMAND_PARAMS];
    for (int i = 1; i < params; i++) {
        if (!debug_command_parameter_number(machine, param[i], &values[i])) {
            return;
        }
    }
    mini_printf(machine, outBuf, param[0], params - 1, &values[1]);
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
    std::list<Point> spheroids  = build_spheroids(addr);
    std::list<Point> enforcers  = build_enforcers(addr);
    std::list<Point> sparks     = build_sparks(addr);
    std::list<Point> progs      = build_progs(addr);
    std::list<Point> cruz       = build_cruise_missiles(addr);
    std::list<Point> quarks     = build_quarks(addr);
    std::list<Point> tanks      = build_tanks(addr);
    std::list<Point> shells     = build_shells(addr);

    WaveState state(player, waveNum, humans, electrodes, grunts, hulks, brains,
                    spheroids, enforcers, sparks, progs, cruz, quarks, tanks,
                    shells);
    return state;
}

Player build_player(address_space *addr) {
    uint8_t x = addr->read_byte(RAM_PLAYER_X) - BOARD_MIN_X;
    uint8_t y = addr->read_byte(RAM_PLAYER_Y) - BOARD_MIN_Y;
    Point position = { x, y };
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
    debug_printf("Searching for humans: ");
    return read_ptr_list(addr, RAM_HUMANS, { OBJ_TYPE_MOMMY, OBJ_TYPE_DADDY, OBJ_TYPE_MIKEY });
}

std::list<Point> build_electrodes(address_space *addr) {
    debug_printf("Searching for electrodes: ");
    return read_ptr_list(addr, RAM_ELECTRODES, { OBJ_TYPE_ELEC1 });
}

std::list<Point> build_grunts(address_space *addr) {
    debug_printf("Searching for grunts: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_GRUNT });
}

std::list<Point> build_hulks(address_space *addr) {
    debug_printf("Searching for hulks: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_HULK1, OBJ_TYPE_HULK2 });
}

std::list<Point> build_brains(address_space *addr) {
    debug_printf("Searching for brains: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_BRAIN });
}

std::list<Point> build_spheroids(address_space *addr) {
    debug_printf("Searching for spheroids: ");
    return read_ptr_list(addr, RAM_ENEMIES2, { OBJ_TYPE_SPHE1, OBJ_TYPE_SPHE2 });
}

std::list<Point> build_enforcers(address_space *addr) {
    debug_printf("Searching for enforcers: ");
    return read_ptr_list(addr, RAM_ENEMIES2, { OBJ_TYPE_ENFOR });
}

std::list<Point> build_sparks(address_space *addr) {
    debug_printf("Searching for sparks: ");
    return read_ptr_list(addr, RAM_ENEMIES2, { OBJ_TYPE_SPARK });
}

std::list<Point> build_progs(address_space *addr) {
    debug_printf("Searching for progs: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_PROG });
}

std::list<Point> build_cruise_missiles(address_space *addr) {
    debug_printf("Searching for cruise missiles: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_CRUZ });
}

std::list<Point> build_quarks(address_space *addr) {
    debug_printf("Searching for quarks: ");
    return read_ptr_list(addr, RAM_ENEMIES2, { OBJ_TYPE_QUARK });
}

std::list<Point> build_tanks(address_space *addr) {
    debug_printf("Searching for tanks: ");
    return read_ptr_list(addr, RAM_ENEMIES1, { OBJ_TYPE_TANK });
}

std::list<Point> build_shells(address_space *addr) {
    debug_printf("Searching for shells: ");
    return read_ptr_list(addr, RAM_ENEMIES2, { OBJ_TYPE_SHELL });
}

std::list<Point> read_ptr_list(address_space *addr, uint16_t startPtr, std::set<uint8_t> types) {
    std::list<Point> result;
    uint16_t ptr = addr->read_word(startPtr);
    if (!ptr) {
        return result;
    }
    debug_printf(" TYPES:");
    do {
        debug_printf(" %02X", addr->read_byte(ptr + 2));
        if (types.find(addr->read_byte(ptr + 2)) != types.end()) {
            uint8_t x = addr->read_byte(ptr + 4) - BOARD_MIN_X;
            uint8_t y = addr->read_byte(ptr + 5) - BOARD_MIN_Y;
            result.push_back({ x, y});
        }
        ptr = addr->read_word(ptr);
    } while (ptr);
    debug_printf("\n");
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
                     std::list<Point> gruntList, std::list<Point> hulkList, std::list<Point> brainList,
                     std::list<Point> spheroidList, std::list<Point> enforcerList, std::list<Point> sparkList,
                     std::list<Point> progList, std::list<Point> cruzList, std::list<Point> quarkList,
                     std::list<Point> tankList, std::list<Point> shellList) {
    player = p;
    wave   = waveNum;
    humans.assign(humanList.begin(), humanList.end());
    electrodes.assign(electrodeList.begin(), electrodeList.end());
    grunts.assign(gruntList.begin(), gruntList.end());
    hulks.assign(hulkList.begin(), hulkList.end());
    brains.assign(brainList.begin(), brainList.end());
    spheroids.assign(spheroidList.begin(), spheroidList.end());
    enforcers.assign(enforcerList.begin(), enforcerList.end());
    sparks.assign(sparkList.begin(), sparkList.end());
    progs.assign(progList.begin(), progList.end());
    cruiseMissiles.assign(cruzList.begin(), cruzList.end());
    quarks.assign(quarkList.begin(), quarkList.end());
    tanks.assign(tankList.begin(), tankList.end());
    shells.assign(shellList.begin(), shellList.end());
}

void WaveState::debugPrint() {
    if(!DEBUG_PRINT){
        return;
    }
    printf("\n--------------------------------------------\n");
    printf(" :: player :: waveNum: %d, pos(%d,%d), lives %d, score: %d\n",
           wave, player.pos.x, player.pos.y, player.lives, player.score);
    printPoints("humans", humans);
    printPoints("electrodes", electrodes);
    printPoints("grunts", grunts);
    printPoints("hulks", hulks);
    printPoints("brains", brains);
    printPoints("spheroids", spheroids);
    printPoints("enforcers", enforcers);
    printPoints("sparks", sparks);
    printPoints("progs", progs);
    printPoints("cruiseMissiles", cruiseMissiles);
    printPoints("quarks", quarks);
    printPoints("tanks", tanks);
    printPoints("shells", shells);
}

void WaveState::printPoints(const char *name, std::list<Point> points) {
    printf(" :: %s (%ld) :: ", name, points.size());
    std::list<Point>::iterator iter;
    for (iter = points.begin(); iter != points.end(); ++iter) {
        printf("(%d, %d) ", iter->x, iter->y);
    }
    printf("\n");
}

// A printf that's easy to en/disable
void debug_printf(const char *format, ...){
    if(DEBUG_PRINT){
        va_list args;
        va_start (args, format);
        vprintf (format, args);
        va_end (args);
    }
}
