#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include<vector>
#include<string>


class SExpr;
class ListSExpr;

typedef union
{
  int int_;
  char char_;
  double double_;
  char* string_;
  SExpr* sexpr_;
  ListSExpr* listsexpr_;
} YYSTYPE;

SExpr* pSExpr(FILE *inp);
SExpr* pSExpr(const char *str);
ListSExpr* pListSExpr(FILE *inp);
ListSExpr* pListSExpr(const char *str);



#define _ERROR_ 258
#define _SYMB_0 259
#define _SYMB_1 260
#define _SYMB_2 261
#define _STRING_ 262
#define _INTEGER_ 263
#define _IDENT_ 264

extern YYSTYPE yylval;

#endif
