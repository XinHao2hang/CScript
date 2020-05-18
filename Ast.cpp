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
	return operand(MD_PTR, table.findAddress(exp_name),true);
}