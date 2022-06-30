#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define NRM  "\x1B[0m"
#define BOLD "\x1B[1m"
#define BLK  "\x1B[30m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"

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

float float_rand(float min, float max)
{
    srand(time(0));
    rand(); // Empty call to remove related values.
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );
}

void sleep(float sec)
{
    sec*=1000;
    #ifdef _WIN32
    Sleep(sec);
    #else
    usleep(sec*1000);
    #endif
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
        printf(RED"Insufficient balance in your account!\n"NRM);
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
        printf(YEL);
        scanf("%s",usrn);
        printf(NRM);
        printf("\nEnter password: ");
        printf(BLK);
        scanf("%s",pass);
        printf(NRM);
        int status;
        status = logchk(usrn, pass, det);
        if (status == 0){//Verifying step
            return 0;
        }
        else if (status == 1)
        printf(RED"\nWrong password!\n"NRM);
        else
        {
            printf(RED"Account with name '%s' doesn't exist. Sign-Up to create one!\n"NRM,usrn);
            return -1;
        }
    }
    printf(RED"Too many invalid attempts. Try again later!\n"NRM);
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
    printf(YEL);
    scanf("%s",&u1.name);
    printf(NRM);
    printf("Enter your email address: ");
    printf(YEL);
    scanf("%s",&u1.email);
    printf(NRM);
    printf("Enter password: ");
    printf(BLK);
    scanf("%s",&u1.passwd);
    printf(NRM);
    int stat;
    stat = logchk(u1.name, u1.passwd, &u1);
    if (!(stat==-1))
    {
        printf(RED"Username '%s' already exists! Kindly login to access.\n"NRM,u1.name);
        fclose(fptr);
        return;
    }
    char dstr[100]="";
    cur_time(dstr);
    fprintf(fptr, ",%s,%s,%s\n%ld", u1.name, u1.email, u1.passwd, u1.acc_no+1);//Append data to file.
    printf(GRN"Account created successfully!\n"NRM);
    printf("Your account number is: ");
    printf(YEL"%ld\n"NRM,u1.acc_no);
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
    char border[] = "===================================================================================\n";
    printf(MAG"%s"NRM,border);
    printf(MAG BOLD"======================== Welcome to N.P. Banking Services =========================\n" NRM);
    printf(MAG"%s"NRM,border);
    int flag=1,log=0;
    struct details det;
    while (flag)
    {
        printf(CYN"\n1. Log-In\n2. Sign-Up\n3. Exit\nEnter your choice(integer only): " NRM);
        int ch1;
        printf(YEL);
        scanf("%d",&ch1);
        printf(NRM);
        switch (ch1)
        {
        case 1:
            printf(BOLD MAG"================================ Log-In ===========================================\n"NRM);
            int resp1;
            resp1 = login(&det);
            if (resp1==0)
            {
                log = 1;
                printf(GRN"Successfully logged in!\n"NRM);
                printf(BOLD MAG"=============================== Welcome %s ======================================\n"NRM,det.name);
            }
            break;
        case 2:
            printf(BOLD MAG"================================ Sign-Up ===========================================\n"NRM);
            sign_up();//
            break;
        default:
            flag=0;
            printf(BOLD BLU"\nThank you for using our service. Good Day!"NRM);
            break;
        }
        while (log==1)
        {
            printf(CYN"\n1. Check your account balance\n2. View your transactions\n3. Money Transfer\n4. Crypto Invest\n5. Exit\nEnter your choice(integer only): "NRM);
            int ch2=0;
            int arsz; // Array Size
            struct trans tar[100];
            float amount; // Transaction amount for money transfer

            printf(YEL);
            scanf("%d",&ch2);
            printf(NRM);
            char line[] = "---------------------------------------------------------------------------------------\n";
            switch(ch2)
            {
                case 1:
                    printf(GRN"\nYour account balance is:");
                    printf(YEL" %.2f Rupees Only\n"NRM,balance(det.name));
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
                        sleep(0.01);
                    }
                    printf("%s",line);
                    break;
                case 3:
                    printf(BOLD MAG"===================== MONEY TRANSFER ====================\n"NRM);
                    struct details det2;
                    long int accno2;
                    printf("Enter account number to which amount must be transferred: ");
                    printf(YEL);
                    scanf("%ld",&accno2);
                    printf(NRM);
                    printf("Enter name of account to which amount must be transferred: ");
                    printf(YEL);
                    scanf("%s",det2.name);
                    printf(NRM);
                    // Check if account exists
                    int resp2 = logchk(det2.name, "ABC", &det2);
                    if (resp2==-1)
                    {
                        printf(RED"Account with name '%s' doesn't exist.\n"NRM,det2.name);
                        break;
                    }
                    else if (resp2==1 && (accno2 != det2.acc_no))
                    {
                        printf(RED"Receiver's account name and account number don't match.\n"NRM);
                        break;
                    }
                    else if (resp2==1 && (accno2 == det.acc_no))
                    {
                        printf(YEL"You cannot transfer money to your own account.\n"NRM);
                        break;
                    }
                    printf("Enter amount to be transferred: ");
                    printf(YEL);
                    scanf("%f",&amount);
                    printf(NRM);
                    printf("Enter password to confirm: ");
                    printf(BLK);
                    char passwd[50];
                    scanf("%s",&passwd);
                    printf(NRM);
                    if (strcmp(passwd, det.passwd)!=0)
                    {
                        printf(RED"Wrong password\n"NRM);
                        printf(RED"Transaction failed!\n"NRM);
                        break;
                    }
                    char str1[100]="Transfer to account ", buff1[20];
                    sprintf(buff1, "%ld", accno2);
                    strcat(str1,buff1);
                    char str2[100]="Transfer from account ", buff2[20];
                    sprintf(buff2, "%ld", det.acc_no);
                    strcat(str2,buff2);
                    if (withdraw(det.name, amount, str1)==0 && deposit(det2.name, amount, str2)==0)
                    {
                        printf(GRN"\nMoney transferred successfully!\n"NRM);
                    }
                    else
                    {
                        printf(RED"\nTransaction failed!\n"NRM);
                    }
                    break;
                case 4:
                    printf(BOLD MAG"===================== CRYPTO INVEST ====================\n"NRM);
                    printf("Enter amount to be invested: ");
                    printf(YEL);
                    scanf("%f",&amount);
                    printf(NRM);
                    printf(CYN"Choose in which currency you want to invest: \n1. Bitcoin \n2. Ethereum \n3. Litecoin \n4. Ripple \n5. Back\nEnter your choice(integer only): "NRM);                    
                    printf(YEL);
                    int ch3;
                    scanf("%d",&ch3);
                    printf(NRM);
                    char desc[50]="Investment in ";
                    int resp3=1;
                    switch (ch3)
                    {
                        case 1:
                            strcat(desc,"Bitcoin");
                            break;
                        case 2:
                            strcat(desc,"Ethereum");
                            break;
                        case 3:
                            strcat(desc,"Litecoin");
                            break;
                        case 4:
                            strcat(desc,"Ripple");
                            break;
                        case 5:
                            resp3=0;
                            break;
                        default:
                            resp3=0;
                            printf(RED"Invalid choice!\n"NRM);
                            break;
                    }
                    if (resp3==0)
                    {
                        break;
                    }
                    printf("%s",line);
                    if (withdraw(det.name, amount, desc)==0)
                    {
                        printf(GRN"Investment successful!\n"NRM);
                    }
                    else
                    {
                        printf(RED"Investment failed!\n"NRM);
                        break;
                    }
                    sleep(2);
                    float rate = float_rand(-15,15);
                    if (rate<0)
                    {
                        printf(BOLD YEL"\n%s has plummeted by %.2f%%!\n\n"NRM,desc,rate);
                    }
                    else
                    {
                        printf(BOLD GRN"\n%s has soared by %.2f%%!\n\n"NRM,desc,rate);
                    }
                    amount = amount + (amount*rate/100);
                    if (deposit(det.name, amount, "Investment Returns")==0)
                    {
                        printf(GRN"Returns deposited successfully!\n"NRM);
                    }
                    else
                    {
                        printf(RED"Depositing failed!\n"NRM);
                    }
                    break;
                default:
                    log = 0;
                    printf(YEL"\nLogged out!\n"NRM);
                    break;
            }
            printf("%s",border);
        }
    }
    return 0;
}
