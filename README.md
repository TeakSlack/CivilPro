# CivilPro
=====

## This project is heavily in development. Most features are not added or complete.

CivilPro is an interface targeted for the timeless TL866II+. Meant as a replacment for the rather dated XgPro,
CivilPro is meant to be used from the command-line. More specifically, this project is being developed to assist in the hardware and software development
of the Civil25, a single board computer based around the 8088. The process of building the firmware for the Civil25 involves a makefile, and since XgPro
does not have a command-line interface, it takes extra steps to do something as simple as flashing firmware to the EEPROM.

## How it (is going to) work

CivilPro can flash Intel hex or binary.

To flash firmware:

```
civilpro -w <file to flash> -c <chip name>
```

To read firmware:
```
civilpro -r <output file name> -c <chip name>
```

## Current development roadmap

CivilPro is nowhere near feature-complete, or even close to working yet. To simply be able to flash to a chip, we need to:
* Begin a transaction (0x03 on endpoint 1)
* Flash 64 bytes in alternating chunks to endpoints 2 and 3
* End transaction (0x04 on endpoint 1)
This is much easier said than done. Much work is going to have to go into the most basic of functions.

In the future, CivilPro will idealy be able to:
* Flash all 13k supported chips
* Test for faulty pin connections
* Flash e-fuses
* Have a graphical interfact built in Qt
