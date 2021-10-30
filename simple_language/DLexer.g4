lexer grammar DLexer;

NumberLiteral:
	DecimalLiteral
	| FloatingLiteral
	;

DecimalLiteral:
	NONZERODIGIT DIGIT*
	;

FloatingLiteral:
	Fractionalconstant Exponentpart?
	| Digitsequence Exponentpart
	;

fragment Fractionalconstant:
	Digitsequence? '.' Digitsequence
	| Digitsequence '.'
	;

fragment Exponentpart:
	('e' | 'E') SIGN? Digitsequence;

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

fragment Digitsequence: DIGIT DIGIT*;

fragment NONDIGIT:
	[a-zA-Z_];

fragment SIGN:
	[+-];

fragment DIGIT:
	[0-9];

fragment NONZERODIGIT:
	[1-9];

WHITESPACE: [ \t]+ -> skip;
NEWLINE: ('\r' '\n'? | '\n') -> skip;



