#pragma once
#include"common.h"
#include<string>
#include<tuple>
#include<vector>
#include<fstream>
#include<unordered_map>
#include<iostream>
class Parser
{
	//��ǰ����ʹ�õ�token
	std::tuple<Token, std::string> currentToken;

	//token������У������﷨������ʱ�������鿴���token
	std::vector<std::tuple<Token, std::string>> tokenQueue;

	//�ؼ��ִ洢
	std::unordered_map<std::string, Token> keyWords;

	//����Ĵ���
	std::fstream fs;
	//��
	int line = 1;
	//��
	int column = 0;
public:
	Parser(){}
	Parser(std::string fileName);
	//��ʼ��token����
	void initTokenQueue(int n);
	//��ȡ��һ��token
	std::tuple<Token, std::string> nextToken();

	//��ȡ��һ��token������
	std::tuple<Token, std::string> pushNextToken();

	//��ȡ��һ���ַ��������������
	inline char peekChar() { return static_cast<char>(fs.peek());}
	inline char nextChar()
	{
		column++;
		return static_cast<char>(fs.get());
	}

	//��ȡ��nλ��tokenֵ
	inline Token getToken(int n = 0) { return std::get<Token>(tokenQueue[n]); }
	//��ȡ��nλ��token��
	inline std::string getCurrentLexeme(int n = 0) { return std::get<std::string>(tokenQueue[n]);}

	static void testPrint(const std::string& fileName)
	{		
		Parser p(fileName);
		//p.initTokenQueue(3);
		std::tuple<Token, std::string> tk;
		do {
			tk = p.pushNextToken();
			std::cout << "[" << std::get<0>(tk) << "," << std::get<1>(tk) << "]\n";
		} while (std::get<0>(tk) != TK_EOF);
	}
	~Parser() {}
};

