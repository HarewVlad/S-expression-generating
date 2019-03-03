#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "utils.c"
#include "gen.c"

typedef enum TokenKind
{
	TOKEN_NONE,
	TOKEN_INT,
	TOKEN_CHAR,
	TOKEN_LSHIFT,
	TOKEN_RSHIFT,
} TokenKind;

typedef struct Token
{
	TokenKind kind;
	int int_val;
} Token;

char *stream;
Token token;

void next()
{
	repeat:
	token.kind = TOKEN_NONE;
	switch (*stream)
	{
		case ' ':
			stream++;
			goto repeat;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			int value = 0;
			while (isdigit(*stream))
			{
				value *= 10;
				value += *stream++ - '0';
			}
			token.int_val = value;
			token.kind = TOKEN_INT;
			break;
		default:
			token.kind = *stream++;
			break;
	}
}

bool is_token(TokenKind kind)
{
	return token.kind == kind;
}

bool match_token(TokenKind kind)
{
	if (is_token(kind))
	{
		next();
		return true;
	}
	else
		return false;
}

bool expect_token(TokenKind kind)
{
	if (is_token(kind))
	{
		next();
		return true;
	}
	else
		fatal("expect_token: error");
}

Expr *parse_expr();

Expr *parse_expr3()
{
	if (is_token(TOKEN_INT))
	{
		Expr *e = new_expr_int(token.int_val);
		next();
		return e;
	}
	else if (match_token('('))
	{
		Expr *e = parse_expr();
		expect_token(')');
		return e;
	}
	else
		fatal("parse_expr3: error");
}

Expr *parse_expr2()
{
	if (match_token('~'))
	{
		Expr *e = new_expr_negate(parse_expr3());
		return e;
	}
	else
	{
		Expr *e = parse_expr3();
		return e;
	}
}

Expr *parse_expr1()
{
	Expr *e = parse_expr2();
	if (is_token('*') || is_token('/'))
	{
		char op = token.kind;
		next();
		Expr *er = parse_expr2();
		e = new_expr_op(op, e, er);
	}
	return e;
}

Expr *parse_expr0()
{
	Expr *e = parse_expr1();
	if (is_token('+') || is_token('-'))
	{
		char op = token.kind;
		next();
		Expr *er = parse_expr1();
		e = new_expr_op(op, e, er);
	}
	return e;
}

Expr *parse_expr()
{
	return parse_expr0();
}

void init_stream(char *source)
{
	stream = source;
	next();
}

void test_sexpr(const char *source)
{
	init_stream(source);
	buf_push(list, parse_expr());
	dump_list();
}

int main(int argc, char **argv)
{
	if (argc < 2)
		fatal("main: error");
	test_sexpr(argv[1]);
}
