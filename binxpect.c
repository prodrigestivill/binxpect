#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>

#include "binxpect.h"
#include "speeds.h"
#include "serial.h"

#define MAXBUFLEN 4096
#define MAXREADLEN MAXBUFLEN*2

enum {
	NONE,
	EXPECT,
	SEND
} state = NONE;
unsigned char buf[MAXBUFLEN];
int buflen = 0;
int timeout = 60;

int in = STDIN_FILENO, out = STDOUT_FILENO, tty = -1;
FILE *fd;

void buffer_expect(int fd){
	int i, l, tl = 0, err = 0;
	unsigned char b[MAXBUFLEN];
	fd_set readfds;
	struct timeval to;
	do {
		to.tv_sec = timeout;
		to.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		l = select(fd + 1, &readfds, NULL, NULL, &to);
		if (l == 0){
			final(5, "TIMEOUT");
		}else if (l==-1){
			final(2, "Error waiting input");
		}
		l = read(fd, b, buflen-tl);
		if (l < 1){
			final(2, "Error reading input");
		}
#ifdef DEBUG
		debuglog("Recived: ");
		for (i=0;i<l;i++){
			debuglog("%02X",b[i]);
		}
#endif
		if ((l==1 && buf[tl] == b[0]) || (l>1 && memcmp(&buf[tl], b, l)==0)){
			debuglog(" OK\n");
			tl += l;
		}else{
#ifdef DEBUG
			if (err){
				debuglog(" ERR\nExpected:");
			}else{
				debuglog(" ERR FIRST FAIL\nExpected:");
			}
			for (i=0;i<l;i++){
				debuglog("%02X",buf[tl+i]);
			}
			debuglog("\n");
//EVEN MORE DEBUG
			do {
				to.tv_sec = timeout;
				to.tv_usec = 0;
				FD_ZERO(&readfds);
				FD_SET(fd, &readfds);
				l = select(fd + 1, &readfds, NULL, NULL, &to);
				if (l == 0 || l==-1) break;
				l = read(fd, b, buflen-tl);
				if (l<1) break;
				debuglog("Recived: ");
				for (i=0;i<l;i++){
					debuglog("%02X",b[i]);
				}
				debuglog(" ---\n");
			}while(1);
//END
			tl += l;
			err = 1;
			break;
#else
			err = 1;
			break;
#endif
		}
	}while(tl < buflen);
	if (err || tl!=buflen)
		final(2, "Unexpected content.");
	buflen = 0;
}

void buffer_submit(){
	int i;
	if (buflen == 0) return;
	switch (state){
		case EXPECT:
			buffer_expect(in);
		break;
		case SEND:
			if (write(out, buf, buflen) != buflen){
				final(3, "Error writing output");
			}else if (tty != -1){
				if (tcdrain(tty) == -1){
	  				perror("Failed to drain tty");
					final(3, NULL);
				}
			}
#ifdef DEBUG
			debuglog("Send:    ");
			for (i=0;i<buflen;i++){
				debuglog("%02X",buf[i]);
			}
			debuglog("\n");
#endif	
			buflen = 0;
		break;
		case NONE:
		break;
	}
}

void buffer_fill(FILE *fd){
	int i,l;
	char b[MAXREADLEN];
	char c[3] = {0,0,0};
	unsigned int h;
	if (state == NONE) return;
	while (fgets(b, sizeof b, fd) != NULL){
		l = strnlen(b, sizeof b);
		if (l<1 || b[0] == '#') continue;
		if (l>2 && strncmp("END", b, 3)==0){
			end();
			break;
		}else{
			// Until last-1 coz hex are 2 chars
			for(i=0; i<(l-1); i++){
				if (b[i]=='\0' || b[i+1]=='\0')
					break;
				// inc by 1, so only test pos i, no need for i+1
				if (b[i]==' ' || b[i]=='\t' || b[i]== '\r' || b[i]=='\n')
					continue;
				c[0] = b[i];
				c[1] = b[++i];
				if (sscanf(c, "%02X", &h) < 1){
					final(2, "Error parsing file");
				}
				if (buflen+1>sizeof buf){
					buffer_submit();
				}
				buf[buflen++] = h;
			}
		}
	}
}

void end(){
	buffer_submit();
	state = NONE;
	buflen = 0;
}

void speed(long s){
	speed_t speed = getspeed(s);
	if (speed == -1){
  		perror("Failed to find speed on tty");
		final(2, NULL);
	}
	if (tty!=-1 && isatty(tty))
		serial_speed(in, speed);
	else
		errorlog("WARN: SPEED ignored no tty.\n");
	//delay 0.1 seconds so term parameters have time to change
	usleep(100000);
}

void hangup(){
	const speed_t speed = B0; //to 0 to hangup
	if (tty != -1 && isatty(tty))
		serial_speed(tty, speed);		
	else
		errorlog("WARN: HANGUP ignored no tty specified.\n");
}

void parse_file(FILE *fd){
	int l;
	char b[MAXREADLEN];
	while (fgets(b, sizeof b, fd) != NULL){
		l = strnlen(b, sizeof b);
		if (l<1 || b[0] == '#') continue;
		if (l>8 && strncmp("TIMEOUT ", b, 8)==0){
			b[(sizeof b)-1] = '\0';
			timeout = atoi(&b[8]);
		}else if (l>6 && strncmp("SLEEP ", b, 6)==0){
			b[(sizeof b)-1] = '\0';
			sleep(atoi(&b[8]));
		}else if (l>6 && strncmp("SPEED ", b, 6)==0){
			b[(sizeof b)-1] = '\0';
			speed(atol(&b[6]));
		}else if (l>5 && strncmp("HANGUP", b, 6)==0){
			hangup();
		}else if (l>2 && strncmp("END", b, 3)==0){
			errorlog("WARN: incorrect END placement.\n");
			end();
		}else if (l>3 && strncmp("SEND", b, 4)==0){
			state = SEND;
			buffer_fill(fd);
		}else if (l>5 && strncmp("EXPECT", b, 6)==0){
			state = EXPECT;
			buffer_fill(fd);
		}else{
			final(2, "Error parsing file commands.");
		}
	}
}


void final(int i, char str[]){
	if (fd != NULL)
		fclose(fd);
	serial_restore();
	if (tty != -1)
		close(tty);
	if (str != NULL)
		errorlog("\n%s\n", str);
	exit(i);
}

void usage(char *str){
	errorlog("Usage: %s [xpect-file] <[tty to use]>\n\n", str);
	//errorlog("       File format:");
	final(1, NULL);
}

void sigcatch(int sig){
	final(1, "SIGNAL");
}

int main(int argc, char **argv){
	/* Catch the most popular signals. */
	if(signal(SIGINT,sigcatch) == SIG_ERR){
		perror("signal");
		final(1, NULL);
	}
	if(signal(SIGQUIT,sigcatch) == SIG_ERR){
		perror("signal");
		final(1, NULL);
	}
	if(signal(SIGTERM,sigcatch) == SIG_ERR){
		perror("signal");
		final(1, NULL);
	}
	/* Parse args */
	if (argc < 2){
		usage(argc>0?argv[0]:"prog");
	}
	fd = fopen(argv[1], "r");
	if (fd == NULL){
		perror("Failed to open file");
		final(1, NULL);
	}
	if (argc > 2){
		tty = open(argv[2], O_RDWR | O_NOCTTY | O_SYNC);
		if(tty == -1) {
	  		perror("Failed to open tty");
			final(1, NULL);
		}else{
			in = out = tty;
			serial_init(tty);
		}
	}
	/* Run */
	parse_file(fd);
	final(0, NULL);
	return 0;
}
