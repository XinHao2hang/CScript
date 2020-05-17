#include "Ast.h"

operand VariableDeclareStatement::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	int typeSize = 0;
	//�жϱ�������
	if (type == KW_INT)
		typeSize = 4;
	//����ջ�ռ�
	memory.malloc(typeSize);
	//����Ǹ�ֵ
	operand value;
	//������ʽ���
	if (typeid(*ident) == typeid(AssignExpression))
	{
		//��ȡֵ
		auto rightValue = std::dynamic_pointer_cast<AssignExpression>(ident);
		value = rightValue->right_value->evaluation(stms,memory,table);
	}
	else
	{
		//��дֵ
		value = operand(type, 0, false);
	}
	stms.push_back(Quaternion(PSH, value, operand(), operand()));
	return value;
}

operand AssignExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	//������ֵ��ֵ
	right_value->evaluation(stms,memory,table);
	return operand();
}

operand IntExpression::evaluation(std::vector<Quaternion>& stms, Memory& memory, NameTable& table)
{
	return operand(KW_INT,exp_value,false);
}