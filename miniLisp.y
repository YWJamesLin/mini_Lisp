%{
#include "ast.h"
#include "astOP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
void yyerror (const char*);
%}
%union {
 ANode* TNode;
 ANode tmpNode;
}

%type <null> PROGRAM
%type <tmpNode> STMTS
%type <TNode> STMT
%type <TNode> PRINT_STMT
%type <tmpNode> EXPs
%type <TNode> EXP
%type <TNode> NUM_OP
%type <TNode> PLUS
%type <TNode> MINUS
%type <TNode> MULTIPLY
%type <TNode> DIVIDE
%type <TNode> MODULUS
%type <TNode> GREATER
%type <TNode> SMALLER
%type <TNode> EQUAL
%type <TNode> LOGICAL_OP
%type <TNode> AND_OP
%type <TNode> OR_OP
%type <TNode> NOT_OP
%type <tmpNode> DEF_STMTS
%type <TNode> DEF_STMT
%type <TNode> VARIABLE
%type <TNode> FUN_EXP
%type <tmpNode> FUN_IDs
%type <TNode> FUN_BODY
%type <TNode> FUN_CALL
%type <tmpNode> PARAMs
%type <TNode> PARAM
%type <TNode> FUN_NAME
%type <TNode> IF_EXP
%type <TNode> TEST_EXP
%type <TNode> THEN_EXP
%type <TNode> ELSE_EXP

%token <null> LP
%token <null> RP
%token <null> DEFINE
%token <null> IF
%token <null> PRINT_NUM
%token <null> PNUM
%token <null> PBOOL
%token <null> FUN

%token <null> PLU
%token <null> MIN
%token <null> MUL
%token <null> DIV
%token <null> MOD

%token <null> GRE
%token <null> SMA
%token <null> EQU

%token <null> AND
%token <null> OR
%token <null> NOT

%token <tmpNode> NUMBER
%token <tmpNode> ID
%token <tmpNode> BOOL_VAL

%%
PROGRAM : STMTS {
          struct Stack* stack = malloc (sizeof (struct Stack));
          stack -> size = 0;

          struct VarMapStack *vStack = malloc (sizeof (struct VarMapStack));
          vStack -> size = 0;
          vMapStackPush(vStack);

          int i;
          for(i = 0; i < $1.childSize; ++ i) {
            evaluateTree (stack, $1.child[i], vStack);
          }
          for(i = 0; i < $1.childSize; ++ i) {
            deleteTree ($1.child[i]);
          }

          vMapStackPop (vStack);
          free (vStack);
          free (stack);
        }
        ;
STMTS : STMTS STMT {
        $$.child[$$.childSize] = $2;
        ++ $$.childSize;
      }
      | STMT {
        $$.child[0] = $1;
        $$.childSize = 1;
      }
      ;
STMT : EXP {
       $$ = $1;
     }
     | DEF_STMT {
       $$ = $1;
     }
     | PRINT_STMT {
       $$ = $1;
     }
     ;
PRINT_STMT : LP PNUM EXP RP {
             $$ = malloc (sizeof (ANode));
             $$ -> type = 11;
             $$ -> child[0] = $3;
             $$ -> childSize = 1;
           }
           | LP PBOOL EXP RP {
             $$ = malloc (sizeof (ANode));
             $$ -> type = 12;
             $$ -> child[0] = $3;
             $$ -> childSize = 1;
           }
           ;
EXPs : EXPs EXP {
      $$.child[$$.childSize] = $2;
      ++ $$.childSize;
     }
     | EXP {
       $$.child[0] = $1;
       $$.childSize = 1;
     }
     ;
EXP : BOOL_VAL {
      $$ = malloc (sizeof (ANode));
      $$ -> type = 2;
      $$ -> val = $1.val;
      $$ -> child[0] = NULL;
      $$ -> childSize = 0;
    }
    | NUMBER {
      $$ = malloc (sizeof (ANode));
      $$ -> type = 0;
      $$ -> val = $1.val;
      $$ -> child[0] = NULL;
      $$ -> childSize = 0;
    }
    | VARIABLE {
      $$ = $1;
    }
    | NUM_OP {
      $$ = $1;
    }
    | LOGICAL_OP {
      $$ = $1;
    }
    | FUN_EXP {
      $$ = $1;
    }
    | FUN_CALL {
      $$ = $1;
    }
    | IF_EXP {
      $$ = $1;
    }
    ;

NUM_OP : PLUS {
         $$ = $1;
       }
       | MINUS {
         $$ = $1;
       }
       | MULTIPLY {
         $$ = $1;
       }
       | DIVIDE {
         $$ = $1;
       }
       | MODULUS {
         $$ = $1;
       }
       | GREATER {
         $$ = $1;
       }
       | SMALLER {
         $$ = $1;
       }
       | EQUAL {
         $$ = $1;
       }
       ;
PLUS : LP PLU EXPs EXP RP {
       $$ = malloc (sizeof(ANode));
       $$ -> type = 31;
       $$ -> childSize = 0;

       int i;
       for (i = 0; i < $3.childSize; ++ i) {
         $$ -> child[$$ -> childSize] = $3.child[i];
         ++ $$ -> childSize;
       }
       $$ -> child[$$ -> childSize] = $4;
       ++ $$ -> childSize;
     }
     ;
