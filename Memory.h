#pragma once
#include<vector>
//�������ڴ����飬���б����Ľ��������������
class Memory
{
public:
	Memory()
	{
		//��ʼ���ڴ�
		memory = new char[8192];
	}
	//�����м�����õĵ�ַ���������
	int address = 0;
	//�ڴ�ռ�
	int esp=0;
	int ebp = 0;
	int malloc(int size)
	{
		address += size;
		return address-size;
	}
	char & operator[](int addr)
	{
		return memory[addr];
	}
	char * memory;
	~Memory(){}
};

