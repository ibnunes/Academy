//------------------------------------------------------------------------------
//
// SISM SuperLite - Smart Interactive State Machine, SuperLite version
//
//    Copyright (C) 2018 Igor Cordeiro Bordalo Nunes (www.igornunes.com)
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
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DESCRIPTION:
// -----------
// A State Machine is particularly useful to know what is happening to the
// program at any given moment. The State Machine should neve directly interfere
// with the program execution, but its information may influence its flow and
// operations.
// SISM is a system devised for this purpose. Originally written with Object
// Pascal and not yet released to the public, its concepts are henceby the
// basis for this SuperLite version written in C originally for Academy.
//
// VERSION:
// ----------------------------
// SuperLite-C 1.0.0
//------------------------------------------------------------------------------


#include "sism.h"
#include <string.h>


/* Verifica se é indicado o modo de debug.
   Pode estar em qualquer posição, não verifica a sintaxe dos comandos. */
/*
int check_debug(int argc, char const **argv) {
   for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "--debug") == 0) {
         return 1;
      }
   }
   return 0;
}
*/

/* Method that starts SISM given the console arguments.
   This method must be adapted for each program specifically.*/
void start_app(SISM* machine, int argc, char const **argv) {
   /*
      Assumimos inicialmente que o programa vai correr no terminal.
      Caso haja argumentos em contrário, mudamos o Operation Mode em conformidade.
      Também se assume de início que o programa irá correr com sucesso.
   */

   /* Definição inicial das flags */
   machine->error = __ERROR_SUCCESS;
   machine->operation_mode = __OPERMODE_TERMINAL;
   machine->flow = __NEXT_CONTINUE;
   machine->io = 1;

   /* Interpretação dos argumentos

      SINTAXE:
      cmd:  app [<--tui | --gui>] [options]
      arg:   1          2             3

      - Se só for recebido 1 argumento, o programa deverá correr em modo TUI normal.
      - Se for recebido mais que 1 argumento, o 2º deve ser analisado:
         * Se for '--tui', o Operation Mode é definido como TUI alternativo;
         * Se for '--gui', o Operation Mode é definido como Graphical User Interface;
         * Se não for nenhum dos anteriores, é modo consola.
   */
   if (argc == 1) {
      machine->operation_mode = __OPERMODE_TUI;
   } else {
      if (strcmp(argv[1], "--tui") == 0) {
         machine->operation_mode = __OPERMODE_TUIPLUS;
      } else if (strcmp(argv[1], "--gui") == 0) {
         machine->operation_mode = __OPERMODE_GUI;
      } else {
         machine->operation_mode = __OPERMODE_TERMINAL;
      }
   }
}
