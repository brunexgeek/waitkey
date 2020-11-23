/*
 * MIT License
 *
 * Copyright (c) 2020 Bruno Ribeiro
 * Website: https://github.com/brunexgeek/waitkey
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SINGLEFILE_WAITWKK_H
#define SINGLEFILE_WAITWKK_H


#ifdef __cplusplus
extern "C" {
#endif

#ifdef WK_DEBUG
#include <stdio.h>
#endif


enum
{
    // ASCII keys
    WKK_NONE = 0x00,
    WKK_TAB = 0x09,
    WKK_NEWLINE,
    WKK_RETURN = 0x0D,
    WKK_ESCAPE = 0x1B,
    WKK_SPACE = 0x20,
    WKK_EXCLAMATION,
    WKK_DOUBLE_QUOTE,
    WKK_HASH,
    WKK_DOLLAR,
    WKK_PERCENT,
    WKK_AMPERSAND,
    WKK_SINGLE_QUOTE,
    WKK_LEFT_PAREN,
    WKK_RIGHT_PAREN,
    WKK_ASTERISK,
    WKK_PLUS,
    WKK_COMMA,
    WKK_MINUS,
    WKK_PERIOD,
    WKK_SLASH,
    WKK_0,
    WKK_1,
    WKK_2,
    WKK_3,
    WKK_4,
    WKK_5,
    WKK_6,
    WKK_7,
    WKK_8,
    WKK_9,
    WKK_COLON,
    WKK_SEMICOLON,
    WKK_LESS_THAN,
    WKK_EQUALS,
    WKK_GREATER_THAN,
    WKK_QUESTION,
    WKK_AT,
    WKK_CAP_A,
    WKK_CAP_B,
    WKK_CAP_C,
    WKK_CAP_D,
    WKK_CAP_E,
    WKK_CAP_F,
    WKK_CAP_G,
    WKK_CAP_H,
    WKK_CAP_I,
    WKK_CAP_J,
    WKK_CAP_K,
    WKK_CAP_L,
    WKK_CAP_M,
    WKK_CAP_N,
    WKK_CAP_O,
    WKK_CAP_P,
    WKK_CAP_Q,
    WKK_CAP_R,
    WKK_CAP_S,
    WKK_CAP_T,
    WKK_CAP_U,
    WKK_CAP_V,
    WKK_CAP_W,
    WKK_CAP_X,
    WKK_CAP_Y,
    WKK_CAP_Z,
    WKK_LEFT_BRACKET,
    WKK_BACKSLASH,
    WKK_RIGHT_BRACKET,
    WKK_CARET,
    WKK_UNDERSCORE,
    WKK_GRAVE,
    WKK_A,
    WKK_B,
    WKK_C,
    WKK_D,
    WKK_E,
    WKK_F,
    WKK_G,
    WKK_H,
    WKK_I,
    WKK_J,
    WKK_K,
    WKK_L,
    WKK_M,
    WKK_N,
    WKK_O,
    WKK_P,
    WKK_Q,
    WKK_R,
    WKK_S,
    WKK_T,
    WKK_U,
    WKK_V,
    WKK_W,
    WKK_X,
    WKK_Y,
    WKK_Z,
    WKK_LEFT_CURLY,
    WKK_PIPE,
    WKK_RIGHT_CURLY,
    WKK_TILDE,
    WKK_BACKSPACE = 0x7F,

    // special keys
    WKK_UP = 0x1000,
    WKK_DOWN,
    WKK_RIGHT,
    WKK_LEFT,
    WKK_DELETE,
    WKK_HOME,
    WKK_END,
    WKK_PGUP,
    WKK_PGDN,
    WKK_INS,
};

enum wk_color_t
{
    WKC_BLACK = 0,
    WKC_RED,
    WKC_GREEN,
    WKC_YELLOW,
    WKC_BLUE,
    WKC_MAGENTA,
    WKC_CYAN,
    WKC_WHITE,
    WKC_KEEP,
    WKC_DEFAULT
};

#ifndef WK_NO_NAMES
const char *WkGetKeyName( int key );
#endif

int WkWaitKey();
const char *WkGetTerminal();
void WkGetScreenSize( int *rows, int *cols );
void WkSetColor( int foreground, int background );
int WkIsTerminal();

#define WkResetColor()  WkSetColor(WKC_DEFAULT, WKC_DEFAULT)

#ifdef WK_IMPLEMENTATION


#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
#include <windows.h>
#include <conio.h>
#define WK_WINDOWS
#else
#include <unistd.h>
#include <sys/stat.h>
#include <langinfo.h>
#include <termios.h>
#include <sys/ioctl.h>
#endif
#include <stdlib.h>
#include <string.h>


static char WK_BUFFER[8];
static int WK_BEGIN = 0;
static int WK_COUNT = 0;


static int wk_read_input()
{
    if (WK_COUNT > 0) return 0;

    WK_BEGIN = WK_COUNT = 0;

    #ifdef WK_WINDOWS

    // wait for the first character
    WK_BUFFER[WK_COUNT++] = _getch();
    // read all remaining characters
    while (_kbhit() && WK_COUNT < sizeof(WK_BUFFER))
        WK_BUFFER[WK_COUNT++] = _getch();

    #else

    struct termios t;
    tcflag_t tc_previous = 0;
    size_t pending = 0;
    ssize_t result = 0;

    // set terminal to non-canonical mode
    tcgetattr(STDIN_FILENO, &t);
    tc_previous = t.c_lflag;
    t.c_lflag = t.c_lflag & (tcflag_t) (~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    // wait for the first character
    result = read(STDIN_FILENO, WK_BUFFER, 1);
    if (result == 1)
    {
        WK_COUNT = 1;
        // check whether we have any more data
        ioctl(STDIN_FILENO, FIONREAD, &pending);
        if (pending > sizeof(WK_BUFFER) - (size_t) WK_COUNT)
            pending = sizeof(WK_BUFFER) - (size_t) WK_COUNT;
        if (pending)
        {
            result = read(STDIN_FILENO, WK_BUFFER + WK_COUNT, pending);
            if (result == (ssize_t) pending) WK_COUNT += (int) pending;
        }
        //printf("Read %d characters\n", WK_COUNT);
    }

    // restore terminal configuration
    t.c_lflag = tc_previous;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    #endif

    return WK_COUNT;
}


#ifndef WK_NO_NAMES

const char *WkGetKeyName( int key )
{
    switch (key)
    {
        case WKK_TAB:           return "WKK_TAB";
        case WKK_NEWLINE:       return "WKK_NEWLINE";
        case WKK_RETURN:        return "WKK_RETURN";
        case WKK_ESCAPE:        return "WKK_ESCAPE";
        case WKK_SPACE:         return "WKK_SPACE";
        case WKK_EXCLAMATION:   return "WKK_EXCLAMATION";
        case WKK_DOUBLE_QUOTE:  return "WKK_DOUBLE_QUOTE";
        case WKK_HASH:          return "WKK_HASH";
        case WKK_DOLLAR:        return "WKK_DOLLAR";
        case WKK_PERCENT:       return "WKK_PERCENT";
        case WKK_AMPERSAND:     return "WKK_AMPERSAND";
        case WKK_SINGLE_QUOTE:  return "WKK_SINGLE_QUOTE";
        case WKK_LEFT_PAREN:    return "WKK_LEFT_PAREN";
        case WKK_RIGHT_PAREN:   return "WKK_RIGHT_PAREN";
        case WKK_ASTERISK:      return "WKK_ASTERISK";
        case WKK_PLUS:          return "WKK_PLUS";
        case WKK_COMMA:         return "WKK_COMMA";
        case WKK_MINUS:         return "WKK_MINUS";
        case WKK_PERIOD:        return "WKK_PERIOD";
        case WKK_SLASH:         return "WKK_SLASH";
        case WKK_0:             return "WKK_0";
        case WKK_1:             return "WKK_1";
        case WKK_2:             return "WKK_2";
        case WKK_3:             return "WKK_3";
        case WKK_4:             return "WKK_4";
        case WKK_5:             return "WKK_5";
        case WKK_6:             return "WKK_6";
        case WKK_7:             return "WKK_7";
        case WKK_8:             return "WKK_8";
        case WKK_9:             return "WKK_9";
        case WKK_COLON:         return "WKK_COLON";
        case WKK_SEMICOLON:     return "WKK_SEMICOLON";
        case WKK_LESS_THAN:     return "WKK_LESS_THAN";
        case WKK_EQUALS:        return "WKK_EQUALS";
        case WKK_GREATER_THAN:  return "WKK_GREATER_THAN";
        case WKK_QUESTION:      return "WKK_QUESTION";
        case WKK_AT:            return "WKK_AT";
        case WKK_CAP_A:         return "WKK_CAP_A";
        case WKK_CAP_B:         return "WKK_CAP_B";
        case WKK_CAP_C:         return "WKK_CAP_C";
        case WKK_CAP_D:         return "WKK_CAP_D";
        case WKK_CAP_E:         return "WKK_CAP_E";
        case WKK_CAP_F:         return "WKK_CAP_F";
        case WKK_CAP_G:         return "WKK_CAP_G";
        case WKK_CAP_H:         return "WKK_CAP_H";
        case WKK_CAP_I:         return "WKK_CAP_I";
        case WKK_CAP_J:         return "WKK_CAP_J";
        case WKK_CAP_K:         return "WKK_CAP_K";
        case WKK_CAP_L:         return "WKK_CAP_L";
        case WKK_CAP_M:         return "WKK_CAP_M";
        case WKK_CAP_N:         return "WKK_CAP_N";
        case WKK_CAP_O:         return "WKK_CAP_O";
        case WKK_CAP_P:         return "WKK_CAP_P";
        case WKK_CAP_Q:         return "WKK_CAP_Q";
        case WKK_CAP_R:         return "WKK_CAP_R";
        case WKK_CAP_S:         return "WKK_CAP_S";
        case WKK_CAP_T:         return "WKK_CAP_T";
        case WKK_CAP_U:         return "WKK_CAP_U";
        case WKK_CAP_V:         return "WKK_CAP_V";
        case WKK_CAP_W:         return "WKK_CAP_W";
        case WKK_CAP_X:         return "WKK_CAP_X";
        case WKK_CAP_Y:         return "WKK_CAP_Y";
        case WKK_CAP_Z:         return "WKK_CAP_Z";
        case WKK_LEFT_BRACKET:  return "WKK_LEFT_BRACKET";
        case WKK_BACKSLASH:     return "WKK_BACKSLASH";
        case WKK_RIGHT_BRACKET: return "WKK_RIGHT_BRACKET";
        case WKK_CARET:         return "WKK_CARET";
        case WKK_UNDERSCORE:    return "WKK_UNDERSCORE";
        case WKK_GRAVE:         return "WKK_GRAVE";
        case WKK_A:             return "WKK_A";
        case WKK_B:             return "WKK_B";
        case WKK_C:             return "WKK_C";
        case WKK_D:             return "WKK_D";
        case WKK_E:             return "WKK_E";
        case WKK_F:             return "WKK_F";
        case WKK_G:             return "WKK_G";
        case WKK_H:             return "WKK_H";
        case WKK_I:             return "WKK_I";
        case WKK_J:             return "WKK_J";
        case WKK_K:             return "WKK_K";
        case WKK_L:             return "WKK_L";
        case WKK_M:             return "WKK_M";
        case WKK_N:             return "WKK_N";
        case WKK_O:             return "WKK_O";
        case WKK_P:             return "WKK_P";
        case WKK_Q:             return "WKK_Q";
        case WKK_R:             return "WKK_R";
        case WKK_S:             return "WKK_S";
        case WKK_T:             return "WKK_T";
        case WKK_U:             return "WKK_U";
        case WKK_V:             return "WKK_V";
        case WKK_W:             return "WKK_W";
        case WKK_X:             return "WKK_X";
        case WKK_Y:             return "WKK_Y";
        case WKK_Z:             return "WKK_Z";
        case WKK_LEFT_CURLY:    return "WKK_LEFT_CURLY";
        case WKK_PIPE:          return "WKK_PIPE";
        case WKK_RIGHT_CURLY:   return "WKK_RIGHT_CURLY";
        case WKK_TILDE:         return "WKK_TILDE";
        case WKK_BACKSPACE:     return "WKK_BACKSPACE";
        case WKK_UP:            return "WKK_UP";
        case WKK_DOWN:          return "WKK_DOWN";
        case WKK_RIGHT:         return "WKK_RIGHT";
        case WKK_LEFT:          return "WKK_LEFT";
        case WKK_DELETE:        return "WKK_DELETE";
        case WKK_HOME:          return "WKK_HOME";
        case WKK_END:           return "WKK_END";
        case WKK_PGUP:          return "WKK_PGUP";
        case WKK_PGDN:          return "WKK_PGDN";
        case WKK_INS:           return "WKK_INS";
        default:                return "WKK_NONE";
    }
}

#endif // !WK_NO_NAMES


static int wk_interpret( const char value )
{
    if (value != WKK_ESCAPE)
    {
        if (value < WKK_TAB ||
           (value > WKK_RETURN && value < WKK_ESCAPE) ||
           (value > WKK_ESCAPE && value < WKK_SPACE))
            return WKK_NONE;
    }
    return (int) value;
}


static int wk_read( int deviceRead )
{
    if (WK_COUNT != 0 || (deviceRead && wk_read_input() > 0))
    {
        //printf("Consumed 1 character from %d\n", WK_COUNT);
        WK_COUNT--;
        #ifdef WK_DEBUG
        printf("%d [%s]\n", WK_BUFFER[WK_BEGIN], WkGetKeyName(WK_BUFFER[WK_BEGIN]));
        #endif
        return wk_interpret(WK_BUFFER[WK_BEGIN++]);
    }
    return WKK_NONE;
}


/***** BEGIN OF AUTO-GENERATED CODE *****/
#define RESET_AND_RETURN(x)  do { WK_COUNT = 0; return (x); } while(0)
static const char *WKT_WINDOWS = "windows";
static const char *WKT_XTERM = "xterm";
static const char *WKT_LINUX = "linux";

