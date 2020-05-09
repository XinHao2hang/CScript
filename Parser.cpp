#include"Parser.h"
#include <sstream>
Parser::Parser() :
	keyWords({
		{"if",KW_IF},
		{"while",KW_WHILE},
		{"return",KW_RETURN},
		{"break",KW_BREAK},
		{"function",KW_FUNC},
		{"true",KW_TRUE},
		{"false",KW_FALSE},
		{"else",KW_ELSE},
		{"int",KW_INT},
		{"double",KW_FLOAT},
		{"bool",KW_BOOL},
		{"string",KW_STRING},
		{"char",KW_CHAR},
		{"using",KW_USING},
		{"extern",KW_EXTERN},
		{"library",KW_LIBRARY},
		{"string",KW_STRING},
		{"list",KW_LIST},
		{"class",KW_CLASS}
		})
{}
Parser::Parser(std::string fileName):
	keyWords({
		{"if",KW_IF},
		{"while",KW_WHILE},
		{"return",KW_RETURN},
		{"break",KW_BREAK},
		{"function",KW_FUNC},
		{"true",KW_TRUE},
		{"false",KW_FALSE},
		{"else",KW_ELSE},
		{"int",KW_INT},
		{"double",KW_FLOAT},
		{"using",KW_USING},
		{"extern",KW_EXTERN},
		{"library",KW_LIBRARY},
		{"string",KW_STRING},
		{"list",KW_LIST},
		{"class",KW_CLASS}
		})
{
	fs.open(fileName);
	//初始化token队列
	initTokenQueue(3);
}
void Parser::initTokenQueue(int n)
{
	while (n > 0)
	{
		n--;
		tokenQueue.push_back(nextToken());
	}
}
void Parser::parse(std::string fileName)
{
	//读取代码
	fs.open(fileName);
	initTokenQueue(3);
	//如果当前没有任何源码直接返回
	if (getToken() == TK_EOF)
		return;
	while (getToken() != TK_EOF)
	{
		stms.push_back(parseStatement());
	}
}
std::tuple<Token, std::string> Parser::nextToken()
{
	char c = nextChar();
	//遇到结尾
	if (c == EOF)
		return std::make_tuple(TK_EOF, "");
	//清除空白无意义字符
	if (c == '\t' || c == ' ' || c == '\n' || c == '\r')
	{
		//通过循环去掉空白字符
		while (c == '\t' || c == ' ' || c == '\n' || c == '\r')
		{
			//如果是换行就特殊处理
			if (c == '\n')
			{
				line++;
				column = 0;
			}
			c = nextChar();
		}
		//遇到结尾返回
		if (c == EOF)
			return std::make_tuple(TK_EOF, "");
	}

	//遇到注释
	//如果遇到一个/然后判断下一个是不是/若是则表示注释符号
	if (c == '/' && peekChar() == '/')
	{
		//跳过第二个/符号
		nextChar();
		c = nextChar();
		//跳过注释内容,当没有换行和文件结尾的时候，说明这还是一行注释中的内容
		while (c != '\n' && c != EOF)
		{
			c = nextChar();
		}
		return std::make_tuple(TK_NOTE, "note");
	}

	//如果遇到数字
	if (c >= '0' && c <= '9')
	{
		bool isFloat = false;
		std::string lexem;
		lexem += c;
		char c_next = peekChar();
		//遇到数字的时候继续获取数字
		while (c_next >= '0' && c_next <= '9' || (!isFloat&&c_next == '.'))
		{
			c = nextChar();
			lexem += c;
			//如果遇到小数点，处理小数点,前提当前数字是整数
			if (c == '.')
			{
				c = nextChar();
				lexem += c;
				isFloat = true;
			}
			c_next = peekChar();
		}
		return isFloat ? make_tuple(TK_FLOAT, lexem) : make_tuple(TK_INT, lexem);
	}

	//如果遇到标识符，关键字
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
	{
		char c_next;
		std::string lexem;
		lexem += c;
		c_next = peekChar();
		//包含数字字符和下划线
		while ((c_next >= 'a' && c_next <= 'z') || (c_next >= 'A' && c_next <= 'Z') || (c_next >= '0' && c_next <= '9') || c_next == '_')
		{
			c = nextChar();
			lexem += c;
			c_next = peekChar();
		}

		//如果是关键字
		if (keyWords.find(lexem) != keyWords.end())
			return std::make_tuple(keyWords[lexem], lexem);
		return std::make_tuple(TK_IDENT, lexem);
	}

	//字符,遇到单引号
	if (c == '\'')
	{
		std::string lexem;
		//获取字符
		lexem += nextChar();
		if (peekChar() != '\'')
		{
			//cout << "error" << endl;
		}
		//吞掉下一个单引号
		c = nextChar();
		return std::make_tuple(TK_CHAR, lexem);
	}

	//字符串,遇到双引号
	if (c == '\"')
	{
		std::string lexem;
		char c_next = peekChar();
		while (c_next != '\"')
		{
			lexem += nextChar();
			c_next = peekChar();
		}

		//吞掉"号
		c = nextChar();
		return std::make_tuple(TK_STR, lexem);
	}

	//遇到括号
	if (c == '[')
		return std::make_tuple(TK_LBRACKET, "[");
	if (c == ']')
		return std::make_tuple(TK_RBRACKET, "[");
	if (c == '(')
		return std::make_tuple(TK_LPAREN, "(");
	if (c == ')')
		return std::make_tuple(TK_RPAREN, ")");
	if (c == '{')
		return std::make_tuple(TK_LBRACE, "{");
	if (c == '}')
		return std::make_tuple(TK_RBRACE, "}");
	if (c == ',')
		return std::make_tuple(TK_COMMA, ",");
	if (c == ';')
		return std::make_tuple(TK_SEM, ";");
	if (c == '.')
		return std::make_tuple(TK_DOT, ".");
	if (c == ':')
		return std::make_tuple(TK_COLON, ":");
	//遇到运算符+
	if (c == '+')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_PLUS_AGN, "+=");
		}
		if (peekChar() == '+')
		{
			c = nextChar();
			return std::make_tuple(TK_PLUS_SELF, "++");
		}
		return std::make_tuple(TK_PLUS, "+");
	}

	//遇到运算符-
	if (c == '-')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_MINUS_AGN, "-=");
		}
		if (peekChar() == '+')
		{
			c = nextChar();
			return std::make_tuple(TK_MINUS_SELF, "--");
		}
		return std::make_tuple(TK_MINUS, "-");
	}

	//遇到运算符*
	if (c == '*')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_TIMES_AGN, "*=");
		}
		return std::make_tuple(TK_TIMES, "*");
	}

	//遇到运算符*
	if (c == '/')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_DIV_AGN, "/=");
		}
		return std::make_tuple(TK_DIV, "/");
	}

	//遇到运算符%
	if (c == '%')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_MOD_AGN, "%=");
		}
		return std::make_tuple(TK_MOD, "%");
	}

	//遇到运算符~
	if (c == '`')
	{
		return std::make_tuple(TK_BITNOT, "~");
	}

	//遇到符号=
	if (c == '=')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_EQ, "==");
		}
		return std::make_tuple(TK_ASSIGN, "=");
	}

	//遇到运算符！
	if (c == '!')
	{
		if (peekChar() == '!')
		{
			c = nextChar();
			return std::make_tuple(TK_NE, "!=");
		}
		return std::make_tuple(TK_LOGNOT, "!");
	}

	//遇到运算符|
	if (c == '|')
	{
		if (peekChar() == '|')
		{
			c = nextChar();
			return std::make_tuple(TK_LOGOR, "||");
		}
		return std::make_tuple(TK_BITOR, "|");
	}

	//遇到运算符&
	if (c == '&')
	{
		if (peekChar() == '&')
		{
			c = nextChar();
			return std::make_tuple(TK_LOGAND, "&&");
		}
		return std::make_tuple(TK_BITAND, "&");
	}

	//遇到运算符>
	if (c == '>')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_GE, ">=");
		}
		return std::make_tuple(TK_GT, ">");
	}

	//遇到运算符>
	if (c == '<')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_LE, "<=");
		}
		return std::make_tuple(TK_LT, "<");
	}
	return std::make_tuple(INVALID, "invalid");
}
std::tuple<Token, std::string> Parser::pushNextToken()
{
	//存放一个token
	tokenQueue.push_back(nextToken());
	//获取队列里第一个token下标
	auto index = tokenQueue.begin();
	//删除队头token
	tokenQueue.erase(index);
	return tokenQueue[0];
}


