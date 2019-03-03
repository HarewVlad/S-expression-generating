typedef enum ExprKind
{
	NONE,
	VAL,
	OP,
	NEGATE,
} ExprKind;

typedef struct Expr Expr;

struct Expr
{
	ExprKind kind;
	union
	{
		Expr *operand;
		int int_val;
		struct
		{
			char op;
			Expr *left;
			Expr *right;
		};
	};
};

Expr **list;

Expr *new_expr(ExprKind kind)
{
	Expr *e = malloc(sizeof(Expr));
	e->kind = kind;
	return e;
}

Expr *new_expr_int(int int_val)
{
	Expr *e = new_expr(VAL);
	e->int_val = int_val;
	return e;
}

Expr *new_expr_negate(Expr *operand)
{
	Expr *e = new_expr(NEGATE);
	e->operand = operand;
	return e;
}

Expr *new_expr_none()
{
	Expr *e = new_expr(NONE);
	return e;
}

Expr *new_expr_op(char op, Expr *left, Expr *right)
{
	Expr *e = new_expr(OP);
	e->op = op;
	e->left = left;
	e->right = right;
	return e;
}

void dump_expr(Expr *expr)
{
	switch (expr->kind)
	{
		case VAL:
			printf("%d", expr->int_val);
			break;
		case OP:
			printf("(%c", expr->op);
			printf(" ");
			dump_expr(expr->left);
			printf(" ");
			dump_expr(expr->right);
			printf(")");
			break;
		case NEGATE:
			printf("(~");
			printf(" ");
			dump_expr(expr->operand);
			printf(")");
			break;
		case NONE:
			break;
		default:
			fatal("dump_expr: error");
			break;
	}
}

void dump_list()
{
	for (int i = 0; i < buf_len(list); i++)
	{
		dump_expr(list[i]);
	}
}