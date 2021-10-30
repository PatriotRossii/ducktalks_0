lexer grammar DLexer;

NumberLiteral:
	DecimalInteger
	| FloatingLiteral
	;

DecimalLiteral:
	NONZERODIGIT DIGIT*
	;

Def: 'def';

LeftParen: '(';

RightParen: ')';

Begin: 'begin';

End: 'end';

Comma: ',';

Plus: '+';

Minus: '-';

Slash: '/';

Star: '*';

Return: 'return';

Identifier:
	NONDIGIT (NONDIGIT | DIGIT)*
	;

fragment NONDIGIT:
	[a-zA-Z_];

fragment DIGIT:
	[0-9];

fragment NONZERODIGIT:
	[1-9];

WHITESPACE: [ \t]+ -> skip;
NEWLINE: ('\r' '\n'? | '\n') -> skip;



