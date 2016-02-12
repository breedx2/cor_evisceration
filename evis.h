
#ifndef EVIS_H
#define EVIS_H

#include "emu.h"

struct Point {
    uint8_t x;
    uint8_t y;
};

struct Player {
    Point pos;
    uint8_t lives;
};

class WaveState {
public:
    WaveState(Player);
private:
    Player player;
};

void execute_evis_init(running_machine &machine, int ref, int params, const char **param);
void execute_evis_print(running_machine &machine, int ref, int params, const char **param);
WaveState build_wave(running_machine &machine);

#endif
