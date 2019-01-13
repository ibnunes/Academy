//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    ucmanagement.h
//
// DESCRIPTION:
// -----------
// Methods to manage curricular units.
//------------------------------------------------------------------------------


#ifndef UCMANAGEMENT_H
#define UCMANAGEMENT_H

#include <stdint.h>
#include "types.h"

unicurr* uc_by_id(unicurr*, const uint32_t, const uint16_t);
unicurr* uc_by_name(unicurr*, const uint32_t, const char*);
unicurr** list_uc_by_name(uint32_t*, unicurr*, const uint32_t, const char*);
unicurr* add_uc(unicurr*, uint32_t*, unicurr*);
void remove_uc(unicurr*, uint32_t*, uint16_t);
void remove_uc_p(unicurr*, uint32_t*);
void refresh_uc(unicurr*, uint32_t, unicurr*);
void refresh_uc_p(unicurr*, unicurr*);

#endif
