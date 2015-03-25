#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "binxpect.h"
#include "speeds.h"

struct termios termold;
int termset = 0;

struct termios serial_set_raw_speed(int tty, speed_t speed){
	struct termios term, ret;
	if(tcgetattr(tty, &ret) < 0){
		final(1, "Failed to get tty settings.");
	}
	term = ret;

	cfmakeraw(&term);
	cfsetspeed(&term, speed);

	tcflush(tty, TCIFLUSH);
	if(tcsetattr(tty, TCSANOW, &term) < 0){
		final(1, "Failed to set tty settings.");
	}
	return ret;
}

void serial_init(int tty){
	termold = serial_set_raw_speed(tty, getspeed(115200));
	termset = 1;
}

int serial_restore(){
	int ret = 0;
	if (!termset)
		return ret;
	if (isatty(tty)){
		if(tcsetattr(tty, TCSAFLUSH, &termold) < 0){
			errorlog("Failed to reset tty settings.\n");
			ret++;
		}
	}

	return ret; //0 All OK
}

void serial_speed(int tty, speed_t speed){
	serial_set_raw_speed(tty, speed);
}