MINUS : LP MIN EXP EXP RP {
        $$ = malloc (sizeof(ANode));
        $$ -> type = 32;
        $$ -> childSize = 0;

        $$ -> child[$$ -> childSize] = $3;
        ++ $$ -> childSize;
        $$ -> child[$$ -> childSize] = $4;
        ++ $$ -> childSize;
      }
      ;
MULTIPLY : LP MUL EXPs EXP RP {
           $$ = malloc (sizeof(ANode));

           $$ -> type = 33;
           $$ -> childSize = 0;

           int i;
           for (i = 0; i < $3.childSize; ++ i) {
             $$ -> child[$$ -> childSize] = $3.child[i];
             ++ $$ -> childSize;
           }
           $$ -> child[$$ -> childSize] = $4;
           ++ $$ -> childSize;
         }
         ;
DIVIDE : LP DIV EXP EXP RP {
         $$ = malloc (sizeof(ANode));
         $$ -> type = 34;
         $$ -> childSize = 0;

         $$ -> child[$$ -> childSize] = $3;
         ++ $$ -> childSize;
         $$ -> child[$$ -> childSize] = $4;
         ++ $$ -> childSize;
       }
MODULUS : LP MOD EXP EXP RP {
          $$ = malloc (sizeof(ANode));
          $$ -> type = 35;
          $$ -> childSize = 0;

          $$ -> child[$$ -> childSize] = $3;
          ++ $$ -> childSize;
          $$ -> child[$$ -> childSize] = $4;
          ++ $$ -> childSize;
        }
        ;
GREATER : LP GRE EXP EXP RP {
          $$ = malloc (sizeof(ANode));
          $$ -> type = 36;
          $$ -> childSize = 0;

          $$ -> child[$$ -> childSize] = $3;
          ++ $$ -> childSize;
          $$ -> child[$$ -> childSize] = $4;
          ++ $$ -> childSize;
        }
        ;
SMALLER : LP SMA EXP EXP RP {
          $$ = malloc (sizeof(ANode));
          $$ -> type = 37;
          $$ -> childSize = 0;

          $$ -> child[$$ -> childSize] = $3;
          ++ $$ -> childSize;
          $$ -> child[$$ -> childSize] = $4;
          ++ $$ -> childSize;
        }
        ;
EQUAL : LP EQU EXP EXP RP {
        $$ = malloc (sizeof(ANode));
        $$ -> type = 38;
        $$ -> childSize = 0;

        $$ -> child[$$ -> childSize] = $3;
        ++ $$ -> childSize;
        $$ -> child[$$ -> childSize] = $4;
        ++ $$ -> childSize;
      }
      ;

LOGICAL_OP : AND_OP {
             $$ = $1;
           }
           | OR_OP {
             $$ = $1;
           }
           | NOT_OP {
             $$ = $1;
           }
           ;
AND_OP : LP AND EXPs EXP RP {
         $$ = malloc (sizeof(ANode));

         $$ -> type = 41;
         $$ -> childSize = 0;

         int i;
         for (i = 0; i < $3.childSize; ++ i) {
           $$ -> child[$$ -> childSize] = $3.child[i];
           ++ $$ -> childSize;
         }
         $$ -> child[$$ -> childSize] = $4;
         ++ $$ -> childSize;
       }
       ;
OR_OP : LP OR EXPs EXP RP {
        $$ = malloc (sizeof(ANode));

        $$ -> type = 42;
        $$ -> childSize = 0;

        int i;
        for (i = 0; i < $3.childSize; ++ i) {
          $$ -> child[$$ -> childSize] = $3.child[i];
          ++ $$ -> childSize;
        }
        $$ -> child[$$ -> childSize] = $4;
        ++ $$ -> childSize;
      }
      ;
NOT_OP : LP NOT EXP RP {
         $$ = malloc (sizeof(ANode));

         $$ -> type = 43;
         $$ -> childSize = 0;

         $$ -> child[$$ -> childSize] = $3;
         ++ $$ -> childSize;
       }
       ;

DEF_STMTS : DEF_STMTS DEF_STMT {
            $$ = $1;
            $$.child[$$.childSize] = $2;
            ++ $$.childSize;
          }
          | DEF_STMT {
            $$.child[0] = $1;
            $$.childSize = 1;
          }
          ;
DEF_STMT : LP DEFINE VARIABLE EXP RP {
           $$ = malloc (sizeof (ANode));
           $$ -> type = 60;
           $$ -> childSize = 2;
           $$ -> child[0] = $3;
           $$ -> child[1] = $4;
         }
         ;
VARIABLE : ID {
           $$ = malloc (sizeof (ANode));
           $$ -> type = 1;
           strcpy ($$ -> id, $1.id);
           $$ -> child[0] = NULL;
           $$ -> childSize = 0;
         }
         ;
