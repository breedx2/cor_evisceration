
#ifndef EVIS_H
#define EVIS_H

#include "emu.h"

#define RAM_PLAYER_X    0x985E
#define RAM_PLAYER_Y    0x985F
#define RAM_P1_SCORE    0xBDE4
#define RAM_P1_LIVES    0xBDEC
#define RAM_P1_WAVE     0xBDED
#define RAM_HUMANS      0x981F
#define RAM_ELECTRODES  0x9823
#define RAM_GRUNTS      0x988B

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
        std::list<Point> grunts);
    void debugPrint();
private:
    Player player;
    uint8_t wave;
    std::list<Point> humans;
    std::list<Point> electrodes;
    std::list<Point> grunts;

    void printPoints(const char *name, std::list<Point> points);
};

void execute_evis_init(running_machine &machine, int ref, int params, const char **param);
void execute_evis_print(running_machine &machine, int ref, int params, const char **param);
WaveState build_wave(running_machine &machine);
Player build_player(address_space *addr);
std::list<Point> build_humans(address_space *addr);
std::list<Point> build_electrodes(address_space *addr);
std::list<Point> build_grunts(address_space *addr);
uint32_t read_score(address_space *addr);

std::list<Point> read_ptr_list(address_space *addr, uint16_t startPtr);
address_space *find_ram(running_machine &machine);

#endif
