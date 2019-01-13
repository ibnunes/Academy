//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    iostream.h
//
// DESCRIPTION:
// -----------
// Transfers data between memory (stack and heap) and disk.
//------------------------------------------------------------------------------


#ifndef IOSTREAM_H
#define IOSTREAM_H

#include <stdio.h>
#include "types.h"

int loadfromfile(struct world*, const char*);
int savetofile(const struct world*, const char*);

#endif
