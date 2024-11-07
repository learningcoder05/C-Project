#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct user{
    char name[50];
    char password[50];
    float balance;
};
struct user users[100];

void menu(){
    printf("                  WELCOME TO CASHSIM\n");
    printf("                    1. Account Signup\n");
    printf("                    2. Account Login\n");
    printf("                    3. Exit\n");
}

void signup(){
    char ans = 'y';
    int i=0;
    while (ans=='y')
    {           
        printf("Enter your name: ");
        scanf("%s",users[i].name);
        printf("Enter your password: ");
        scanf("%s",users[i].password);
        printf("Enter your balance: ");
        scanf("%g",&users[i].balance);
        i++;
        printf("Account created successfully!\n");
        printf("Do you want to add another user? (y/n): ");
        scanf(" %c",&ans);
    }
}    
    
void login(){
    char name[50],password[20];
    printf("Enter your name: ");
    scanf("%s",&name);
    printf("Enter your password: ");
    scanf("%s",&password);
    int flag=0;
    for (int i=0;i<100;i++){
        if (strcmp(users[i].name,name) && 
            strcmp(users[i].password,password)){
            printf("Login successful!\n");
            flag=1;
            break;
        }
        printf("Login failed!\n");
    }
}

if (flag==1){
    transactionMenu();
}

void transactionMenu(){
    printf("                    1. View Balance\n");
    printf("                    2. Deposit Money\n");
    printf("                    3. Withdraw Money\n");
    printf("                    4. Transfer Money\n");
}

int main() {
    char ans='y';
    while (ans=='y'){
    menu();
    int choice;
    printf("Enter your choice: ");
    scanf("%d",&choice);
    if (choice==1){
        signup();
    }
    else if(choice==2){
        login();
    }
    }
    printf("Do you want to continue? (y/n): ");
    scanf(" %c",&ans);
    
    return 0;
}
