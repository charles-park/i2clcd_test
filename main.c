//------------------------------------------------------------------------------
//
// 2022.04.14 Argument parser app. (chalres-park)
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <getopt.h>

#include "typedefs.h"
#include "i2c-lcd.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
static void tolowerstr (char *p)
{
	int i, c = strlen(p);

	for (i = 0; i < c; i++, p++)
		*p = tolower(*p);
}

//------------------------------------------------------------------------------
static void toupperstr (char *p)
{
	int i, c = strlen(p);

	for (i = 0; i < c; i++, p++)
		*p = toupper(*p);
}

//------------------------------------------------------------------------------
static void print_usage(const char *prog)
{
	printf("Usage: %s [-Dxywhicbs]\n", prog);
	puts("  -D --device        device name. (default /dev/i2c-0).\n"
		 "  -a --i2c_addr      i2c chip address. (default 0x3f).\n"
	     "  -x --x_pos         cursor x pos.(default x = 0)\n"
	     "  -y --y_pos         cursor y pos.(default y = 0)\n"
	     "  -w --width         lcd width.(default w = 16)\n"
	     "  -h --height        lcd height.(default h = 2)\n"
	     "  -i --init          lcd init & cursor home.\n"
	     "  -c --clear_line    lcd clear line.( -1 is clear all)\n"
	     "  -b --backlight     backlight off.(default backlight on)\n"
	     "  -s --str           display string.\n"
		 "  e.g) i2c-lcd -D /dev/i2c-2 -i -s Hello.\n"
	);
	exit(1);
}

//------------------------------------------------------------------------------
static char 	*OPT_DEVICE_NAME = "/dev/i2c-0";
static char 	*OPT_STRING = NULL;
static char 	OPT_X_POS = 0, OPT_Y_POS = 0, OPT_WIDTH = 16, OPT_HEIGHT = 2;
static int		OPT_CLR_LINE = -1;	// -1 is all clear
static uchar_t	OPT_DEVICE_ADDR = 0x3f;
static bool OPT_INIT = false, OPT_BACKLIGHT = true, OPT_CLR_SCREEN = false;

//------------------------------------------------------------------------------
static void parse_opts (int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device_name",	1, 0, 'D' },
			{ "device_addr",	1, 0, 'a' },
			{ "x_pos",			1, 0, 'x' },
			{ "y_pos",			1, 0, 'y' },
			{ "width",			1, 0, 'w' },
			{ "height",			1, 0, 'h' },
			{ "init",			0, 0, 'i' },
			{ "clear_line",		1, 0, 'c' },
			{ "backlight",		0, 0, 'b' },
			{ "string",			1, 0, 's' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:a:x:y:w:h:ic:bs:", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			tolowerstr (optarg);
			OPT_DEVICE_NAME = optarg;
			break;
		case 'a':
			OPT_DEVICE_ADDR = strtol(optarg, NULL, 16) & 0xFF;
			break;
		case 'x':
			OPT_X_POS = atoi(optarg);
			break;
		case 'y':
			OPT_Y_POS = atoi(optarg);
			break;
		case 'w':
			OPT_WIDTH = atoi(optarg);
			break;
		case 'h':
			OPT_HEIGHT = atoi(optarg);
			break;
		case 'i':
			OPT_INIT = true;
			break;
		case 'c':
			OPT_CLR_SCREEN = true;
			OPT_CLR_LINE = atoi(optarg);
			break;
		case 'b':
			OPT_BACKLIGHT = false;
			break;
		case 's':
			OPT_STRING = optarg;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int fd, err = 0;
    parse_opts(argc, argv);

	if ((fd = lcd_open(OPT_DEVICE_NAME,	OPT_DEVICE_ADDR)) == false) {
		err ("i2c-lcd init fail!\n");
		return 0;
	}

	if (OPT_INIT) {
		info ("LCD Init. Device Name = %s. Device Addr = 0x%02X\n",
				OPT_DEVICE_NAME, OPT_DEVICE_ADDR);
		info ("LCD Width = %d, LCD Height = %d, LCD BL = %s\n",
				OPT_WIDTH, OPT_HEIGHT, OPT_BACKLIGHT ? "enable" : "disable");
		if (!lcd_init (fd, OPT_WIDTH, OPT_HEIGHT, OPT_BACKLIGHT)) {
			err ("LCD Init Error!\n");
			return 0;
		}
	}

	err += (lcd_backlight (fd, OPT_BACKLIGHT)) ? 0 : 1;
	if (OPT_CLR_SCREEN)		err += (lcd_clear (fd, OPT_CLR_LINE)) ? 0 : 1;
	if (OPT_STRING != NULL)	err += (lcd_printf (fd, OPT_X_POS, OPT_Y_POS, "%s", OPT_STRING)) ? 0 : 1;
	if (err)
		err ("LCD Control Fail! Device name = %s, Device Addr = 0x%02x\n",
			OPT_DEVICE_NAME, OPT_DEVICE_ADDR);

	return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
