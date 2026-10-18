#include "text_input.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    char field[7] = "";
    const char *alphabet = "23456789ABCDEFGHJKMNPQRSTUVWXYZ";
    text_input_character('X'); /* outside a field: no stale input */
    text_input_begin();
    assert(text_input_active());
    const char *s = "az01o!234567";
    while (*s) text_input_character((unsigned char)*s++);
    assert(!text_input_edit(field, sizeof field, alphabet));
    assert(!strcmp(field, "AZ2345")); /* filtering, case folding, bounded append */
    text_input_character(8);
    text_input_character('q');
    text_input_character(13);
    assert(text_input_edit(field, sizeof field, alphabet) == 1);
    assert(!strcmp(field, "AZ234Q"));
    text_input_character(27);
    assert(text_input_edit(field, sizeof field, alphabet) == -1);
    text_input_character('X');
    text_input_clear();
    assert(!text_input_edit(field, sizeof field, alphabet));
    text_input_end();
    assert(!text_input_active());
    puts("text input: ok");
    return 0;
}
