#include <stdio.h>

int login(){ //fn1
    int i1=0;
    char usrn[100],pass[100];
    for (i1;i1<3;i1++)
    {
        printf("\nEnter username: ");
        scanf("%s",usrn);
        printf("\nEnter password: ");
        scanf("%s",pass);
        if (usrn[0]=='0'){//Verifying step
            printf("\nSuccess!");
            return 0;
        }
        else
        printf("Wrong username or password!\n");
    }
    printf("Too many invalid attempts. Try again later!\n");
    return -1;
}

int main()
{
    printf("=============== Welcome to N.P. Banking Services ===============");
    int flag=1;
    while (flag)
    {
        printf("\n1. Log-In\n2. Sign-Up\n3. Exit\nEnter your choice(integer only): ");
        int ch1;
        scanf("%d",&ch1);
        switch (ch1)
        {
        case 1:
            printf("Log-In\n");
            int resp1;
            resp1 = login();
            if (resp1==0){
                printf("Successfully logged in!");//next menu
            }
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