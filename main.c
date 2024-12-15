#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "chat_ui.h"
#include "log.h"

int main() {
  // Initialize ncurses
  init_ncurses();

  // Get screen dimensions
  int height, width;
  getmaxyx(stdscr, height, width);
  int input_height = 3;
  int user_width = 20;
  int chat_height = height - input_height;
  int chat_width = width - user_width;

  // Create windows
  WINDOW *chat_win = newwin(chat_height, chat_width, 0, 0);
  WINDOW *user_win = newwin(chat_height, user_width, 0, chat_width);
  WINDOW *input_win = newwin(input_height, width, chat_height, 0);

  // Data structures
  MessageNode *messages = NULL;
  MessageNode *tail = NULL;
  int message_count = 0;
  int offset = 0;

  User *users = NULL;
  char *user_name = "User1";
  add_user(user_name, &users);

  char input_buffer[MAX_INPUT] = {0};
  int running = 1;

  while (running) {
    // Get updated dimensions
    getmaxyx(stdscr, height, width);
    chat_height = height - input_height;
    chat_width = width - user_width;

    // Resize windows
    wresize(chat_win, chat_height, chat_width);
    wresize(user_win, chat_height, user_width);
    mvwin(user_win, 0, chat_width);
    wresize(input_win, input_height, width);
    mvwin(input_win, chat_height, 0);

    // Update maximum messages displayable
    int max_displayable = chat_height - 2; // Buffer for borders

    // Draw windows
    draw_chat_window(chat_win, messages, message_count, offset, max_displayable);
    draw_user_list(user_win, users);
    draw_input_window(input_win);

    // Refresh windows
    wrefresh(chat_win);
    wrefresh(user_win);
    wrefresh(input_win);

    // Handle input
    echo();
    curs_set(1);
    mvwgetnstr(input_win, 1, 1, input_buffer, MAX_INPUT - 1);
    noecho();
    curs_set(0);

    if (strcmp(input_buffer, "/quit") == 0) {
      running = 0;
    } else if (strcmp(input_buffer, "/up") == 0) {
      if (offset > 0) offset--;
    } else if (strcmp(input_buffer, "/down") == 0) {
      if (offset < message_count - max_displayable) offset++;
    } else if (strcmp(input_buffer, "/nick") == 0) {
      printf("nick cmd used %s", input_buffer);
    } else {
      // Add timestamp and username to the message
      char timestamp[20];
      time_t now = time(NULL);
      strftime(timestamp, sizeof(timestamp), "%H:%M:%S", localtime(&now));
      
      // Truncate input to ensure formatted message fits
      char truncated_input[MAX_INPUT - 20]; // Reserve space for timestamp and formatting
      snprintf(truncated_input, sizeof(truncated_input), "%s", input_buffer);

      // Safely format the message
      char formatted_message[MAX_INPUT];
      snprintf(formatted_message, sizeof(formatted_message), "[%s] | User1 | %s", timestamp, truncated_input);

      add_message(formatted_message, &messages, &tail, &message_count);

      printf("saving to log");
      write_message_to_log(formatted_message);

      if(message_count > MAX_MESSAGE_DISPLAY) {
        if (offset < message_count - max_displayable) offset++; 
      }
    }

    memset(input_buffer, 0, sizeof(input_buffer));
  }

  // Clean up
  free_messages(messages);
  free_users(users);
  delwin(chat_win);
  delwin(user_win);
  delwin(input_win);
  shutdown_ncurses();

  return 0;
}

