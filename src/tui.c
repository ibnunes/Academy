//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    tui.h
//
// DESCRIPTION:
// -----------
// Includes methods for showing and executing menus in text-based interaction.
//------------------------------------------------------------------------------


#include "tui.h"
#include "types.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>


/* === Principal método, sensível ao OS === */
void tui_output(char const* message, char const* fallback, ...) {
   va_list args;
   va_start(args, fallback);
#ifdef _POSIX_C_SOURCE
   if (isatty(fileno(stdout)))
      vfprintf(stdout, message, args);
   else
#endif
      vfprintf(stdout, fallback, args);
   va_end(args);
}


/* === Menus === */
/* Mostra o conteúdo de um menu. */
void show_menu(const struct item *menu) {
   for (int i = 0; menu[i].text != NULL; i++)
#ifdef _POSIX_C_SOURCE
      if (isatty(fileno(stdout)))
         printf("\x1b[97m%2d > %s\x1b[0m\n", i+1, menu[i].text);
      else
#endif
         printf("%2d > %s\n", i+1, menu[i].text);
}

/* Apresenta um título formatado especificamente para menus. */
void title_menu(const char* title) {
   tui_output("\n\x1b[30;107m %s \x1b[0m\n", "\n %s \n", title);
}

/* Apresenta um título formatado especificamente para métodos interactivos. */
void tui_title(const char* title) {
   #ifdef _POSIX_C_SOURCE
      if (isatty(fileno(stdout)))
         printf("\n\x1b[30;102m %s \x1b[0m\n", title);
      else
   #endif
         printf("\n %s \n", title);
}

/* Calcula quantos items tem um dado menu. */
int size_menu(const struct item *menu) {
   int i = 0;
   for(; menu[i].text != NULL; i++);
   return i;
}

/* Executa o menu, i.e., apresenta um prompt, espera por uma opção do utilizador
   e dispara a função associada.
   TO-DO: permitir definir caracteres para cada opção. */
void exec_menu(struct world *w, const struct item *menu, int *opt, const char *prompt) {
   #ifdef _POSIX_C_SOURCE
      if (isatty(fileno(stdout)))
         printf("\x1b[97;100m%s\x1b[0m ", prompt);
      else
   #endif
         printf("%s ", prompt);
   scanf("%d", opt);
   clear_buffer(stdin);       // Menu é apresentado indexado a 1
   (*opt)--;               // Opção deve ser convertida para zero-based
   if (*opt != size_menu(menu) - 1) {
      if (menu[*opt].func != NULL)
         menu[*opt].func(w);
   }
}

/* Métodos para apresentar mensagens de diferentes categorias */
// Erro (vermelho vivo)
void tui_write_error(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output("\x1b[91%s\x1b[0m", "%s", fmt, args);
   va_end(args);
}

// Aviso (amarelo vivo)
void tui_write_warning(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output("\x1b[93m%s\x1b[0m", "%s", fmt, args);
   va_end(args);
}

// Informação (ciano)
void tui_write_info(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output("\x1b[36m%s\x1b[0m", "%s", fmt, args);
   va_end(args);
}

/* Alerta (fundo vermelho) - faz parágrafo final */
void tui_writeln_alert(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output("\x1b[97;101m %s \x1b[0m\n", " %s ", fmt, args);
   va_end(args);
}

// Normal (branco)
void tui_writeb(const char* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output("\x1b[97m%s\x1b[0m", "%s", fmt, args);
   va_end(args);
}

void tui_write(char const* fmt, ...) {
   va_list args;
   va_start(args, fmt);
   tui_output(fmt, fmt, args);
   va_end(args);
}


/* Coloca parágrafos no stdout. */
void println(void) {
   printf("\n");
}

void printlnn(unsigned n) {
   for (; n > 0; n--)
      println();
}