//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    profmanagement.h
//
// DESCRIPTION:
// -----------
// Methods to manage professors.
//------------------------------------------------------------------------------


#ifndef PROFMANAGEMENT_H
#define PROFMANAGEMENT_H

#include "types.h"

professor* professor_by_id(professor*, const uint32_t, const uint16_t);
professor* professor_by_name(professor*, const uint32_t, const char*);
professor** list_professor_by_name(uint32_t*, professor*, const uint32_t, const char*);
professor** list_professor_by_uc_id(professor*, const uint32_t, const uint16_t, int*);
professor** list_professor_by_uc_name(professor*, const uint32_t, const char*, int*);
professor* add_professor(professor*, uint32_t*, professor*);
void remove_professor(professor*, uint32_t*, uint16_t);
void remove_professor_p(professor*, uint32_t*);
void refresh_professor(professor*, uint32_t, professor*);
void refresh_professor_p(professor*, professor*);
void professor_add_uc(professor*, unicurr*);
int professor_remove_uc(professor*, uint16_t);

#endif
