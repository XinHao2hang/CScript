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
	//�﷨�������﷨������
	Parser parser;
	void execute();
	//��Ŵ���
	std::vector<Quaternion> code;
	//��ű���
	Memory memory;
	//��Ų�ѯ�ĺ����ͱ���
	NameTable table;
	//��������
	std::unordered_map<OP, std::string> optName;
	~Intermediate(){}
};

