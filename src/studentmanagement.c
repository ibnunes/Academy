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


#include "studentmanagement.h"
#include <stdint.h>
#include "types.h"
#include "utils.h"

/* Pesquisar estudante por ID único
   Retorna o apontador para o primeiro registo com o ID dado. */
student* student_by_id(student* stud, const uint32_t size, const uint16_t id) {
   for (uint32_t i = 0, j = 0; j < size; j += (stud[i].id != 0), i++) {
      if (stud[i].id == 0) continue;
      if (stud[i].id == id)
         return &stud[i];
   }
   return NULL;
}

/* Pesquisar estudante por nome.
   Retorna o apontador para o primeiro registo que contenha o nome dado. */
student* student_by_name(student* stud, const uint32_t size, const char* name) {
   for (uint32_t i = 0, j = 0; j < size; j += (stud[i].id != 0), i++) {
      if (stud[i].id == 0) continue;
      if (strcontains(name, stud[i].name))
         return &stud[i];
   }
   return NULL;
}

/* Pesquisar estudante por nome.
   Retorna uma lista de apontadores para todos os registos que contenham o nome à procura. */
student** list_student_by_name(uint32_t* count, student* ptr, const uint32_t size, const char* name) {
   student** list = NULL;
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

/* Pesquisar estudantes que frequentem uma dada UC identificada por ID */
student** list_student_by_uc_id(student* cin, const uint32_t csize, const uint16_t ucid, int* count) {
   *count = 0;
   student** cout = NULL;
   for (uint32_t i = 0, x = 0; x < csize; x += (cin[i].id != 0), i++) {
      for (uint32_t j = 0, y = 0; y < cin[i].n_uc; y += (cin[i].eval[j].uc->id != 0), j++) {
         if (cin[i].eval[j].uc->id == ucid) {
            (*count)++;
            cout = realloc(cout, *count * sizeof(student*));
            cout[*count-1] = &cin[i];
            break;
         }
      }
   }
   return cout;
}

/* Pesquisar estudantes que frequentem uma dada UC identificada por nome.
   Nota: Basta conter o nome, não precisa ser absolutamente igual.
         Essa responsabilidade deve ser de quem invoca a função. */
student** list_student_by_uc_name(student* cin, const uint32_t csize, const char* name, int* count) {
   *count = 0;
   student** cout = NULL;
   for (uint32_t i = 0, x = 0; x < csize; x += (cin[i].id != 0), i++) {
      for (uint32_t j = 0, y = 0; y < cin[i].n_uc; y += (cin[i].eval[j].uc->id != 0), j++) {
         if (strcontains(name, cin[i].eval[j].uc->name)) {
            (*count)++;
            cout = realloc(cout, *count * sizeof(student*));
            cout[*count-1] = &cin[i];
            break;
         }
      }
   }
   return cout;
}

/* Adiciona um novo estudante.
   'ptr' aponta para o array de tamanho 'size' de dados onde gravar.
   'new' é o novo dado a armazenar. */
student* add_student(student* ptr, uint32_t* size, student* new) {
   if (student_by_id(ptr, *size, new->id) == NULL) {
      (*size)++;
      ptr = realloc(ptr, *size * sizeof(student));
      ptr[*size-1] = *new;
   }
   return ptr;
}

/* Remove estudante com um dado ID.
   Sé será implementado este método. Outros deverão basear-se neste e nos anteriores
   definidos a gosto pelo programador consoante os objectivos particulares da aplicação. */
void remove_student(student* ptr, uint32_t* size, uint16_t id) {
   student* q = student_by_id(ptr, *size, id);
   if (q != NULL) {
      q->id = 0;
      (*size)--;
   }
}

// Se já tivermos o apontador, fazemos a eliminação directa.
void remove_student_p(student* ptr, uint32_t* size) {
   if (ptr != NULL) {
      ptr->id = 0;
      (*size)--;
   }
}

/* Actualiza um estudante com um dado ID.
   Se o ID não existir, nada faz por definição. */
void refresh_student(student* ptr, uint32_t size, student* new) {
   student* q = student_by_id(ptr, size, new->id);
   if (ptr != NULL)
      *q = *new;
}

// Se já tivermos o apontador, fazemos a actualização directa.
void refresh_student_p(student* ptr, student* new) {
   if (new != NULL) {
      *ptr = *new;
   }
}

/* Adiciona UC à lista de UCs via apontador */
void student_add_assessment(student* ptr, assessment* new_assessment) {
   (ptr->n_uc)++;
   ptr->eval = realloc(ptr->eval, ptr->n_uc * sizeof(assessment));
   ptr->eval[ptr->n_uc - 1] = *new_assessment;
}

/* Remove UC da lista de UCs via ID */
int student_remove_assessment(student* ptr, uint16_t assessment_id) {
   for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += ((ptr->eval[i].uc->id != 0) && (ptr->eval[i].uc != NULL)), i++) {
      if (ptr->eval[i].uc->id == assessment_id) {
         ptr->eval[i].uc = NULL;
         ptr->eval[i].attending = false;
         ptr->eval[i].evaluation = -1;
         return 1;      // Success - permite usar directamente numa condição
      }
   }
   return 0;            // ID não existente, nada a fazer
}

assessment* assessment_by_id(student* ptr, const uint16_t id) {
   for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += ((ptr->eval[i].uc->id != 0) && (ptr->eval[i].uc != NULL)), i++) {
      if (ptr->eval[i].uc->id == id) {
         return &ptr->eval[i];
      }
   }
   return NULL;
}
