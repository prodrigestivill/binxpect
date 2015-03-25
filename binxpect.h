#define errorlog(...) { fprintf(stderr, ## __VA_ARGS__); /*fflush(stderr);*/ }
#ifdef DEBUG
#define debuglog(...) { fprintf(stderr, ## __VA_ARGS__); /*fflush(stderr);*/ }
#else
#define debuglog(...)
#endif

extern int in, out, tty;

void buffer_submit();
void end();
void speed(long s);
void hangup();
void parse_file(FILE *fd);
void final(int i, char str[]);
