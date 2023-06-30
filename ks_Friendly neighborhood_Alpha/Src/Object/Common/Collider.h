#pragma once
#include<map>
class Collider
{
public :

	// 衝突種別
	enum class TYPE
	{
		STAGE
	};

	// コンストラクタ
	Collider(TYPE type, int modelId);

	// デストラクタ
	~Collider(void);

	// 衝突種別
	TYPE type_;

	// モデルのハンドルID
	int modelId_;
	std::map<int, int> col_;
};

