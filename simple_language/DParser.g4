parser grammar DParser;
options {
	tokenVocab = DLexer;
}

program: declarationseq? EOF;

primaryExpr:
	NumberLiteral
	| primaryExpr Plus primaryExpr
	| primaryExpr Minus primaryExpr
	| primaryExpr Slash primaryExpr
	| primaryExpr Star  primaryExpr
	| '(' primaryExpr ')'
	;

returnExpr:
	Return primaryExpr;

expr:
	returnExpr
	| primaryExpr;

exprSeq:
	expr+;

paramList:
	Identifier (Comma Identifier)+
	;

functionBody:
	exprSeq?;	

functionDeclaration:
	Def Identifier LeftParen paramList RightParen Begin functionBody End;

declaration:
	functionDeclaration
	;

declarationseq:
	declaration+
	;
