#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>   // For getch() on Windows
#include <windows.h> // For Sleep() on Windows
#define sleep(x) Sleep(1000 * (x))
#else
#include <unistd.h>  // For sleep() on Unix-based systems
#include <termios.h> // For getch() on Unix
#endif

struct user
{
    char name[50];
    char password[50];
    float balance;
    char accountType[50];
    int accountKey;
};
struct user users[100];
int userCount = 0;

// Function Prototypes
void menu();
void signup();
void login();
void accountOperations(int userIndex);
void viewBalance(int userIndex);
void depositMoney(int userIndex);
void withdrawMoney(int userIndex);
void transferMoney(int userIndex);
void generateAccountKey(int userIndex);
void saveUsersToFile();
void loadUsersFromFile();
void waitMoment(const char *message);

void menu()
{
    printf("\n=================== WELCOME TO CASHSIM ===================\n");
    printf("Where your money is simulated to feel safe!\n");
    printf("1. Account Signup - Because who doesn't want a fake account?\n");
    printf("2. Account Login - Prove you belong here.\n");
    printf("3. Exit - Run while you still can!\n");
    printf("========================================================\n");
}

void loadUsersFromFile()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf("No existing user data found. Starting fresh like your 2024 resolutions.\n");
        return;
    }

    userCount = 0;
    while (fscanf(file, "%s %s %f", users[userCount].name, users[userCount].password, &users[userCount].balance) != EOF)
    {
        userCount++;
    }

    fclose(file);
}

void saveUsersToFile()
{
    FILE *file = fopen("users.txt", "w");
    if (file == NULL)
    {
        printf("Error saving user data. Maybe the system is feeling lazy.\n");
        return;
    }

    for (int i = 0; i < userCount; i++)
    {
        fprintf(file, "%s %s %.2f\n", users[i].name, users[i].password, users[i].balance);
    }

    fclose(file);
}

void generateAccountKey(int userIndex)
{
    srand(time(0) + userIndex);
    users[userIndex].accountKey = rand() % 100000 + 10000; // 5-digit key
}

void waitMoment(const char *message)
{
    printf("%s\n", message);
    for (int i = 0; i < 3; i++)
    {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
}

void signup()
{
    char ans = 'y';
    while (ans == 'y')
    {
        printf("\nEnter your name (or alias if you're feeling mysterious): ");
        scanf("%s", users[userCount].name);
        printf("Enter your password (don't worry, we won't judge): ");
        scanf("%s", users[userCount].password);
        printf("Enter your initial balance (imaginary riches are welcome): ");
        scanf("%f", &users[userCount].balance);
        printf("Enter your account type (Savings/Checking): ");
        scanf("%s", users[userCount].accountType);
        generateAccountKey(userCount);
        printf("Your unique account key is: %d. Keep it safe!\n", users[userCount].accountKey);
        userCount++;
        printf("Account created successfully!\n");
        saveUsersToFile();
        printf("Add another hopeful soul? (y/n): ");
        scanf(" %c", &ans);
    }
}

void viewBalance(int userIndex)
{
    printf("\nYour current balance is: $%.2f. Treat yourself to something nice!\n", users[userIndex].balance);
}

void depositMoney(int userIndex)
{
    float amount;
    printf("Enter the amount to deposit (no Monopoly money, please): ");
    scanf("%f", &amount);
    if (amount > 0)
    {
        users[userIndex].balance += amount;
        printf("Deposit successful! Your new balance is: $%.2f. You're rolling in cash now!\n", users[userIndex].balance);
    }
    else
    {
        printf("Invalid amount! Come on, don't try to break the system.\n");
    }
}

void withdrawMoney(int userIndex)
{
    float amount;
    printf("Enter the amount to withdraw (don't get too greedy): ");
    scanf("%f", &amount);
    if (amount > 0 && amount <= users[userIndex].balance && amount <= 5000)
    {
        users[userIndex].balance -= amount;
        printf("Withdrawal successful! New balance: $%.2f. Spend wisely, my friend.\n", users[userIndex].balance);
    }
    else if (amount > users[userIndex].balance)
    {
        printf("Insufficient balance! Maybe try a piggy bank next time.\n");
    }
    else if (amount > 5000)
    {
        printf("Withdrawal limit exceeded! You can only withdraw $5000 at a time.\n");
    }
    else
    {
        printf("Invalid amount! Are you trying to withdraw negative money?\n");
    }
}

void transferMoney(int userIndex)
{
    int recipientKey;
    float amount;
    printf("Enter the recipient's account key: ");
    scanf("%d", &recipientKey);
    printf("Enter the amount to transfer: ");
    scanf("%f", &amount);

    int recipientIndex = -1;
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].accountKey == recipientKey)
        {
            recipientIndex = i;
            break;
        }
    }

    if (recipientIndex == -1)
    {
        printf("Recipient not found! Double-check the account key.\n");
        return;
    }

    if (amount > 0 && amount <= users[userIndex].balance)
    {
        users[userIndex].balance -= amount;
        users[recipientIndex].balance += amount;
        printf("Transfer successful! Your new balance is: $%.2f.\n", users[userIndex].balance);
        printf("Recipient (%s) now has a balance of $%.2f.\n", users[recipientIndex].name, users[recipientIndex].balance);
    }
    else if (amount > users[userIndex].balance)
    {
        printf("Insufficient balance! Looks like you need a fundraiser.\n");
    }
    else
    {
        printf("Invalid amount! Transfers require actual money.\n");
    }
}

