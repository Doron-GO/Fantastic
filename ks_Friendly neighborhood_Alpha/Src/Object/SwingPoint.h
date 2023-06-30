#pragma once
#include<DxLib.h>
#include<map>
#include<string>
#include<list>
#include<fstream>
#include"../Json/json.hpp"
using json = nlohmann::json;

class SwingPoint
{
public:
	SwingPoint();
	~SwingPoint();

	void Load(void);

private:

	std::map < std::string, std::list < std::pair <VECTOR,VECTOR>>> swingPoint_;
	
	json json_;

};

