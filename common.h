#pragma once
enum Token {
	INVALID = 0,  // <invalid>

	TK_IDENT,    // <identifier>
	TK_EOF,      // <eof>
	TK_INT,     // <integer> ����
	TK_STR,     // <string> �ַ���
	TK_DOUBLE,  // <double> ��������
	TK_CHAR,    // <character> �ַ�����
	TK_LIST,    // list����
	TK_BOOL,    // bool����
	TK_ARRAY,   // ����

	TK_BITAND,  // &
	TK_BITOR,   // |
	TK_BITNOT,  // ~
	TK_LOGAND,  // &&
	TK_LOGOR,   // ||
	TK_LOGNOT,  // !

	TK_PLUS,   // +
	TK_MINUS,  // -
	TK_TIMES,  // *
	TK_DIV,    // /
	TK_MOD,    // %

	TK_EQ,  // ==
	TK_NE,  // !=
	TK_GT,  // >
	TK_GE,  // >=
	TK_LT,  // <
	TK_LE,  // <=


	TK_ASSIGN,     // =
	TK_PLUS_AGN,   // +=
	TK_PLUS_SELF,  // ++
	TK_MINUS_AGN,  // -=
	TK_MINUS_SELF,  // --
	TK_TIMES_AGN,  // *=
	TK_DIV_AGN,    // /=
	TK_MOD_AGN,    // %=
	TK_COMMA,      // ,
	TK_LPAREN,     // (
	TK_RPAREN,     // )
	TK_LBRACE,     // {
	TK_RBRACE,     // }
	TK_LBRACKET,   // [
	TK_RBRACKET,   // ]
	TK_COLON,      //:

	KW_IF,        // if
	KW_ELSE,      // else
	KW_TRUE,      // true
	KW_FALSE,     // false
	KW_WHILE,     // while
	KW_FOR,       // for
	KW_NULL,      // null
	KW_FUNC,      // func
	KW_RETURN,    // return
	KW_BREAK,     // break
	KW_CONTINUE,  // continue
	KW_INT,       //int
	KW_DOUBLE,    //double
	KW_STRING,    //string
	KW_BOOL,      //bool
	KW_USING,     //using
	KW_EXTERN,    //extern
	KW_LIBRARY,   //library
	KW_LIST,      //list
	KW_ARRAY,     //����
	KW_CLASS,     //��
	TK_DOT,       //.
	TK_SEM,		  //;
	TK_NOTE, //ע��


	KW_OBJECT
};
