#include <stdio.h>

#define WK_IMPLEMENTATION
#include "waitkey.h"

int main (int argc, char **argv )
{
	(void) argc;
	(void) argv;

    printf("Press a key between 0 to 7 to print colored text.\nPress capital 'q' to exit.\n");
    WkSetColor(WKC_BLUE, WKC_KEEP);
    int key = 0;
    while (key != WKK_CAP_Q)
    {
        key = WkWaitKey();
        if (key >= '0' && key <= '7')
        {
            WkSetColor((int)(key - '0'), WKC_KEEP);
            printf("Sample text\n");
        }
    };

	WkResetColor();

	return 0;
}
