//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    types.h
//
// DESCRIPTION:
// -----------
// Provides all data structures for the entire program.
//------------------------------------------------------------------------------


#include "types.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "debug.h"

/* Inicialização da estrutura WORLD. */
void init_world(struct world* w) {
   memset(w, 0, sizeof *w);
}

/* Libertação de recursos da estrutura WORLD */
void free_world(struct world* w) {
   free(w->uc);
   for (uint32_t i = 0; i < w->count.courses; ++i)
      free(w->courses[i].uc);
   free(w->courses);

   for (uint32_t i = 0; i < w->count.professors; ++i)
      free(w->professors[i].uc);
   free(w->professors);

   for (uint32_t i = 0; i < w->count.students; ++i)
      free(w->students[i].eval);
   free(w->students);
}

/* Actualiza os contadores de dados relacionais (unlink de dados) */
void refresh_world(struct world* w) {
   uint32_t count;

   // cursos <-> UCs
   for(uint32_t i = 0, x = 0; x < w->count.courses; x += (w->courses[i].id != 0), i++) {
      count = 0;
      debug("Checking course #%" PRIu16 " with %" PRIu32 " UCs for unlinked data...\n", w->courses[i].id, w->courses[i].n_uc);
      for (uint32_t j = 0; j < w->courses[i].n_uc; j++) {
         if (w->courses[i].uc[j] != NULL) {
            if (w->courses[i].uc[j]->id != 0)
               count++;
         }
      }
      w->courses[i].n_uc = count;
      debug("Unlinked: got %" PRIu32 " existing UCs.\n", w->courses[i].n_uc);
   }

   // professores <-> UCs
   for(uint32_t i = 0, x = 0; x < w->count.professors; x += (w->professors[i].id != 0), i++) {
      count = 0;
      debug("Checking professor #%" PRIu16 " with %" PRIu32 " UCs for unlinked data...\n", w->professors[i].id, w->professors[i].n_uc);
      for (uint32_t j = 0; j < w->professors[i].n_uc; j++) {
         if (w->professors[i].uc[j] != NULL) {
            if (w->professors[i].uc[j]->id != 0)
               count++;
         }
      }
      w->professors[i].n_uc = count;
      debug("Unlinked: got %" PRIu32 " existing UCs.\n", w->professors[i].n_uc);
   }

   // estudantes <-> UCs
   for(uint32_t i = 0, x = 0; x < w->count.students; x += (w->students[i].id != 0), i++) {
      count = 0;
      debug("Checking student #%" PRIu16 " with %" PRIu32 " UCs for unlinked data...\n", w->students[i].id, w->students[i].n_uc);
      for (uint32_t j = 0; j < w->students[i].n_uc; j++) {
         if (w->students[i].eval[j].uc != NULL) {
            if (w->students[i].eval[j].uc->id != 0)
               count++;
         }
      }
      w->students[i].n_uc = count;
      debug("Unlinked: got %" PRIu32 " existing UCs.\n", w->students[i].n_uc);
   }
}

/* Método geral - conversão de date_t para string */
char* datetostr(char* s, date_t date) {
   sprintf(s, "%04" PRIu8 "/%02" PRIu8 "/%02" PRIu8, date.year, date.month, date.day);
   return s;
}
