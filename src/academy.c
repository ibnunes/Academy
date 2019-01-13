//------------------------------------------------------------------------------
//
// Academy - An university manager
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
//  	Copyright (C) 2018 Universidade da Beira Interior (www.ubi.pt)
//
//  	This program is free software: you can redistribute it and/or modify
//  	it under the terms of the GNU General Public License as published by
//  	the Free Software Foundation, either version 3 of the License, or
//  	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	Contacts:
//			Igor Nunes <igor.bordalo.nunes@ubi.pt>
//			Abel Gomes <agomes@di.ubi.pt>
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DESCRIPTION:
// -----------
// This program manages the essential parts of an university: curricular units,
// courses, professors and students.
// All data is available through a binary file acting as a database which is
// read at startup and written at close.
//
// SYNOPSIS:
// --------
// 	academy [<--tui | --gui>] [options]
//
// where:
//		  --tui: Alternative text-based user interface
//		  --gui: Graphical user interface
//    options: arguments for the application, if applicable
//
// FILE FORMAT:
// -----------
//	Described at 'iostream.h'
//
// LANGUAGE AND CURRENT VERSION:
// ----------------------------
// Portuguese, version 0.2.0
//------------------------------------------------------------------------------


#include <stdio.h>
#include "types.h"
#include "interact.h"
#include "iostream.h"
#include "tui.h"
#include "sism.h"
#include "debug.h"

/* Nome do ficheiro binário de dados */
static const char* WORLD_FILE = "university.curr";

/* Menus para modo TUI clássico */
int main_tui(struct world*);
void tui_courses(struct world*);
void tui_ucs(struct world*);
void tui_students(struct world*);
void tui_professors(struct world*);
void tui_search(struct world*);
void tui_about(void);


int main(int argc, char const *argv[]) {
   /* 1. Inicialização em memória RAM */
   struct world w;
   init_world(&w);
   start_app(&w.status, argc, argv);
   debug("Operation mode: %X.\n", w.status.operation_mode);

   /* 2. Carregamento de dados a partir da memória permanente */
   w.status.io = loadfromfile(&w, WORLD_FILE);
   debug("loadfromfile returned %X.\n", w.status.io);

   /* 3. Execução do programa conforme o Operation Mode */
   switch (w.status.operation_mode) {
      case __OPERMODE_TUI:
         main_tui(&w);
         break;
      default:
         tui_write("MODE NOT AVAILABLE\n");
         fprintf(stderr, "Mode not available");
   }

   /* 4. Guardar em disco as alterações efectuadas */
   refresh_world(&w);
   w.status.io = savetofile(&w, WORLD_FILE);
   debug("savetofile returned %X.\n", w.status.io);

   /* 5. Libertar recursos e encerrar */
   free_world(&w);
   debug("Returning %X to the OS.\n", w.status.error);
   return w.status.error;
}


//------------------------------------------------------------------------------
// TUI - TEXT-BASED USER INTERFACE
//    Todos os métodos associados à interface TUI têm a numeclatura 'tui_*'.
//    Excepção: 'main_tui' - o método 'main' associado a uma interface tem a
//    numeclatura 'main_<interface>'.
//------------------------------------------------------------------------------

/* TUI - 'main'
   Menu principal da interface TUI */
int main_tui(struct world *w) {
   struct item menu[] = {
      {.text = "Gerir unidades curriculares", .func = tui_ucs},
      {.text = "Gerir cursos", .func = tui_courses},
      {.text = "Gerir professores", .func = tui_professors},
      {.text = "Gerir alunos", .func = tui_students},
      {.text = "Pesquisa", .func = tui_search},
      {.text = "SOBRE", .func = NULL},
      {.text = "SAIR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("MENU PRINCIPAL");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
      if (opt == size_menu(menu) - 2)
         tui_about();
   } while (opt != size_menu(menu) - 1);

   return w->status.error;
}

/* TUI - menu de gestão de cursos */
void tui_courses(struct world *w) {
   struct item menu[] = {
      {.text = "Inserir", .func = tui_set_course},
      {.text = "Actualizar", .func = tui_refresh_course},
      {.text = "Eliminar", .func = tui_remove_course},
      {.text = "VOLTAR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("CURSOS");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
   } while (opt != size_menu(menu) - 1);
}

/* TUI - menu de gestão de unidades curriculares */
void tui_ucs(struct world *w) {
   struct item menu[] = {
      {.text = "Inserir", .func = tui_set_uc},
      {.text = "Actualizar", .func = tui_refresh_uc},
      {.text = "Eliminar", .func = tui_remove_uc},
      {.text = "VOLTAR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("UNIDADES CURRICULARES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
   } while (opt != size_menu(menu) - 1);
}

/* TUI - menu de gestão de estudantes */
void tui_students(struct world *w) {
   struct item menu[] = {
      {.text = "Inserir", .func = tui_set_student},
      {.text = "Actualizar", .func = tui_refresh_student},
      {.text = "Eliminar", .func = tui_remove_student},
      {.text = "VOLTAR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("ESTUDANTES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
   } while (opt != size_menu(menu) - 1);
}

/* TUI - menu de gestão de professores */
void tui_professors(struct world *w) {
   struct item menu[] = {
      {.text = "Inserir", .func = tui_set_professor},
      {.text = "Actualizar", .func = tui_refresh_professor},
      {.text = "Eliminar", .func = tui_remove_professor},
      {.text = "VOLTAR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("PROFESSORES");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
   } while (opt != size_menu(menu) - 1);
}

/* TUI - menu de pesquisa de dados */
void tui_search(struct world *w) {
   struct item menu[] = {
      {.text = "Consultar unidades curriculares", .func = tui_get_uc},
      {.text = "Consultar cursos", .func = tui_get_course},
      {.text = "Consultar professores", .func = tui_get_professor},
      {.text = "Consultar estudantes", .func = tui_get_student},
      {.text = "VOLTAR", .func = NULL},
      {.text = NULL, .func = NULL}
   };

   int opt;
   do {
      title_menu("PESQUISA");
      show_menu(menu);
      exec_menu(w, menu, &opt, "Opção?");
   } while (opt != size_menu(menu) - 1);
}

/* Acerca do programa.
   Não é colocado directamente na struct item pois não deve receber a estrutura WORLD. */
void tui_about(void) {
   tui_write("\n\n");
   tui_write("        ACADEMY        \n");
   tui_write(    " An university manager \n\n");
   tui_write("Version: 0.2.0\n");
   tui_write("   Date: %s %s\n", __DATE__, __TIME__);
   tui_write(" Author: Igor Cordeiro Bordalo Nunes\n");
   tui_write(      "         igor.bordalo.nunes@ubi.pt\n");
   tui_write(      "         Universidade da Beira Interior (www.ubi.pt)\n");
   tui_write("\nThis program is free software: you can redistribute it and/or modify it under\nthe terms of the GNU General Public License as published by the Free Software\nFoundation, either version 3 of the License, or (at your option) any later\nversion.\n");
   tui_write("\n\n");
}
