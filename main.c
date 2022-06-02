#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct details
{
    long int acc_no;
    char name[100];
    char passwd[100];
    char email[100];  
};

struct trans
{
    int trno;//Personal transaction number
    char tstr[100];//timesting
    char desc[200];//Description
    float tramt;//transaction amount
    float bal;//balance after transaction
};

int login()
{ //fn1
    int i1=0;
    char usrn[100],pass[100],temp[100]="Hello";
    for (i1;i1<3;i1++)
    {
        printf("\nEnter username: ");
        scanf("%s",usrn);
        printf("\nEnter password: ");
        scanf("%s",pass);
        if (!strcmp(pass,temp)){//Verifying step
            printf("\nSuccess!");
            return 0;
        }
        else
        printf("Wrong username or password!\n");
    }
    printf("Too many invalid attempts. Try again later!\n");
    return -1;
}

void sign_up ()
{
    struct details u1;
    FILE *fptr;
    fptr = fopen("D:\\Programming\\C-Project-Sem-2\\acc_data.txt","a+");
    fseek(fptr, -7, SEEK_END); //Read next account number
    fscanf(fptr,"%d", &u1.acc_no);
    printf("Enter your name: ");
    scanf("%s",&u1.name);
    printf("Enter your email address: ");
    scanf("%s",&u1.email);
    printf("Enter password: ");
    scanf("%s",&u1.passwd); //Re-enter passwd if needed
    fprintf(fptr, ",%s,%s,%s\n%ld", u1.name, u1.email, u1.passwd, u1.acc_no+1);//Append data to file.
    printf("Account created successfully!\n");
    printf("Your account number is: %d\n",u1.acc_no);
    fclose(fptr);
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
            sign_up();
            break;
        default:
            flag=0;
            printf("Thank you for using our service. Good Day!");
            break;
        }
    }
    return 0;
}
