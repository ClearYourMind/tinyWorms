#!/bin/bash

cd "/home/me/Documents/Arduboy"
./arduino-ide_2.3.7_Linux_64bit.AppImage &> /dev/null &
thunar "./Sketches/worm_anim/build/arduino.avr.leonardo"
cd "./Sketches/worm_anim"
git status
git --no-pager log --oneline --graph --decorate -n 15
xdg-open "https://felipemanga.github.io/ProjectABE/?url=new" &> /dev/null &
