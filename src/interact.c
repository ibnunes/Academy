//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
// RUNTIME LIBRARIES PACKAGE
//    interact.h
//
// DESCRIPTION:
// -----------
// Includes all methods that interact with the user.
// It binds to the core of the program, acting like an interface.
//------------------------------------------------------------------------------

#include "interact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "utils.h"
#include "coursemanagement.h"
#include "profmanagement.h"
#include "studentmanagement.h"
#include "ucmanagement.h"
#include "tui.h"
#include "input.h"

#include "debug.h"

//------------------------------------------------------------------------------
// CURSOS
//------------------------------------------------------------------------------

/* Output de dados de um curso */
void tui_show_course(course const *const ptr)
{
   if (ptr != NULL)
   {
      tui_write("\t  ID: %" PRIu16 "\n", ptr->id);
      tui_write("\tNome: %s\n", ptr->name);
      tui_write("\tConstituído por %" PRIu32 " unidades curriculares:\n", ptr->n_uc);
      for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += (ptr->uc[i] != NULL), i++)
      {
         if (ptr->uc[i]->id == 0)
            continue;
         tui_show_uc(ptr->uc[i]);
      }
   }
}

/* Pesquisa de cursos */
void tui_get_course(struct world *w)
{
   struct item menu[] = {
       {.text = "Pesquisar por ID", .func = NULL},
       {.text = "Pesquisar por nome", .func = NULL},
       {.text = "VOLTAR", .func = NULL},
       {.text = NULL, .func = NULL}};

   int opt;
   do
   {
      println();
      tui_title("PESQUISA DE CURSOS");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
      

      switch (opt)
      {
      // Pesquisar por ID
      case 0:
      {
         uint16_t id = prompt_id("ID a pesquisar: ");
         if (id == 0)
         {
            tui_write_info("Pesquisa cancelada.\n");
         }
         else
         {
            course *ptr = course_by_id(w->courses, w->count.courses, id);
            if (ptr != NULL)
            {
               tui_show_course(ptr);
            }
            else
            {
               tui_write_warning("ID não existe.\n");
            }
         }
      }
      break;

      // Pesquisar por nome
      case 1:
      {
         char name[STRMAX];
         prompt_string("Nome a pesquisar: ", name, STRMAX);
         uint32_t count = 0;
         course **list = list_course_by_name(&count, w->courses, w->count.courses, name);
         if (count > 0)
         {
            for (uint32_t i = 0; i < count; i++)
            {
               tui_write("RESULTADO #%" PRIu32 ":\n", i + 1);
               tui_show_course(list[i]);
            }
         }
         else
         {
            tui_write("Não existe nenhum curso que contém '%s'.\n", name);
         }
         free(list);
      }
      break;
      }
   } while (opt != size_menu(menu) - 1);
}

/* Criar novo curso */
void tui_set_course(struct world *w)
{
   tui_title("CRIAR CURSO");
   course new;

   /* Determinação do ID único */
   {
      course *ptr; // 'ptr' só é necessário aqui: bloco para delimitar scope
      do
      {

         new.id = prompt_id("ID único: ");
         if (new.id == 0)
            return;
         ptr = course_by_id(w->courses, w->count.courses, new.id);
         if (ptr != NULL)
         {
            tui_write("ID já existe, referente a '%s'.\n", ptr->name);
         }
      } while (ptr != NULL);
   }
   

   /* Determinação do nome do curso e das UCs associadas (via ID) */
   prompt_string("Nome do curso: ", new.name, STRMAX);

   tui_writeb("Quantas unidades curriculares tem o curso? ");
   scanf("%" SCNu32, &new.n_uc);
   clear_buffer(stdin);

   new.uc = malloc(new.n_uc * sizeof(unicurr *));
   {
      uint16_t id;
      for (uint32_t i = 0; i < new.n_uc; i++)
      {
         do
         {
            id = prompt_id("ID da UC: ");
            new.uc[i] = uc_by_id(w->uc, w->count.uc, id);
            if (new.uc[i] == NULL)
               tui_write_error("UC inexistente. Insira de novo.\n");
            else
               tui_write("UC '%s' adicionada.\n", new.uc[i]->name);
         } while (new.uc[i] == NULL);
      }
   }

   if ((w->courses = add_course(w->courses, &w->count.courses, &new)) == NULL)
   {
      tui_write_error("ERR::MEM -> Erro ao inserir o novo curso.\n");
   }
   else
   {
      tui_write_info("Curso criado com sucesso.\n");
   }
}

