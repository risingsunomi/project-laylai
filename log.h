#ifndef LOG_H
#define LOG_H

#include "chat_ui.h"

// Append a message to the log file
void write_message_to_log(const char *message);

// Load messages from the log file into a linked list
MessageNode *read_log_messages(int max_messages);

// Utility function to remove the oldest message
void remove_oldest_message(MessageNode **head);

// Count messages in the linked list
int count_messages(MessageNode *head);

#endif

