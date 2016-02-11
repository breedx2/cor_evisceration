#!/bin/bash

MAME_HOME=~/software/mame

rsync -avv debug/ ${MAME_HOME}/src/emu/debug/
rsync -avv evis.cpp evis.h ${MAME_HOME}/src/emu/
rsync -avv emu.lua ${MAME_HOME}/scripts/src/emu.lua

cd ${MAME_HOME} && make
