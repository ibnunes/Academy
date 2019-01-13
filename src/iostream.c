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


#include "iostream.h"
#include <stdio.h>
#include <stdint.h>
#include "types.h"
#include "ucmanagement.h"
#include "coursemanagement.h"
#include "utils.h"
#include "sism.h"

//------------------------------------------------------------------------------
// FOR DEBUG PURPOSES ONLY
#include <inttypes.h>
#include "debug.h"
//------------------------------------------------------------------------------


/* Carrega dados do ficheiro de nome 'fname' para uma estrutura WORLD apontada por 'w'. */
int loadfromfile(struct world* w, const char* fname) {
   FILE *f = fopen(fname, "r");

   if (!f) {
      return __ERROR_FILE;

   } else {
      fseek(f, 0L, SEEK_SET);

      /* Ler nº de dados (contadores) */
      fread(&w->count, sizeof(w->count), 1, f);
      debug("Count: uc = %" PRIu32 "; courses = %" PRIu32 "; professors = %" PRIu32 "; students = %" PRIu32 "\n",
         w->count.uc, w->count.courses, w->count.professors, w->count.students);

      /* Ler dados e relações */
      // Unidades Curriculares
      w->uc = malloc(w->count.uc * sizeof(unicurr));
      for(uint32_t i = 0; i < w->count.uc; i++) {
         fread(&w->uc[i], sizeof(w->uc[i]), 1, f);
         debug("Read uc[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->uc[i].id);
      }

      uint16_t id;

      // Cursos
      w->courses = malloc(w->count.courses * sizeof(course));
      for(uint32_t i = 0; i < w->count.courses; i++) {
         // Dados
         fread(&w->courses[i].id, sizeof(uint16_t), 1, f);
         fread(&w->courses[i].name, STRMAX, 1, f);
         debug("Read courses[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->courses[i].id);

         // Relações
         fread(&w->courses[i].n_uc, sizeof(uint32_t), 1, f);
         w->courses[i].uc = malloc(w->courses[i].n_uc * sizeof(unicurr*));
         for (uint32_t j = 0; j < w->courses[i].n_uc; j++) {
            fread(&id, sizeof(uint16_t), 1, f);
            w->courses[i].uc[j] = uc_by_id(w->uc, w->count.uc, id);
            debug("\tLinked uc[%" PRIu32 "] with ID %" PRIu16 "\n", j, w->courses[i].uc[j]->id);
         }
      }

      // Professores
      w->professors = malloc(w->count.professors * sizeof(professor));
      for(uint32_t i = 0; i < w->count.professors; i++) {
         // Dados
         fread(&w->professors[i].id, sizeof(uint16_t), 1, f);
         fread(&w->professors[i].name, STRMAX, 1, f);
         debug("Read professors[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->professors[i].id);

         // Relações
         fread(&w->professors[i].n_uc, sizeof(uint32_t), 1, f);
         w->professors[i].uc = malloc(w->professors[i].n_uc * sizeof(unicurr*));
         for (uint32_t j = 0; j < w->professors[i].n_uc; j++) {
            fread(&id, sizeof(uint16_t), 1, f);
            w->professors[i].uc[j] = uc_by_id(w->uc, w->count.uc, id);
            debug("\tLinked uc[%" PRIu32 "] with ID %" PRIu16 "\n", j, w->professors[i].uc[j]->id);
         }
      }

      // Estudantes
      w->students = malloc(w->count.students * sizeof(student));
      for(uint32_t i = 0; i < w->count.students; i++) {
         // Dados
         fread(&w->students[i].id, sizeof(uint16_t), 1, f);
         fread(&w->students[i].name, STRMAX, 1, f);
         fread(&w->students[i].enrollment, sizeof(date_t), 1, f);
         fread(&id, sizeof(uint16_t), 1, f);
         w->students[i].course_enrolled = course_by_id(w->courses, w->count.courses, id);
         debug("Read students[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->students[i].id);

         // Relações
         fread(&w->students[i].n_uc, sizeof(uint32_t), 1, f);
         w->students[i].eval = malloc(w->students[i].n_uc * sizeof(assessment));
         for (uint32_t j = 0; j < w->students[i].n_uc; j++) {
            fread(&id, sizeof(uint16_t), 1, f);
            w->students[i].eval[j].uc = uc_by_id(w->uc, w->count.uc, id);
            fread(&w->students[i].eval[j].attending, sizeof(bool), 1, f);
            fread(&w->students[i].eval[j].evaluation, sizeof(int8_t), 1, f);
            debug("\tLinked eval[%" PRIu32 "].uc with ID %" PRIu16 "\n", j, w->students[i].eval[j].uc->id);
         }
      }
   }

   fclose(f);
   return __ERROR_SUCCESS;
}

/* Guarda dados no ficheiro de nome 'fname' armazenados numa estrutura WORLD apontada por 'w'.
   ATENÇÃO: efectua reescrita! */

int savetofile(const struct world *w, const char *fname) {
   FILE *f = fopen(fname, "w");

   if (!f) {
      return __ERROR_FILE;       // Ficheiro não pôde ser aberto ou gerado

   } else {
      /* Escrever nº de dados */
      fwrite(&w->count, sizeof(w->count), 1, f);
      debug("Count: uc = %" PRIu32 "; courses = %" PRIu32 "; professors = %" PRIu32 "; students = %" PRIu32 "\n",
         w->count.uc, w->count.courses, w->count.professors, w->count.students);

      /* Escrever dados e relações */
      // Unidades Curriculares
      for(uint32_t i = 0, j = 0; j < w->count.uc; j += (w->uc[i].id != 0), i++) {
         fwrite(&w->uc[i], sizeof(w->uc[i]), 1, f);
         debug("Wrote uc[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->uc[i].id);
      }

      // Cursos
      for(uint32_t i = 0, x = 0; x < w->count.courses; x += (w->courses[i].id != 0), i++) {
         // Dados
         fwrite(&w->courses[i].id, sizeof(uint16_t), 1, f);
         fwrite(&w->courses[i].name, STRMAX, 1, f);
         debug("Wrote courses[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->courses[i].id);

         // Relações
         fwrite(&w->courses[i].n_uc, sizeof(uint32_t), 1, f);
         for (uint32_t j = 0, y = 0; y < w->courses[i].n_uc; y += (w->courses[i].uc[j] != NULL), j++) {
            if (w->courses[i].uc[j]->id == 0) continue;
            fwrite(&w->courses[i].uc[j]->id, sizeof(uint16_t), 1, f);
            debug("\tWrote linked uc[%" PRIu32 "] with ID %" PRIu16 "\n", j, w->courses[i].uc[j]->id);
         }
      }

      // Professores
      for(uint32_t i = 0, x = 0; x < w->count.professors; x += (w->professors[i].id != 0), i++) {
         // Dados
         fwrite(&w->professors[i].id, sizeof(uint16_t), 1, f);
         fwrite(&w->professors[i].name, STRMAX, 1, f);
         debug("Wrote professors[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->professors[i].id);

         // Relações
         fwrite(&w->professors[i].n_uc, sizeof(uint32_t), 1, f);
         for (uint32_t j = 0, y = 0; y < w->professors[i].n_uc; y += (w->professors[i].uc[j] != NULL), j++) {
            if (w->professors[i].uc[j]->id == 0) continue;
            fwrite(&w->professors[i].uc[j]->id, sizeof(uint16_t), 1, f);
            debug("\tWrote linked uc[%" PRIu32 "] with ID %" PRIu16 "\n", j, w->professors[i].uc[j]->id);
         }
      }

      // Estudantes
      for(uint32_t i = 0, x = 0; x < w->count.students; x += (w->students[i].id != 0), i++) {
         // Dados
         fwrite(&w->students[i].id, sizeof(uint16_t), 1, f);
         fwrite(&w->students[i].name, STRMAX, 1, f);
         fwrite(&w->students[i].enrollment, sizeof(date_t), 1, f);
         fwrite(&w->students[i].course_enrolled->id, sizeof(uint16_t), 1, f);
         debug("Wrote students[%" PRIu32 "] with ID %" PRIu16 "\n", i, w->students[i].id);

         // Relações
         fwrite(&w->students[i].n_uc, sizeof(uint32_t), 1, f);
         for (uint32_t j = 0, y = 0; y < w->students[i].n_uc; y += (w->students[i].eval[j].uc != NULL), j++) {
            if (w->students[i].eval[j].uc->id == 0) continue;
            fwrite(&w->students[i].eval[j].uc->id, sizeof(uint16_t), 1, f);
            fwrite(&w->students[i].eval[j].attending, sizeof(bool), 1, f);
            fwrite(&w->students[i].eval[j].evaluation, sizeof(int8_t), 1, f);
            debug("\tWrote linked eval[%" PRIu32 "].uc with ID %" PRIu16 "\n", j, w->students[i].eval[j].uc->id);
         }
      }
   }

   fclose(f);
   return __ERROR_SUCCESS;
}
