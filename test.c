#include <stdio.h>
void    hhhh(char *s)
{
    int i = 0;
    while (s[i])
    {
        i++;
    }
    printf("%d", i);
}

int main()
{
    char *s = "taoufiq";
    hhhh(s);
}
