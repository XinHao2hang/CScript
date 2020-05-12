#pragma once
#include<string>
#include<vector>
#include"Quaternion.h"
#include"Memory.h"
#include"common.h"
#include<memory>
//Ast���Ľڵ����
class AstNode
{
public:
	int line, column;
	AstNode() {};
	AstNode(int _line, int _column) :line(_line), column(_column) {}
	//�����﷨�������м���
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AstNode() {};
};



//���ʽ����
class Expression : public AstNode
{
	//��������Ѱ�ұ���
public:

	Expression(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Expression() {}
};

//bool���ʽ
class BoolExpression : public Expression
{
public:
	BoolExpression(bool _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//���ʽ��ֵ
	bool exp_value;
	//���ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~BoolExpression() {}

};
//������ʽ
class FloatExpression : public Expression
{
public:
	FloatExpression(float _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//���ʽ��ֵ
	float exp_value;
	~FloatExpression() {}
};

//�ַ����ʽ
class CharExpression : public Expression
{
public:
	CharExpression(char _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//���ʽ��ֵ
	char exp_value;
	~CharExpression() {}
};

//�ַ������ʽ
class StringExpression : public Expression
{
public:
	StringExpression(std::string _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//���ʽ��ֵ
	std::string exp_value;
	~StringExpression() {}
};

//���α��ʽ
class IntExpression : public Expression
{
public:
	IntExpression(int _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//���ʽ��ֵ
	int exp_value;
	~IntExpression() {}
};

//��ʶ�����ʽ
class IdentifierExpression : public Expression
{
public:
	IdentifierExpression(std::string identName, int _line, int _column) :Expression(_line, _column), exp_name(move(identName)) {}
	std::string exp_name;
	//���ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~IdentifierExpression() {}
};

//��Ԫ���ʽ
class BinaryExpression : public Expression
{
public:
	BinaryExpression(int _line, int _column) :
		Expression(_line, _column) {}
	//����ʽ��֧
	std::shared_ptr<Expression> left_expression = nullptr;
	//������
	Token opt;
	//�ұ��ʽ��֧
	std::shared_ptr<Expression> right_expression = nullptr;
	//���ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
};

//��ֵ���ʽ
class AssignExpression : public Expression
{
public:
	AssignExpression(int _line, int _column) :Expression(_line, _column) {}
	//��ֵ���ʽ
	std::shared_ptr<Expression> left_value;
	//�����
	Token opt;
	//��ֵ���ʽ
	std::shared_ptr<Expression> right_value;
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AssignExpression() {}
};

//Ѱַ���ʽ
class AddressingExpression : public Expression
{
public:
	AddressingExpression(int _line, int _column) :Expression(_line, _column) {}
	//����ַ
	std::shared_ptr<Expression> base;
	//�±�,ƫ�Ƶ�ַ
	std::vector<std::shared_ptr<Expression>> offset;
	~AddressingExpression(){}
};

//����ֵ���ʽ
class ArrayValueExpression : public Expression
{
public:
	ArrayValueExpression(int _line, int _column) :Expression(_line, _column) {}
	//��ֵ���ʽ
	std::vector<std::shared_ptr<Expression>> exprs;
	~ArrayValueExpression() {}
};

//���
class Statement : public AstNode
{
public:
	Statement(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Statement() {}
};

//���ʽ���
class ExpressionStatement : public Statement
{
public:
	ExpressionStatement(std::shared_ptr<Expression> expr, int _line, int _column) :Statement(_line, _column), expression(expr) {}
	//����һ�����ʽ�����а�����ֵ���������ã�һԪ��Ԫ����
	std::shared_ptr<Expression> expression;
	virtual operand evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
	{
		return expression->evaluation(stms, memory, table);
	}
	~ExpressionStatement() {}

};

//�����������
class VariableDeclareStatement : public Statement
{
public:
	VariableDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//����
	Token type;
	//��������
	std::string typeName;
	//��ʶ�����ʽ
	//std::string identName;
	std::shared_ptr<Expression> ident;
	//���ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~VariableDeclareStatement() {}
};

//�����������
class ArrayDeclareStatement :public Statement
{
public:
	ArrayDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//��������
	Token type;
	//������
	std::string typeName;
	//����Ԫ�ظ���
	std::vector<std::shared_ptr<Expression>> elementNums;
	//��������
	std::string name;
	//�����ʼֵ
	std::shared_ptr<Expression> initValue;
	~ArrayDeclareStatement() {}
};

//�������ʽ
class CodeBlockStatement : public Statement
{
public:
	CodeBlockStatement(int _line, int _column) :Statement(_line, _column) {}
	//�����
	std::vector<std::shared_ptr<Statement>> statements;
	~CodeBlockStatement() {}
};

//�����������ʽ
class FunctionDeclareStatement :public Statement
{
public:
	FunctionDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//����
	Token type;
	//��������
	std::string typeName;
	//������
	std::string functionName;
	//���������б������б��б��붼���������ʽ
	std::vector<std::shared_ptr<Statement>> argsList;
	//�����
	std::shared_ptr<CodeBlockStatement> block;
	~FunctionDeclareStatement(){}
};

