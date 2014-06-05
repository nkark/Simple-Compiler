#include "compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FORWARD DECLARATIONS
struct varNode * make_varNode(char *name, int value);
struct varNode * get_varNode(char *name);
struct printStatement * printStatement();
struct assignmentStatement * assignmentStatement();
struct ifStatement * ifStatement();
struct ifStatement * whileStatement();
struct statementNode * switchStatement();
struct statementNode * caseList();
struct statementNode * parse_case();
struct statementNode * statement();
struct statementNode * statementList();
struct statementNode * body();

struct varNode* id_list[50];	//array of var node pointers
struct varNode *switchID;		//ID for switch case
struct statementNode *no_opNode;


struct varNode * make_varNode(char *name, int value)
{
	struct varNode *resultNode = (struct varNode *)malloc(sizeof(struct varNode));
	resultNode->name = (char *)malloc(MAX_TOKEN_LENGTH);
	strcpy(resultNode->name,name);
	resultNode->value = value;
	
	return resultNode;
} 

struct varNode * get_varNode(char *name)
{
	int index = 0;
	while(id_list[index])
	{
		if(strcmp(id_list[index]->name,name)==0)
		{
			return id_list[index];
		}
		index++;
	}
	
	return NULL;
}

struct printStatement * printStatement()
{
	struct printStatement *printStmtNode = (struct printStatement *)malloc(sizeof(struct printStatement));
	ttype = getToken();
	if(ttype==ID)
	{
		struct varNode *varToPrint = get_varNode(token);
		printStmtNode->id = varToPrint;
		ttype = getToken();
		if(ttype==SEMICOLON)
		{
			return printStmtNode;
		}
		else
		{
			exit(1);
		}
	}
	else
	{
		printf("Expected ID for print statement.\n");
		exit(1);
	}
}

struct assignmentStatement * assignmentStatement()
{
	struct assignmentStatement *assignStmtNode = (struct assignmentStatement *)malloc(sizeof(struct assignmentStatement));
	
	//LEFT HAND SIDE
	if(ttype==ID)
	{
		struct varNode *lhs = get_varNode(token);
		if(lhs)
		{
			assignStmtNode->lhs = lhs;
		}
		else
		{
			printf("lhs is NULL.\n");
		}
	}
	else
	{
		printf("Expected ID in lhs of assignment statement.\n");
		exit(1);
	}
			
	//EQUAL
	ttype = getToken();
	if(ttype!=EQUAL)
	{
		printf("Expected EQUAL in assignment statement.\n");
		exit(1);
	}		
	
	//RIGHT HAND SIDE		
	ttype = getToken();
	if(ttype==NUM)
	{
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		assignStmtNode->op1 = tempNode;
	}
	else if(ttype==ID)
	{
		struct varNode *searchNode = get_varNode(token);
		assignStmtNode->op1 = searchNode;
	}
	else
	{
		printf("Expected NUM or ID for op1 of assignment.\n");
		exit(1);
	}
		

	//OP and OP2
	ttype = getToken();
	if(ttype==MINUS|ttype==PLUS|ttype==DIV|ttype==MULT)
	{
		assignStmtNode->op = ttype;
		ttype = getToken();
		if(ttype==NUM)
		{
			struct varNode *tempNode = make_varNode("NUM",atoi(token));
			assignStmtNode->op2 = tempNode;
		}
		else if(ttype==ID)
		{
			struct varNode *searchNode = get_varNode(token);
			assignStmtNode->op2 = searchNode;
		}
		else
		{
			printf("Expected NUM or ID for op2 of assignment.\n");
			exit(1);
		}
		
		ttype = getToken();
		if(ttype!=SEMICOLON)
		{
			printf("Expected SEMICOLON at end of assignment statement.\n");
			exit(1);
		}
	}
	else if(ttype==SEMICOLON)
	{
		assignStmtNode->op = 0;
	}
	else
	{
		printf("Expected SEMICOLON at end of assignment statement.\n");
		exit(1);
	}
	
	return assignStmtNode;
}

struct ifStatement * ifStatement()
{
	struct ifStatement *ifStmtNode = (struct ifStatement *)malloc(sizeof(struct ifStatement));

	//CONDITION - OP1
	ttype = getToken();
	if(ttype==NUM)
	{
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		ifStmtNode->op1 = tempNode;
	}
	else if(ttype==ID)
	{
		struct varNode *searchNode = get_varNode(token);
		ifStmtNode->op1 = searchNode;
	}
	else
	{
		printf("Expected NUM or ID in op1 condition of if Statement.\n");
		exit(1);
	}
	
