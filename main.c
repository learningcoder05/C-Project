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
    int accountNumber;
    int accountKey;
    char transactionHistory[500];
    int achievements;
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
void logTransaction(int userIndex, const char *transactionDetail);
void checkAchievements(int userIndex);
void saveUsersToFile();
void loadUsersFromFile();
void currencyConverter(int userIndex);
void forgotPassword();
void viewAccountDetails(int userIndex);
void printTransactionHistory(int userIndex);

void menu()
{
    printf("\n=================== WELCOME TO CASHSIM ===================\n");
    printf("Where your money is simulated to feel safe!\n");
    printf("1. Account Signup - Because who doesn't want a fake account?\n");
    printf("2. Account Login - Prove you belong here.\n");
    printf("3. Exit - Run while you still can!\n");
    printf("============================================================\n");
}

void logTransaction(int userIndex, const char *transactionDetail)
{
    strcat(users[userIndex].transactionHistory, transactionDetail);
    strcat(users[userIndex].transactionHistory, "\n");
}

void generateAccountKey(int userIndex)
{
    srand(time(0) + userIndex);
    users[userIndex].accountKey = rand() % 100000 + 10000; // 5-digit key
    strcpy(users[userIndex].transactionHistory, "Account created.\n");
}

void generateAccountNumber(int userIndex)
{
    srand(time(0) + userIndex);
    users[userIndex].accountNumber = rand() % 100000 + 1000000; // 5-digit number
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

        generateAccountNumber(userCount);
        printf("Your account number is: %d.\n", users[userCount].accountNumber);

        generateAccountKey(userCount);
        printf("Your unique account key is: %d. Keep it safe!\n", users[userCount].accountKey);

        userCount++;
        printf("Congrats! Your account has been created!\n");

        printf("Do you want to add another user to our growing list of financial adventurers? (y/n): ");
        scanf(" %c", &ans);
    }
}

void viewAccountDetails(int userIndex)
{
    printf("\nAccount Details:\n");
    printf("Name: %s\n", users[userIndex].name);
    printf("Balance: $%.2f\n", users[userIndex].balance);
    printf("Account Type: %s\n", users[userIndex].accountType);
    printf("Account Number: %d\n", users[userIndex].accountNumber);
    printf("Account Key: %d\n", users[userIndex].accountKey);
    printf("Transaction History:\n%s\n", users[userIndex].transactionHistory);
}

void printTransactionHistory(int userIndex)
{
    printf("\n================ TRANSACTION RECEIPT ================\n");
    printf("Account Name   : %s\n", users[userIndex].name);
    printf("Account Type   : %s\n", users[userIndex].accountType);
    printf("Account Number : %d\n", users[userIndex].accountNumber);
    printf("-----------------------------------------------------\n");
    printf("Transaction History:\n");

    char *transaction = strtok(users[userIndex].transactionHistory, "\n"); // Split by line
    int count = 1;

    while (transaction != NULL)
    {
        printf("%d. %s\n", count++, transaction);
        transaction = strtok(NULL, "\n");
    }

    printf("-----------------------------------------------------\n");
    printf("Current Balance: $%.2f\n", users[userIndex].balance);
    printf("=====================================================\n");
}