int WkWaitKey()
{
    if (WkGetTerminal() == WKT_WINDOWS) /* yes, comparing pointers */
    {
        int input = wk_read(1);
        if (input == 0)
        {
            int input = wk_read(0);
            if (input == 77) RESET_AND_RETURN(WKK_LEFT);
            else
            if (input == 75) RESET_AND_RETURN(WKK_RIGHT);
            else
            if (input == 73) RESET_AND_RETURN(WKK_PGUP);
            else
            if (input == 72) RESET_AND_RETURN(WKK_UP);
            else
            if (input == 71) RESET_AND_RETURN(WKK_HOME);
            else
            if (input == 82) RESET_AND_RETURN(WKK_INS);
            else
            if (input == 83) RESET_AND_RETURN(WKK_DELETE);
            else
            if (input == 80) RESET_AND_RETURN(WKK_DOWN);
            else
            if (input == 81) RESET_AND_RETURN(WKK_PGDN);
            else
            if (input == 79) RESET_AND_RETURN(WKK_END);
            RESET_AND_RETURN(WKK_NONE);
        }
        else
        if (input == 224)
        {
            int input = wk_read(0);
            if (input == 77) RESET_AND_RETURN(WKK_LEFT);
            else
            if (input == 75) RESET_AND_RETURN(WKK_RIGHT);
            else
            if (input == 73) RESET_AND_RETURN(WKK_PGUP);
            else
            if (input == 72) RESET_AND_RETURN(WKK_UP);
            else
            if (input == 71) RESET_AND_RETURN(WKK_HOME);
            else
            if (input == 82) RESET_AND_RETURN(WKK_INS);
            else
            if (input == 83) RESET_AND_RETURN(WKK_DELETE);
            else
            if (input == 80) RESET_AND_RETURN(WKK_DOWN);
            else
            if (input == 81) RESET_AND_RETURN(WKK_PGDN);
            else
            if (input == 79) RESET_AND_RETURN(WKK_END);
            RESET_AND_RETURN(WKK_NONE);
        }
        RESET_AND_RETURN(input);
    }
    if (WkGetTerminal() == WKT_XTERM) /* yes, comparing pointers */
    {
        int input = wk_read(1);
        if (input == WKK_ESCAPE)
        {
            int input = wk_read(0);
            if (input == WKK_LEFT_BRACKET)
            {
                int input = wk_read(0);
                if (input == WKK_CAP_H) RESET_AND_RETURN(WKK_HOME);
                else
                if (input == WKK_3)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_DELETE);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_2)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_INS);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_5)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_PGUP);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_6)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_PGDN);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_CAP_A) RESET_AND_RETURN(WKK_UP);
                else
                if (input == WKK_CAP_B) RESET_AND_RETURN(WKK_DOWN);
                else
                if (input == WKK_CAP_C) RESET_AND_RETURN(WKK_RIGHT);
                else
                if (input == WKK_CAP_D) RESET_AND_RETURN(WKK_LEFT);
                else
                if (input == WKK_CAP_F) RESET_AND_RETURN(WKK_END);
                RESET_AND_RETURN(WKK_NONE);
            }
            RESET_AND_RETURN(WKK_NONE);
        }
        RESET_AND_RETURN(input);
    }
    if (WkGetTerminal() == WKT_LINUX) /* yes, comparing pointers */
    {
        int input = wk_read(1);
        if (input == WKK_ESCAPE)
        {
            int input = wk_read(0);
            if (input == WKK_LEFT_BRACKET)
            {
                int input = wk_read(0);
                if (input == WKK_1)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_HOME);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_3)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_DELETE);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_2)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_INS);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_5)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_PGUP);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_4)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_END);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_6)
                {
                    int input = wk_read(0);
                    if (input == WKK_TILDE) RESET_AND_RETURN(WKK_PGDN);
                    RESET_AND_RETURN(WKK_NONE);
                }
                else
                if (input == WKK_CAP_A) RESET_AND_RETURN(WKK_UP);
                else
                if (input == WKK_CAP_B) RESET_AND_RETURN(WKK_DOWN);
                else
                if (input == WKK_CAP_C) RESET_AND_RETURN(WKK_RIGHT);
                else
                if (input == WKK_CAP_D) RESET_AND_RETURN(WKK_LEFT);
                RESET_AND_RETURN(WKK_NONE);
            }
            RESET_AND_RETURN(WKK_NONE);
        }
        RESET_AND_RETURN(input);
    }
    RESET_AND_RETURN(WKK_NONE);
}


