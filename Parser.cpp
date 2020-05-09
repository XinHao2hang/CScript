#include"Parser.h"
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
		{"double",KW_DOUBLE},
		{"using",KW_USING},
		{"extern",KW_EXTERN},
		{"library",KW_LIBRARY},
		{"string",KW_STRING},
		{"list",KW_LIST},
		{"class",KW_CLASS}
		})
{
	fs.open(fileName);
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
		bool isDouble = false;
		std::string lexem;
		lexem += c;
		char c_next = peekChar();
		//遇到数字的时候继续获取数字
		while (c_next >= '0' && c_next <= '9')
		{
			c = nextChar();
			lexem += c;
			//如果遇到小数点，处理小数点,前提当前数字是整数
			if (isDouble == false && peekChar() == '.')
			{
				c = nextChar();
				lexem += c;
				isDouble = true;
			}
			c_next = peekChar();
		}
		return isDouble ? make_tuple(TK_DOUBLE, lexem) : make_tuple(TK_INT, lexem);
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