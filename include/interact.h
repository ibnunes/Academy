//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    interact.h
//
// DESCRIPTION:
// -----------
// Includes all methods that interact with the user.
// It binds to the core of the program, acting like an interface.
//------------------------------------------------------------------------------


#ifndef INTERACT_H
#define INTERACT_H

#include "types.h"

void tui_show_course(course const* const);
void tui_get_course(struct world*);
void tui_set_course(struct world*);
void tui_remove_course(struct world*);
void tui_refresh_course(struct world*);

void tui_show_professor(professor const* const);
void tui_get_professor(struct world*);
void tui_set_professor(struct world*);
void tui_remove_professor(struct world*);
void tui_refresh_professor(struct world*);

void tui_show_student(student const* const);
void tui_get_student(struct world*);
void tui_set_student(struct world*);
void tui_remove_student(struct world*);
void tui_refresh_student(struct world*);

void tui_show_uc(unicurr const* const);
void tui_get_uc(struct world*);
void tui_set_uc(struct world*);
void tui_remove_uc(struct world*);
void tui_refresh_uc(struct world*);

#endif
