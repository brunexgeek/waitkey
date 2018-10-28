#include <stdio.h>

#define WK_DEBUG
#define WK_IMPLEMENTATION
#include "waitkey.h"

int main (int argc, char **argv )
{
	(void) argc;
	(void) argv;
	printf("Terminal: '%s'\n", WkGetTerminal());
	printf("Press capital 'q' to exit\n\n");
	int input = 0;
	while (input != WKK_CAP_Q) printf("%s\n", WkGetKeyName(input = WkWaitKey()));
	return 0;
}