/* Eliminar um curso */
void tui_remove_course(struct world *w)
{
   tui_title("REMOVER CURSO");
   tui_writeb("Só é possível remover por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a eliminar: ");
   

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      course *ptr = course_by_id(w->courses, w->count.courses, id);
      if (ptr != NULL)
      {
         tui_writeb("Curso existente. Confirme os dados antes de eliminar.\n");
         tui_writeln_alert("Esta operação é IRREVERSÍVEL!");
         tui_show_course(ptr);

         // Deseja mesmo remover?
         char opt;
         do
         {
            tui_writeb("Deseja MESMO remover? (N/s) ");
            opt = fgetc(stdin);
            if (opt == '\n')
               opt = 'N';
            else
               opt = charupr(opt);
         } while ((opt != 'N') && (opt != 'S'));

         if (opt == 'S')
         {
            remove_course_p(ptr, &w->count.courses);
            tui_write_info("Curso eliminado com sucesso.\n");
         }
      }
      else
      {
         tui_write_warning("Curso não existe. A operação não vai prosseguir.\n");
      }
   }
}

/* Actualizar os dados de um curso. */
void tui_refresh_course(struct world *w)
{
   tui_title("ACTUALIZAR CURSO");
   tui_writeb("Só é possível actualizar por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a actualizar: ");

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      course *ptr = course_by_id(w->courses, w->count.courses, id);
      if (ptr != NULL)
      {
         course new = *ptr;
         tui_writeb("Curso existente. Confirme os dados antes de actualizar.\n");
         tui_writeb("Esta operação é reversível antes da confirmação final.\n");
         tui_writeln_alert("ATENÇÃO! A mudança de ID NÃO é permitida!");

         struct item menu[] = {
             {.text = "Actualizar nome", .func = NULL},
             {.text = "Adicionar unidade curricular", .func = NULL},
             {.text = "Remover unidade curricular", .func = NULL},
             {.text = "CONFIRMAR ALTERAÇÕES", .func = NULL},
             {.text = "CANCELAR", .func = NULL},
             {.text = NULL, .func = NULL}};

         int opt;
         do
         {
            println();
            show_menu(menu);
            exec_menu(w, menu, &opt, "Opção?");
            

            switch (opt)
            {
            // Actualizar nome
            case 0:
               prompt_string("Novo nome: ", new.name, STRMAX);
               break;

            // Adicionar UC
            case 1:
            {
               uint16_t uc_id = prompt_id("ID da UC: ");
               unicurr *uc_to_add = uc_by_id(w->uc, w->count.uc, id);
               if (uc_to_add != NULL)
               {
                  course_add_uc(&new, uc_to_add);
                  tui_write_info("UC adicionada.\n");
               }
               else
               {
                  tui_write_error("UC não existente.\n");
               }
            }
            break;

            // Remover UC
            case 2:
            {
               uint16_t uc_id = prompt_id("ID da UC: ");
               if (course_remove_uc(&new, uc_id))
                  tui_write_info("Removido com sucesso.\n");
               else
                  tui_write_error("ID não associado ao curso.\n");
            }
            break;

            // Confirmar alterações
            case 3:
               tui_writeb("A actualizar em memória...");
               refresh_course_p(ptr, &new);
               tui_write(" [OK]\n");
               break;

            // Cancelado
            case 4:
               tui_write_warning("Operação cancelada pelo utilizador.\n");
            }
         } while ((opt != 3) && (opt != 4));

         tui_writeb("Dados actualizados:\n");
         tui_show_course(ptr);
      }
      else
      {
         tui_write_error("Curso não existe. A operação não vai prosseguir.\n");
      }
   }
}

//------------------------------------------------------------------------------
// PROFESSORES
//------------------------------------------------------------------------------

/* Output de dados de um professor. */
void tui_show_professor(professor const *const ptr)
{
   if (ptr != NULL)
   {
      tui_write("\t  ID: %" PRIu16 "\n", ptr->id);
      tui_write("\tNome: %s\n", ptr->name);
      tui_write("\tLecciona %" PRIu32 " unidades curriculares:\n", ptr->n_uc);
      for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += (ptr->uc[i] != NULL), i++)
      {
         if (ptr->uc[i]->id == 0)
            continue;
         tui_show_uc(ptr->uc[i]);
      }
   }
}

