@echo off
set savepath=%path%
set path=..\tools2;%path%
rmac os2ccp
link os2ccp[l0]
set path=%savepath%
set savepath=
