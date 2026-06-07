#include<stdio.h>
#include<stdlib.h>

#define PI 3.14
#define MAX 100
#define MSG "hello world"

/* multi line
   comment test */

int main()
{
    // single line comment
    int r = 5;

    /* inline block comment */ int x = 10;

    float area = PI * r * r;

    int arr[MAX];

    int val = MAX;

    printf("%s\n", MSG);
    printf("area = %f\n", area);
    printf("x = %d\n", x);
    printf("val = %d\n", val);

    char *str = "PI is not /* a comment */ here";
    printf("%s\n", str);

    return 0;
}
