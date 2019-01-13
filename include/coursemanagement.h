//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    coursemanagement.h
//
// DESCRIPTION:
// -----------
// Methods to manage courses.
//------------------------------------------------------------------------------


#ifndef COURSEMANAGEMENT_H
#define COURSEMANAGEMENT_H

#include <stdint.h>
#include "types.h"

course* course_by_id(course*, const uint32_t, const uint16_t);
course* course_by_name(course*, const uint32_t, const char*);
course** list_course_by_name(uint32_t*, course*, const uint32_t, const char*);
course** list_course_by_uc_id(course*, const uint32_t, const uint16_t, int*);
course** list_course_by_uc_name(course*, const uint32_t, const char*, int*);
course* add_course(course*, uint32_t*, course*);
void remove_course(course*, uint32_t*, uint16_t);
void remove_course_p(course*, uint32_t*);
void refresh_course(course*, uint32_t, course*);
void refresh_course_p(course*, course*);
void course_add_uc(course*, unicurr*);
int course_remove_uc(course*, uint16_t);

#endif