	//CONDITION - OP
	ttype = getToken();
	if(ttype==GREATER|ttype==LESS|ttype==NOTEQUAL)
	{
		ifStmtNode->relop = ttype;
	}
	else
	{
		printf("Expected relop in condition of if Statement.\n");
		exit(1);
	}
	
	//CONDITION - OP2
	ttype = getToken();
	if(ttype==NUM)
	{
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		ifStmtNode->op2 = tempNode;
	}
	else if(ttype==ID)
	{
		struct varNode *searchNode = get_varNode(token);
		ifStmtNode->op2 = searchNode;
	}
	else
	{
		printf("Expected NUM or ID in op2 condition of if Statement.\n");
		exit(1);
	}
	
	struct statementNode * noopNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	noopNode->stmt_type = NOOPSTMT;
	
	//TRUE BRANCH
	ifStmtNode->true_branch = body();

	struct statementNode * tempNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	tempNode = ifStmtNode->true_branch;
	
	while(tempNode->next != NULL)
	{		
		tempNode = tempNode->next;
	}
	
	tempNode->next = noopNode;
	
	//FALSE BRANCH
	ifStmtNode->false_branch = noopNode;

	return ifStmtNode;
}

struct ifStatement * whileStatement()
{
	struct ifStatement *ifStmtNode = (struct ifStatement *)malloc(sizeof(struct ifStatement));

	//CONDITION - OP1
	ttype = getToken();
	if(ttype==NUM)
	{
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		ifStmtNode->op1 = tempNode;
	}
	else if(ttype==ID)
	{
		struct varNode *searchNode = get_varNode(token);
		ifStmtNode->op1 = searchNode;
	}
	else
	{
		printf("Expected NUM or ID in op1 condition of while Statement.\n");
		exit(1);
	}
	
	//CONDITION - OP
	ttype = getToken();
	if(ttype==GREATER|ttype==LESS|ttype==NOTEQUAL)
	{
		ifStmtNode->relop = ttype;
	}
	else
	{
		printf("Expected relop in condition of while Statement.\n");
		exit(1);
	}
	
	//CONDITION - OP2
	ttype = getToken();
	if(ttype==NUM)
	{
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		ifStmtNode->op2 = tempNode;
	}
	else if(ttype==ID)
	{
		struct varNode *searchNode = get_varNode(token);
		ifStmtNode->op2 = searchNode;
	}
	else
	{
		printf("Expected NUM or ID in op2 condition of while Statement.\n");
		exit(1);
	}
	
	struct statementNode * gt = (struct statementNode *)malloc(sizeof(struct statementNode));
	gt->stmt_type = GOTOSTMT;
	gt->next = NULL;
	
	//TRUE BRANCH
	ifStmtNode->true_branch = body();
	
	struct statementNode * tempNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	
	tempNode = ifStmtNode->true_branch;
	
	while(tempNode->next != NULL)
	{		
		tempNode = tempNode->next;
	}
	
	tempNode->next = gt;
	
	struct gotoStatement * gotoNode = (struct gotoStatement *)malloc(sizeof(struct gotoStatement));
	gt->goto_stmt = gotoNode;

	
	//FALSE BRANCH
	struct statementNode * noopNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	noopNode->stmt_type = NOOPSTMT;
	ifStmtNode->false_branch = noopNode;

	return ifStmtNode;
}

struct statementNode * switchStatement()
{
	no_opNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	no_opNode->stmt_type = NOOPSTMT;
	no_opNode->next = NULL;
	
	//SWITCH ID
	ttype = getToken();
	if(ttype == ID){
		switchID = get_varNode(token);
	}
	else if(ttype == NUM){
		struct varNode *tempNode = make_varNode("NUM",atoi(token));
		switchID = tempNode;
	}
	else{
		printf("Expected ID in switch statement.\n");
		exit(1);
	}
	
	struct statementNode * resultNode;
	ttype = getToken();
	if(ttype == LBRACE)
	{
		resultNode = caseList();
	}
	else
	{
		printf("Expected LBRACE in switch statement.\n");
		exit(1);
	}
	
	return resultNode;
}

struct statementNode * caseList()
{
	struct statementNode *caseNode = parse_case();
	ttype = getToken();
	if(ttype!=RBRACE)
	{
		ungetToken();
		caseNode->if_stmt->true_branch = caseList();		
		caseNode->next = caseNode->if_stmt->true_branch;
		return caseNode;
	}
	else
	{
		ungetToken();
		return caseNode;
	}
}

struct statementNode * parse_case()
{
	struct statementNode *resultNode = (struct statementNode *)malloc(sizeof(struct statementNode));
	resultNode->stmt_type = IFSTMT;
	
