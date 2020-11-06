#!/bin/bash
#==============================================================================
# Copy CP/M files from A: to a fresh HDD disk image (on C:) and place the
# banked system on the system tracks.
# S.J.Kay 18 February 2009
#==============================================================================
dosemu -dumb "cd z80em86|z80em86 -A scripts\make_hdd.sub"
