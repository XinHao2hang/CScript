#pragma once
#include<vector>
#include<string>
#include<tuple>
struct operand
{
	operand(){}
	//���ͣ���ֵ����ֵ���(��ֵ����ַ)
	operand(int t,int n,bool addr):vaild(true), address(addr),num(n),type(t){}
	bool vaild = false;
	bool address = true;
	int num;
	int type;
};
enum OP
{
	ADD,
	ASSIGN,
	SUB,
	PSH,
	POP,
	CALL,
	RET,
};
//��Ԫʽ
class Quaternion
{
public:
	Quaternion(){}
	//������������1��������2�����
	Quaternion(OP _op, operand a, operand b, operand r) :op(_op), operand1(a), operand2(b), result(r) {}
	//����
	OP op;
	operand operand1;
	operand operand2;
	operand result;
	~Quaternion(){}
};

//������¼��ջ��
class NameTable
{
public:
	NameTable() {}
	~NameTable() {}
	//�����ֵ����ַ����������
	std::vector<std::tuple<int, int,int, std::string>> s;
	void push(std::tuple<int, int,int, std::string> num)
	{
		s.push_back(num);
	}

	std::tuple<int, int,int, std::string> top()
	{
		return s[s.size() - 1];
	}

	void pop()
	{
		s.pop_back();
	}

	int findAddress(std::string name)
	{
		for (int i = s.size() - 1; i >= 0; i--)
		{
			if (std::get<std::string>(s[i]) == name)
				return std::get<2>(s[i]);
		}
		return -1;
	}

	int findValue(std::string name)
	{
		for (int i = s.size() - 1; i >= 0; i--)
		{
			if (std::get<std::string>(s[i]) == name)
				return std::get<1>(s[i]);
		}
		return -1;
	}

	int getType(std::string name)
	{
		for (int i = s.size() - 1; i >= 0; i--)
		{
			if (std::get<std::string>(s[i]) == name)
				return std::get<0>(s[i]);
		}
		return -1;
	}
};