	struct ifStatement *caseNode = (struct ifStatement *)malloc(sizeof(struct ifStatement));
	
	caseNode->op1 = switchID;
	caseNode->relop = NOTEQUAL;
	
	ttype = getToken();
	if(ttype == CASE)
	{
		caseNode->true_branch = no_opNode;
		ttype = getToken();
		if(ttype == NUM)
		{	
			ttype = getToken();
			if(ttype == COLON)
			{
				struct varNode *caseNum = make_varNode("NUM",atoi(token));
			
				//store op2 into caseNode
				caseNode->op2 = caseNum;
			
				//false branch
				caseNode->false_branch = body();
			
				struct statementNode * tempNode;
			
				tempNode = caseNode->false_branch;
			
				struct statementNode * gotoStatementNode = (struct statementNode *)malloc(sizeof(struct statementNode));
				gotoStatementNode->stmt_type = GOTOSTMT;
	
				struct gotoStatement *gotoNode = (struct gotoStatement *)malloc(sizeof(struct gotoStatement));
				gotoNode->target = no_opNode;
			
				gotoStatementNode->goto_stmt = gotoNode;
			
				while(tempNode->next != NULL)
				{
					tempNode = tempNode->next;
				}
				tempNode->next = gotoStatementNode;
			}
			
		}
		else
		{
			printf("Expected NUM in case statement.\n");
			exit(1);
		}
	} 
	else
	{
		ttype = getToken();
		struct statementNode *tempNode = body();

		struct statementNode *iteratorNode = tempNode;
		while(iteratorNode->next != NULL)
		{
			iteratorNode = iteratorNode->next;
		}
		iteratorNode->next = no_opNode;
		
		return tempNode;
	}
	
	resultNode->if_stmt = caseNode;
	return resultNode;
}

struct statementNode * statement()
{
	struct statementNode * stmtNode = (struct statementNode *)malloc(sizeof(struct statementNode));

	ttype = getToken();

	switch (ttype)
	{
		case ID:
			//printf("assignment statement.\n");
			stmtNode->stmt_type = ASSIGNSTMT;
			stmtNode->assign_stmt = assignmentStatement();
			break;
		case PRINT:
			//printf("print statement.\n");
			stmtNode->stmt_type = PRINTSTMT;
			stmtNode->print_stmt = printStatement();
			break;
		case IF:
			//printf("if statement.\n");
			stmtNode->stmt_type = IFSTMT;
			stmtNode->if_stmt = ifStatement();
			stmtNode->next = stmtNode->if_stmt->false_branch;
			break;
		case WHILE:
			//printf("while statement.\n");
			stmtNode->stmt_type = IFSTMT;
			stmtNode->if_stmt = whileStatement();
			
			struct statementNode * tempNode = (struct statementNode *)malloc(sizeof(struct statementNode));
			tempNode = stmtNode->if_stmt->true_branch;
			
			while(tempNode->next != NULL)
			{
				tempNode = tempNode->next;
			}
			tempNode->goto_stmt->target = stmtNode;
			
			stmtNode->next = stmtNode->if_stmt->false_branch;
			break;
		case SWITCH:
			//printf("switch statement.\n");
			stmtNode = switchStatement();
			break;
		case RBRACE:
			ungetToken();
			stmtNode = NULL;
			break;
	}	

	return stmtNode;
}

struct statementNode * statementList()
{
	struct statementNode * stmtNode = statement();
	ttype = getToken();
	if(ttype!=RBRACE)
	{
		ungetToken();
		if(stmtNode->stmt_type==IFSTMT){
			struct statementNode * tempNode = (struct statementNode *)malloc(sizeof(struct statementNode));
			tempNode = stmtNode;
			
			while(tempNode->next != NULL)
			{
				tempNode = tempNode->next;
			}
			tempNode->next = statementList();
		}
		else{
			stmtNode->next = statementList();
		}
		return stmtNode;
	}
	else
	{
		ungetToken();
		return stmtNode;
	}
}

struct statementNode * body()
{
	ttype = getToken();
	struct statementNode * resultNode = statementList();
	ttype = getToken();
	if(ttype==RBRACE)
	{
		return resultNode;
	}
	else
	{
		exit(1);
	}
}

struct statementNode * parse_program_and_generate_intermediate_representation()
{	
	//VAR SECTION
	int index = 0;
	ttype = getToken();
	while(ttype!=SEMICOLON)
	{
		if(ttype==ID)
		{
			struct varNode *currVariable = make_varNode(token,0);
			id_list[index] = currVariable;
			index++;
		}
		ttype = getToken();
	}
		
	//BODY
	struct statementNode * result = body();
	//printf("Input was parsed.\n");
	return result;
	
}       

 