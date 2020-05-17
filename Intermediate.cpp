#include "Intermediate.h"
#include<iostream>
Intermediate::Intermediate(std::string filename):
	optName({
		{ADD,"ADD"},
		{ASSIGN,"ASSIGN"},
		{SUB,"SUB"},
		{CALL,"CALL"},
		{RET,"RET"},
		{PSH,"PSH"}
		})
{
	//Óï·¨·ÖÎö
	parser.parse(filename);
}

void Intermediate::execute()
{
	auto stms = parser.stms;
	for (auto stm : stms)
	{
		operand op = stm->evaluation(code,memory,table);
	}
	for (auto c : code)
	{
		std::cout << "(" << optName[c.op] << ",";
		
		if (c.operand1.vaild)
		{
			if (c.operand1.address)
			{
				std::cout << "[" << c.operand1.num << "]";
			}
			else
			{
				std::cout << c.operand1.num;
			}
		}
		else
		{
			std::cout <<"_" ;
		}
		std::cout << ",";
		if (c.operand2.vaild)
		{
			if (c.operand2.address)
			{
				std::cout << "[" << c.operand2.num << "]";
			}
			else
			{
				std::cout << c.operand2.num;
			}
		}
		else
		{
			std::cout << "_";
		}
		std::cout << ",";
		if (c.result.vaild)
		{
			if (c.result.address)
			{
				std::cout << "[" << c.result.num << "]";
			}
			else
			{
				std::cout << c.result.num;
			}
		}
		else
		{
			std::cout << "_";
		}
		std::cout << ")" << std::endl;
	}
}
