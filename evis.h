
#ifndef EVIS_H
#define EVIS_H

#include "emu.h"

void execute_evis_init(running_machine &machine, int ref, int params, const char **param);
void execute_evis_print(running_machine &machine, int ref, int params, const char **param);

#endif
