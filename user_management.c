#include <stdio.h>
#include <string.h>
#include <conio.h>  // For getch()
#include <stdlib.h> // For system()

#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30

typedef struct {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();
void input_credentials(char* username, char* password);
void fix_fgets_input(char* str);

int main() {
    int option;
    int user_index;

    while (1) {
        printf("\n=== User Management System ===");
        printf("\n1. Register");
        printf("\n2. Login");
        printf("\n3. Exit");
        printf("\nChoose option: ");
        scanf("%d", &option);
        getchar(); // consume newline

        switch (option) {
            case 1:
                register_user();
                break;
            case 2:
                user_index = login_user();
                if (user_index >= 0) {
                    printf("\nLogin successful! Welcome, %s!\n", users[user_index].username);
                } else {
                    printf("\nLogin failed! Invalid credentials.\n");
                }
                break;
            case 3:
                printf("\nExiting program. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid option! Try again.\n");
        }
    }
    return 0;
}

void register_user() {
    if (user_count == MAX_USERS) {
        printf("\nMaximum user limit reached (%d users)!\n", MAX_USERS);
        return;
    }

    printf("\n--- Register New User ---\n");
    input_credentials(users[user_count].username, users[user_count].password);
    user_count++;
    printf("\nUser registered successfully!\n");
}

int login_user() {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];

    printf("\n--- User Login ---\n");
    input_credentials(username, password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            return i;
        }
    }
    return -1;
}

void input_credentials(char* username, char* password) {
    printf("Username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_input(username);

    printf("Password: ");
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < CREDENTIAL_LENGTH - 1) {
        if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void fix_fgets_input(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}
