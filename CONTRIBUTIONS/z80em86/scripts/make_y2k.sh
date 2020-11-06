#!/bin/bash
#==============================================================================
# Create a CP/M 3 (Y2K) floppy boot disk and image.
# S.J.Kay 26 February 2009
#==============================================================================
FLOPPY_DIR=floppy

cp -a bios/ cpm3/ cpm3_y2k/ extras/ support/ scripts/ ~/.dosemu/drive_c/z80em86/
rm -f ~/.dosemu/drive_c/z80em86/bios/*.*~ ~/.dosemu/drive_c/z80em86/support/*.*~

dosemu -dumb "cd z80em86|z80em86 scripts\make_y2k.sub"

# make a raw disk image of the floppy assuming 1.44MB HD type
echo "Creating raw disk image from floppy disk"

if (test ! -d $FLOPPY_DIR); then
   mkdir floppy
fi

dd if=/dev/fd0 of=$FLOPPY_DIR/cpm3_y2k_floppy.img bs=1k count=1440
