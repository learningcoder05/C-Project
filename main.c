#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct user {
    char name[50];
    char password[50];
    float balance;
};
struct user users[100];
int userCount = 0;

// Function Prototypes
void menu();
void signup();
void login();
void transactionMenu(int userIndex);
void viewBalance(int userIndex);
void depositMoney(int userIndex);

void menu() {
    printf("                  WELCOME TO CASHSIM\n");
    printf("                    1. Account Signup\n");
    printf("                    2. Account Login\n");
    printf("                    3. Exit\n");
}

void signup() {
    char ans = 'y';
    while (ans == 'y') {
        printf("Enter your name: ");
        scanf("%s", users[userCount].name);
        printf("Enter your password: ");
        scanf("%s", users[userCount].password);
        printf("Enter your balance: ");
        scanf("%f", &users[userCount].balance);
        userCount++;
        printf("Account created successfully!\n");
        printf("Do you want to add another user? (y/n): ");
        scanf(" %c", &ans);
    }
}

void transactionMenu(int userIndex) {
    int choice;
    do {
        printf("\nTransaction Menu:\n");
        printf("  1. View Balance\n");
        printf("  2. Deposit Money\n");
        printf("  3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBalance(userIndex);
                break;
            case 2:
                depositMoney(userIndex);
                break;
            case 3:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 3);
}

void viewBalance(int userIndex) {
    printf("Your current balance is: %.2f\n", users[userIndex].balance);
}

void depositMoney(int userIndex) {
    float amount;
    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);
    if (amount > 0) {
        users[userIndex].balance += amount;
        printf("Deposit successful! Your new balance is: %.2f\n", users[userIndex].balance);
    } else {
        printf("Invalid amount! Please enter a positive value.\n");
    }
}

void login() {
    char name[50], password[50];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your password: ");
    scanf("%s", password);

    int flag = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0 &&
            strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            flag = 1;
            transactionMenu(i);
            break;
        }
    }
    if (flag == 0) {
        printf("Login failed! Please check your credentials.\n");
    }
}

int main() {
    char ans = 'y';
    while (ans == 'y') {
        menu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            signup();
        } else if (choice == 2) {
            login();
        } else if (choice == 3) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice, try again.\n");
        }
        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &ans);
    }

    return 0;
}