FUN_EXP : LP FUN FUN_IDs FUN_BODY RP {
          int i;
          $$ = malloc (sizeof (ANode));
          $$ -> type = 70;
          $$ -> childSize = 2;

          if ( ! $3.childSize) {
            $$ -> child[0] = NULL;
          } else {
            $$ -> child[0] = malloc (sizeof(ANode));
            $$ -> child[0] -> type = 71;
            $$ -> child[0] -> childSize = $3.childSize;
            for (i = 0; i < $3.childSize; ++ i) {
              $$ -> child[0] -> child[i] = $3.child[i];
            }
          }
          
          $$ -> child[1] = $4;
        }
        ;
FUN_IDs : LP RP {
          $$.child[0] = NULL;
          $$.childSize = 0;
        }
        | LP FUN_IDs RP {
          $$ = $2;
        }
        | FUN_IDs ID {
          $$ = $1;
          $$.child[$$.childSize] = malloc (sizeof(ANode));
          $$.child[$$.childSize] -> type = 1;
          strcpy ($$.child[$$.childSize] -> id, $2.id);
          $$.child[$$.childSize] -> child[0] = NULL;
          $$.child[$$.childSize] -> childSize = 0;
          ++ $$.childSize;
        }
        | ID {
           $$.child[0] = malloc (sizeof(ANode));
           $$.child[0] -> type = 1;
           strcpy ($$.child[0] -> id, $1.id);
           $$.child[0] -> child[0] = NULL;
           $$.child[0] -> childSize = 0;
           $$.childSize = 1;
        }
        ;
FUN_BODY : EXP {
           $$ = malloc (sizeof (ANode));
           $$ -> type = 75;
           $$ -> childSize = 2;

           $$ -> child[0] = NULL;
           $$ -> child[1] = $1;
         }
         |DEF_STMTS EXP {
           int i;
           $$ = malloc (sizeof (ANode));
           $$ -> type = 75;
           $$ -> childSize = 2;

           $$ -> child[0] = malloc (sizeof (ANode));
           for (i = 0; i < $1.childSize; ++ i) {
             $$ -> child[0] -> child[i] = $1.child[i];
           }
           $$ -> child[0] -> type = 76;
           $$ -> child[0] -> childSize = $1.childSize;
 
           $$ -> child[1] = $2;
         }
         ;
FUN_CALL : LP FUN_EXP RP {
           $$ = malloc (sizeof (ANode));
           $$ -> type = 72;
           $$ -> childSize = 2;
           $$ -> child[0] = $2;
           $$ -> child[1] = NULL;
         }
         | LP FUN_EXP PARAMs RP {
           int i;
           $$ = malloc (sizeof (ANode));
           $$ -> type = 72;
           $$ -> child[0] = $2;
           $$ -> childSize = 2;

           $$ -> child[1] = malloc (sizeof (ANode));
           $$ -> child[1] -> type = 73;
           $$ -> child[1] -> childSize = $3.childSize;
           for (i = 0; i < $3.childSize; ++ i) {
             $$ -> child[1] -> child[i] = $3.child[i];
           }
         }
         | LP FUN_NAME RP {
           $$ = malloc (sizeof (ANode));
           $$ -> type = 74;
           $$ -> childSize = 2;
           $$ -> child[0] = $2;
           $$ -> child[1] = NULL;
         }
         | LP FUN_NAME PARAMs RP {
           int i;
           $$ = malloc (sizeof (ANode));
           $$ -> type = 74;
           $$ -> childSize = 2;
           $$ -> child[0] = $2;
           $$ -> child[1] = malloc (sizeof (ANode));
           $$ -> child[1] -> type = 73;
           $$ -> child[1] -> childSize = $3.childSize;
           for (i = 0; i < $3.childSize; ++ i) {
             $$ -> child[1] -> child[i] = $3.child[i];
           }
         }
         ;
PARAMs : PARAMs PARAM {
         $$ = $1;
         $$.child[$$.childSize] = $2;
         ++ $$.childSize;
       }
       | PARAM {
         $$.child[0] = $1;
         $$.childSize = 1;
       }
       ;
PARAM : EXP {
        $$ = $1;
      }
      ;
FUN_NAME : ID {
           $$ = malloc (sizeof(ANode));
           $$ -> type = 1;
           strcpy ($$ -> id, $1.id);
           $$ -> child[0] = NULL;
           $$ -> childSize = 0;
         }
         ;
IF_EXP : LP IF TEST_EXP THEN_EXP ELSE_EXP RP {
         $$ = malloc (sizeof (ANode));
         $$ -> type = 51;
         $$ -> childSize = 3;
         $$ -> child[0] = $3;
         $$ -> child[1] = $4;
         $$ -> child[2] = $5;
       }
       ;
TEST_EXP : EXP {
           $$ = $1;
         }
         ;
THEN_EXP : EXP {
           $$ = $1;
         }
         ;
ELSE_EXP : EXP {
           $$ = $1;
         }
         ;
%%

void yyerror (const char* message) {
  if (strcmp (message, "syntax error") == 0) {
    puts ("Syntax Error");
  } else {
    puts ("fuck");
  }
}

int main (int argc, char* argv[]) {
  yyparse ();
  return 0;
}
