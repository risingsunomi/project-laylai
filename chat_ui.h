#ifndef CHAT_UI_H
#define CHAT_UI_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 200
#define MAX_MESSAGE_DISPLAY 10
#define MAX_USERNAME_LEN 50

typedef struct MessageNode {
  char message[MAX_INPUT];
  struct MessageNode *next;
} MessageNode;

// -- NOTE [12/15/2024]
// should change from linked list, can just be 2d char array
typedef struct User {
  char username[50];
  struct User *next;
} User;

void init_ncurses();
void shutdown_ncurses();
void add_message(const char *message, MessageNode **head, MessageNode **tail, int *message_count);
void draw_chat_window(WINDOW *chat_win, MessageNode *head, int message_count, int offset, int max_displayable);
void draw_input_window(WINDOW *input_win);
void draw_user_list(WINDOW *user_win, User *head);
void free_messages(MessageNode *head);
void free_users(User *head);

// Updated functions for paging and user list management
void add_user(const char *username, User **head);

#endif
