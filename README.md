# BWTA2 #

This is a fork of [BWTA2](https://bitbucket.org/auriarte/bwta2) and has been modified by [Adakite](https://github.com/adakitesystems) to compile on GNU/Linux for use with OpenBW's  [BWAPI fork](https://github.com/OpenBW/bwapi) (2018-04-12).

## Instructions

The Makefile scripts assume the following directory structure:
````
bwapi/
openbw/
BWTA2/
````
*Note: `BWTA2/` will not exist until you clone this repository.*

Run the following commands:
````
git clone https://github.com/adakitesystems/BWTA2.git
cd BWTA2/ && make
````

If the build was successful and complete, you should see a `Release/libBWTA2.so` file.
