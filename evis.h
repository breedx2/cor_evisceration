
#ifndef EVIS_H
#define EVIS_H

#include <set>
#include "emu.h"

#define RAM_PLAYER_X    0x985E
#define RAM_PLAYER_Y    0x985F
#define RAM_P1_SCORE    0xBDE4
#define RAM_P1_LIVES    0xBDEC
#define RAM_P1_WAVE     0xBDED
#define RAM_HUMANS      0x981F
#define RAM_ELECTRODES  0x9823
//#define RAM_GRUNTS      0x988B
#define RAM_ENEMIES1    0x9821
#define RAM_ENEMIES2    0x9817

#define OBJ_TYPE_GRUNT  0x40
#define OBJ_TYPE_HULK1  0x0C
#define OBJ_TYPE_HULK2  0x0D
#define OBJ_TYPE_BRAIN  0x21
#define OBJ_TYPE_MOMMY  0x05
#define OBJ_TYPE_DADDY  0x08
#define OBJ_TYPE_MIKEY  0x0B
#define OBJ_TYPE_ELEC1  0x3B
#define OBJ_TYPE_SPHE1  0x14
#define OBJ_TYPE_SPHE2  0x15
#define OBJ_TYPE_ENFOR  0x18
#define OBJ_TYPE_SPARK  0x1A
#define OBJ_TYPE_PROG   0x05
#define OBJ_TYPE_CRUZ   0x20
#define OBJ_TYPE_TANK   0x50
#define OBJ_TYPE_QUARK  0x50
#define OBJ_TYPE_SHELL  0x4F

#define BOARD_MIN_X     0x07
#define BOARD_MIN_Y     0x18

/* Event constants */
#define GAME_BOOTED      "GAME_BOOTED\n"
#define GAME_STARTED     "GAME_STARTED\n"
#define COIN_INSERTED    "COIN_INSERTED\n"
#define WAVE             "WAVE\n"
#define ENFORCER_SHOT    "ENFORCER_SHOT\n"
#define ELECTRODE_KILLED "ELECTRODE_KILLED\n"
#define GRUNT_KILLED     "GRUNT_KILLED\n"
#define GAME_OVER        "GAME_OVER\n"
#define TANK_SPAWN       "TANK_SPAWN\n"
#define SAVIOR_DIED      "SAVIOR_DIED\n"
#define HULK_SHOT        "HULK_SHOT\n"
#define HUMAN_KILLED     "HUMAN_KILLED\n"
#define HUMAN_SAVED      "HUMAN_SAVED\n"
#define HUMAN_PROGGED    "HUMAN_PROGGED\n"
#define BRAIN_KILLED     "BRAIN_KILLED\n"
#define SPHEROID_KILLED  "SPHEROID_KILLED\n"
#define ENFORCER_SPAWN   "ENFORCER_SPAWN\n"

#define SEND_HOST       "127.0.0.1"
#define SEND_PORT       2084

struct Point {
    uint8_t x;
    uint8_t y;
};

struct Player {
    Point pos;
    uint8_t lives;
    uint32_t score;
};

