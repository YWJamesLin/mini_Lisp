%{
#include "ast.h"
#include "y.tab.h"
#include <string.h>
%}
separator [\t\n\r ]
letter [a-z]
digit [0-9]
%%
{separator} {}

\( { return LP; }
\) { return RP; }
\+ { return PLU; }
\- { return MIN; }
\* { return MUL; }
\/ { return DIV; }
mod { return MOD; }
\> { return GRE; }
\< { return SMA; }
\= { return EQU; }
and { return AND; }
or { return OR; }
not { return NOT; }
if { return IF; }
define { return DEFINE; }
print-num { return PNUM; }
print-bool { return PBOOL; }
fun { return FUN; }

(0|[1-9]{digit}*|-[1-9]{digit}*) {
  yylval.tmpNode.type = 0;
  yylval.tmpNode.val = atoi (yytext);
  return NUMBER;
}
{letter}({letter}|{digit}|-)* {
  yylval.tmpNode.type = 1;
  yylval.tmpNode.id = strdup (yytext);
  return ID;
}
(#t|#f) {
  if (strcmp (yytext, "#t") == 0) {
    yylval.tmpNode.type = 2;
    yylval.tmpNode.val = 1;
  } else {
    yylval.tmpNode.type = 2;
    yylval.tmpNode.val = 0;
  }
  return BOOL_VAL;
}
%%
