#include <stdio.h>

#define WK_IMPLEMENTATION
#include "waitkey.h"

int main (int argc, char **argv )
{
	int input = 0;
	while (input != KEY_CAP_Q) printf("%s\n", wk_keyName(input = wk_waitKey()));
	return 0;
}
