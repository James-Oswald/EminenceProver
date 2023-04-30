/* This Bison file was machine-generated by BNFC */
%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include "Absyn.H"
typedef struct yy_buffer_state *YY_BUFFER_STATE;
int yyparse(void);
int yylex(void);
YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buf);
int yy_mylinenumber;
int initialize_lexer(FILE * inp);
int yywrap(void)
{
  return 1;
}
void yyerror(const char *str)
{
  extern char *yytext;
  fprintf(stderr,"error: line %d: %s at %s\n", 
    yy_mylinenumber, str, yytext);
}



static SExpr* YY_RESULT_SExpr_ = 0;
SExpr* pSExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_SExpr_;
  }
}
SExpr* pSExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_SExpr_;
  }
}

static ListSExpr* YY_RESULT_ListSExpr_ = 0;
ListSExpr* pListSExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListSExpr_;
  }
}
ListSExpr* pListSExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListSExpr_;
  }
}



%}

%union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  SExpr* sexpr_;
  ListSExpr* listsexpr_;
}

%token _ERROR_
%token _SYMB_0    //    
%token _SYMB_1    //   (
%token _SYMB_2    //   )

%type <sexpr_> SExpr
%type <listsexpr_> ListSExpr

%start SExpr
%token<string_> _STRING_
%token<int_> _INTEGER_
%token<string_> _IDENT_

%%
SExpr : _INTEGER_ {  $$ = new SNum($1); YY_RESULT_SExpr_= $$; } 
  | _STRING_ {  $$ = new SString($1); YY_RESULT_SExpr_= $$; }
  | _IDENT_ {  $$ = new SAtom($1); YY_RESULT_SExpr_= $$; }
  | _SYMB_1 ListSExpr _SYMB_2 {  $$ = new SCons($2); YY_RESULT_SExpr_= $$; }
;
ListSExpr : /* empty */ {  $$ = new ListSExpr(); YY_RESULT_ListSExpr_= $$; } 
  | ListSExpr SExpr _SYMB_0 {  $1->push_back($2) ; $$ = $1 ; YY_RESULT_ListSExpr_= $$; }
;

