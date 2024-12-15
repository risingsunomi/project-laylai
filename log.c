// chat log to file
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "chat_log.txt"

void write_message_to_log(const char *message) {
  FILE *log_file = fopen(LOG_FILE, "a");
  if (!log_file) {
    fprintf(stderr, "Error: Could not open log file for writing.\n");
    return;
  }
  fprintf(log_file, "%s\n", message);
  fclose(log_file);
}

MessageNode *read_log_messages(int max_messages) {
  FILE *log_file = fopen(LOG_FILE, "r");
  if (!log_file) {
    fprintf(stderr, "Error: Could not open log file for reading.\n");
    return NULL;
  }

  MessageNode *messages = NULL;
  MessageNode *tail = NULL;
  char line[MAX_INPUT];

  // Read messages from the log file
  while (fgets(line, sizeof(line), log_file)) {
    // Remove newline character if present
    line[strcspn(line, "\n")] = '\0';

    // Add message to the linked list
    add_message(line, &messages, &tail, NULL);
    if (max_messages > 0 && count_messages(messages) > max_messages) {
      remove_oldest_message(&messages);
    }
  }

  fclose(log_file);
  return messages;
}

void remove_oldest_message(MessageNode **head) {
  if (*head == NULL) return;

  MessageNode *temp = *head;
  *head = (*head)->next;
  free(temp);
}

int count_messages(MessageNode *head) {
  int count = 0;
  MessageNode *current = head;
  while (current) {
    count++;
    current = current->next;
  }
  return count;
}

