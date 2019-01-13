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


#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "sism.h"

#define STRMAX 256

/* === TIPOS GERAIS === */
/* Data */
typedef struct {
   uint8_t year, month, day;
} date_t;


/* === ESTRUTURAS DE DADOS === */
/* Unidade curricular */
typedef struct {
   uint16_t id;               // ID único
   char name[STRMAX];         // Designação
   uint8_t semester;          // Semestre em que decorre
} unicurr;

/* Curso */
typedef struct {
   uint16_t id;               // ID único
   char name[STRMAX];         // Designação
   uint32_t n_uc;             // Nº unidades curriculares
   unicurr** uc;              // Lista de unidades curriculares
} course;

/* Professor */
typedef struct {
   uint16_t id;               // ID único
   char name[STRMAX];         // Nome
   uint32_t n_uc;             // Nº unidades curriculares que lecciona
   unicurr** uc;              // Lista de unidades curriculares
} professor;

/* Estudante */
// Elemento da tabela de avaliação
typedef struct {
   unicurr* uc;               // Unidade curricular
   bool attending;            // Está a frequentar de momento?
   int8_t evaluation;         // Qual a avaliação?
} assessment;

// Dados do estudante
typedef struct {
   uint16_t id;               // ID único
   char name[STRMAX];         // Nome
   course* course_enrolled;   // Curso actual
   date_t enrollment;         // Data de inscrição
   uint32_t n_uc;             // Nº unidades curriculares
   assessment* eval;          // Tabela de avaliação
} student;

/* === "THE WORLD" === */
struct world {
   struct {
      uint32_t
         uc,                  // Contador de unidades curriculares
         courses,             // Contador de cursos
         professors,          // Contador de professores
         students;            // Contador de estudantes
   } count;

   unicurr* uc;               // Lista de unidades curriculares
   course* courses;           // Lista de cursos
   professor* professors;     // Lista de professores
   student* students;         // Lista de estudantes

   SISM status;             // State Machine (SISM System)
};


/* Métodos gerais */
char* datetostr(char*, date_t);

/* Inicialização, actualização e finalização do "world" */
void init_world(struct world*);
void refresh_world(struct world*);
void free_world(struct world*);

#endif
