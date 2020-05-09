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
	//������β
	if (c == EOF)
		return std::make_tuple(TK_EOF, "");
	//����հ��������ַ�
	if (c == '\t' || c == ' ' || c == '\n' || c == '\r')
	{
		//ͨ��ѭ��ȥ���հ��ַ�
		while (c == '\t' || c == ' ' || c == '\n' || c == '\r')
		{
			//����ǻ��о����⴦��
			if (c == '\n')
			{
				line++;
				column = 0;
			}
			c = nextChar();
		}
		//������β����
		if (c == EOF)
			return std::make_tuple(TK_EOF, "");
	}

	//����ע��
	//�������һ��/Ȼ���ж���һ���ǲ���/�������ʾע�ͷ���
	if (c == '/' && peekChar() == '/')
	{
		//�����ڶ���/����
		nextChar();
		c = nextChar();
		//����ע������,��û�л��к��ļ���β��ʱ��˵���⻹��һ��ע���е�����
		while (c != '\n' && c != EOF)
		{
			c = nextChar();
		}
		return std::make_tuple(TK_NOTE, "note");
	}

	//�����������
	if (c >= '0' && c <= '9')
	{
		bool isDouble = false;
		std::string lexem;
		lexem += c;
		char c_next = peekChar();
		//�������ֵ�ʱ�������ȡ����
		while (c_next >= '0' && c_next <= '9')
		{
			c = nextChar();
			lexem += c;
			//�������С���㣬����С����,ǰ�ᵱǰ����������
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

	//���������ʶ�����ؼ���
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
	{
		char c_next;
		std::string lexem;
		lexem += c;
		c_next = peekChar();
		//���������ַ����»���
		while ((c_next >= 'a' && c_next <= 'z') || (c_next >= 'A' && c_next <= 'Z') || (c_next >= '0' && c_next <= '9') || c_next == '_')
		{
			c = nextChar();
			lexem += c;
			c_next = peekChar();
		}

		//����ǹؼ���
		if (keyWords.find(lexem) != keyWords.end())
			return std::make_tuple(keyWords[lexem], lexem);
		return std::make_tuple(TK_IDENT, lexem);
	}

	//�ַ�,����������
	if (c == '\'')
	{
		std::string lexem;
		//��ȡ�ַ�
		lexem += nextChar();
		if (peekChar() != '\'')
		{
			//cout << "error" << endl;
		}
		//�̵���һ��������
		c = nextChar();
		return std::make_tuple(TK_CHAR, lexem);
	}

	//�ַ���,����˫����
	if (c == '\"')
	{
		std::string lexem;
		char c_next = peekChar();
		while (c_next != '\"')
		{
			lexem += nextChar();
			c_next = peekChar();
		}

		//�̵�"��
		c = nextChar();
		return std::make_tuple(TK_STR, lexem);
	}

	//��������
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
	//���������+
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

	//���������-
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

	//���������*
	if (c == '*')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_TIMES_AGN, "*=");
		}
		return std::make_tuple(TK_TIMES, "*");
	}

	//���������*
	if (c == '/')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_DIV_AGN, "/=");
		}
		return std::make_tuple(TK_DIV, "/");
	}

	//���������%
	if (c == '%')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_MOD_AGN, "%=");
		}
		return std::make_tuple(TK_MOD, "%");
	}

	//���������~
	if (c == '`')
	{
		return std::make_tuple(TK_BITNOT, "~");
	}

	//��������=
	if (c == '=')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_EQ, "==");
		}
		return std::make_tuple(TK_ASSIGN, "=");
	}

	//�����������
	if (c == '!')
	{
		if (peekChar() == '!')
		{
			c = nextChar();
			return std::make_tuple(TK_NE, "!=");
		}
		return std::make_tuple(TK_LOGNOT, "!");
	}

	//���������|
	if (c == '|')
	{
		if (peekChar() == '|')
		{
			c = nextChar();
			return std::make_tuple(TK_LOGOR, "||");
		}
		return std::make_tuple(TK_BITOR, "|");
	}

	//���������&
	if (c == '&')
	{
		if (peekChar() == '&')
		{
			c = nextChar();
			return std::make_tuple(TK_LOGAND, "&&");
		}
		return std::make_tuple(TK_BITAND, "&");
	}

	//���������>
	if (c == '>')
	{
		if (peekChar() == '=')
		{
			c = nextChar();
			return std::make_tuple(TK_GE, ">=");
		}
		return std::make_tuple(TK_GT, ">");
	}

	//���������>
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
	//���һ��token
	tokenQueue.push_back(nextToken());
	//��ȡ�������һ��token�±�
	auto index = tokenQueue.begin();
	//ɾ����ͷtoken
	tokenQueue.erase(index);
	return tokenQueue[0];
}