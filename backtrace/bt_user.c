#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#define BT_MAX_TRACE 100

void print_trace (void)
{
    void *array[BT_MAX_TRACE];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, BT_MAX_TRACE);
    strings = backtrace_symbols (array, size);

    printf ("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf ("%s\n", strings[i]);

    free (strings);
}


void dummy_function (void)
{
    print_trace ();
}

void test1()
{
 dummy_function();
}

void test2()
{
 test1();
}

int main (void)
{
    test2();
    return 0;
}
