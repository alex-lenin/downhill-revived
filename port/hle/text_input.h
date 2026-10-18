#ifndef DHD_TEXT_INPUT_H
#define DHD_TEXT_INPUT_H
/* One focused field; window thread produces characters, game thread edits the field. */
void text_input_begin(void);
void text_input_end(void);
int text_input_active(void);
void text_input_character(unsigned character);
void text_input_clear(void);
int text_input_key_allowed(unsigned virtual_key);
/* 1 confirm, -1 cancel, 0 editing. allowed is the field's printable alphabet. */
int text_input_edit(char *buffer, int capacity, const char *allowed);
#endif
