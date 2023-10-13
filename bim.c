#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios original_terminal;

void enable_terminal_raw_mode() {
    tcgetattr(STDOUT_FILENO, &original_terminal);
    struct termios raw = original_terminal;
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_terminal_raw_mode() {
    tcsetattr(STDERR_FILENO, TCSAFLUSH, &original_terminal);
}


int main() {
    atexit(disable_terminal_raw_mode);
    enable_terminal_raw_mode();
    char keyboardInput;
    while (read(STDIN_FILENO, &keyboardInput, 1) == 1);

    return 0;
}