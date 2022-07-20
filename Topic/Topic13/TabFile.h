#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>


using namespace std;

class TabFile
{
public:
	void Load(const char* FileName);
	string Find(const char* Key);

private:
	map<string, string> Data;
};

