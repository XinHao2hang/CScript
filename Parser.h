#pragma once
#include"common.h"
#include"Ast.h"
#include<string>
#include<tuple>
#include<vector>
#include<fstream>
#include<unordered_map>
#include<iostream>
class Parser
{
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

	//��ȡ��һ��token
	std::tuple<Token, std::string> nextToken();

	//��ȡ��һ��token������
	std::tuple<Token, std::string> pushNextToken();

	//��ȡ��nλ��tokenֵ
	inline Token getToken(int n = 0) { return std::get<Token>(tokenQueue[n]); }

	//��ȡ��nλ��token��
	inline std::string getLexeme(int n = 0) { return std::get<std::string>(tokenQueue[n]); }

	//��ȡ��һ���ַ��������������
	inline char peekChar() { return static_cast<char>(fs.peek()); }
	inline char nextChar()
	{
		column++;
		return static_cast<char>(fs.get());
	}


	//���ʽ����
	std::shared_ptr<Expression> parseExpression();
	std::shared_ptr<Expression> parseUnaryExpr();
	std::shared_ptr<Expression> parsePrimaryExpr();

	//�������﷨�����õĺ���
	//�﷨���
	std::shared_ptr<Statement> parseStatement();
	//�������ʽ
	std::shared_ptr<Statement> decalre();
	//��ͨ���ʽ
	std::shared_ptr<Statement> expression();
	//��������
	std::shared_ptr<VariableDeclareStatement> variableDeclare();
public:
	Parser();
	Parser(std::string fileName);
	//��ŷ����õ����
	std::vector<std::shared_ptr<Statement>> stms;
	//��ʼ��token����
	void initTokenQueue(int n);
	void parse(std::string fileName);

	

	
	//�ʷ���������
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

