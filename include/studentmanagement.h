//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    studentmanagement.h
//
// DESCRIPTION:
// -----------
// Methods to manage students.
//------------------------------------------------------------------------------


#ifndef STUDENTMANAGEMENT_H
#define STUDENTMANAGEMENT_H

#include <stdint.h>
#include "types.h"

student* student_by_id(student*, const uint32_t, const uint16_t);
student* student_by_name(student*, const uint32_t, const char*);
student** list_student_by_name(uint32_t*, student*, const uint32_t, const char*);
student** list_student_by_uc_id(student*, const uint32_t, const uint16_t, int*);
student** list_student_by_uc_name(student*, const uint32_t, const char*, int*);
student* add_student(student*, uint32_t*, student*);
void remove_student(student*, uint32_t*, uint16_t);
void remove_student_p(student*, uint32_t*);
void refresh_student(student*, uint32_t, student*);
void refresh_student_p(student*, student*);
void student_add_assessment(student*, assessment*);
int student_remove_assessment(student*, uint16_t);
assessment* assessment_by_id(student*, const uint16_t);

#endif
