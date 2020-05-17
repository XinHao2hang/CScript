#include "Ast.h"

operand VariableDeclareStatement::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	int typeSize = 0;
	//判断变量类型
	if (type == KW_INT)
		typeSize = 4;
	//申请栈空间
	memory.malloc(typeSize);
	//如果是赋值
	operand value;
	//计算表达式结果
	if (typeid(*ident) == typeid(AssignExpression))
	{
		//获取值
		auto rightValue = std::dynamic_pointer_cast<AssignExpression>(ident);
		value = rightValue->right_value->evaluation(stms,memory,table);
	}
	else
	{
		//填写值
		value = operand(type, 0, false);
	}
	stms.push_back(Quaternion(PSH, value, operand(), operand()));
	return value;
}

operand AssignExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	//计算右值数值
	right_value->evaluation(stms,memory,table);
	return operand();
}

operand IntExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	return operand(KW_INT,exp_value,false);
}