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


#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void clear_buffer(FILE*);
void strlf(char*);
long fsize(FILE*);
int strcontains(const char*, const char*);
char charupr(char);

#endif
