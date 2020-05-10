#pragma once
#include<string>
#include<iostream>

//�������
enum ErrID
{
	MISSING_BARCKET = 0,//ȱ��������
	ERROR_EXPRESSION,//���ʽ����
	ARRAY_DEC_ELEMENT,//���������±����
	BAD_IDENT//�����ʶ��

};
//������ʾ
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

