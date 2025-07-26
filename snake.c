#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define HEIGHT 20
#define WIDTH 60

enum Direction { UP, DOWN, LEFT, RIGHT, STOP };
enum Direction dir;

int score;
int fruit_x, fruit_y;
int head_x, head_y;
int tail_length;
int tail_x[100];
int tail_y[100];

void setup();
void draw();
void input();
void game_play();
void clear_screen();

int main() {
    setup();
    while (1) {
        draw();
        input();
        game_play();
        Sleep(100); // Sleep in milliseconds
    }
    return 0;
}

void setup() {
    dir = STOP;
    score = 0;
    head_x = WIDTH / 2;
    head_y = HEIGHT / 2;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
    tail_length = 0;
}

void draw() {
    clear_screen();
    printf("Welcome to Snake Game! Press 'X' to quit.\n");

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");

            if (i == head_y && j == head_x)
                printf("O");
            else if (i == fruit_y && j == fruit_x)
                printf("F");
            else {
                int printed = 0;
                for (int k = 0; k < tail_length; k++) {
                    if (tail_x[k] == j && tail_y[k] == i) {
                        printf("o");
                        printed = 1;
                        break;
                    }
                }
                if (!printed) printf(" ");
            }

            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\nScore: %d\n", score);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
                exit(0);
        }
    }
}

void game_play() {
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev2_x, prev2_y;
    tail_x[0] = head_x;
    tail_y[0] = head_y;

    for (int i = 1; i < tail_length; i++) {
        prev2_x = tail_x[i];
        prev2_y = tail_y[i];
        tail_x[i] = prev_x;
        tail_y[i] = prev_y;
        prev_x = prev2_x;
        prev_y = prev2_y;
    }

    switch (dir) {
        case LEFT:
            head_x--;
            break;
        case RIGHT:
            head_x++;
            break;
        case UP:
            head_y--;
            break;
        case DOWN:
            head_y++;
            break;
        default:
            break;
    }

    // wall collision
    if (head_x >= WIDTH || head_x < 0 || head_y >= HEIGHT || head_y < 0) {
        printf("\nGame Over! You hit the wall.\n");
        exit(0);
    }

    // tail collision
    for (int i = 0; i < tail_length; i++) {
        if (tail_x[i] == head_x && tail_y[i] == head_y) {
            printf("\nGame Over! You hit your tail.\n");
            exit(0);
        }
    }

    if (head_x == fruit_x && head_y == fruit_y) {
        score += 10;
        fruit_x = rand() % WIDTH;
        fruit_y = rand() % HEIGHT;
        tail_length++;
    }
}

void clear_screen() {
    system("cls"); // for Windows
}
