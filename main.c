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

int logchk(char *usr, char *pass)
{
    FILE *fptr;
    long int acc_no;
    fptr = fopen("D:\\Programming\\C-Project-Sem-2\\acc_data.txt","r");
    fseek(fptr, -7, SEEK_END);
    fscanf(fptr,"%ld", &acc_no);//Last non-existent account number
    long int a;//Current account number
    char u[100],e[100],p[100];
    fseek(fptr, 0, SEEK_SET);
    fscanf(fptr,"%ld",&a);
    while (a!=acc_no)
    {
        fscanf(fptr, ",%[^,],%[^,],%s", u, e, p);
        if (!strcmp(usr,u))
        {
            if (!strcmp(pass,p))
            return 0;//Correct password
            else
            return 1;//Wrong password
        }
        fscanf(fptr,"%ld",&a);
    };
    fclose(fptr);
    return -1;//Username not found
}

int login()
{ //fn1
    int i1=0;
    char usrn[100],pass[100];
    for (i1;i1<3;i1++) //Three chances for wrong password.
    {
        printf("\nEnter username: ");
        scanf("%s",usrn);
        printf("\nEnter password: ");
        scanf("%s",pass);
        int status;
        status = logchk(usrn, pass);
        if (status == 0){//Verifying step
            printf("\nSuccess!");
            return 0;
        }
        else if (status == 1)
        printf("Wrong password!\n");
        else
        {
            printf("Account with name '%s' doesn't exist. Sign-Up to create one!\n",usrn);
            return -1;
        }
    }
    printf("Too many invalid attempts. Try again later!\n");
    return -1;
}

void sign_up ()
{
    struct details u1;
    FILE *fptr,*fptr2;
    fptr = fopen("D:\\Programming\\C-Project-Sem-2\\acc_data.txt","a+");
    fseek(fptr, -7, SEEK_END); //Read next account number
    fscanf(fptr,"%ld", &u1.acc_no);
    printf("Enter your name: ");
    scanf("%s",&u1.name);
    printf("Enter your email address: ");
    scanf("%s",&u1.email);
    printf("Enter password: ");
    scanf("%s",&u1.passwd); //Re-enter passwd if needed
    int stat;
    stat = logchk(u1.name, u1.passwd);
    if (!(stat==-1))
    {
        printf("Username '%s' already exists! Kindly login to access.\n",u1.name);
        fclose(fptr);
        return;
    }
    fprintf(fptr, ",%s,%s,%s\n%ld", u1.name, u1.email, u1.passwd, u1.acc_no+1);//Append data to file.
    printf("Account created successfully!\n");
    printf("Your account number is: %ld\n",u1.acc_no);
    char fname[110]="";
    strcat(fname,u1.name);
    strcat(fname,"_trs.txt");
    fptr2 = fopen(fname,"a+");
    fprintf(fptr2, "001,Initial Deposit,10000,10000\n002");//TRID,Desc,Transaction_amt,Balance
    fclose(fptr);
    fclose(fptr2);
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
