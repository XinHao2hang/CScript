#pragma once
#include<vector>
//这里是内存数组，所有变量的结果都会存放在这里
class Memory
{
public:
	Memory()
	{
		//初始化内存
		memory = new char[8192];
	}
	//生成中间代码用的地址分配计数器
	int address = 0;
	//内存空间
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

