#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

unordered_map<string,vector<string>> ies;
unordered_map<string,vector<string>> nas;

int ies_init();
int nas_init();

void Dump();

void GenerateFile(const string str);

vector<string> BreakString(string str);

bool IsLower(char ch);
bool IsUpper(char ch);

string FileName(const string str);
string HFileName(const string str);
string CFileName(const string str);
string DataName(const string str);
string DataTypeName(const string str);

string DefineName(const string str);
string DefineNameMini(const string str);
string DefineNameMaxi(const string str);

string FunctionName(const string str);
string FunctionNameEncode(const string str);
string FunctionNameDecode(const string str);


string Upper(const string str);
string Lower(const string str);

char Upper(const char ch);
char Lower(const char ch);
