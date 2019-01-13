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


#ifndef TUI_H
#define TUI_H

#include "types.h"

struct item {
   char const* text;
   void (*func)(struct world* w);
};

int size_menu(const struct item*);
void title_menu(const char*);
void tui_title(const char*);
void show_menu(const struct item*);
void exec_menu(struct world*, const struct item*, int*, const char*);

void tui_write_error(const char*, ...);
void tui_write_warning(const char*, ...);
void tui_write_info(const char*, ...);
void tui_writeln_alert(const char*, ...);
void tui_writeb(const char*, ...);

void tui_output(char const*, char const*, ...);
void tui_write(char const*, ...);

void println(void);
void printlnn(unsigned);

#endif
