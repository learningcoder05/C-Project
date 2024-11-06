#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct user
{
    char name[50];
    char password[50];
    int balance;
};
int main()
{
    int *ptr;
    ptr = (int *)malloc(3 * sizeof(int));
    for (int i = 0; i < 3; i++)
    {
        printf("Enter the value at %d: ", i);
        scanf("%d", &ptr[i]);
    }

    for (int i = 0; i < 3; i++)
    {
        printf("The value at %d is %d\n", i, ptr[i]);
    }

    ptr = realloc(ptr, 5 * sizeof(int));
    for (int i = 0; i < 5; i++)
    {
        printf("Enter the value at %d: ", i);
        scanf("%d", &ptr[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        printf("The value at %d is %d\n", i, ptr[i]);
    }
    return 0;
}