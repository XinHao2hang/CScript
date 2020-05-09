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
	//token缓存队列，方便语法分析的时候能向后查看多个token
	std::vector<std::tuple<Token, std::string>> tokenQueue;

	//关键字存储
	std::unordered_map<std::string, Token> keyWords;

	//输入的代码
	std::fstream fs;
	//行
	int line = 1;
	//列
	int column = 0;

	//获取下一个token
	std::tuple<Token, std::string> nextToken();

	//获取下一个token到队列
	std::tuple<Token, std::string> pushNextToken();

	//获取第n位置token值
	inline Token getToken(int n = 0) { return std::get<Token>(tokenQueue[n]); }

	//获取第n位置token串
	inline std::string getLexeme(int n = 0) { return std::get<std::string>(tokenQueue[n]); }

	//获取下一个字符，不从流中清除
	inline char peekChar() { return static_cast<char>(fs.peek()); }
	inline char nextChar()
	{
		column++;
		return static_cast<char>(fs.get());
	}


	//表达式解析
	std::shared_ptr<Expression> parseExpression();
	std::shared_ptr<Expression> parseUnaryExpr();
	std::shared_ptr<Expression> parsePrimaryExpr();

	//以下是语法分析用的函数
	//语法入口
	std::shared_ptr<Statement> parseStatement();
	//声明表达式
	std::shared_ptr<Statement> decalre();
	//普通表达式
	std::shared_ptr<Statement> expression();
	//变量声明
	std::shared_ptr<VariableDeclareStatement> variableDeclare();
public:
	Parser();
	Parser(std::string fileName);
	//存放分析好的语句
	std::vector<std::shared_ptr<Statement>> stms;
	//初始化token队列
	void initTokenQueue(int n);
	void parse(std::string fileName);

	

	
	//词法分析测试
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

