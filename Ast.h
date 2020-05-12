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
//浮点表达式
class FloatExpression : public Expression
{
public:
	FloatExpression(float _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//表达式的值
	float exp_value;
	~FloatExpression() {}
};

//字符表达式
class CharExpression : public Expression
{
public:
	CharExpression(char _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//表达式的值
	char exp_value;
	~CharExpression() {}
};

//字符串表达式
class StringExpression : public Expression
{
public:
	StringExpression(std::string _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//表达式的值
	std::string exp_value;
	~StringExpression() {}
};

//整形表达式
class IntExpression : public Expression
{
public:
	IntExpression(int _value, int _line, int _column) :Expression(_line, _column), exp_value(_value) {}
	//表达式的值
	int exp_value;
	~IntExpression() {}
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
	Token opt;
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
	Token opt;
	//右值表达式
	std::shared_ptr<Expression> right_value;
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~AssignExpression() {}
};

//寻址表达式
class AddressingExpression : public Expression
{
public:
	AddressingExpression(int _line, int _column) :Expression(_line, _column) {}
	//基地址
	std::shared_ptr<Expression> base;
	//下标,偏移地址
	std::vector<std::shared_ptr<Expression>> offset;
	~AddressingExpression(){}
};

//数组值表达式
class ArrayValueExpression : public Expression
{
public:
	ArrayValueExpression(int _line, int _column) :Expression(_line, _column) {}
	//数值表达式
	std::vector<std::shared_ptr<Expression>> exprs;
	~ArrayValueExpression() {}
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
	ExpressionStatement(std::shared_ptr<Expression> expr, int _line, int _column) :Statement(_line, _column), expression(expr) {}
	//包含一个表达式，其中包括赋值，函数调用，一元二元计算
	std::shared_ptr<Expression> expression;
	virtual operand evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
	{
		return expression->evaluation(stms, memory, table);
	}
	~ExpressionStatement() {}

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
	//std::string identName;
	std::shared_ptr<Expression> ident;
	//表达式解析
	virtual operand evaluation(std::vector<Quaternion>& context, Memory& memory, NameTable& table) { return operand(); }
	~VariableDeclareStatement() {}
};

//数组声明语句
class ArrayDeclareStatement :public Statement
{
public:
	ArrayDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//数组类型
	Token type;
	//类型名
	std::string typeName;
	//数组元素个数
	std::vector<std::shared_ptr<Expression>> elementNums;
	//数组名字
	std::string name;
	//数组初始值
	std::shared_ptr<Expression> initValue;
	~ArrayDeclareStatement() {}
};

//代码块表达式
class CodeBlockStatement : public Statement
{
public:
	CodeBlockStatement(int _line, int _column) :Statement(_line, _column) {}
	//代码块
	std::vector<std::shared_ptr<Statement>> statements;
	~CodeBlockStatement() {}
};

//函数声明表达式
class FunctionDeclareStatement :public Statement
{
public:
	FunctionDeclareStatement(int _line, int _column) :Statement(_line, _column) {}
	//类型
	Token type;
	//类型名字
	std::string typeName;
	//函数名
	std::string functionName;
	//函数参数列表，参数列表中必须都是声明表达式
	std::vector<std::shared_ptr<Statement>> argsList;
	//代码块
	std::shared_ptr<CodeBlockStatement> block;
	~FunctionDeclareStatement(){}
};