/* Pesquisa de professores */
void tui_get_professor(struct world *w)
{
   struct item menu[] = {
       {.text = "Pesquisar por ID", .func = NULL},
       {.text = "Pesquisar por nome", .func = NULL},
       {.text = "VOLTAR", .func = NULL},
       {.text = NULL, .func = NULL}};

   int opt;
   do
   {
      println();
      tui_title("PESQUISA DE PROFESSORES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
      

      switch (opt)
      {
      // Pesquisar por ID
      case 0:
      {
         uint16_t id = prompt_id("ID a pesquisar: ");
         if (id == 0)
         {
            tui_write_info("Pesquisa cancelada.\n");
         }
         else
         {
            professor *ptr = professor_by_id(w->professors, w->count.professors, id);
            if (ptr != NULL)
            {
               tui_show_professor(ptr);
            }
            else
            {
               tui_write_warning("ID não existe.\n");
            }
         }
      }
      break;

      // Pesquisar por nome
      case 1:
      {
         char name[STRMAX];
         prompt_string("Nome: ", name, STRMAX);

         uint32_t count = 0;
         professor **list = list_professor_by_name(&count, w->professors, w->count.professors, name);
         if (count > 0)
         {
            for (uint32_t i = 0; i < count; i++)
            {
               tui_write("RESULTADO #%" PRIu32 ":\n", i + 1);
               tui_show_professor(list[i]);
            }
         }
         else
         {
            tui_write("Não existe nenhum professor que contém '%s'.\n", name);
         }
         free(list);
      }
      break;
      }
   } while (opt != size_menu(menu) - 1);
}

/* Criar novo professor */
void tui_set_professor(struct world *w)
{
   tui_title("CRIAR PROFESSOR");
   professor new;

   /* Determinação do ID único */
   {
      professor *ptr; // 'ptr' só é necessário aqui: bloco para delimitar scope
      do
      {
         new.id = prompt_id("ID único: ");
         if (new.id == 0)
            return;
         ptr = professor_by_id(w->professors, w->count.professors, new.id);
         if (ptr != NULL)
         {
            tui_write("ID já existe, referente a '%s'.\n", ptr->name);
         }
      } while (ptr != NULL);
   }
   

   /* Determinação do nome e das UCs leccionadas (via ID) */
   prompt_string("Nome do professor: ", new.name, STRMAX);

   tui_writeb("Quantas unidades curriculares lecciona? ");
   scanf("%" SCNu32, &new.n_uc);
   clear_buffer(stdin);

   new.uc = malloc(new.n_uc * sizeof(unicurr *));
   {
      uint16_t id;
      for (uint32_t i = 0; i < new.n_uc; i++)
      {
         do
         {
            id = prompt_id("ID da UC: ");
            new.uc[i] = uc_by_id(w->uc, w->count.uc, id);
            if (new.uc[i] == NULL)
               tui_write("UC inexistente. Insira de novo.\n");
            else
               tui_write("UC '%s' adicionada.\n", new.uc[i]->name);
         } while (new.uc[i] == NULL);
      }
   }

   if ((w->professors = add_professor(w->professors, &w->count.professors, &new)) == NULL)
   {
      tui_write_error("ERR::MEM -> Erro ao inserir o novo professor.\n");
   }
   else
   {
      tui_write_info("Professor adicionado com sucesso.\n");
   }
}

/* Eliminar um professor */
void tui_remove_professor(struct world *w)
{
   tui_title("REMOVER PROFESSOR");
   tui_writeb("Só é possível remover por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a eliminar: ");
   

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      professor *ptr = professor_by_id(w->professors, w->count.professors, id);
      if (ptr != NULL)
      {
         tui_writeb("Professor existente. Confirme os dados antes de eliminar.\n");
         tui_writeln_alert("Esta operação é IRREVERSÍVEL!");
         tui_show_professor(ptr);

         // Deseja mesmo remover?
         char opt;
         do
         {
            tui_writeb("Deseja MESMO remover? (N/s) ");
            opt = fgetc(stdin);
            if (opt == '\n')
               opt = 'N';
            else
               opt = charupr(opt);
         } while ((opt != 'N') && (opt != 'S'));

         if (opt == 'S')
         {
            remove_professor_p(ptr, &w->count.professors);
            tui_write_info("Professor eliminado com sucesso.\n");
         }
      }
      else
      {
         tui_write_error("Professor não existente. A operação não vai prosseguir.\n");
      }
   }
}

