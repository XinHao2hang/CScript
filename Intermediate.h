#pragma once
#include"Parser.h"
#include"Memory.h"
#include"Quaternion.h"
#include<string>
#include<vector>
#include<unordered_map>
class Intermediate
{
public:
	Intermediate() {}
	Intermediate(std::string filename);
	//语法分析，语法树建立
	Parser parser;
	void execute();
	//存放代码
	std::vector<Quaternion> code;
	//存放变量
	Memory memory;
	//存放查询的函数和变量
	NameTable table;
	//操作名称
	std::unordered_map<OP, std::string> optName;
	~Intermediate(){}
};

