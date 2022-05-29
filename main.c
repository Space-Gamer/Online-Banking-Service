#include <stdio.h>
void sign_in ()
{
    char name[50] ;
    char address[50];
    int phonenor;
    int password;
    printf("Enter your name,address,phonenor,passowrd\n");
    scanf("%s",&name);
    scanf("%s",&address);
    scanf("%d",&phonenor);
    scanf("%d",&password);
}
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
            sign_in();
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
