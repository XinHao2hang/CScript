#pragma once
#include<string>
#include<iostream>

//�������
enum ErrID
{
	MISSING_BARCKET = 0,//ȱ��������
	ERROR_EXPRESSION,//���ʽ����
	ARRAY_DEC_ELEMENT,//���������±����
	BAD_IDENT,//�����ʶ��
	MISSING_SEM,//ȱ�ٷֺ�
	MISSING_ARRAY_NAME,//ȱ��������
	MISSING_INDEX,//ȱ���±�
	NOT_LEFT_VALUE,//������ֵ
	INVAILD_ARRAY_VALUE,//��Ч����ֵ��ֵ
};
//������ʾ
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

