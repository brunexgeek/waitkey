#!/usr/bin/python

#
# MIT License
#
# Copyright (c) 2018 Bruno Ribeiro
# Website: https://github.com/brunexgeek/waitkey
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#


#
# This script is used to generate functions of 'waitkey.h'
# that depends on 'terms.txt' (terminal information file).
#
# You only need to run this if you make changes in 'terms.txt'.
#
# PS: I know, this stuff is pretty ugly :p
#

import sys

terms = { }
tabSize = 4


def makeTab(count):
    return tabSize * count * ' '


def printLevel( table, no ):
    indent = makeTab(no + 1)
    if ('value' in table):
        print(' RESET_AND_RETURN(%s);' % table['value'])
    else:
        print('\n%s{' % makeTab(no))
        print('%sint input = wk_read(%d);' % (indent, 1 if no == 1 else 0))
        count = len(table)
        for kl, vl in table.items():
            sys.stdout.write('%sif (input == %s)' % (indent, kl) )
            printLevel(vl, no + 1)
            count = count - 1
            if (count > 0):
                print('%selse' % indent)
        if (no == 1):
            print('%sRESET_AND_RETURN(input);' % indent)
        else:
            print('%sRESET_AND_RETURN(WKK_NONE);' % indent)
        print('%s}' % makeTab(no))


def printWaitKey(terms):
    indent = makeTab(1)

    # static variables for each known terminal
    for kterm, vterm in terms.items():
        print('static const char *WKT_%s = "%s";' % (kterm.upper(), kterm))

    # print 'WkWaitKey' function
    print('\int WkWaitKey()\n{')
    for kterm, vterm in terms.items():
        sys.stdout.write('%sif (WkGetTerminal() == WKT_%s) /* yes, comparing pointers */' % (indent, kterm.upper()))
        printLevel(vterm, 1)
    print('%sRESET_AND_RETURN(WKK_NONE);\n}' % indent)


def printGetTerm(terms):
    print('''
const char *WkGetTerminal()
{
    static const char *wk_currentTerm = NULL;
    #ifdef WK_WINDOWS
    return wk_currentTerm = WKT_WINDOWS;
    #else
    if (wk_currentTerm != NULL) return wk_currentTerm;
    wk_currentTerm = getenv("TERM");
    if (wk_currentTerm == NULL) return wk_currentTerm = WKT_XTERM;''')
    for kterm, vterm in terms.items():
        print('    if (strcmp(wk_currentTerm, "%s") == 0) return wk_currentTerm = WKT_%s;' % (kterm, kterm.upper()))
    print('''    return wk_currentTerm = WKT_XTERM;
    #endif
}''')


with open('terms.txt', 'rt') as f:
    for line in f:
        line = line.strip()
        if (line == ''): continue
        if line.endswith(':'):
            table = { }
            terms[line[0:-1]] = table
        else:
            currentTable = table
            items = line.split(' ')
            for i in items[:-1]:
                if (i == '='):
                    continue
                if (i not in currentTable):
                    current = { }
                    currentTable[i] = current
                else:
                    current = currentTable[i]
                currentTable = current
            current['value'] = items[-1]

print('/***** BEGIN OF AUTO-GENERATED CODE *****/')
print('#define RESET_AND_RETURN(x)  do { WK_COUNT = 0; return (x); } while(0)')
printWaitKey(terms)
printGetTerm(terms)
print('#undef RESET_AND_RETURN')
print('/***** END OF AUTO-GENERATED CODE *****/')