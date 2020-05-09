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



//����ʽ����
class Expression : public AstNode
{
	//��������Ѱ�ұ���
public:

	Expression(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Expression() {}
};

//bool����ʽ
class BoolExpression : public Expression
{
public:
	BoolExpression(bool _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//����ʽ��ֵ
	bool exp_value;
	//����ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~BoolExpression() {}

};



//���
class Statement : public AstNode
{
public:
	Statement(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Statement() {}
};

//����ʽ���
class ExpressionStatement : public Statement
{
public:
	ExpressionStatement(Expression* expr, int _line, int _column) :Statement(_line, _column), expression(expr) {}
	//����һ������ʽ�����а�����ֵ���������ã�һԪ��Ԫ����
	std::shared_ptr<Expression> expression;
	virtual operand evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
	{
		return expression->evaluation(stms, memory, table);
	}
	~ExpressionStatement() {}

};

//��ʶ������ʽ
class IdentifierExpression : public Expression
{
public:
	IdentifierExpression(std::string identName, int _line, int _column) :Expression(_line, _column), exp_name(move(identName)) {}
	std::string exp_name;
	//����ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~IdentifierExpression() {}
};

//��Ԫ����ʽ
class BinaryExpression : public Expression
{
public:
	BinaryExpression(int _line, int _column) :
		Expression(_line, _column) {}
	//�����ʽ��֧
	std::shared_ptr<Expression> left_expression = nullptr;
	//������
	int opt;
	//�ұ���ʽ��֧
	std::shared_ptr<Expression> right_expression = nullptr;
	//����ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
};

//��ֵ����ʽ
class AssignExpression : public Expression
{
public:
	AssignExpression(int _line, int _column) :Expression(_line, _column) {}
	//��ֵ����ʽ
	std::shared_ptr<Expression> left_value;
	//�����
	int opt;
	//��ֵ����ʽ
	std::shared_ptr<Expression> right_value;
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AssignExpression() {}
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
	//��ʶ������ʽ
	std::string identName;
	//����ʽ����
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~VariableDeclareStatement() {}
};