static const char *wk_match_term( const char *def )
{
    const char *value = getenv("TERM");
    if (strncmp(value, "windows", 7) == 0) return WKT_WINDOWS;
    if (strncmp(value, "xterm", 5) == 0) return WKT_XTERM;
    if (strncmp(value, "linux", 5) == 0) return WKT_LINUX;
    return def;
}
#undef RESET_AND_RETURN
/***** END OF AUTO-GENERATED CODE *****/

const char *WkGetTerminal()
{
    #ifdef WK_WINDOWS
    return wk_match_term(WKT_WINDOWS);
    #else
    return wk_match_term(WKT_XTERM);
    #endif
}

void WkGetScreenSize( int *rows, int *cols )
{
    #ifdef WK_WINDOWS

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	if (GetConsoleScreenBufferInfoEx(handle, &info) == TRUE)
	{
		if (rows) *rows = info.srWindow.Bottom - info.swWindow.Top;
		if (cols) *cols = info.srWindow.Right - info.swWindow.Left;
	}

    #else

    struct winsize sz;
    if (ioctl(0, TIOCGWINSZ, &sz) == 0)
    {
        if (rows) *rows = sz.ws_row;
        if (cols) *cols = sz.ws_col;
    }

    #endif
}


void WkSetColor(
    int foreground,
    int background )
{
    if (!WkIsTerminal()) return;
    if (foreground < 0 || foreground > 9) return;
    if (background < 0 || background > 9) return;

    #ifdef WK_WINDOWS

    #else

    if (!isatty(STDOUT_FILENO)) return;

    if (foreground != WKC_KEEP)
        printf("\033[3%dm", foreground);
    if (background != WKC_KEEP)
        printf("\033[4%dm", background);

    #endif
}


int WkIsTerminal()
{
    #ifdef WK_WINDOWS
    return _isatty( _fileno( stdout ) );
    #else
    return isatty(STDIN_FILENO);
    #endif
}


#endif // WK_IMPLEMENTATION


#ifdef __cplusplus
}
#endif


#endif // #ifndef SINGLEFILE_WAITWKK_H
