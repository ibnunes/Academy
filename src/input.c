#include "input.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "utils.h"

#include "debug.h"

static
void accept_string(FILE *stream, char * const dest, size_t const size)
{
   (void)fgets(dest, size, stream);
   size_t len = strlen(dest);

   if (dest[len-1] != '\n') {
      clear_buffer(stdin);
   } else {
      dest[len - 1] = '\0';
   }
}

size_t prompt_string(const char * const promptmsg, char * const dest, size_t const size)
{
   char buffer[size + 1];

   printf("%s", promptmsg);
   accept_string(stdin, buffer, size + 1);
   strncpy(dest, buffer, size);

   return strlen(buffer);
}

uint16_t prompt_id(const char * const promptmsg)
{
   char buffer[16] = {0};
   bool valid = false;
   uint16_t result = 0;

   while (!valid) {
      printf("%s", promptmsg);
      accept_string(stdin, buffer, 16);

      int len = 0;
      int res = sscanf(buffer, "%" SCNu16 "%n", &result, &len);
      if (res != 1 || len != strlen(buffer))
         continue;   // Invalid integer
      else
         valid = true;
   }

   return result;
}