void accountOperations(int userIndex)
{
    int choice;
    char continueChoice;

    do
    {
        printf("\n========== %s Account Menu =========="
               "\n1. View Balance - See how rich (or poor) you are."
               "\n2. Deposit Money - Make it rain!"
               "\n3. Withdraw Money - Treat yo'self!"
               "\n4. Transfer Money - Share the wealth (or debts)."
               "\n5. Logout - Take a break from all this financial stress."
               "\nEnter your choice: ",
               users[userIndex].accountType);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewBalance(userIndex);
            break;
        case 2:
            depositMoney(userIndex);
            saveUsersToFile();
            break;
        case 3:
            withdrawMoney(userIndex);
            saveUsersToFile();
            break;
        case 4:
            transferMoney(userIndex);
            saveUsersToFile();
            break;
        case 5:
            printf("Logging out of %s Account... Don't forget to come back!\n", users[userIndex].accountType);
            break;
        default:
            printf("Invalid choice! Try again, money maestro.\n");
        }

        if (choice != 5)
        {
            printf("Do you want to continue? (Y/N): ");
            scanf(" %c", &continueChoice);
        }

    } while (choice != 5 && (continueChoice == 'Y' || continueChoice == 'y'));
}

void login()
{
    char name[50], password[50];
    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter your password: ");

    int i = 0;
    char ch;
    while ((ch = getch()) != '\r')
    { // '\r' is the Enter key
        if (ch == '\b' && i > 0)
        { // Handle backspace
            i--;
            printf("\b \b");
        }
        else if (ch != '\b')
        {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");

    int flag = 0;
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].name, name) == 0 &&
            strcmp(users[i].password, password) == 0)
        {
            printf("Login successful! Welcome back, financial wizard!\n");
            flag = 1;
            accountOperations(i);
            break;
        }
    }

    if (flag == 0)
    {
        printf("Login failed! Did you forget your password? Or your name?\n");
    }
}

int main()
{
    loadUsersFromFile();
    char ans = 'y';
    while (ans == 'y')
    {
        menu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            signup();
        }
        else if (choice == 2)
        {
            login();
        }
        else if (choice == 3)
        {
            printf("Exiting... Don't forget to come back and play with fake money!\n");
            exit(1);
        }
        else
        {
            printf("Invalid choice, try again. Don't worry, we're patient.\n");
        }
        printf("Do you want to continue? (y/n): ");
        scanf(" %c", &ans);
    }

    return 0;
}
