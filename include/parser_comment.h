#ifndef PARSER_COMMENT_H_INCLUDED
#define PARSER_COMMENT_H_INCLUDED

char *AST_comment[]=
{
    "",
    "nil",
    "error",
    "stmt",
    "let",
    "if",
    "while",
    "for",
    "exp",
    "return",
    "continue",
    "break",
    "try",
    "int",
    "float",
    "str",
    "true",
    "false",
    "iden",
    "visit.",
    "=",
    "+",
    "-",
    "(-)",
    "**",
    "*",
    "/",
    "%",
    ">",
    ">=",
    "<",
    "<=",
    "!=",
    "not",
    "==",
    "||",
    "&&",
    "func",
    "def",
    "argv",
    "args",
    "end",
};

char *token_name[]=
{
    "t_error1",
    "t_error2",
    "int",
    "float",
    "=",
    ".",
    ":",
    "str",
    ",",
    ";",
    "\\n",
    "(",
    ")",
    "[",
    "]",
    "{",
    "}",
    "+",
    "-",
    "*",
    "/",
    "%",
    "**",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "=>",
    "||",
    "&&",
    "!",
    "lambda",
    "null",
    "let",
    "true",
    "false",
    "def",
    "return",
    "class",
    "continue",
    "break",
    "try",
    "catch",
    "throw",
    "if",
    "elif",
    "else",
    "while",
    "for",
    "do",
    "in",
    "iden",
    "file_end",
};

#endif // PARSER_COMMENT_H_INCLUDED
