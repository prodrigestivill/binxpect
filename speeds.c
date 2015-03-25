#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>

struct speed_struct
{
  long value;
  speed_t internal;
};

static const struct speed_struct speeds[] =
  {
#ifdef B0
    { 0, B0 },
#endif
#ifdef B50
    { 50, B50 },
#endif
#ifdef B75
    { 75, B75 },
#endif
#ifdef B110
    { 110, B110 },
#endif
#ifdef B134
    { 134, B134 },
#endif
#ifdef B150
    { 150, B150 },
#endif
#ifdef B200
    { 200, B200 },
#endif
#ifdef B300
    { 300, B300 },
#endif
#ifdef B600
    { 600, B600 },
#endif
#ifdef B1200
    { 1200, B1200 },
#endif
#ifdef B1200
    { 1200, B1200 },
#endif
#ifdef B1800
    { 1800, B1800 },
#endif
#ifdef B2400
    { 2400, B2400 },
#endif
#ifdef B4800
    { 4800, B4800 },
#endif
#ifdef B9600
    { 9600, B9600 },
#endif
#ifdef B19200
    { 19200, B19200 },
#endif
#ifdef B38400
    { 38400, B38400 },
#endif
#ifdef B57600
    { 57600, B57600 /* | CBAUDEX */ },
#endif
#ifdef B76800
    { 76800, B76800 /* | CBAUDEX */ },
#endif
#ifdef B115200
    { 115200, B115200 /* | CBAUDEX */ },
#endif
#ifdef B153600
    { 153600, B153600 /* | CBAUDEX */ },
#endif
#ifdef B230400
    { 230400, B230400 /* | CBAUDEX */ },
#endif
#ifdef B307200
    { 307200, B307200 /* | CBAUDEX */ },
#endif
#ifdef B460800
    { 460800, B460800 /* | CBAUDEX */ },
#endif
#ifdef B500000
    { 500000, B500000 /* | CBAUDEX */ },
#endif
#ifdef B576000
    { 576000, B576000 /* | CBAUDEX */ },
#endif
#ifdef B614400
    { 614400, B614400 /* | CBAUDEX */ },
#endif
#ifdef B921600
    { 921600, B921600 /* | CBAUDEX */ },
#endif
#ifdef B1000000
    { 1000000, B1000000 /* | CBAUDEX */ },
#endif
#ifdef B1152000
    { 1152000, B1152000 /* | CBAUDEX */ },
#endif
#ifdef B1500000
    { 1500000, B1500000 /* | CBAUDEX */ },
#endif
#ifdef B1843200
    { 1843200, B1843200 /* | CBAUDEX */ },
#endif
#ifdef B2000000
    { 2000000, B2000000 /* | CBAUDEX */ },
#endif
#ifdef B2500000
    { 2500000, B2500000 /* | CBAUDEX */ },
#endif
#ifdef B3000000
    { 3000000, B3000000 /* | CBAUDEX */ },
#endif
#ifdef B3500000
    { 3500000, B3500000 /* | CBAUDEX */ },
#endif
#ifdef B4000000
    { 4000000, B4000000 /* | CBAUDEX */ },
#endif
#ifdef B6250000
    { 6250000, B6250000 /* | CBAUDEX */ },
#endif
#ifdef B12500000
    { 12500000, B12500000 /* | CBAUDEX */ },
#endif
  };

speed_t getspeed (long speed)
{
  size_t cnt;

  for (cnt = 0; cnt < sizeof (speeds) / sizeof (speeds[0]); ++cnt)
    if (speed == speeds[cnt].value)
      {
	return speeds[cnt].internal;
      }

  errno = EINVAL;

  return -1;
}
