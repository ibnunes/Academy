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


#include "profmanagement.h"
#include <stdint.h>
#include "types.h"
#include "utils.h"

/* Pesquisar professor por ID único
   Retorna o apontador para o primeiro registo com o ID dado. */
professor* professor_by_id(professor* prof, const uint32_t size, const uint16_t id) {
   for (uint32_t i = 0, j = 0; j < size; j += (prof[i].id != 0), i++) {
      if (prof[i].id == 0) continue;
      if (prof[i].id == id)
         return &prof[i];
   }
   return NULL;
}

/* Pesquisar professor por nome.
   Retorna o apontador para o primeiro registo que contenha o nome dado. */
professor* professor_by_name(professor* prof, const uint32_t size, const char* name) {
   for (uint32_t i = 0, j = 0; j < size; j += (prof[i].id != 0), i++) {
      if (prof[i].id == 0) continue;
      if (strcontains(name, prof[i].name))
         return &prof[i];
   }
   return NULL;
}

/* Pesquisar professor por nome.
   Retorna uma lista de apontadores para todos os registos que contenham o nome à procura. */
professor** list_professor_by_name(uint32_t* count, professor* ptr, const uint32_t size, const char* name) {
   professor** list = NULL;
   *count = 0;
   for (uint32_t i = 0, j = 0; j < size; j += (ptr[i].id != 0), i++) {
      if (ptr[i].id == 0) continue;
      if (strcontains(name, ptr[i].name)) {
         (*count)++;
         list = realloc(list, *count * sizeof(unicurr*));
         list[*count-1] = &ptr[i];
      }
   }
   return list;
}

/* Pesquisar professores que leccionem uma dada UC identificada por ID */
professor** list_professor_by_uc_id(professor* cin, const uint32_t csize, const uint16_t ucid, int* count) {
   *count = 0;
   professor** cout = NULL;
   for (uint32_t i = 0, x = 0; x < csize; x += (cin[i].id != 0), i++) {
      for (uint32_t j = 0, y = 0; y < cin[i].n_uc; y += (cin[i].uc[j]->id != 0), j++) {
         if (cin[i].uc[j]->id == ucid) {
            (*count)++;
            cout = realloc(cout, *count * sizeof(professor*));
            cout[*count-1] = &cin[i];
            break;
         }
      }
   }
   return cout;
}

/* Pesquisar professores que leccionem uma dada UC identificada por nome.
   Nota: Basta conter o nome, não precisa ser absolutamente igual.
         Essa responsabilidade deve ser de quem invoca a função. */
professor** list_professor_by_uc_name(professor* cin, const uint32_t csize, const char* name, int* count) {
   *count = 0;
   professor** cout = NULL;
   for (uint32_t i = 0, x = 0; x < csize; x += (cin[i].id != 0), i++) {
      for (uint32_t j = 0, y = 0; y < cin[i].n_uc; y += (cin[i].uc[j]->id != 0), j++) {
         if (strcontains(name, cin[i].uc[j]->name)) {
            (*count)++;
            cout = realloc(cout, *count * sizeof(professor*));
            cout[*count-1] = &cin[i];
            break;
         }
      }
   }
   return cout;
}

/* Adiciona um novo professor.
   'ptr' aponta para o array de tamanho 'size' de dados onde gravar.
   'new' é o novo dado a armazenar. */
professor* add_professor(professor* ptr, uint32_t* size, professor* new) {
   if (professor_by_id(ptr, *size, new->id) == NULL) {
      (*size)++;
      ptr = realloc(ptr, *size * sizeof(professor));
      ptr[*size-1] = *new;
   }
   return ptr;
}

/* Remove professor com um dado ID.
   Sé será implementado este método. Outros deverão basear-se neste e nos anteriores
   definidos a gosto pelo programador consoante os objectivos particulares da aplicação. */
void remove_professor(professor* ptr, uint32_t* size, uint16_t id) {
   professor* q = professor_by_id(ptr, *size, id);
   if (q != NULL) {
      q->id = 0;
      (*size)--;
   }
}

// Se já tivermos o apontador, fazemos a eliminação directa.
void remove_professor_p(professor* ptr, uint32_t* size) {
   if (ptr != NULL) {
      ptr->id = 0;
      (*size)--;
   }
}

/* Actualiza um professor com um dado ID.
   Se o ID não existir, nada faz por definição. */
void refresh_professor(professor* ptr, uint32_t size, professor* new) {
   professor* q = professor_by_id(ptr, size, new->id);
   if (ptr != NULL)
      *q = *new;
}


// Se já tivermos o apontador, fazemos a actualização directa.
void refresh_professor_p(professor* ptr, professor* new) {
   if (new != NULL) {
      *ptr = *new;
   }
}

/* Adiciona UC à lista de UCs via apontador */
void professor_add_uc(professor* ptr, unicurr* new_uc) {
   (ptr->n_uc)++;
   ptr->uc = realloc(ptr->uc, ptr->n_uc * sizeof(unicurr*));
   ptr->uc[ptr->n_uc - 1] = new_uc;
}

/* Remove UC da lista de UCs via ID */
int professor_remove_uc(professor* ptr, uint16_t uc_id) {
   for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += (ptr->uc[i]->id != 0), i++) {
      if (ptr->uc[i]->id == uc_id) {
         ptr->uc[i] = NULL;
         return 1;      // Success - permite usar directamente numa condição
      }
   }
   return 0;            // ID não existente, nada a fazer
}
