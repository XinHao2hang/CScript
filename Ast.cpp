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
	int type = table.getType(exp_name);
	return operand(type, table.findAddress(exp_name),true);
}

operand BinaryExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	operand result;
	operand rightValue = right_expression->evaluation(stms, memory, table);
	operand leftValue = left_expression->evaluation(stms, memory, table);
	Token resType;
	if (opt == TK_PLUS)
	{
		//类型判断
		if (rightValue.type == KW_STRING || leftValue.type == KW_STRING)
		{
			//字符串不支持运算，报错
		}
		else if (rightValue.type == KW_INT || leftValue.type == KW_INT)
			resType = KW_INT;

		int addr = memory.malloc(4);
		result = operand(resType,addr,true);
		table.push(std::make_tuple(KW_INT,0,addr,"temp@"+addr));
		stms.push_back(Quaternion(ADD,leftValue,rightValue,result));
	}

	return result;
	
}