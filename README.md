# CivilPro

## This project is heavily in development. Most features are not added or complete.

CivilPro is an interface targeted for the timeless TL866II+. Meant as a replacement for the rather dated XgPro,
CivilPro is meant to be used from the command-line. This project is being developed to assist in the hardware and software development
of the Civil25, a single board computer based around the 8088. One chip is being targeted above all the rest, the SST39SF\*\*\* series.

## How it (is going to) work

CivilPro is primarily meant to flash code memory on EEPROMs. Support for chips with data memory and fuses, like most ATMEL & AVR chips, will be added in the future.

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

- Begin a transaction (0x03 on endpoint 1). This is a 64 byte data structure which communicates information about the chip being flashed to the programmer.
- Flash 64 bytes in alternating chunks to endpoints 2 and 3
- End transaction (0x04 on endpoint 1)

In the future, CivilPro will idealy be able to:

- Flash all 13k supported chips
- Test for faulty pin connections
- Flash e-fuses
- Have overcurrent protection
- Be able to take advantage of the TSOP adapter & ICSP.
- Have a graphical interfact built in Qt
