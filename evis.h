
#ifndef EVIS_H
#define EVIS_H

#include "emu.h"

#define RAM_PLAYER_X 0x985E
#define RAM_PLAYER_Y 0x985F

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
    void debugPrint();
private:
    Player player;
};

void execute_evis_init(running_machine &machine, int ref, int params, const char **param);
void execute_evis_print(running_machine &machine, int ref, int params, const char **param);
WaveState build_wave(running_machine &machine);
Player build_player(running_machine &machine);
address_space *find_ram(running_machine &machine);

#endif