std::shared_ptr<Statement> Parser::parseStatement()
{
	std::shared_ptr<Statement> result;
	switch (getToken())
	{
	//声明语句
	case KW_BOOL:
	case KW_INT:
	case KW_CHAR:
	case KW_STRING:
	case KW_FLOAT:
		result = decalre();
		break;
	//表达式语句
	default:
		result = expression();
		break;
	}
	return result;
}

std::shared_ptr<Statement> Parser::decalre()
{
	return variableDeclare();
}

std::shared_ptr<VariableDeclareStatement> Parser::variableDeclare()
{
	auto result = std::make_shared<VariableDeclareStatement>(line,column);
	//获取类型，类型名字
	result->type = getToken();
	result->typeName = getLexeme();
	//吃掉类型token
	pushNextToken();
	//获取标识符
	result->identName = getLexeme();
	//吃掉标识符
	pushNextToken();
	return result;
}



std::shared_ptr<Expression> Parser::parseExpression()
{
	//检测一元表达式
	auto unary_result = parseUnaryExpr();
	//如果是赋值运算符
	Token token = getToken();
	if (token == TK_ASSIGN)
	{
		//判断左值是不是普通标识符
		if (typeid(*unary_result) != typeid(IdentifierExpression))
		{
			throw("error");
		}
		auto result = std::make_shared<AssignExpression>(line, column);
		//获取赋值符号
		result->opt = getToken();
		//获得左值
		result->left_value = unary_result;
		//吃掉赋值符号
		pushNextToken();
		//获得右值
		result->right_value = parseExpression();
		return result;
	}

	//检测二元运算符
	//如果是以下这些运算符
	while (token == TK_BITOR || token == TK_BITAND || token == TK_LOGAND || token == TK_LOGNOT ||
		token == TK_EQ || token == TK_NE || token == TK_GT || token == TK_GE ||
		token == TK_LT || token == TK_LE || token == TK_PLUS || token == TK_MINUS ||
		token == TK_MOD || token == TK_TIMES || token == TK_DIV || token == TK_LOGOR || token == TK_BITNOT)
	{
		//二元表达式
		auto result = std::make_shared<BinaryExpression>(line, column);
		//这里放之前的一元表达式
		result->left_expression = unary_result;
		//保存预算符
		result->opt = getToken();
		//下一个符号
		pushNextToken();
		//右分支也是表达式
		result->right_expression = parseExpression();
		//这里是为了让二元表达式可以连续连接
		unary_result = result;
		token = getToken();
	}
	return unary_result;
}

