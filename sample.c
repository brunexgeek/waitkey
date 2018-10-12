#include <stdio.h>

#define WK_DEBUG
#define WK_IMPLEMENTATION
#include "waitkey.h"

int main (int argc, char **argv )
{
	(void) argc;
	(void) argv;
	//wk_read_input(STDIN_FILENO);
	//return 0;
	int input = 0;
	while (input != WKK_CAP_Q) printf("%s\n", wk_keyName(input = wk_waitKey()));
	return 0;
}
