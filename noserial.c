#include <termios.h>

//Stub to not touch any serial parameters

struct termios serial_set_raw(int tty){
	struct termios ret;
	return ret;
	
}
void serial_init(){
}
int serial_restore(){
	return 0;
}
void serial_speed(int tty, speed_t speed){
}