std::shared_ptr<Expression> Parser::parseUnaryExpr()
{
	//什么情况下是一元表达式
	Token token = getToken();
	//有符号前缀的情况下
	if (token == TK_MINUS || token == TK_LOGNOT || token == TK_BITNOT)
	{
		//创建结点
		auto result = std::make_shared<BinaryExpression>(line, column);
		//左表达式是空
		result->left_expression = nullptr;
		//获取运算符
		result->opt = token;
		//吃掉运算符
		pushNextToken();
		//右分支还是一元表达式，例如---1，!!!!x等，一元表达式运算符可以叠加
		result->right_expression = parseUnaryExpr();
		return result;
	}

	//一些不带符号的基础表达式也列为无符号一元表达式
	else if (token == TK_INT || token == TK_FLOAT || token == TK_STR || token == TK_CHAR
		|| token == TK_IDENT || token == KW_FALSE || token == KW_TRUE || token == TK_LPAREN || token == TK_LBRACE)
	{
		//返回基础表达式
		return parsePrimaryExpr();
	}
	return nullptr;
}

std::shared_ptr<Expression> Parser::parsePrimaryExpr()
{
	//如果是标识符
	if (getToken() == TK_IDENT)
	{
		//保留当前标识符名字
		std::string identName = getLexeme();
		//吃掉标识符
		pushNextToken();
		return std::make_shared<IdentifierExpression>(identName, line, column);
	}
	//如果是布尔
	else if (getToken() == KW_TRUE || getToken() == KW_FALSE)
	{
		//auto tVal = getCurrentLexeme();
		bool val = getLexeme() == "true" ? true : false;
		auto result = std::make_shared<BoolExpression>(val, line, column);
		pushNextToken();
		return result;
	}
	else if (getToken() == TK_INT)
	{
		//获取数字
		int val = atoi(getLexeme().c_str());
		//吃掉数字
		pushNextToken();
		return  std::make_shared<IntExpression>(val,line,column);
	}
	else if (getToken() == TK_STR)
	{
		//获取字符串
		std::string val = getLexeme();
		//吃掉字符串
		pushNextToken();
		return std::make_shared<StringExpression>(val, line, column);
	}
	else if (getToken() == TK_CHAR)
	{
		//获取字符
		char val = getLexeme()[0];
		//吃掉字符
		pushNextToken();
		return std::make_shared<CharExpression>(val, line, column);
	}
	else if (getToken() == TK_FLOAT)
	{
		std::istringstream in;
		//获取数字
		in.str(getLexeme());
		float val = 0;
		in >> val;
		//吃掉数字
		pushNextToken();
		return std::make_shared<FloatExpression>(val, line, column);
	}
	return nullptr;
}

std::shared_ptr<Statement> Parser::expression()
{
	//普通表达式
	auto expr = parseExpression();
	return std::make_shared<ExpressionStatement>(expr,line,column);
}