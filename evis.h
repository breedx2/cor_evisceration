
#ifndef EVIS_H
#define EVIS_H

#include "emu.h"

#define RAM_PLAYER_X 0x985E
#define RAM_PLAYER_Y 0x985F
#define RAM_P1_SCORE 0xBDE4
#define RAM_P1_LIVES 0xBDEC
#define RAM_P1_WAVE  0xBDED
#define RAM_HUMANS   0x981F

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
    WaveState(Player, uint8_t, std::list<Point> humans);
    void debugPrint();
private:
    Player player;
    uint8_t wave;
    std::list<Point> humans;
};

void execute_evis_init(running_machine &machine, int ref, int params, const char **param);
void execute_evis_print(running_machine &machine, int ref, int params, const char **param);
WaveState build_wave(running_machine &machine);
Player build_player(address_space *addr);
std::list<Point> build_humans(address_space *addr);
uint32_t read_score(address_space *addr);
address_space *find_ram(running_machine &machine);

#endif
