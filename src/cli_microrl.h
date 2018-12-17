#ifndef CLI_MICRORL_H
#define CLI_MICRORL_H

typedef struct rfid_card {
    char *UID;
    char *name;
    char *size;
    struct rfid_card *next;
} rfid_card_t;

int cli_execute(int argc, const char *const *argv);
extern rfid_card_t *head;
#endif /* CLI_MICRORL_H */

