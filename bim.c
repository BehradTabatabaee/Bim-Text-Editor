#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

// a global variable containing the original terminal state of the logged in user
struct termios original_terminal;

void die(const char *s)
{
    perror(s);
    exit(1);
}

void enable_terminal_raw_mode()
{
    if (tcgetattr(STDIN_FILENO, &original_terminal) == -1)
    {
        die("tcgetattr");
    }

    struct termios raw = original_terminal;

    // turning off the echo, canonical and interrupt signal flags
    // disabling interrupt signals prevents the use of ctrl-c and ctrl-z
    // turning the canonical flag off results in us being able to read data byte-by-byte
    // this nullifies the need to enter an entire line and the need to press enter after typing a line
    // the IEXTEN flag specifies the use of ctrl-v, which while enabled forces the terminal to read the next control character literally

    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

    // disabling the ctrl-s and ctrl-q and other similar software flow control shortcuts
    // to see exactly what each of these flags do, please search the internet
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);

    raw.c_cflag |= (CS8);

    // disabling the OPOST flags prevents the terminal from changing the literal output and stops any interpretration
    raw.c_oflag &= ~(OPOST);

    // return outside the read function with a minimum bytes read of 0 and a maximum time of 100 milliseconds
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}

void disable_terminal_raw_mode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal) == -1)
    {
        die("tcsetattr");
    }
}

int main()
{
    atexit(disable_terminal_raw_mode);
    enable_terminal_raw_mode();
    char keyboardInput;
    while (1)
    {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
        {
            die("read");
        }
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')
        {
            break;
        }
    }
    return 0;
}