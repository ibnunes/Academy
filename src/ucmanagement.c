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


#include "ucmanagement.h"
#include <stdint.h>
#include "types.h"
#include "utils.h"

#include "tui.h"     // DEBUG ONLY
#include <inttypes.h>

/* Pesquisar UC por ID único
   Retorna o apontador para o primeiro registo com o ID dado. */
unicurr* uc_by_id(unicurr* uc, const uint32_t size, const uint16_t id) {
   for (uint32_t i = 0, j = 0; j < size; i++, j += (uc[i-1].id != 0)) {
      if (uc[i].id == 0) continue;
      if (uc[i].id == id)
         return &uc[i];
   }
   return NULL;
}

/* Pesquisar UC por nome.
   Retorna o apontador para o primeiro registo que contenha o nome dado. */
unicurr* uc_by_name(unicurr* uc, const uint32_t size, const char* name) {
   for (uint32_t i = 0, j = 0; j < size; i++, j += (uc[i-1].id != 0)) {
      if (uc[i].id == 0) continue;
      if (strcontains(name, uc[i].name))
         return &uc[i];
   }
   return NULL;
}

/* Pesquisar UC por nome.
   Retorna uma lista de apontadores para todos os registos que contenham o nome à procura. */
unicurr** list_uc_by_name(uint32_t* count, unicurr* ptr, const uint32_t size, const char* name) {
   unicurr** list = NULL;
   *count = 0;
   for (uint32_t i = 0, j = 0; j < size; i++, j += (ptr[i-1].id != 0)) {
      if (ptr[i].id == 0) continue;
      if (strcontains(name, ptr[i].name)) {
         (*count)++;
         list = realloc(list, *count * sizeof(unicurr*));
         list[*count-1] = &ptr[i];
      }
   }
   return list;
}

/* Adiciona uma nova UC.
   'ptr' aponta para o array de tamanho 'size' de dados onde gravar.
   'new' é o novo dado a armazenar. */
unicurr* add_uc(unicurr* ptr, uint32_t* size, unicurr* new) {
   if (uc_by_id(ptr, *size, new->id) == NULL) {
      (*size)++;
      ptr = realloc(ptr, *size * sizeof(unicurr));
      ptr[*size-1] = *new;
   }
   return ptr;
}

/* Remove UC com um dado ID. */
void remove_uc(unicurr* ptr, uint32_t* size, uint16_t id) {
   unicurr* q = uc_by_id(ptr, *size, id);
   if (q != NULL) {
      q->id = 0;
      (*size)--;
   }
}

/* Remove UC apontado por 'ptr'. */
void remove_uc_p(unicurr* ptr, uint32_t* size) {
   if (ptr != NULL) {
      ptr->id = 0;
      (*size)--;
   }
}

/* Actualiza uma UC com um dado ID. */
void refresh_uc(unicurr* ptr, uint32_t size, unicurr* new) {
   unicurr* q = uc_by_id(ptr, size, new->id);
   if (ptr != NULL)
      *q = *new;
}

/* Actualiza uma UC apontada por 'ptr'. */
void refresh_uc_p(unicurr* ptr, unicurr* new) {
   if (new != NULL) {
      *ptr = *new;
   }
}
