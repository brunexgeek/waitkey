

#ifndef BRUNEXGEEK_WAITKEY_H
#define BRUNEXGEEK_WAITKEY_H


#ifdef __cplusplus
extern "C" {
#endif


enum
{
    // ASCII keys
    KEY_NONE = 0x00,
    KEY_TAB = 0x09,
    KEY_NEWLINE,
    KEY_RETURN = 0x0D,
    KEY_ESCAPE = 0x1B,
    KEY_SPACE = 0x20,
    KEY_EXCLAMATION,
    KEY_DOUBLE_QUOTE,
    KEY_HASH,
    KEY_DOLLAR,
    KEY_PERCENT,
    KEY_AMPERSAND,
    KEY_SINGLE_QUOTE,
    KEY_LEFT_PAREN,
    KEY_RIGHT_PAREN,
    KEY_ASTERISK,
    KEY_PLUS,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_COLON,
    KEY_SEMICOLON,
    KEY_LESS_THAN,
    KEY_EQUALS,
    KEY_GREATER_THAN,
    KEY_QUESTION,
    KEY_AT,
    KEY_CAP_A,
    KEY_CAP_B,
    KEY_CAP_C,
    KEY_CAP_D,
    KEY_CAP_E,
    KEY_CAP_F,
    KEY_CAP_G,
    KEY_CAP_H,
    KEY_CAP_I,
    KEY_CAP_J,
    KEY_CAP_K,
    KEY_CAP_L,
    KEY_CAP_M,
    KEY_CAP_N,
    KEY_CAP_O,
    KEY_CAP_P,
    KEY_CAP_Q,
    KEY_CAP_R,
    KEY_CAP_S,
    KEY_CAP_T,
    KEY_CAP_U,
    KEY_CAP_V,
    KEY_CAP_W,
    KEY_CAP_X,
    KEY_CAP_Y,
    KEY_CAP_Z,
    KEY_LEFT_BRACKET,
    KEY_BACKSLASH,
    KEY_RIGHT_BRACKET,
    KEY_CARET,
    KEY_UNDERSCORE,
    KEY_GRAVE,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_CURLY,
    KEY_PIPE,
    KEY_RIGHT_CURLY,
    KEY_TILDE,
    KEY_BACKSPACE = 0x7F,

    // special keys
    KEY_UP = 0x1000,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DELETE,
    KEY_HOME,
    KEY_END,
    KEY_PGUP,
    KEY_PGDN,
    KEY_INS,
};


const char *wk_keyName( int key );
int wk_waitKey();


#ifdef WK_IMPLEMENTATION


#ifdef __WINDOWS__
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <langinfo.h>
#include <termios.h>
#endif


const char *wk_keyName(
    int key )
{
    switch (key)
    {
        case KEY_TAB:           return "KEY_TAB";
        case KEY_NEWLINE:       return "KEY_NEWLINE";
        case KEY_RETURN:        return "KEY_RETURN";
        case KEY_ESCAPE:        return "KEY_ESCAPE";
        case KEY_SPACE:         return "KEY_SPACE";
        case KEY_EXCLAMATION:   return "KEY_EXCLAMATION";
        case KEY_DOUBLE_QUOTE:  return "KEY_DOUBLE_QUOTE";
        case KEY_HASH:          return "KEY_HASH";
        case KEY_DOLLAR:        return "KEY_DOLLAR";
        case KEY_PERCENT:       return "KEY_PERCENT";
        case KEY_AMPERSAND:     return "KEY_AMPERSAND";
        case KEY_SINGLE_QUOTE:  return "KEY_SINGLE_QUOTE";
        case KEY_LEFT_PAREN:    return "KEY_LEFT_PAREN";
        case KEY_RIGHT_PAREN:   return "KEY_RIGHT_PAREN";
        case KEY_ASTERISK:      return "KEY_ASTERISK";
        case KEY_PLUS:          return "KEY_PLUS";
        case KEY_COMMA:         return "KEY_COMMA";
        case KEY_MINUS:         return "KEY_MINUS";
        case KEY_PERIOD:        return "KEY_PERIOD";
        case KEY_SLASH:         return "KEY_SLASH";
        case KEY_0:             return "KEY_0";
        case KEY_1:             return "KEY_1";
        case KEY_2:             return "KEY_2";
        case KEY_3:             return "KEY_3";
        case KEY_4:             return "KEY_4";
        case KEY_5:             return "KEY_5";
        case KEY_6:             return "KEY_6";
        case KEY_7:             return "KEY_7";
        case KEY_8:             return "KEY_8";
        case KEY_9:             return "KEY_9";
        case KEY_COLON:         return "KEY_COLON";
        case KEY_SEMICOLON:     return "KEY_SEMICOLON";
        case KEY_LESS_THAN:     return "KEY_LESS_THAN";
        case KEY_EQUALS:        return "KEY_EQUALS";
        case KEY_GREATER_THAN:  return "KEY_GREATER_THAN";
        case KEY_QUESTION:      return "KEY_QUESTION";
        case KEY_AT:            return "KEY_AT";
        case KEY_CAP_A:         return "KEY_CAP_A";
        case KEY_CAP_B:         return "KEY_CAP_B";
        case KEY_CAP_C:         return "KEY_CAP_C";
        case KEY_CAP_D:         return "KEY_CAP_D";
        case KEY_CAP_E:         return "KEY_CAP_E";
        case KEY_CAP_F:         return "KEY_CAP_F";
        case KEY_CAP_G:         return "KEY_CAP_G";
        case KEY_CAP_H:         return "KEY_CAP_H";
        case KEY_CAP_I:         return "KEY_CAP_I";
        case KEY_CAP_J:         return "KEY_CAP_J";
        case KEY_CAP_K:         return "KEY_CAP_K";
        case KEY_CAP_L:         return "KEY_CAP_L";
        case KEY_CAP_M:         return "KEY_CAP_M";
        case KEY_CAP_N:         return "KEY_CAP_N";
        case KEY_CAP_O:         return "KEY_CAP_O";
        case KEY_CAP_P:         return "KEY_CAP_P";
        case KEY_CAP_Q:         return "KEY_CAP_Q";
        case KEY_CAP_R:         return "KEY_CAP_R";
        case KEY_CAP_S:         return "KEY_CAP_S";
        case KEY_CAP_T:         return "KEY_CAP_T";
        case KEY_CAP_U:         return "KEY_CAP_U";
        case KEY_CAP_V:         return "KEY_CAP_V";
        case KEY_CAP_W:         return "KEY_CAP_W";
        case KEY_CAP_X:         return "KEY_CAP_X";
        case KEY_CAP_Y:         return "KEY_CAP_Y";
        case KEY_CAP_Z:         return "KEY_CAP_Z";
        case KEY_LEFT_BRACKET:  return "KEY_LEFT_BRACKET";
        case KEY_BACKSLASH:     return "KEY_BACKSLASH";
        case KEY_RIGHT_BRACKET: return "KEY_RIGHT_BRACKET";
        case KEY_CARET:         return "KEY_CARET";
        case KEY_UNDERSCORE:    return "KEY_UNDERSCORE";
        case KEY_GRAVE:         return "KEY_GRAVE";
        case KEY_A:             return "KEY_A";
        case KEY_B:             return "KEY_B";
        case KEY_C:             return "KEY_C";
        case KEY_D:             return "KEY_D";
        case KEY_E:             return "KEY_E";
        case KEY_F:             return "KEY_F";
        case KEY_G:             return "KEY_G";
        case KEY_H:             return "KEY_H";
        case KEY_I:             return "KEY_I";
        case KEY_J:             return "KEY_J";
        case KEY_K:             return "KEY_K";
        case KEY_L:             return "KEY_L";
        case KEY_M:             return "KEY_M";
        case KEY_N:             return "KEY_N";
        case KEY_O:             return "KEY_O";
        case KEY_P:             return "KEY_P";
        case KEY_Q:             return "KEY_Q";
        case KEY_R:             return "KEY_R";
        case KEY_S:             return "KEY_S";
        case KEY_T:             return "KEY_T";
        case KEY_U:             return "KEY_U";
        case KEY_V:             return "KEY_V";
        case KEY_W:             return "KEY_W";
        case KEY_X:             return "KEY_X";
        case KEY_Y:             return "KEY_Y";
        case KEY_Z:             return "KEY_Z";
        case KEY_LEFT_CURLY:    return "KEY_LEFT_CURLY";
        case KEY_PIPE:          return "KEY_PIPE";
        case KEY_RIGHT_CURLY:   return "KEY_RIGHT_CURLY";
        case KEY_TILDE:         return "KEY_TILDE";
        case KEY_BACKSPACE:     return "KEY_BACKSPACE";
        case KEY_UP:            return "KEY_UP";
        case KEY_DOWN:          return "KEY_DOWN";
        case KEY_RIGHT:         return "KEY_RIGHT";
        case KEY_LEFT:          return "KEY_LEFT";
        case KEY_DELETE:        return "KEY_DELETE";
        case KEY_HOME:          return "KEY_HOME";
        case KEY_END:           return "KEY_END";
        case KEY_PGUP:          return "KEY_PGUP";
        case KEY_PGDN:          return "KEY_PGDN";
        case KEY_INS:           return "KEY_INS";
        default:                return "KEY_NONE";
    }
}


static int wk_interpret(
    const char value )
{
    // standard ASCII character
    if (value != KEY_ESCAPE)
    {
        if (value < KEY_TAB ||
           (value > KEY_RETURN && value < KEY_ESCAPE) ||
           (value > KEY_ESCAPE && value < KEY_SPACE) ||
            value > KEY_BACKSPACE)
            return KEY_NONE;
    }

    return (int) value;
}


static int wk_read()
{
    char current;
    read(STDIN_FILENO, &current, sizeof(current));
    //printf("%d\n", (int) current );
    return wk_interpret(current);
}


int wk_waitKey()
{
    #ifdef __WINDOWS__

    input = _getch();
	// https://msdn.microsoft.com/en-us/library/078sfkak.aspx
	if (input == 0xE0)
		input = 0x00004000 | _getch();

    #else

    struct termios t;

    int input[4];
    int output = KEY_NONE;
    int ii = 0;

    // disable input buffering
    tcgetattr(STDIN_FILENO, &t);
    cc_t cc_previous[2];
    cc_previous[0] = t.c_cc[VTIME];
	cc_previous[1] = t.c_cc[VMIN];
    tcflag_t tc_previous = t.c_lflag;

    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    input[0] = wk_read();
    if (input[0] == KEY_ESCAPE)
    {
        // enable timeout reading
        t.c_cc[VTIME] = 0;
        t.c_cc[VMIN] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);

        input[1] = wk_read();
        //std::cout << "Got #2 " << input << std::endl;
        if (input[1] == KEY_LEFT_BRACKET)
        {
            // disable timeout reading
            t.c_cc[VTIME] = cc_previous[0];
            t.c_cc[VMIN] = cc_previous[1];
            tcsetattr(STDIN_FILENO, TCSANOW, &t);

            input[2] = wk_read();
            if (input[2] == KEY_CAP_A)
                output = KEY_UP;
            else
            if (input[2] == KEY_CAP_B)
                output = KEY_DOWN;
            else
            if (input[2] == KEY_CAP_C)
                output = KEY_RIGHT;
            else
            if (input[2] == KEY_CAP_D)
                output = KEY_LEFT;
            else
            if (input[2] == KEY_2 || input[2] == KEY_3 || input[2] == KEY_5 || input[2] == KEY_6)
            {
                input[3] = wk_read();
                if (input[2] == KEY_2 && input[3] == KEY_TILDE)
                    output = KEY_INS;
                else
                if (input[2] == KEY_3 && input[3] == KEY_TILDE)
                    output = KEY_DELETE;
                else
                if (input[2] == KEY_5 && input[3] == KEY_TILDE)
                    output = KEY_PGUP;
                else
                if (input[2] == KEY_6 && input[3] == KEY_TILDE)
                    output = KEY_PGDN;
            }
            else
            if (input[2] == KEY_CAP_H)
                output = KEY_HOME;
            else
            if (input[2] == KEY_CAP_F)
                output = KEY_END;
        }
        else
            output = KEY_ESCAPE;
    }
    else
        output = input[0];

    // enable input buffering
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag = tc_previous;
	t.c_cc[VTIME] = cc_previous[0];
	t.c_cc[VMIN] = cc_previous[1];
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    #endif

    return output;
}


#endif // WK_IMPLEMENTATION


#ifdef __cplusplus
}
#endif


#endif // #ifndef BRUNEXGEEK_WAITKEY_H