void transferMoney(int userIndex)
{
    int recipientNumber;
    float amount;
    printf("Enter the recipient's account Number: ");
    scanf("%d", &recipientNumber);
    printf("Enter the amount to transfer: ");
    scanf("%f", &amount);

    int recipientIndex = -1;
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].accountNumber == recipientNumber)
        {
            recipientIndex = i;
            break;
        }
    }

    if (recipientIndex == -1)
    {
        printf("Recipient not found! Double-check the account Number.\n");
        return;
    }

    if (amount > 0 && amount <= users[userIndex].balance)
    {
        users[userIndex].balance -= amount;
        users[recipientIndex].balance += amount;
        printf("Transfer successful! Your new balance is: $%.2f.\n", users[userIndex].balance);

        char logEntry[100];
        sprintf(logEntry, "Transferred $%.2f to account Number %d.", amount, recipientNumber);
        logTransaction(userIndex, logEntry);

        sprintf(logEntry, "Received $%.2f from account Number %d.", amount, users[userIndex].accountNumber);
        logTransaction(recipientIndex, logEntry);

        saveUsersToFile(); // Save after transfer
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

void withdrawMoney(int userIndex)
{
    float amount;
    printf("Enter the amount to withdraw (don't get too greedy): ");
    scanf("%f", &amount);
    if (amount > 0 && amount <= users[userIndex].balance)
    {
        users[userIndex].balance -= amount;
        printf("Withdrawal successful! New balance: $%.3f. Spend wisely, my friend.\n", users[userIndex].balance);
        char logEntry[100];
        sprintf(logEntry, "Withdrew $%.2f.", amount);
        logTransaction(userIndex, logEntry);
        saveUsersToFile(); // Save after withdrawal
    }
    else if (amount > users[userIndex].balance)
    {
        printf("Insufficient balance! Maybe try a piggy bank next time.\n");
    }
    else
    {
        printf("Invalid amount! Are you trying to withdraw negative money?\n");
    }
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
        char logEntry[100];
        sprintf(logEntry, "Deposited $%.2f.", amount);
        logTransaction(userIndex, logEntry);
        checkAchievements(userIndex);
        saveUsersToFile(); // Save after deposit
    }
    else
    {
        printf("Invalid amount! Come on, don't try to break the system.\n");
    }
}

void viewBalance(int userIndex)
{
    printf("\nYour current balance is: $%.2f. Treat yourself to something nice!\n", users[userIndex].balance);
    logTransaction(userIndex, "Checked balance.");
}

void checkAchievements(int userIndex)
{
    printf("\nAchievements:\n");

    if (users[userIndex].achievements & 1)
    { // First deposit
        printf(" First Deposit!\n");
    }
    else
    {
        printf(" First Deposit: Deposit money to unlock.\n");
    }

    if (users[userIndex].achievements & 2)
    { // Big saver
        printf(" Big Saver: Balance reached $10,000!\n");
    }
    else
    {
        printf(" Big Saver: Maintain a balance of $10,000 to unlock.\n");
    }
}

void currencyConverter(int userIndex)
{
    int choice;
    float exchangeRate;
    char continueChoice;

    do
    {
        printf("\nCurrency Converter:\n");
        printf("1. USD to EUR\n2. USD to GBP\n3. USD to INR\n4. USD to AUD\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            exchangeRate = 0.96;
            printf("Balance in EUR: %.3f Euro\n", users[userIndex].balance * exchangeRate);
            break;
        case 2:
            exchangeRate = 0.79;
            printf("Balance in GBP: %.3f British Pound\n", users[userIndex].balance * exchangeRate);
            break;
        case 3:
            exchangeRate = 85.12;
            printf("Balance in INR: %.3f Indian Rupees\n", users[userIndex].balance * exchangeRate);
            break;
        case 4:
            exchangeRate = 1.60;
            printf("Balance in AUD: %.3f Australian Dollars\n", users[userIndex].balance * exchangeRate);
            break;
        case 5:
            printf("Exiting Currency Converter...\n");
            return; // Exit the function
        default:
            printf("Invalid choice! Please try again.\n");
        }

        printf("Do you want to perform another conversion? (Y/N): ");
        scanf(" %c", &continueChoice);

    } while (continueChoice == 'Y' || continueChoice == 'y');
}

