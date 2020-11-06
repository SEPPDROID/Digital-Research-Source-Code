> Here is the source for a PLM to C translator. I believe it is written in C, and was targeted for unix type systems. It may be able to compile with Turbo C, but will need some work. There is a lot of text in with the source code that is not surrounded with "/* */" C comment delimiters. If someone gets it to compile, please send it back to me so I can post the fixed version. This package is GNU, so enjoy.

\README

This is the file README for the gzip distribution, version 1.2.4.

gzip (GNU zip) is a compression utility designed to be a replacement
for 'compress'. Its main advantages over compress are much better
compression and freedom from patented algorithms.  The GNU Project
uses it as the standard compression program for its system.

gzip currently uses by default the LZ77 algorithm used in zip 1.9 (the
portable pkzip compatible archiver). The gzip format was however
designed to accommodate several compression algorithms. See below
for a comparison of zip and gzip.

gunzip can currently decompress files created by gzip, compress or
pack. The detection of the input format is automatic.  For the
gzip format, gunzip checks a 32 bit CRC. For pack, gunzip checks the
uncompressed length.  The 'compress' format was not designed to allow
consistency checks. However gunzip is sometimes able to detect a bad
.Z file because there is some redundancy in the .Z compression format.
If you get an error when uncompressing a .Z file, do not assume that
the .Z file is correct simply because the standard uncompress does not
complain.  This generally means that the standard uncompress does not
check its input, and happily generates garbage output.

