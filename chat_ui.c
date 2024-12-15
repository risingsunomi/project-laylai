// Chat UI
// for displaying and interacting with AI chat


#include "chat_ui.h"

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
  }
}

void shutdown_ncurses() {
  endwin();
}

void add_message(const char *message, MessageNode **head, MessageNode **tail, int *message_count) {
  MessageNode *new_node = (MessageNode *)malloc(sizeof(MessageNode));
  if (!new_node) {
    perror("Failed to allocate memory for new message");
    return;
  }
  strncpy(new_node->message, message, MAX_INPUT - 1);
  new_node->message[MAX_INPUT - 1] = '\0';
  new_node->next = NULL;

  if (*tail) {
    (*tail)->next = new_node;
  } else {
    *head = new_node;
  }
  *tail = new_node;
  (*message_count)++;
}

void draw_chat_window(WINDOW *chat_win, MessageNode *head, int message_count, int offset, int max_displayable) {
  werase(chat_win);
  box(chat_win, 0, 0);

  MessageNode *current = head;
  int start = offset;
  int line = 1;
  int index = 0;

  while (current && line <= max_displayable) {
    if (index >= start) {
      mvwprintw(chat_win, line++, 1, "%s", current->message);
    }
    current = current->next;
    index++;
  }
}

void draw_input_window(WINDOW *input_win) {
  werase(input_win);
  box(input_win, 0, 0);
}

void draw_user_list(WINDOW *user_win, User *head) {
  werase(user_win);
  box(user_win, 0, 0);
  mvwprintw(user_win, 0, 1, "Users");

  User *current = head;
  int line = 1;
  while (current && line < getmaxy(user_win) - 1) {
    mvwprintw(user_win, line++, 1, "%s", current->username);
    current = current->next;
  }
}

void add_user(const char *username, User **head) {
  User *new_user = (User *)malloc(sizeof(User));
  if (!new_user) {
    perror("Failed to allocate memory for new user");
    return;
  }
  strncpy(new_user->username, username, sizeof(new_user->username) - 1);
  new_user->username[sizeof(new_user->username) - 1] = '\0';
  new_user->next = *head;
  *head = new_user;
}

void free_messages(MessageNode *head) {
  while (head) {
    MessageNode *temp = head;
    head = head->next;
    free(temp);
  }
}

void free_users(User *head) {
  while (head) {
    User *temp = head;
    head = head->next;
    free(temp);
  }
}

