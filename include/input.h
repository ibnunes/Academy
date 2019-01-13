#ifndef INPUT_H
#define INPUT_H

#include <stdlib.h>
#include <stdint.h>

size_t prompt_string(
   const char * const promptmsg,
   char * const dest,
   size_t const size);

uint16_t prompt_id(const char * const promptmsg);

#endif
