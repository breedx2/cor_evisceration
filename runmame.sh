#!/bin/bash

#mame64 -autoframeskip -waitvsync -debug robotron
mame64 -autoframeskip -waitvsync -debug -debugscript events.debugscript robotron &
