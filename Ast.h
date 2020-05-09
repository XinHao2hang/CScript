#pragma once
#include<string>
#include<vector>
#include"Quaternion.h"
#include"Memory.h"
#include"common.h"
#include<memory>
//Ast树的节点基类
class AstNode
{
public:
	int line, column;
	AstNode() {};
	AstNode(int _line, int _column) :line(_line), column(_column) {}
	//分析语法后生成中间码
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AstNode() {};
};



//表达式基类
class Expression : public AstNode
{
	//在上下文寻找变量
public:

	Expression(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Expression() {}
};

//bool表达式
class BoolExpression : public Expression
{
public:
	BoolExpression(bool _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//表达式的值
	bool exp_value;
	//表达式解析
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~BoolExpression() {}

};



//语句
class Statement : public AstNode
{
public:
	Statement(int _line, int _column) :AstNode(_line, _column) {}
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~Statement() {}
};

//表达式语句
class ExpressionStatement : public Statement
{
public:
	ExpressionStatement(Expression* expr, int _line, int _column) :Statement(_line, _column), expression(expr) {}
	//包含一个表达式，其中包括赋值，函数调用，一元二元计算
	std::shared_ptr<Expression> expression;
	virtual operand evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
	{
		return expression->evaluation(stms, memory, table);
	}
	~ExpressionStatement() {}

};

//标识符表达式
class IdentifierExpression : public Expression
{
public:
	IdentifierExpression(std::string identName, int _line, int _column) :Expression(_line, _column), exp_name(move(identName)) {}
	std::string exp_name;
	//表达式解析
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~IdentifierExpression() {}
};

//二元表达式
class BinaryExpression : public Expression
{
public:
	BinaryExpression(int _line, int _column) :
		Expression(_line, _column) {}
	//左表达式分支
	std::shared_ptr<Expression> left_expression = nullptr;
	//操作符
	int opt;
	//右表达式分支
	std::shared_ptr<Expression> right_expression = nullptr;
	//表达式解析
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
};

//赋值表达式
class AssignExpression : public Expression
{
public:
	AssignExpression(int _line, int _column) :Expression(_line, _column) {}
	//左值表达式
	std::shared_ptr<Expression> left_value;
	//运算符
	int opt;
	//右值表达式
	std::shared_ptr<Expression> right_value;
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AssignExpression() {}
};

//变量声明语句
class VariableDeclareStatement : public Statement
{
public:
	VariableDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//类型
	Token type;
	//类型名字
	std::string typeName;
	//标识符表达式
	std::string identName;
	//表达式解析
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~VariableDeclareStatement() {}
};