/* Actualizar os dados de um professor */
void tui_refresh_professor(struct world *w)
{
   tui_title("ACTUALIZAR PROFESSOR");
   tui_writeb("Só é possível actualizar por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a actualizar: ");

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      professor *ptr = professor_by_id(w->professors, w->count.professors, id);
      if (ptr != NULL)
      {
         professor new = *ptr;
         tui_writeb("Professor existente. Confirme os dados antes de actualizar.\n");
         tui_writeb("Esta operação é reversível antes da confirmação final.\n");
         tui_writeln_alert("ATENÇÃO! A mudança de ID NÃO é permitida!");

         struct item menu[] = {
             {.text = "Actualizar nome", .func = NULL},
             {.text = "Adicionar unidade curricular", .func = NULL},
             {.text = "Remover unidade curricular", .func = NULL},
             {.text = "CONFIRMAR ALTERAÇÕES", .func = NULL},
             {.text = "CANCELAR", .func = NULL},
             {.text = NULL, .func = NULL}};

         int opt;
         do
         {
            println();
            show_menu(menu);
            exec_menu(w, menu, &opt, "Opção?");
            

            switch (opt)
            {
            // Actualizar nome
            case 0:
               prompt_string("Novo nome: ", new.name, STRMAX);
               break;

            // Adicionar UC
            case 1:
            {
               uint16_t uc_id = prompt_id("ID da UC: ");
               unicurr *uc_to_add = uc_by_id(w->uc, w->count.uc, id);
               if (uc_to_add != NULL)
               {
                  professor_add_uc(&new, uc_to_add);
                  tui_write_info("UC adicionada.\n");
               }
               else
               {
                  tui_write_error("UC não existente.\n");
               }
            }
            break;

            // Remover UC
            case 2:
            {

               uint16_t uc_id = prompt_id("ID da UC: ");
               if (professor_remove_uc(&new, uc_id))
                  tui_write_info("Removido com sucesso.\n");
               else
                  tui_write_error("ID não associado ao professor.\n");
            }
            break;

            // Confirmar alterações
            case 3:
               tui_writeb("A actualizar em memória...");
               refresh_professor_p(ptr, &new);
               tui_write(" [OK]\n");
               break;

            // Cancelado
            case 4:
               tui_write_warning("Operação cancelada pelo utilizador.\n");
            }
         } while ((opt != 3) && (opt != 4));

         tui_writeb("Dados actualizados:\n");
         tui_show_professor(ptr);
      }
      else
      {
         tui_write_error("Professor não existe. A operação não vai prosseguir.\n");
      }
   }
}

//------------------------------------------------------------------------------
// ESTUDANTES
//------------------------------------------------------------------------------

/* Output de dados de um estudante. */
void tui_show_student(student const *const ptr)
{
   if (ptr != NULL)
   {
      tui_write("\t       ID: %" PRIu16 "\n", ptr->id);
      tui_write("\t     Nome: %s\n", ptr->name);
      /* TO-DO further ahead 
      {
         char date_s[11];
         datetostr(date_s, ptr->enrollment);
         tui_write("\tInscrição: %s\n", date_s);
      } */
      tui_write("\t    Curso:\n");
      tui_write("\t\t  ID: %" PRIu16 "\n", ptr->course_enrolled->id);
      tui_write("\t\tNome: %s\n", ptr->course_enrolled->name);
      tui_write("\tInscrito em %" PRIu32 " unidades curriculares:\n", ptr->n_uc);
      for (uint32_t i = 0, j = 0; j < ptr->n_uc; j += (ptr->eval[i].uc != NULL), i++)
      {
         if (ptr->eval[i].uc->id == 0)
            continue;
         tui_write("\t\t       ID: %" PRIu16 "\n", ptr->eval[i].uc->id);
         tui_write("\t\t     Nome: %s\n", ptr->eval[i].uc->name);
         tui_write("\t\tAvaliação: ");
         if (ptr->eval[i].attending)
         {
            tui_write("A frequentar...");
         }
         else
         {
            if (ptr->eval[i].evaluation > -1) // Se == -1 então UC foi removida
               tui_write("%" PRIi8 " valores.", ptr->eval[i].evaluation);
            else
               tui_write("N/A");
         }
      }
   }
}