class WaveState {
public:
    WaveState(Player player, uint8_t waveNum, std::list<Point> humans, std::list<Point> electrodes,
        std::list<Point> grunts, std::list<Point> hulks, std::list<Point> brains,
        std::list<Point> spheroids, std::list<Point> enforcers, std::list<Point> sparks,
        std::list<Point> progs, std::list<Point> cruiseMissiles, std::list<Point> quarks,
        std::list<Point> tanks, std::list<Point> shells);
    void debugPrint();  //todo: deprecate
    Player getPlayer(){ return player; }
    uint8_t getWave(){ return wave; }
    std::list<Point> getHumans(){ return std::list<Point>(humans); }
    std::list<Point> getElectrodes(){ return std::list<Point>(electrodes); }
    std::list<Point> getGrunts(){ return std::list<Point>(grunts); }
    std::list<Point> getHulks(){ return std::list<Point>(hulks); }
    std::list<Point> getBrains(){ return std::list<Point>(brains); }
    std::list<Point> getSpheroids(){ return std::list<Point>(spheroids); }
    std::list<Point> getEnforcers(){ return std::list<Point>(enforcers); }
    std::list<Point> getSparks(){ return std::list<Point>(sparks); }
    std::list<Point> getProgs(){ return std::list<Point>(progs); }
    std::list<Point> getCruiseMissiles(){ return std::list<Point>(cruiseMissiles); }
    std::list<Point> getQuarks(){ return std::list<Point>(quarks); }
    std::list<Point> getTanks(){ return std::list<Point>(tanks); }
    std::list<Point> getShells(){ return std::list<Point>(shells); }
private:
    Player player;
    uint8_t wave;
    std::list<Point> humans;
    std::list<Point> electrodes;
    std::list<Point> grunts;
    std::list<Point> hulks;
    std::list<Point> brains;
    std::list<Point> spheroids;
    std::list<Point> enforcers;
    std::list<Point> sparks;
    std::list<Point> progs;
    std::list<Point> cruiseMissiles;
    std::list<Point> quarks;
    std::list<Point> tanks;
    std::list<Point> shells;

    void printPoints(const char *name, std::list<Point> points);
};

void evis_init(running_machine &machine, int ref, int params, const char **param);
void evis_print(running_machine &machine, int ref, int params, const char **param);
void evis_game_booted(running_machine &machine, int ref, int params, const char **param);
void evis_game_start(running_machine &machine, int ref, int params, const char **param);
void evis_coin(running_machine &machine, int ref, int params, const char **param);
void evis_wave(running_machine &machine, int ref, int params, const char **param);
void evis_electrode_killed(running_machine &machine, int ref, int params, const char **param);
void evis_grunt_killed(running_machine &machine, int ref, int params, const char **param);
void evis_enforcer_shot(running_machine &machine, int ref, int params, const char **param);
void evis_game_over(running_machine &machine, int ref, int params, const char **param);
void evis_tank_spawn(running_machine &machine, int ref, int params, const char **param);
void evis_savior_died(running_machine &machine, int ref, int params, const char **param);
void evis_hulk_shot(running_machine &machine, int ref, int params, const char **param);
void evis_human_killed(running_machine &machine, int ref, int params, const char **param);
void evis_human_saved(running_machine &machine, int ref, int params, const char **param);
void evis_human_progged(running_machine &machine, int ref, int params, const char **param);
void evis_brain_killed(running_machine &machine, int ref, int params, const char **param);
void evis_spheroid_killed(running_machine &machine, int ref, int params, const char **param);
void evis_enforcer_spawn(running_machine &machine, int ref, int params, const char **param);

void expand_param(running_machine &machine, int params, const char **param, char *outBuf);

WaveState build_wave(running_machine &machine);
Player build_player(address_space *addr);

std::list<Point> build_humans(address_space *addr);
std::list<Point> build_electrodes(address_space *addr);
std::list<Point> build_grunts(address_space *addr);
std::list<Point> build_hulks(address_space *addr);
std::list<Point> build_brains(address_space *addr);
std::list<Point> build_spheroids(address_space *addr);
std::list<Point> build_enforcers(address_space *addr);
std::list<Point> build_sparks(address_space *addr);
std::list<Point> build_progs(address_space *addr);
std::list<Point> build_cruise_missiles(address_space *addr);
std::list<Point> build_quarks(address_space *addr);
std::list<Point> build_tanks(address_space *addr);
std::list<Point> build_shells(address_space *addr);
uint32_t read_score(address_space *addr);

std::list<Point> read_ptr_list(address_space *addr, uint16_t startPtr, std::set<uint8_t> types);
address_space *find_ram(running_machine &machine);

#endif
