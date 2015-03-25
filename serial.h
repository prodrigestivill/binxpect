#include <termios.h>

struct termios serial_set_raw(int tty);
void serial_init();
int serial_restore();
void serial_speed(int tty, speed_t speed);