void accountOperations(int userIndex)
{
    int choice;
    char continueChoice;

    do
    {
        printf("\n========== %s Account Menu =========="
               "\n1. View Balance - Check your riches."
               "\n2. View Account Details - Know your account better."
               "\n3. Deposit Money - Grow your wealth."
               "\n4. Withdraw Money - Spend wisely."
               "\n5. Transfer Money - Share or settle."
               "\n6. View Achievements - Check your unlocked achievements."
               "\n7. Currency Converter - See your balance in other currencies."
               "\n8. View Transaction History - Check your past transactions."
               "\n9. Logout - Take a break."
               "\nEnter your choice: ",
               users[userIndex].accountType);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewBalance(userIndex);
            break;
        case 2:
            viewAccountDetails(userIndex);
            break;
        case 3:
            depositMoney(userIndex);
            saveUsersToFile();
            break;
        case 4:
            withdrawMoney(userIndex);
            saveUsersToFile();
            break;
        case 5:
            transferMoney(userIndex);
            saveUsersToFile();
            break;
        case 6:
            checkAchievements(userIndex);
            break;
        case 7:
            currencyConverter(userIndex);
            break;
        case 8:
            printTransactionHistory(userIndex);
            break;
        case 9:
            printf("Logging out of %s Account... Don't forget to come back!\n", users[userIndex].accountType);
            break;
            printf("Invalid choice! Try again.\n");
        }

        if (choice != 9)
        {
            printf("Do you want to continue using CashSim? (Y/N): ");
            scanf(" %c", &continueChoice);
        }

    } while (choice != 9 && (continueChoice == 'Y' || continueChoice == 'y'));
}
void forgotPassword()
{
    char name[50];
    int found = 0;

    printf("Forgot Password:\n");
    printf("Enter your name: ");
    scanf("%s", name);

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].name, name) == 0)
        {
            found = 1;
            printf("Unique key: ");
            int answer;
            scanf("%d", &answer);

            if ((users[i].accountKey == answer))
            {
                char newPassword[50];
                printf("Verification successful!\n");
                printf("Enter your new password: ");
                scanf("%s", newPassword);
                strcpy(users[i].password, newPassword);
                printf("Password reset successfully!\n");
                saveUsersToFile(); // Save updated password to file
            }
            else
            {
                printf("Incorrect answer to the security question! Password reset failed.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("No account found with the given name.\n");
    }
}

void login()
{
    char name[50], password[50];
    int loginAttempts = 0; // Track failed login attempts
    int maxAttempts = 3;   // Limit to 3 attempts

    printf("Enter your name: ");
    scanf("%s", name);

    while (loginAttempts < maxAttempts)
    {
        printf("Enter your password: ");
        int i = 0;
        char ch;

        // Hide password input
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

        // Check credentials
        int flag = 0;
        for (int i = 0; i < userCount; i++)
        {
            if (strcmp(users[i].name, name) == 0 &&
                strcmp(users[i].password, password) == 0)
            {
                printf("Login successful! Welcome back, financial wizard!\n");
                flag = 1;
                accountOperations(i);
                return; // Exit after successful login
            }
        }

        if (!flag)
        {
            loginAttempts++;
            if (loginAttempts < maxAttempts)
            {
                printf("Incorrect password! Attempts remaining: %d\n", maxAttempts - loginAttempts);
            }
        }
    }

    // If max attempts reached
    if (loginAttempts == maxAttempts)
    {
        printf("Maximum login attempts exceeded.\n");
        char choice;
        printf("Would you like to reset your password? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            forgotPassword();
        }
        else
        {
            printf("Returning to the main menu.\n");
        }
    }
}

void saveUsersToFile()
{
    FILE *file = fopen("users.txt", "w");
    if (!file)
    {
        printf("Error saving data!\n");
        return;
    }
    fwrite(&userCount, sizeof(userCount), 1, file);
    fwrite(users, sizeof(struct user), userCount, file);
    fclose(file);
}

void loadUsersFromFile()
{
    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        // printf("No previous data found. Starting fresh!\n");
        return;
    }
    fread(&userCount, sizeof(userCount), 1, file);
    fread(users, sizeof(struct user), userCount, file);
    fclose(file);
}

int main()
{
    loadUsersFromFile();
    char ans = 'y';
    while (ans == 'y')
    {
        menu();
        int choice;
        printf("\nEnter your choice: ");
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
            printf("\nExiting... Goodbye!");
            saveUsersToFile(); // Save before exit
            exit(1);
        }
        else
        {
            printf("Invalid choice, try again. Don't worry, we're patient.\n");
        }
        printf("Do you want to return to the main menu? (y/n): ");
        scanf(" %c", &ans);
    }

    return 0;
}
