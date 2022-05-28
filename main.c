#include <stdio.h>

int main()
{
    printf("=============== Welcome to N.P. Banking Services ===============");
    int flag=1;
    while (flag)
    {
        printf("\n1. Sign-In\n2. Sign-Up\n3. Exit\nEnter your choice(integer only): ");
        int ch1;
        scanf("%d",&ch1);
        switch (ch1)
        {
        case 1:
            printf("Sign-In\n");
            break;
        case 2:
            printf("Sign-Up\n");
            break;
        default:
            flag=0;
            printf("Thank you for using our service. Good Day!");
            break;
        }
    }
    return 0;
}