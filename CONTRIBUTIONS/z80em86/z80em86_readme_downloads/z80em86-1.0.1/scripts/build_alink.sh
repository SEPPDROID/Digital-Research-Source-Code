#!/bin/bash
#
# Script to optionally download, modify and build the 'alink' v1.6 linker
# under Unix. The web page for the linker is here:
#
# http://alink.sourceforge.net/
#
# usage:
# $build_alink build_directory package_directory url
#
# build_directory:
# A directory to extract the package files to and build. The directory will
# be created if it does not already exist. (Note: files are not in a
# directory when unzipped)
#
# package_directory:
# The location of the package.  This location if it does exist will be
# created and a package downloaded from the Internet if the 3rd parameter
# url is specified.
#
# url:
# Specify an optional url download location.  If not specified then the
# package file already in the package_directory will be used.  If specified
# then downloading only occurs if the required package is not already in
# package_directory.
#
# SJK 14 Feb 09

APP_DIR=$1
URL=$3

# Check that a build directory location has been specified
if [ "$APP_DIR" = "" ]; then
   echo "$0 : A build directory must be specified"
   exit 1
fi

# Check that a package directory location has been specified
if [ "$2" = "" ]; then
   echo "$0 : A package directory must be specified"
   exit 1
fi

# Check if we have a build directory and exit if the 'alink' binary file
# already exists. If not create the directory, optionally download, modify
# and build the package.
if (test -d $APP_DIR); then
   if (test -f $APP_DIR/alink); then
      echo "'alink' binary exists. To rebuild remove it or the build directory."
      exit 0
   fi   
else
   mkdir $APP_DIR
fi   

# Create the package directory if downloading and does not already exist
if (test ! -d $2 ); then
   if [ "$URL" != "" ]; then
      mkdir $2
   else
      echo "$0 : Package directory not found"
      exit 1
   fi
fi

# Get absolute paths to directories
CWD=$PWD
cd $2
PKGDIR=$PWD
cd $CWD

# Download the package if we don't already have it   
if (test ! -f $PKGDIR/alinksrc.zip); then
   if [ "$URL" != "" ]; then
      cd $PKGDIR
      echo "$0 : Downloading: $URL"
      wget $URL
      cd $CWD
   else
      echo "$0 : Package file does not exist"
      exit 1
   fi   
fi

# Change to the build directory
cd $APP_DIR

# Extract all files (with overwrite) forcing names to lower case
unzip -LL -o $PKGDIR/alinksrc.zip

# Check this is version 1.6 of the package
TESTSTR=`grep 'ALINK v1.6 (C)' alink.c`
if [ "$TESTSTR" = "" ]; then
   echo "$0 : Error - this script is only intended for v1.6 of ALINK"
   exit 1
fi   

# Create a new Makefile for Unix (note escaped dollar characters)
rm Makefile_unix
cat >> Makefile_unix << EOT
%.o: %.c
	gcc -c -o \$@ $<

all: alink

alink.o combine.o util.o output.o objload.o coff.o cofflib.o : alink.h

alink: alink.o combine.o util.o output.o objload.o coff.o cofflib.o
	gcc -o \$@ $^
EOT

# Append this to the alink.h file
cat >> alink.h << EOT
/* Added SJK 14 Feb 09 */
#ifdef __unix__
#ifndef alink_mod
#define alink_mod
#define stricmp strcasecmp
#define strupr xstrupr
/* strdup got redefined! put this back to strdup */
#define _strdup strdup
char *xstrupr (char *a);
#endif
#endif
EOT

# Append this to the util.c file
cat >> util.c << EOT
/* Added SJK 14 Feb 09 */
#ifdef __unix__
char *xstrupr (char *a)
{
 char *ret = a;
       
 while (*a)
    {
     if (islower(*a))
        *a = toupper(*a);
     ++a;
    }
 return ret;
}
#endif
EOT

# Compile the program
make -f Makefile_unix