gzip produces files with a .gz extension. Previous versions of gzip
used the .z extension, which was already used by the 'pack'
Huffman encoder. gunzip is able to decompress .z files (packed
or gzip'ed).

Several planned features are not yet supported (see the file TODO).
See the file NEWS for a summary of changes since 0.5.  See the file
INSTALL for installation instructions. Some answers to frequently
asked questions are given in the file INSTALL, please read it. (In
particular, please don't ask me once more for an /etc/magic entry.)

WARNING: on several systems, compiler bugs cause gzip to fail, in
particular when optimization options are on.  See the section "Special
targets" at the end of the INSTALL file for a list of known problems.
For all machines, use "make check" to check that gzip was compiled
correctly.  Try compiling gzip without any optimization if you have a
problem.

Please send all comments and bug reports by electronic mail to:
   Jean-loup Gailly <jloup@chorus.fr>

or, if this fails, to bug-gnu-utils@prep.ai.mit.edu.
Bug reports should ideally include:

    * The complete output of "gzip -V" (or the contents of revision.h
      if you can't get gzip to compile)
    * The hardware and operating system (try "uname -a")
    * The compiler used to compile (if it is gcc, use "gcc -v")
    * A description of the bug behavior
    * The input to gzip, that triggered the bug

If you send me patches for machines I don't have access to, please test them
very carefully. gzip is used for backups, it must be extremely reliable.

The package crypt++.el is highly recommended to manipulate gzip'ed
file from emacs. It recognizes automatically encrypted and compressed
files when they are first visited or written. It is available via
anonymous ftp to roebling.poly.edu [128.238.5.31] in /pub/crypt++.el.
The same directory contains also patches to dired, ange-ftp and info.
GNU tar 1.11.2 has a -z option to invoke directly gzip, so you don't have to
patch it. The package ftp.uu.net:/languages/emacs-lisp/misc/jka-compr19.el.Z
also supports gzip'ed files.

The znew and gzexe shell scripts provided with gzip benefit from
(but do not require) the cpmod utility to transfer file attributes.
It is available by anonymous ftp on gatekeeper.dec.com in
/.0/usenet/comp.sources.unix/volume11/cpmod.Z.

The sample programs zread.c, sub.c and add.c in subdirectory sample
are provided as examples of useful complements to gzip. Read the
comments inside each source file.  The perl script ztouch is also
provided as example (not installed by default since it relies on perl).


gzip is free software, you can redistribute it and/or modify it under
the terms of the GNU General Public License, a copy of which is
provided under the name COPYING. The latest version of gzip are always
available by ftp in prep.ai.mit.edu:/pub/gnu, or in any of the prep
mirror sites:

- sources in gzip-*.tar (or .shar or .tar.gz).
- Solaris 2 executables in sparc-sun-solaris2/gzip-binaries-*.tar
- MSDOS lha self-extracting exe in gzip-msdos-*.exe. Once extracted,
  copy gzip.exe to gunzip.exe and zcat.exe, or use "gzip -d" to decompress.
  gzip386.exe runs much faster but only on 386 and above; it is compiled with
  djgpp 1.10 available in directory omnigate.clarkson.edu:/pub/msdos/djgpp.

A VMS executable is available in ftp.spc.edu:[.macro32.savesets]gzip-1-*.zip
(use [.macro32]unzip.exe to extract). A PRIMOS executable is available
in ftp.lysator.liu.se:/pub/primos/run/gzip.run.
OS/2 executables (16 and 32 bits versions) are available in
ftp.tu-muenchen.de:/pub/comp/os/os2/archiver/gz*-[16,32].zip

Some ftp servers can automatically make a tar.Z from a tar file. If
you are getting gzip for the first time, you can ask for a tar.Z file
instead of the much larger tar file.

Many thanks to those who provided me with bug reports and feedback.
See the files THANKS and ChangeLog for more details.


		Note about zip vs. gzip:

The name 'gzip' was a very unfortunate choice, because zip and gzip
are two really different programs, although the actual compression and
decompression sources were written by the same persons. A different
name should have been used for gzip, but it is too late to change now.

zip is an archiver: it compresses several files into a single archive
file. gzip is a simple compressor: each file is compressed separately.
Both share the same compression and decompression code for the
'deflate' method.  unzip can also decompress old zip archives
(implode, shrink and reduce methods). gunzip can also decompress files
created by compress and pack. zip 1.9 and gzip do not support
compression methods other than deflation. (zip 1.0 supports shrink and
implode). Better compression methods may be added in future versions
of gzip. zip will always stick to absolute compatibility with pkzip,
it is thus constrained by PKWare, which is a commercial company.  The
gzip header format is deliberately different from that of pkzip to
avoid such a constraint.

On Unix, gzip is mostly useful in combination with tar. GNU tar
1.11.2 has a -z option to invoke gzip automatically.  "tar -z"
compresses better than zip, since gzip can then take advantage of
redundancy between distinct files. The drawback is that you must
scan the whole tar.gz file in order to extract a single file near
the end; unzip can directly seek to the end of the zip file. There
is no overhead when you extract the whole archive anyway.
If a member of a .zip archive is damaged, other files can still
be recovered. If a .tar.gz file is damaged, files beyond the failure
point cannot be recovered. (Future versions of gzip will have
error recovery features.)

gzip and gunzip are distributed as a single program. zip and unzip
are, for historical reasons, two separate programs, although the
authors of these two programs work closely together in the info-zip
team. zip and unzip are not associated with the GNU project.
The sources are available by ftp in

	 oak.oakland.edu:/pub/misc/unix/zip19p1.zip
	 oak.oakland.edu:/pub/misc/unix/unz50p1.tar-z

	 
	 
\README.DOS

Please read the generic README file first. Note in particular:

  copy gzip.exe to gunzip.exe and zcat.exe, or use "gzip -d" to decompress.
  gzip386.exe runs much faster but only on 386 and above; it is compiled with
  djgpp 1.10 available in directory omnigate.clarkson.edu:/pub/msdos/djgpp.

Read also gzip.doc, and in particular the description of the -N option
which is very useful for MSDOS to restore the original file names that
have been truncated. You can set it by default by adding
    set GZIP=-N
in your autoexec.bat file.

gzip386.exe includes the djgpp DOS extender (go32.exe) taken from
djdev110.zip. If you already have djgpp 1.10 or later, you can remove
go32.exe to get a smaller executable using:
    exe2aout gzip386.exe
    aout2exe gzip386
    del gzip386

If you get the error message "DMPI: Not enough memory", you are using a
memory manager which allocates physical memory immediately instead of
allocating on demand when pages are used for the firt time. This problem
occurs only when using DMPI. (Try under plain DOS without loading any memory
manager in config.sys.) This problem will be fixed in future versions of
djgpp using the COFF object format instead of a.out. (In the a.out format,
the data segment is loaded at virtual address 0x400000 and the memory manager
thinks that gzip requires more than 4 megs of memory.)

With gzip386.exe, you may have to set the TZ environment variable to
get correct timestamps in the compressed files. For example in France
I must set:
   set TZ=MET-1
The 16 bit version always uses local time.

For other problems related to DJGPP, read the documentation provided
in djdev110.zip. If a problem occurs with gzip386.exe, check first
if it occurs also with gzip.exe before reporting it.

The two programs gzip.exe and gzip386.exe give different compression ratios
because the 16 bit version (gzip.exe) is compiled with -DSMALL_MEM to
reduce memory usage. When compiled without this flag, all versions of
gzip give exactly the same compression ratio. The 386 version runs faster
under plain DOS without any memory manager than when using DMPI.

Please send comments and bug reports to Jean-loup Gailly <jloup@chorus.fr>
or to bug-gnu-utils@prep.ai.mit.edu.
