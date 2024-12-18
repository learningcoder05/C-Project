#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // For sleep()
#ifdef _WIN32
    #include <conio.h>  // For getch() on Windows
    #include <windows.h>  // For Sleep() on Windows
    #define sleep(x) Sleep(1000 * (x))
#else
    #include <unistd.h>  // For sleep() on Unix-based systems
    #include <termios.h> // For getch() on Unix
#endif


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
void loadUsersFromFile();
void saveUsersToFile();
void waitMoment(const char* message);

void menu() {
    printf("\n  Welcome to CASHSIM - The Bank of Broken Dreams\n");
    printf("  We turn your financial stress into life lessons!\n");
    printf("----------------------------------------------------\n");
    printf("                    1. Account Signup\n");
    printf("                    2. Account Login\n");
    printf("                    3. Exit (Escape Reality)\n");
}

void loadUsersFromFile() {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("No existing user data found. Starting fresh like your 2024 resolutions.\n");
        return;
    }

    userCount = 0;
    while (fscanf(file, "%s %s %f", users[userCount].name, users[userCount].password, &users[userCount].balance) != EOF) {
        userCount++;
    }

    fclose(file);
}

void saveUsersToFile() {
    FILE *file = fopen("users.txt", "w");
    if (file == NULL) {
        printf("Error saving user data. Maybe the system is feeling lazy.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %.2f\n", users[i].name, users[i].password, users[i].balance);
    }

    fclose(file);
}

void waitMoment(const char* message) {
    printf("%s\n", message);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void signup() {
    char ans = 'y';
    while (ans == 'y') {
        printf("Enter your name: ");
        scanf("%s", users[userCount].name);
        printf("Create a strong password (or just wing it): ");
        scanf("%s", users[userCount].password);
        printf("Initial deposit amount (suggested: your hopes and dreams): ");
        scanf("%f", &users[userCount].balance);
        userCount++;
        printf("Account created successfully!\n");
        saveUsersToFile();
        printf("Add another hopeful soul? (y/n): ");
        scanf(" %c", &ans);
    }
}

void transactionMenu(int userIndex) {
    int choice;
    char continueChoice;

    do {
        printf("\n\nTransaction Menu - Your Money, Our Rules:\n");
        printf("  1. View Balance (For a confidence boost)\n");
        printf("  2. Deposit Money (Feed the system)\n");
        printf("  3. Logout (Run while you can)\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewBalance(userIndex);
                break;
            case 2:
                depositMoney(userIndex);
                saveUsersToFile();
                break;
            case 3:
                printf("Logging out... May your wallet recover.\n");
                break;
            default:
                printf("Invalid choice! Maybe try something that makes sense.\n");
        }

        if (choice != 3) {
            printf("Want to try your luck again? (Y/N): ");
            scanf(" %c", &continueChoice);
        }

    } while (choice != 3 && (continueChoice == 'Y' || continueChoice == 'y'));
}

void viewBalance(int userIndex) {
    printf("Your current balance is: $%.2f. Consider therapy if expected more.\n", users[userIndex].balance);
}

void depositMoney(int userIndex) {
    float amount;
    printf("Enter the amount to deposit (Or just empty your wallet): ");
    scanf("%f", &amount);
    if (amount > 0) {
        users[userIndex].balance += amount;
        printf("Deposit successful! New balance: $%.2f. Try not to spend it all at once.\n", users[userIndex].balance);
    } else {
        printf("Invalid amount! If only life worked that way.\n");
    }
}

void login() {
    char name[50], password[50];
    printf("Enter your name (We don't judge your choices): ");
    scanf("%s", name);
    printf("Enter your password (Trust issues intensify): ");
    
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    
    int flag = 0;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nLogin successful! Welcome back to the grind.\n");
            flag = 1;
            transactionMenu(i);
            break;
        }
    }
    if (flag == 0) {
        printf("\nLogin failed! Credentials as broken as your dreams?\n");
    }
}

int main() {
    loadUsersFromFile();
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
            printf("Exiting... May your financial luck improve!\n");
            exit(0);
        } else {
            printf("Invalid choice, try again. Or don't. It's your life.\n");
        }
        printf("Continue your struggle? (y/n): ");
        scanf(" %c", &ans);
    }

    return 0;
}