/* Pesquisa de estudantes */
void tui_get_student(struct world *w)
{
   struct item menu[] = {
       {.text = "Pesquisar por ID", .func = NULL},
       {.text = "Pesquisar por nome", .func = NULL},
       {.text = "VOLTAR", .func = NULL},
       {.text = NULL, .func = NULL}};

   int opt;
   do
   {
      println();
      tui_title("PESQUISA DE ESTUDANTES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
      

      switch (opt)
      {
      // Pesquisar por ID
      case 0:
      {
         uint16_t id = prompt_id("ID a pesquisar: ");

         if (id == 0)
         {
            tui_write_info("Pesquisa cancelada.\n");
         }
         else
         {
            student *ptr = student_by_id(w->students, w->count.students, id);
            if (ptr != NULL)
            {
               tui_show_student(ptr);
            }
            else
            {
               tui_write_warning("ID não existe.\n");
            }
         }
      }
      break;

      // Pesquisar por nome
      case 1:
      {
         char name[STRMAX];
         prompt_string("Nome a pesquisar: ", name, STRMAX);

         uint32_t count = 0;
         student **list = list_student_by_name(&count, w->students, w->count.students, name);
         if (count > 0)
         {
            for (uint32_t i = 0; i < count; i++)
            {
               tui_write("RESULTADO #%" PRIu32 ":\n", i + 1);
               tui_show_student(list[i]);
            }
         }
         else
         {
            tui_write("Não existe nenhum estudante que contém '%s'.\n", name);
         }
         free(list);
      }
      break;
      }
   } while (opt != size_menu(menu) - 1);
}

/* Criação de novo estudante */
void tui_set_student(struct world *w)
{
   tui_title("CRIAR ESTUDANTE");
   student new;

   {
      student *ptr; // 'ptr' só é necessário aqui: bloco para delimitar scope
      do
      {
         tui_writeb("Definir ID único: ");
         scanf("%" SCNu16, &new.id);
         if (new.id == 0)
            return;
         ptr = student_by_id(w->students, w->count.students, new.id);
         if (ptr != NULL)
         {
            tui_write("ID já existe, referente a '%s'.\n", ptr->name);
         }
      } while (ptr != NULL);
   }
   

   /* Determinação do nome e do nº de UCs frequentadas */
   prompt_string("Nome do estudante: ", new.name, STRMAX);

   tui_writeb("Quantas unidades curriculares frequenta? ");
   scanf("%" SCNu32, &new.n_uc);
   clear_buffer(stdin);

   new.eval = malloc(new.n_uc * sizeof(assessment));
   {
      uint16_t id;
      for (uint32_t i = 0; i < new.n_uc; i++)
      {
         // Determinação da UC via ID
         do
         {
            id = prompt_id("ID da UC: ");
            new.eval[i].uc = uc_by_id(w->uc, w->count.uc, id);
            if (new.eval[i].uc == NULL)
               tui_write_warning("UC inexistente. Insira de novo.\n");
            else
               tui_write("UC '%s' adicionada.\n", new.eval[i].uc->name);
         } while (new.eval[i].uc == NULL);
         

         // Está a frequentar a UC?
         char yn;
         do
         {
            tui_writeb("Está a frequentar actualmente? (S/N) ");
            yn = charupr(fgetc(stdin));
         } while ((yn != 'S') && (yn != 'N'));

         new.eval[i].attending = (yn == 'S') ? true : false;

         // Se não, qual foi a nota que teve?
         if (!new.eval[i].attending)
         {
            do
            {
               tui_writeb("Que avaliação obteve? ");
               scanf("%" SCNi8, &new.eval[i].evaluation);
               if ((new.eval[i].evaluation > 20) || (new.eval[i].evaluation < 0))
                  tui_write_error("A avaliação deve estar entre 0 e 20.\n");
            } while ((new.eval[i].evaluation > 20) || (new.eval[i].evaluation < 0));
         }
         else
         {
            tui_write_info("A frequentar, avaliação definida a zero.\n");
            new.eval[i].evaluation = 0;
         }
      }

      /* Determinação do curso que frequenta */
      do
      {
         id = prompt_id("ID do curso em que se encontra inscrito: ");
         new.course_enrolled = course_by_id(w->courses, w->count.courses, id);
         if (new.course_enrolled == NULL)
            tui_write_warning("Curso inexistente. Insira de novo.\n");
         else
            tui_write("Inscrito no curso '%s'.\n", new.course_enrolled->name);
      } while (new.course_enrolled == NULL);
   }

   if ((w->students = add_student(w->students, &w->count.students, &new)) == NULL)
   {
      tui_write_error("ERR::MEM -> Erro ao inserir o novo estudante.\n");
   }
   else
   {
      tui_write_info("Estudante adicionado com sucesso.\n");
   }
}

/* Eliminar um estudante */
void tui_remove_student(struct world *w)
{
   tui_title("REMOVER ESTUDANTE");
   tui_writeb("Só é possível remover por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a eliminar: ");
   

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      student *ptr = student_by_id(w->students, w->count.students, id);
      if (ptr != NULL)
      {
         tui_writeb("Estudante existente. Confirme os dados antes de eliminar.\n");
         tui_writeln_alert("Esta operação é IRREVERSÍVEL!");
         tui_show_student(ptr);

         // Deseja mesmo remover?
         char opt;
         do
         {
            tui_writeb("Deseja MESMO remover? (N/s) ");
            opt = fgetc(stdin);
            if (opt == '\n')
               opt = 'N';
            else
               opt = charupr(opt);
         } while ((opt != 'N') && (opt != 'S'));

         if (opt == 'S')
         {
            remove_student_p(ptr, &w->count.students);
            tui_write_info("Estudante eliminado com sucesso.\n");
         }
      }
      else
      {
         tui_write_error("Estudante não existente. A operação não vai prosseguir.\n");
      }
   }
}

