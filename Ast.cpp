#include "Ast.h"

operand VariableDeclareStatement::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	int typeSize = 0;
	//�жϱ�������
	if (type == KW_INT)
		typeSize = 4;
	//����ջ�ռ�
	int addr = memory.malloc(typeSize);
	//����Ǹ�ֵ
	operand value;
	//������ʽ���
	if (typeid(*ident) == typeid(AssignExpression))
	{
		//��ȡֵ
		auto rightValue = std::dynamic_pointer_cast<AssignExpression>(ident);
		auto varName = std::dynamic_pointer_cast<IdentifierExpression>(std::dynamic_pointer_cast<AssignExpression>(ident)->left_value)->exp_name;
		value = rightValue->right_value->evaluation(stms,memory,table);
		//�洢������
		table.push(make_tuple(KW_INT, value.num,addr,varName));
	}
	else
	{
		//��дֵ
		value = operand(type, 0, false);
		//��ȡ��������
		auto varName = std::dynamic_pointer_cast<IdentifierExpression>(ident)->exp_name;
		//�洢������
		table.push(make_tuple(KW_INT,0,addr,varName));
	}
	
	stms.push_back(Quaternion(PSH, value, operand(), operand()));
	return value;
}

operand AssignExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	operand result;
	//�����ֵ����ͨ����
	if ((typeid(*left_value) == typeid(IdentifierExpression)))
	{
		//��������������
		std::string varName = std::dynamic_pointer_cast<IdentifierExpression>(left_value)->exp_name;
		//������ֵ��ֵ
		result = right_value->evaluation(stms, memory, table);
		//��ȡ��ֵ��ַ
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
	//�����ʶ�������ر�ʶ���ĵ�ַ
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
		//�����ж�
		if (rightValue.type == KW_STRING || leftValue.type == KW_STRING)
		{
			//�ַ�����֧�����㣬����
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