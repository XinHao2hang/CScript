#pragma once
#include<vector>
//�������ڴ����飬���б����Ľ��������������
class Memory
{
public:
	Memory()
	{
		//��ʼ���ڴ�
		memory.resize(4096);
	}
	//�����м�����õĵ�ַ���������
	int address = 0;
	//�ڴ�ռ�
	int esp=0;
	int ebp = 0;
	int malloc()
	{
		return ++address;
	}
	int & operator[](int addr)
	{
		return memory[addr];
	}
	std::vector<int> memory;
	~Memory(){}
};

