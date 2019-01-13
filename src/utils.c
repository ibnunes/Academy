//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    utils.h
//
// DESCRIPTION:
// -----------
// Provides some useful functions and alternatives to mainstream functions.
//------------------------------------------------------------------------------


#include "utils.h"
#include <stdio.h>
#include <string.h>

/* Limpa o buffer de entrada.
   A usar depois de um scanf e antes da leitura de uma string do stdin. */
void clear_buffer(FILE* stream) {
   char c;
   while (c = fgetc(stream), (c != EOF) && (c != '\n'))
      ;
}

/* Elimina o LF final de uma string obtida por fgets. */
void strlf(char* s) {
   for (; *s != '\0'; s++);
   s--;
   *s = '\0';
}

/* Devolve quantos bytes tem um ficheiro */
long fsize(FILE* f) {
   fseek(f, 0L, SEEK_END);
   return ftell(f);
}

/* Alternativa ao strstr */
int strcontains(const char* substr, const char* str) {
   return (strstr(str, substr) != NULL);
}

/* Converte uma letra (char) em maiúscula */
char charupr(char c) {
   return ((c > 'a') && (c < 'z') ? c - 'a' + 'A' : c);
}
