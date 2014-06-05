#ifndef _COMPILER_H_
#define _COMPILER_H_

/*
 * compiler.h
 */
 
#define TRUE 1
#define FALSE 0

#define NOOPSTMT    100
#define PRINTSTMT   101
#define ASSIGNSTMT  102
#define IFSTMT      103
#define GOTOSTMT    104

#define KEYWORDS    8
#define RESERVED    28
#define VAR         1
#define IF          2
#define WHILE       3 
#define SWITCH      4
#define CASE        5
#define DEFAULT     6
#define PRINT       7
#define ARRAY       8
#define PLUS        9
#define MINUS       10
#define DIV         11
#define MULT        12
#define EQUAL       13
#define COLON       14
#define COMMA       15
#define SEMICOLON   16
#define LBRAC       17
#define RBRAC       18
#define LPAREN      19
#define RPAREN      20
#define LBRACE      21
#define RBRACE      22
#define NOTEQUAL    23
#define GREATER     24
#define LESS        25
#define ID          26
#define NUM         27
#define ERROR       28

// This implementation does not allow tokens that are more
// than 100 characters long
#define MAX_TOKEN_LENGTH 100

// The following global variables are defined in compiler.c:
extern char token[MAX_TOKEN_LENGTH];
extern int  ttype;
extern int  tokenLength;

//---------------------------------------------------------
// Data structures:

struct varNode
{
    char* name;
    int   value;
};

struct gotoStatement
{
    struct statementNode * target;
};

struct assignmentStatement
{
    struct varNode * lhs;
    struct varNode * op1;
    struct varNode * op2;
    int op;  // PLUS, MINUS, MULT, DIV => lhs = op1 op op2;
             // 0 => lhs = op1;
};

struct printStatement
{
    struct varNode * id;
};

struct ifStatement
{
    struct varNode * op1;
    struct varNode * op2;
    int relop;   // GREATER, LESS, NOTEQUAL
    struct statementNode * true_branch;
    struct statementNode * false_branch;
};

struct statementNode
{
    int stmt_type;     // NOOPSTMT, PRINTSTMT, ASSIGNSTMT, IFSTMT, GOTOSTMT
    struct assignmentStatement * assign_stmt;  // NOT NULL iff stmt_type == ASSIGNSTMT
    struct printStatement      * print_stmt;   // NOT NULL iff stmt_type == PRINTSTMT
    struct ifStatement         * if_stmt;      // NOT NULL iff stmt_type == IFSTMT
    struct gotoStatement       * goto_stmt;    // NOT NULL iff stmt_type == GOTOSTMT
    struct statementNode       * next;         // next statement in the list or NULL 
};

//---------------------------------------------------------


void print_debug(const char * format, ...);
void ungetToken();
int  getToken();

 
 
struct statementNode * parse_program_and_generate_intermediate_representation();

#endif /* _COMPILER_H_ */
