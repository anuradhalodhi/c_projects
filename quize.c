#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

#define MAX_QUES_LEN 300
#define MAX_OPTION_LEN 100

typedef struct {
    char text[MAX_QUES_LEN];
    char options[4][MAX_OPTION_LEN];
    char correct_option;
    int timeout;
    int prize_money;
} Question;

void print_formatted_question(Question question);
int read_questions(const char* file_name, Question** questions);
void play_game(Question* questions, int no_of_questions);
int use_lifeline(Question* question, int* lifeline);

int main() {
    printf("\n\t\tWelcome to KAUN BANEGA CROREPATI (Windows Edition)\n");

    Question* questions;
    int no_of_questions = read_questions("questions.txt", &questions);

    play_game(questions, no_of_questions);

    free(questions);
    return 0;
}

int read_questions(const char* file_name, Question** questions) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    int line_count = 0;
    char buffer[500];
    while (fgets(buffer, sizeof(buffer), file)) {
        line_count++;
    }

    int question_count = line_count / 8;
    *questions = (Question*)malloc(question_count * sizeof(Question));

    rewind(file);
    for (int i = 0; i < question_count; i++) {
        fgets((*questions)[i].text, MAX_QUES_LEN, file);
        for (int j = 0; j < 4; j++) {
            fgets((*questions)[i].options[j], MAX_OPTION_LEN, file);
        }

        char answer_line[10];
        fgets(answer_line, sizeof(answer_line), file);
        (*questions)[i].correct_option = toupper(answer_line[0]);

        char timeout_line[10];
        fgets(timeout_line, sizeof(timeout_line), file);
        (*questions)[i].timeout = atoi(timeout_line);

        char prize_line[10];
        fgets(prize_line, sizeof(prize_line), file);
        (*questions)[i].prize_money = atoi(prize_line);
    }

    fclose(file);
    return question_count;
}

void print_formatted_question(Question question) {
    printf("\n%s", question.text);
    for (int i = 0; i < 4; i++) {
        if (question.options[i][0] != '\0') {
            printf("%c. %s", 'A' + i, question.options[i]);
        }
    }
    printf("\nYou have %d seconds to answer.", question.timeout);
    printf("\nEnter your answer (A/B/C/D) or L for Lifeline: ");
}

int use_lifeline(Question* question, int* lifeline) {
    printf("\nAvailable Lifelines:");
    if (lifeline[0]) printf("\n1. Fifty-Fifty (50/50)");
    if (lifeline[1]) printf("\n2. Skip the Question");
    printf("\nEnter your choice (1/2) or 0 to cancel: ");

    char ch = getch();
    printf("%c\n", ch);

    if (ch == '1' && lifeline[0]) {
        lifeline[0] = 0;
        int removed = 0;
        while (removed < 2) {
            int index = rand() % 4;
            if ((index + 'A') != question->correct_option && question->options[index][0] != '\0') {
                question->options[index][0] = '\0';
                removed++;
            }
        }
        return 1;
    } else if (ch == '2' && lifeline[1]) {
        lifeline[1] = 0;
        return 2;
    }

    printf("Returning to question.\n");
    return 0;
}

void play_game(Question* questions, int no_of_questions) {
    int lifeline[] = {1, 1};
    int money_won = 0;

    for (int i = 0; i < no_of_questions; i++) {
        print_formatted_question(questions[i]);

        time_t start_time = time(NULL);
        char input = '\0';

        while (difftime(time(NULL), start_time) < questions[i].timeout) {
            if (_kbhit()) {
                input = toupper(getch());
                printf("%c\n", input);
                break;
            }
        }

        if (input == '\0') {
            printf("\nTime's up!\n");
            break;
        }

        if (input == 'L') {
            int result = use_lifeline(&questions[i], lifeline);
            if (result != 2) i--;
            continue;
        }

        if (input < 'A' || input > 'D') {
            printf("Invalid input! Try again.\n");
            i--;
            continue;
        }

        if (input == questions[i].correct_option) {
            printf("Correct Answer!\n");
            money_won = questions[i].prize_money;
            printf("You have won Rs. %d\n", money_won);
        } else {
            printf("Wrong! Correct answer was %c\n", questions[i].correct_option);
            break;
        }
    }

    printf("\nGame Over! You won total Rs. %d\n", money_won);
}
