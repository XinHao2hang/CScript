#include "Ast.h"

operand VariableDeclareStatement::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	int typeSize = 0;
	//判断变量类型
	if (type == KW_INT)
		typeSize = 4;
	//申请栈空间
	int addr = memory.malloc(typeSize);
	//如果是赋值
	operand value;
	//计算表达式结果
	if (typeid(*ident) == typeid(AssignExpression))
	{
		//获取值
		auto rightValue = std::dynamic_pointer_cast<AssignExpression>(ident);
		auto varName = std::dynamic_pointer_cast<IdentifierExpression>(std::dynamic_pointer_cast<AssignExpression>(ident)->left_value)->exp_name;
		value = rightValue->right_value->evaluation(stms,memory,table);
		//存储上下文
		table.push(make_tuple(KW_INT, value.num,addr,varName));
	}
	else
	{
		//填写值
		value = operand(type, 0, false);
		//获取变量名字
		auto varName = std::dynamic_pointer_cast<IdentifierExpression>(ident)->exp_name;
		//存储上下文
		table.push(make_tuple(KW_INT,0,addr,varName));
	}
	
	stms.push_back(Quaternion(PSH, value, operand(), operand()));
	return value;
}

operand AssignExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	operand result;
	//如果左值是普通变量
	if ((typeid(*left_value) == typeid(IdentifierExpression)))
	{
		//解析出变量名字
		std::string varName = std::dynamic_pointer_cast<IdentifierExpression>(left_value)->exp_name;
		//计算右值数值
		result = right_value->evaluation(stms, memory, table);
		//获取左值地址
		int leftValue = table.findAddress(varName);
		int type = table.getType(varName);
		stms.push_back(Quaternion(ASSIGN, result,operand(), operand(KW_INT, leftValue, true)));
	}
	return result;
}

operand IntExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	return operand(KW_INT,exp_value,false);
}

operand IdentifierExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	//处理标识符，返回标识符的地址
	return operand(MD_PTR, table.findAddress(exp_name),true);
}