#pragma once

class iString
{
public:
	iString();
	iString(const char* szFormat, ...);
	virtual ~iString();

public:
	iString& operator = (const char* str);
	iString& operator = (iString& str);

	const char* operator + (const char* str);
	const char* operator + (iString& str);

	iString& operator += (const char* str);
	iString& operator += (iString& str);

	bool operator == (const char* str);
	bool operator != (iString& str);

	void setString(const char* szFormat, ...);// =
	void addString(const char* szFormat, ...);// +=
	void trim(int stat);// 0: left, 1:right, 2:all

	const char* cstr() { return str; }

	static char** getStringLine(const char* str, int& lineNum, char d='\n');
	static void freeStringLine(char** line, int lineNum);

	static bool isUnicode(const char* str);
	static char** getStringLineWithWidth(const char* str, int& lineNum, float width, char d='\n');

private:
	char* str;
	int strLength;
};

#define iString_strLength 128
#define iString_lineNumber 20



extern char* strTest;
