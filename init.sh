#!/bin/sh
#/* ----------------------------------------------------------------------- *
# * PCF8574T backpack module uses 4-bit mode, LCD pins D0-D3 are not used.  *
# * backpack module wiring:                                                 *
# *                                                                         *
# *  PCF8574T     LCD                                                       *
# *  ========     ======                                                    *
# *     P0        RS                                                        *
# *     P1        RW                                                        *
# *     P2        Enable                                                    *
# *     P3        Led Backlight                                             *
# *     P4        D4                                                        *
# *     P5        D5                                                        *
# *     P6        D6                                                        *
# *     P7        D7                                                        *
# *                                                                         *
# * I2C-byte: D7 D6 D5 D4 BL EN RW RS                                       *
# * ----------------------------------------------------------------------- */
#
#  Usage: i2cset [-f] [-y] [-m MASK] [-r] [-a] I2CBUS CHIP-ADDRESS DATA-ADDRESS [VALUE] ... [MODE]
#    I2CBUS is an integer or an I2C bus name
#    ADDRESS is an integer (0x03 - 0x77, or 0x00 - 0x7f if -a is given)
#    MODE is one of:
#      c (byte, no value)
#      b (byte data, default)
#      w (word data)
#      i (I2C block data)
#      s (SMBus block data)
#      Append p for SMBus PEC
#
# * ----------------------------------------------------------------------- *
#
# Device Address find : i2cdetect -y -r {i2c channel}
#
# * ----------------------------------------------------------------------- *
#
# Init Script usage : sudo ./Init.sh {i2c channel} {device addr}
#
# * ----------------------------------------------------------------------- */
# Initialisation Routine Start
sleep 0.1

# wait 15msec, Funcset (lcd startup init.)
i2cset -y $1 $2 0 0x34 0x30 i
sleep 0.1

# wait 4.1msec, Funcset (lcd startup init.)
i2cset -y $1 $2 0 0x34 0x30 i
sleep 0.1

# wait 100usec, Funcset (lcd startup init.)
i2cset -y $1 $2 0 0x34 0x30 i
sleep 0.1

# wait 4.1msec, Funcset (lcd startup init. change funcset)
i2cset -y $1 $2 0 0x24 0x20 i
sleep 0.1

# /* -------------------------------------------------------------------- *
#  * 4-bit mode initialization complete. Now configuring the function set *
#  * -------------------------------------------------------------------- */
# Function set : D5 = 1, D3(N) = 1 (2 lune), D2(F) = 0 (5x8 font), 40usec
i2cset -y $1 $2 0 0x24 0x20 0x84 0x80 i

# /* -------------------------------------------------------------------- *
#  * Next turn display off                                                *
#  * -------------------------------------------------------------------- */
# // Display Control : D3=1, display_on = 0, cursor_on = 0, cursor_blink = 0
i2cset -y $1 $2 0 0x04 0x00 0x84 0x80 i

# /* -------------------------------------------------------------------- *
#  * Display clear, cursor home                                           *
#  * -------------------------------------------------------------------- */
i2cset -y $1 $2 0 0x04 0x00 0x14 0x10 i

# /* -------------------------------------------------------------------- *
#  * Set cursor direction                                                 *
#  * -------------------------------------------------------------------- */
# // Entry Mode : D2 = 1, I/D = 1, S = 0
i2cset -y $1 $2 0 0x04 0x00 0x64 0x60 i

# /* -------------------------------------------------------------------- *
#  * Turn on the display                                                  *
#  * -------------------------------------------------------------------- */
i2cset -y $1 $2 0 0x0c 0x08 0xcc 0xc8 i
i2cset -y $1 $2 0 0x0c 0x08 0x0c 0x08 i

# /* -------------------------------------------------------------------- *
# /* -------------------------------------------------------------------- *
#  * x = 0, y = 0, ODROID
#  * -------------------------------------------------------------------- */
#  * -------------------------------------------------------------------- */
i2cset -y $1 $2 0 0x0c 0x08 0x0c 0x08 i
i2cset -y $1 $2 0 0x8c 0x88 0x0c 0x08 i
i2cset -y $1 $2 0 0x4d 0x49 0xfd 0xf9 0x4d 0x49 0x4d 0x49 0x5d 0x59 0x2d 0x29 0x4d 0x49 0xfd 0xf9 0x4d 0x49 0x9d 0x99 0x4d 0x49 0x4d 0x49 i

#  * -------------------------------------------------------------------- */
#  * -------------------------------------------------------------------- */