/* Actualizar os dados de um estudante */
void tui_refresh_student(struct world *w)
{
   tui_title("ACTUALIZAR ESTUDANTE");
   tui_writeb("Só é possível actualizar por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a actualizar: ");

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      student *ptr = student_by_id(w->students, w->count.students, id);
      if (ptr != NULL)
      {
         student new = *ptr;
         tui_writeb("Estudante existente. Confirme os dados antes de actualizar.\n");
         tui_writeb("Esta operação é reversível antes da confirmação final.\n");
         tui_writeln_alert("ATENÇÃO! A mudança de ID NÃO é permitida!");

         struct item menu[] = {
             {.text = "Actualizar nome", .func = NULL},
             {.text = "Adicionar unidade curricular", .func = NULL},
             {.text = "Remover unidade curricular", .func = NULL},
             {.text = "Actualizar avaliação", .func = NULL},
             {.text = "Mudar curso", .func = NULL},
             {.text = "CONFIRMAR ALTERAÇÕES", .func = NULL},
             {.text = "CANCELAR", .func = NULL},
             {.text = NULL, .func = NULL}};

         int opt;
         do
         {
            println();
            show_menu(menu);
            exec_menu(w, menu, &opt, "Opção?");
            

            switch (opt)
            {
            // Actualizar nome
            case 0:
               prompt_string("Novo nome: ", new.name, STRMAX);
               break;

            // Adicionar UC
            case 1:
            {
               uint16_t uc_id = prompt_id("ID da UC: ");
               

               unicurr *uc_to_add = uc_by_id(w->uc, w->count.uc, id);
               if (uc_to_add != NULL)
               {
                  assessment new_assess;
                  new_assess.uc = uc_to_add;

                  // Está a frequentar a UC?
                  char yn;
                  do
                  {
                     tui_writeb("Está a frequentar actualmente? ");
                     yn = charupr(fgetc(stdin));
                  } while ((yn != 'S') && (yn != 'N'));

                  new_assess.attending = (yn == 'S') ? true : false;

                  // Se não, qual foi a nota que teve?
                  if (!new_assess.attending)
                  {
                     do
                     {
                        tui_writeb("Que avaliação obteve? ");
                        scanf("%" SCNi8, &new_assess.evaluation);
                        if ((new_assess.evaluation > 20) || (new_assess.evaluation < 0))
                           tui_write_error("A avaliação deve estar entre 0 e 20.\n");
                     } while ((new_assess.evaluation > 20) || (new_assess.evaluation < 0));
                  }
                  else
                  {
                     tui_write_info("A frequentar, avaliação definida a zero.\n");
                     new_assess.evaluation = 0;
                  }

                  student_add_assessment(&new, &new_assess);
                  tui_write_info("Nova UC adicionada.\n");
               }
               else
               {
                  tui_write_error("UC não existente.\n");
               }
            }
            break;

            // Remover UC
            case 2:
            {
               uint16_t uc_id = prompt_id("ID da UC: ");
               if (student_remove_assessment(&new, uc_id))
                  tui_write_info("Removido com sucesso.\n");
               else
                  tui_write_error("ID não associado ao estudante.\n");
            }
            break;

            // Actualizar avaliação
            case 3:
            {
               uint16_t id = prompt_id("ID da UC: ");
               assessment *assess = assessment_by_id(&new, id);
               tui_writeb("Informação da UC:\n");
               tui_show_uc(assess->uc);
               tui_write("\t%s;\n", (assess->attending) ? "A frequentar" : "Não frequenta ou já terminou");
               tui_write("\tAvaliação: %" PRIi8 "\n", assess->evaluation);

               tui_writeb("    1 > Frequenta\n    2 > NÃO frequenta\nOutro > SALTAR\n");
               {
                  unsigned short yn;
                  scanf("%hu", &yn);

                  switch (yn)
                  {
                  case 1:
                     assess->attending = true;
                     break;

                  case 2:
                     assess->attending = false;
                     break;

                  default:
                     tui_write_info("Não mudou.\n");
                     break;
                  }
               }

               if (!assess->attending)
               {
                  do
                  {
                     tui_writeb("Que avaliação obteve? ");
                     scanf("%" SCNi8, &assess->evaluation);
                     if ((assess->evaluation > 20) || (assess->evaluation < 0))
                        tui_write_error("A avaliação deve estar entre 0 e 20.\n");
                  } while ((assess->evaluation > 20) || (assess->evaluation < 0));
               }
               else
               {
                  assess->evaluation = -1;
               }
            }
            break;

            // Actualizar curso inscrito
            case 4:
            {
               uint16_t id = prompt_id("ID do curso: ");
               course *c = course_by_id(w->courses, w->count.courses, id);
               if (c == NULL)
               {
                  tui_write_error("Curso inexistente. A operação não vai prosseguir.\n");
               }
               else
               {
                  new.course_enrolled = c;
               }
            }
            break;

            // Confirma alterações
            case 5:
               tui_writeb("A actualizar em memória...");
               refresh_student_p(ptr, &new);
               tui_write(" [OK]\n");
               break;

            // Cancela
            case 6:
               tui_write_info("Operação cancelada pelo utilizador.\n");
            }
         } while ((opt != 5) && (opt != 6));

         tui_writeb("Dados actualizados:\n");
         tui_show_student(ptr);
      }
      else
      {
         tui_write_error("Estudante não existe. A operação não vai prosseguir.\n");
      }
   }
}

