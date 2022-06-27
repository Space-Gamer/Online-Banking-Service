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

void cur_time(char *ts)
{
    struct tm* ptr;
    time_t t = time(NULL);
    ptr = gmtime(&t);
    strftime(ts,100,"%d-%m-%Y-%H:%M:%S",ptr);
    // printf("%s\n",ts);
}

int gettrans(char uname[], struct trans *tar)
{
    FILE *fptr;
    char fname[] = "";
    strcat(fname,uname);
    strcat(fname,"_trs.txt");
    fptr = fopen(fname, "r");
    fseek(fptr, -3, SEEK_END);
    int ctr,ltr; //Current, Last transcation number
    fscanf(fptr,"%d",&ltr);
    rewind(fptr);
    fscanf(fptr,"%d",&ctr);
    while(ctr < ltr)
    {
        tar[ctr-1].trno = ctr;
        fscanf(fptr, ",%[^,],%[^,],%f,%f\n", tar[ctr-1].tstr, tar[ctr-1].desc, &tar[ctr-1].tramt, &tar[ctr-1].bal);
        fscanf(fptr,"%d",&ctr);
    }
    fclose(fptr);
    return ltr-1;
}

float balance(char uname[])
{
    struct trans tar[100];
    int n;
    n = gettrans(uname, tar);
    return tar[n-1].bal;
}

int withdraw(char uname[], float amt, char desc[])
{
    float bal = balance(uname);
    if (bal<amt)
    {
        printf("Insufficient balance in your account!\n");
        return -1;
    }
    FILE *fptr;
    char fname[50] = "";
    strcat(fname,uname);
    strcat(fname,"_trs.txt");
    fptr = fopen(fname, "a+");
    fseek(fptr, -3, SEEK_END);
    int ltr; //Current, Last transcation number
    fscanf(fptr,"%d",&ltr);
    char dstr[50]="";
    cur_time(dstr);
    fprintf(fptr,",%s,%s,%.2f,%.2f\n%03d",dstr,desc,-1*amt,bal-amt,ltr+1);
    fclose(fptr);
    return 0;
}

int deposit(char uname[], float amt, char desc[])
{
    float bal = balance(uname);
    FILE *fptr;
    char fname[50] = "";
    strcat(fname,uname);
    strcat(fname,"_trs.txt");
    fptr = fopen(fname, "a+");
    fseek(fptr, -3, SEEK_END);
    int ltr; //Current, Last transcation number
    fscanf(fptr,"%d",&ltr);
    fseek(fptr,0,SEEK_END);
    char dstr[100]="";
    cur_time(dstr);
    fprintf(fptr,",%s,%s,%.2f,%.2f\n%03d",dstr,desc,amt,bal+amt,ltr+1);
    fclose(fptr);
    return 0;
}

int logchk(char *usr, char *pass, struct details *det)
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
            det->acc_no = a;
            if (!strcmp(pass,p))
            {
                strcpy(det->name,u);
                strcpy(det->email,e);
                strcpy(det->passwd,p);
                return 0;//Correct password
            }
            else
            return 1;//Wrong password
        }
        fscanf(fptr,"%ld",&a);
    };
    fclose(fptr);
    return -1;//Username not found
}

int login(struct details *det)
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
        status = logchk(usrn, pass, det);
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
    stat = logchk(u1.name, u1.passwd, &u1);
    if (!(stat==-1))
    {
        printf("Username '%s' already exists! Kindly login to access.\n",u1.name);
        fclose(fptr);
        return;
    }
    char dstr[100]="";
    cur_time(dstr);
    fprintf(fptr, ",%s,%s,%s\n%ld", u1.name, u1.email, u1.passwd, u1.acc_no+1);//Append data to file.
    printf("Account created successfully!\n");
    printf("Your account number is: %ld\n",u1.acc_no);
    char fname[110]="";
    strcat(fname,u1.name);
    strcat(fname,"_trs.txt");
    fptr2 = fopen(fname,"a+");
    fprintf(fptr2, "001,%s,Initial Deposit,10000,10000\n002",dstr);//TRID,date_string,Desc,Transaction_amt,Balance
    fclose(fptr);
    fclose(fptr2);
}

int main()
{
    printf("=============== Welcome to N.P. Banking Services ===============");
    int flag=1,log=0;
    struct details det;
    char border[] = "=======================================================================================\n";
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
            resp1 = login(&det);
            if (resp1==0)
            {
                log = 1;
                printf("Successfully logged in!\n");
                printf("=============================== Welcome %s ======================================\n",det.name);
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
        while (log==1)
        {
            printf("\n1. Check your account balance\n2. View your transactions\n3. Money Transfer\n4. Exit\nEnter your choice(integer only): ");
            int ch2=0;
            int arsz; // Array Size
            struct trans tar[100];
            float amount; // Transaction amount for money transfer

            scanf("%d",&ch2);
            char line[] = "---------------------------------------------------------------------------------------\n";
            switch(ch2)
            {
                case 1:
                    printf("Your balance is: %.2f Rupees\n",balance(det.name));
                    break;
                case 2:
                    arsz = gettrans(det.name, tar);
                    int it=0;
                    printf("%s",line);
                    printf("|TrNo.|    Date-Time-UTC    |           Description          |   Tr.Amt   |  Balance  |\n");
                    printf("%s",line);
                    for (it=0;it<arsz;it++)
                    {
                        printf("|%4ld | %19s | %30s | %10.2f | %10.2f|\n",tar[it].trno, tar[it].tstr, tar[it].desc, tar[it].tramt, tar[it].bal);
                    }
                    printf("%s",line);
                    break;
                case 3:
                    printf("===================== MONEY TRANSFER ====================\n");
                    struct details det2;
                    long int accno2;
                    printf("Enter account number to which amount must be transferred: ");
                    scanf("%ld",&accno2);
                    printf("Enter name of account to which amount must be transferred: ");
                    scanf("%s",det2.name);
                    // Check if account exists
                    int resp2 = logchk(det2.name, "ABC", &det2);
                    if (resp2==-1)
                    {
                        printf("Account with name '%s' doesn't exist.\n",det2.name);
                        break;
                    }
                    else if (resp2==1 && (accno2 != det2.acc_no))
                    {
                        printf("Receiver's account name and account number don't match.\n");
                        break;
                    }
                    else if (resp2==1 && (accno2 == det.acc_no))
                    {
                        printf("You cannot transfer money to your own account.\n");
                        break;
                    }
                    printf("Enter amount to be transferred: ");
                    scanf("%f",&amount);
                    char str1[100]="Transfer to account ", buff1[20];
                    sprintf(buff1, "%ld", accno2);
                    strcat(str1,buff1);
                    char str2[100]="Transfer from account ", buff2[20];
                    sprintf(buff2, "%ld", det.acc_no);
                    strcat(str2,buff2);
                    if (withdraw(det.name, amount, str1)==0 && deposit(det2.name, amount, str2)==0)
                    {
                        printf("Money transferred successfully!\n");
                    }
                    else
                    {
                        printf("Transaction failed!\n");
                    }
                    break;
                default:
                    log = 0;
                    printf("Logged out!\n");
                    break;
            }
            printf("%s",border);
        }
    }
    return 0;
}
