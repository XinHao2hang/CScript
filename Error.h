#pragma once
#include<string>
#include<iostream>

//错误代码
enum ErrID
{
	MISSING_BARCKET = 0,//缺少中括号
	ERROR_EXPRESSION,//表达式错误
	ARRAY_DEC_ELEMENT,//数组声明下标错误
	BAD_IDENT,//错误标识符
	MISSING_SEM,//缺少分号
	MISSING_ARRAY_NAME,//缺少数组名
	MISSING_INDEX,//缺少下标
	NOT_LEFT_VALUE,//不是左值
	INVAILD_ARRAY_VALUE,//无效的数值数值
};
//错误显示
class Error
{
	std::string errorInfo[100] = {
	"Missing bracket",
	"Error expression",
	"Number of elements of the array to be determined",
	"Bad identifier",
	"Missing semicolon",
	"Missing array name",
	"Missing index",
	"Not a left value",
	"Missing comma or invaild array value"
	};
public:
	void operator()(ErrID id,int line,int column)
	{
		std::cout <<"ERR"<< id << ":" << "[" << line << "," << column << "]:" << errorInfo[id] << std::endl;
	}
};