//------------------------------------------------------------------------------
// UNIDADES CURRICULARES
//------------------------------------------------------------------------------

/* Output de dados de uma unidade curricular. */
void tui_show_uc(unicurr const *const ptr)
{
   if (ptr != NULL)
   {
      tui_write("\t      ID: %" PRIu16 "\n", ptr->id);
      tui_write("\t    Nome: %s\n", ptr->name);
      tui_write("\tSemestre: %" PRIu8 "\n", ptr->semester);
   }
}

/* Pesquisa de unidades curriculares */
void tui_get_uc(struct world *w)
{
   struct item menu[] = {
       {.text = "Pesquisar por ID", .func = NULL},
       {.text = "Pesquisar por nome", .func = NULL},
       {.text = "VOLTAR", .func = NULL},
       {.text = NULL, .func = NULL}};

   int opt;
   do
   {
      println();
      tui_title("PESQUISA DE UNIDADES CURRICULARES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
      

      switch (opt)
      {
      // Pesquisar por ID
      case 0:
      {
         uint16_t id = prompt_id("ID a pesquisar: ");

         if (id == 0)
         {
            tui_write_info("Pesquisa cancelada.\n");
         }
         else
         {
            unicurr *ptr = uc_by_id(w->uc, w->count.uc, id);
            if (ptr != NULL)
            {
               tui_show_uc(ptr);
            }
            else
            {
               tui_write_error("ID não existe.\n");
            }
         }
      }
      break;

      // Pesquisar por nome
      case 1:
      {
         char name[STRMAX];
         prompt_string("Nome a pesquisar: ", name, STRMAX);

         uint32_t count = 0;
         unicurr **list = list_uc_by_name(&count, w->uc, w->count.uc, name);
         if (count > 0)
         {
            for (uint32_t i = 0; i < count; i++)
            {
               tui_write("RESULTADO #%" PRIu32 ":\n", i + 1);
               tui_show_uc(list[i]);
            }
         }
         else
         {
            tui_write("Não existe nenhuma unidade curricular que contém '%s'.\n", name);
         }
         free(list);
      }
      break;
      }
   } while (opt != size_menu(menu) - 1);
}

/* Criar nova unidade curricular */
void tui_set_uc(struct world *w)
{
   tui_title("CRIAR UNIDADE CURRICULAR");
   unicurr new;

   /* Determinação do ID único */
   {
      unicurr *ptr; // 'ptr' só é necessário aqui: bloco para delimitar scope
      do
      {
         new.id = prompt_id("ID único: ");
         if (new.id == 0)
            return;
         ptr = uc_by_id(w->uc, w->count.uc, new.id);
         if (ptr != NULL)
         {
            tui_write("ID já existe, referente a '%s'.\n", ptr->name);
         }
      } while (ptr != NULL);
   }
   

   /* Determinação do nome da UC e do semestre */
   prompt_string("Nome da nova UC: ", new.name, STRMAX);

   tui_writeb("Semestre em que decorre? (1/2) ");
   do
   {
      scanf("%" SCNu8, &new.semester);

   } while (new.semester > 2);

   /* Dados determinados, dar ordem de inserção em memória. */
   if ((w->uc = add_uc(w->uc, &w->count.uc, &new)) == NULL)
   {
      tui_write_error("ERR::MEM -> Erro ao inserir a nova unidade curricular.\n");
   }
   else
   {
      tui_write_info("Unidade curricular criada com sucesso.\n");
   }
}

/* Remover unidade curricular */
void tui_remove_uc(struct world *w)
{
   tui_title("REMOVER UNIDADE CURRICULAR");
   tui_writeb("Só é possível remover por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a eliminar: ");
   

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      unicurr *ptr = uc_by_id(w->uc, w->count.uc, id);
      if (ptr != NULL)
      {
         tui_writeb("Unidade curricular existente. Confirme os dados antes de eliminar.\n");
         tui_writeln_alert("Esta operação é IRREVERSÍVEL!");
         tui_show_uc(ptr);

         // Deseja mesmo remover?
         char opt;
         do
         {
            tui_writeb("Deseja MESMO remover? (N/s) ");
            opt = fgetc(stdin);
            if (opt == '\n') // Enter define-se como um NÃO
               opt = 'N';
            else
               opt = charupr(opt);
         } while ((opt != 'N') && (opt != 'S'));

         if (opt == 'S')
         {
            remove_uc_p(ptr, &w->count.uc);
            tui_write_info("Unidade curricular eliminada com sucesso.\n");
         }
      }
      else
      {
         tui_write_error("Unidade curricular não existe. A operação não vai prosseguir.\n");
      }
   }
}

