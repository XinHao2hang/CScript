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
	//��ʼ��token����
	initTokenQueue(3);
}
void Parser::initTokenQueue(int n)
{
	while (n > 0)
	{
		n--;
		tokenQueue.push_back(nextToken());
		lines.push(line);
		columns.push(column);
	}
}
void Parser::parse(std::string fileName)
{
	//��ȡ����
	fs.open(fileName);
	initTokenQueue(3);
	//�����ǰû���κ�Դ��ֱ�ӷ���
	if (getToken() == TK_EOF)
		return;
	while (getToken() != TK_EOF)
	{
		auto statement = parseStatement();
		stms.push_back(statement);
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
		bool isFloat = false;
		std::string lexem;
		lexem += c;
		char c_next = peekChar();
		//�������ֵ�ʱ�������ȡ����
		while (c_next >= '0' && c_next <= '9' || (!isFloat&&c_next == '.'))
		{
			c = nextChar();
			lexem += c;
			//�������С���㣬����С����,ǰ�ᵱǰ����������
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
		return std::make_tuple(TK_RBRACKET, "]");
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
	lines.push(line);
	columns.push(column);
	//��ȡ�������һ��token�±�
	auto index = tokenQueue.begin();
	//ɾ����ͷtoken
	tokenQueue.erase(index);
	lines.pop();
	columns.pop();
	return tokenQueue[0];
}

std::shared_ptr<Statement> Parser::parseStatement()
{
	std::shared_ptr<Statement> result;
	switch (getToken())
	{
	//�������
	case KW_BOOL:
	case KW_INT:
	case KW_CHAR:
	case KW_STRING:
	case KW_FLOAT:
		result = decalre();
		//���ֺ�
		semicolon();
		break;
	//���ʽ���
	default:
		result = expression();
		//���ֺ�
		semicolon();
		break;
	}
	return result;
}

std::shared_ptr<Statement> Parser::decalre()
{
	if (getToken(2) == TK_LBRACKET)
		return arrayDeclare();
	else
		return variableDeclare();
}

std::shared_ptr<VariableDeclareStatement> Parser::variableDeclare()
{
	auto result = std::make_shared<VariableDeclareStatement>(line,column);
	//��ȡ���ͣ���������
	result->type = getToken();
	result->typeName = getLexeme();
	//�Ե�����token
	pushNextToken();
	//��ȡ��ʶ��
	//��������������ǷǱ�ʶ����Ҫ����
	result->ident = parseExpression();
	if(typeid(*(result->ident)) != typeid(IdentifierExpression) && typeid(*(result->ident)) != typeid(AssignExpression))
		error(BAD_IDENT,lines.front(),columns.front());
	return result;
}

std::shared_ptr<ArrayDeclareStatement> Parser::arrayDeclare()
{
	auto result = std::make_shared<ArrayDeclareStatement>(line,column);
	//��ȡ���ͣ�������
	result->type = getToken();
	result->typeName = getLexeme();
	//�Ե�����
	pushNextToken();
	//��ȡ������
	result->name = getLexeme();
	//�Ե�������
	pushNextToken();
	//��ȡ��ά����
	while (getToken() == TK_LBRACKET)
	{
		//�Ե�������
		pushNextToken();
		//����±��Ƿ���Ϲ���Ҫ���±�����������
		std::shared_ptr<Expression> index = parseExpression();
		if (!index)
		{
			//���û���±�Ԫ���򱨴�
			error(MISSING_INDEX, lines.front(), columns.front());
		}
		else
		{
			if (typeid(*index) == typeid(IntExpression))
			{
				result->elementNums.push_back(index);
			}
			else
			{
				//����Ҫ����
				error(ARRAY_DEC_ELEMENT, lines.front(), columns.front());
			}
		}
		//�Ե�������
		if (getToken() != TK_RBRACKET)
			error(MISSING_BARCKET, lines.front(), columns.front());
		else
			pushNextToken();
	}
	//����и�ֵ
	if (getToken() == TK_ASSIGN)
	{
		//�Ե���ֵ����
		pushNextToken();
		//��ȡ��ֵ
		result->initValue = parseExpression();
	}
	return result;
}

void Parser::semicolon()
{
	//����Ƿ�Ϊ�ֺ�
	if (getToken() == TK_SEM)
	{
		//�Ե��ֺ�
		while(getToken() == TK_SEM)
			pushNextToken();
	}
	else
	{
		//����ȱ�ٷֺ�
		error(MISSING_SEM,lines.front(),columns.front());
	}
}

std::shared_ptr<Expression> Parser::parseExpression()
{
	//���һԪ���ʽ
	auto unary_result = parseUnaryExpr();
	//����Ƿ����ʽֱ�ӷ���
	if (!unary_result)
		return unary_result;
	//����Ǹ�ֵ�����
	Token token = getToken();
	if (token == TK_ASSIGN)
	{
		//�ж���ֵ�ǲ�����ͨ��ʶ��
		if (typeid(*unary_result) != typeid(IdentifierExpression))
		{
			//���ǿɸ�ֵ����
			error(NOT_LEFT_VALUE,lines.front(),columns.front());
		}
		auto result = std::make_shared<AssignExpression>(line, column);
		//��ȡ��ֵ����
		result->opt = getToken();
		//�����ֵ
		result->left_value = unary_result;
		//�Ե���ֵ����
		pushNextToken();
		//�����ֵ
		result->right_value = parseExpression();
		return result;
	}
	//����Ԫ�����
	//�����������Щ�����
	while (token == TK_BITOR || token == TK_BITAND || token == TK_LOGAND || token == TK_LOGNOT ||
		token == TK_EQ || token == TK_NE || token == TK_GT || token == TK_GE ||
		token == TK_LT || token == TK_LE || token == TK_PLUS || token == TK_MINUS ||
		token == TK_MOD || token == TK_TIMES || token == TK_DIV || token == TK_LOGOR || token == TK_BITNOT)
	{
		//��Ԫ���ʽ
		auto result = std::make_shared<BinaryExpression>(line, column);
		//�����֮ǰ��һԪ���ʽ
		result->left_expression = unary_result;
		//����Ԥ���
		result->opt = getToken();
		//��һ������
		pushNextToken();
		//�ҷ�֧Ҳ�Ǳ��ʽ
		result->right_expression = parseExpression();
		//������Ϊ���ö�Ԫ���ʽ������������
		unary_result = result;
		token = getToken();
	}
	//��ʽ��Ԫ����
	if (token == TK_LBRACKET)
	{
		
		//��Ԫ���ʽ
		auto result = std::make_shared<AddressingExpression>(line, column);
		//�����֮ǰ��һԪ���ʽ��û���򱨴�
		if (!unary_result)
			error(MISSING_ARRAY_NAME, line - 1, column-3);
		result->base = unary_result;
		while (getToken() == TK_LBRACKET)
		{
			//�Ե�������
			pushNextToken();
			//�ҷ�֧Ҳ�Ǳ��ʽ
			auto expression = parseExpression();
			//��û���±��򱨴�
			if(!expression)
				error(MISSING_INDEX, lines.front(), columns.front());
			result->offset.push_back(expression);
			//�Ե�������
			if (getToken() != TK_RBRACKET)
				error(MISSING_BARCKET,lines.front(),columns.front());
			else
				pushNextToken();
		}
		return result;
	}
	return unary_result;
}

std::shared_ptr<Expression> Parser::parseUnaryExpr()
{
	//ʲô�������һԪ���ʽ
	Token token = getToken();
	//�з���ǰ׺�������
	if (token == TK_MINUS || token == TK_LOGNOT || token == TK_BITNOT)
	{
		//�������
		auto result = std::make_shared<BinaryExpression>(line, column);
		//����ʽ�ǿ�
		result->left_expression = nullptr;
		//��ȡ�����
		result->opt = token;
		//�Ե������
		pushNextToken();
		//�ҷ�֧����һԪ���ʽ������---1��!!!!x�ȣ�һԪ���ʽ��������Ե���
		result->right_expression = parseUnaryExpr();
		return result;
	}

	//һЩ�������ŵĻ������ʽҲ��Ϊ�޷���һԪ���ʽ
	else if (token == TK_INT || token == TK_FLOAT || token == TK_STR || token == TK_CHAR
		|| token == TK_IDENT || token == KW_FALSE || token == KW_TRUE || token == TK_LPAREN || token == TK_LBRACE)
	{
		//���ػ������ʽ
		return parsePrimaryExpr();
	}
	return nullptr;
}

std::shared_ptr<Expression> Parser::parsePrimaryExpr()
{
	//����Ǳ�ʶ��
	if (getToken() == TK_IDENT)
	{
		//������ǰ��ʶ������
		std::string identName = getLexeme();
		//�Ե���ʶ��
		pushNextToken();
		return std::make_shared<IdentifierExpression>(identName, line, column);
	}
	//����ǲ���
	else if (getToken() == KW_TRUE || getToken() == KW_FALSE)
	{
		bool val = getLexeme() == "true" ? true : false;
		auto result = std::make_shared<BoolExpression>(val, line, column);
		pushNextToken();
		return result;
	}
	else if (getToken() == TK_INT)
	{
		//��ȡ����
		int val = atoi(getLexeme().c_str());
		//�Ե�����
		pushNextToken();
		return  std::make_shared<IntExpression>(val,line,column);
	}
	else if (getToken() == TK_STR)
	{
		//��ȡ�ַ���
		std::string val = getLexeme();
		//�Ե��ַ���
		pushNextToken();
		return std::make_shared<StringExpression>(val, line, column);
	}
	else if (getToken() == TK_CHAR)
	{
		//��ȡ�ַ�
		char val = getLexeme()[0];
		//�Ե��ַ�
		pushNextToken();
		return std::make_shared<CharExpression>(val, line, column);
	}
	else if (getToken() == TK_FLOAT)
	{
		std::istringstream in;
		//��ȡ����
		in.str(getLexeme());
		float val = 0;
		in >> val;
		//�Ե�����
		pushNextToken();
		return std::make_shared<FloatExpression>(val, line, column);
	}
	//���������ŵ�ʱ��
	else if (getToken() == TK_LBRACE)
	{
		auto result = std::make_shared<ArrayValueExpression>(lines.front(),columns.front());
		//�Ե�������
		pushNextToken();
		while (getToken() != TK_RBRACE)
		{
			//��ȡһ��Ԫ�أ�Ҳ��һ�����ʽ
			result->exprs.push_back(parseExpression());
			//�����һ�����Ƕ��Ż����Ҵ�����˵�����ʽ����
			while(getToken() != TK_COMMA && getToken() != TK_RBRACE)
			{
				//����
				error(INVAILD_ARRAY_VALUE,lines.front(),columns.front());
				//�Ե�������ʽ
				pushNextToken();
				//���ܻ���������β
				if (getToken() == TK_EOF)
					return nullptr;
			}
			if (getToken() == TK_RBRACE)
			{
				//�Ե�������
				pushNextToken();
				break;
			}
			//�Ե�����
			pushNextToken();
			
		}
		return result;
	}
	return nullptr;
}

std::shared_ptr<Statement> Parser::expression()
{
	//��ͨ���ʽ
	auto expr = parseExpression();
	if (!expr)
	{
		error(ERROR_EXPRESSION, line, column-3);
		exit(0);
	}
	return std::make_shared<ExpressionStatement>(expr,line,column);
}