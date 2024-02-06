#pragma once
#include"../../Vector2D.h"
#include<vector>
#include<memory>
#include<string>
#include<map>

//#define lpImageMng ImageMng::GetInstsnce()
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstsnce()
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	const std::vector<int>& GetID(std::string key);
	const std::vector<int>& GetID(std::string f_name, std::string key);
	const std::vector<int>& GetID(std::string f_name, std::string key, Vector2D divSize, Vector2D divCut);
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, std::vector<int>> ImageMap_;
};