/* Actualizar os dados de uma unidade curricular. */
void tui_refresh_uc(struct world *w)
{
   tui_title("ACTUALIZAR UNIDADE CURRICULAR");
   tui_writeb("Só é possível actualizar por ID.\nSe não souber o ID, use a ferramenta de pesquisa.\n");
   tui_writeb("Insira 0 (zero) no ID para cancelar a operação.\n");

   uint16_t id = prompt_id("ID a actualizar: ");
   

   if (id == 0)
   {
      tui_write_warning("Operação cancelada pelo utilizador.\n");
   }
   else
   {
      unicurr *ptr = uc_by_id(w->uc, w->count.uc, id);
      if (ptr != NULL)
      {
         unicurr new = *ptr;
         tui_writeb("Unidade curricular existente. Confirme os dados antes de actualizar.\n");
         tui_writeb("Esta operação é reversível antes da confirmação final.\n");
         tui_writeln_alert("ATENÇÃO! A mudança de ID NÃO é permitida!");

         struct item menu[] = {
             {.text = "Actualizar nome", .func = NULL},
             {.text = "Actualizar semestre", .func = NULL},
             {.text = "CONFIRMAR ALTERAÇÕES", .func = NULL},
             {.text = "CANCELAR", .func = NULL},
             {.text = NULL, .func = NULL}};

         int opt;
         do
         {
            println();
            show_menu(menu);
            exec_menu(w, menu, &opt, "Opção?");
            

            switch (opt)
            {
            case 0:
               prompt_string("Novo nome: ", new.name, STRMAX);
               break;

            case 1:
               tui_writeb("Novo semestre? (1/2) ");
               do
               {
                  scanf("%" SCNu8, &new.semester);

               } while (new.semester > 2);
               break;

            case 2:
               tui_writeb("A actualizar em memória...");
               refresh_uc_p(ptr, &new);
               tui_write(" [OK]\n");
               break;

            case 3:
               tui_write_info("Operação cancelada pelo utilizador.\n");
            }
         } while ((opt != 2) && (opt != 3));

         tui_writeb("Dados actualizados:\n");
         tui_show_uc(ptr);
      }
      else
      {
         tui_write_error("Unidade curricular não existe. A operação não vai prosseguir.\n");
      }
   }
}
