#pragma once
#include<string>
#include<iostream>

//错误代码
enum ErrID
{
	MISSING_BARCKET = 0,//缺少中括号
	ERROR_EXPRESSION,//表达式错误
	ARRAY_DEC_ELEMENT,//数组声明下标错误
	BAD_IDENT//错误标识符

};
//错误显示
class Error
{
	std::string errorInfo[100] = {
	"Missing bracket",
	"Error expression",
	"Number of elements of the array to be determined",
	"Bad identifier"
	};
public:
	void operator()(ErrID id,int line,int column)
	{
		std::cout <<"ERR"<< id << ":" << "[" << line << "," << column << "]:" << errorInfo[id] << std::endl;
	}
};

