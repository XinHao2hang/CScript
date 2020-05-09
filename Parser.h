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
	//当前正在使用的token
	std::tuple<Token, std::string> currentToken;

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
public:
	Parser(){}
	Parser(std::string fileName);
	//初始化token队列
	void initTokenQueue(int n);
	//获取下一个token
	std::tuple<Token, std::string> nextToken();

	//获取下一个token到队列
	std::tuple<Token, std::string> pushNextToken();

	//获取下一个字符，不从流中清除
	inline char peekChar() { return static_cast<char>(fs.peek());}
	inline char nextChar()
	{
		column++;
		return static_cast<char>(fs.get());
	}

	//获取第n位置token值
	inline Token getToken(int n = 0) { return std::get<Token>(tokenQueue[n]); }
	//获取第n位置token串
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

