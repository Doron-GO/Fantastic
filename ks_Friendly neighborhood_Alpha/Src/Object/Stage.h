#pragma once
#include "Common/Transform.h"
#include<vector>
#include<map>
#include"SwingPoint.h";
#include"StagePiece.h"
class ResourceManager;
class Player;
class SwingPoint;

class Stage 
{
public:

	enum class STAGE_NUM
	{
		S1_1,
		S1_2,
		S1_3,
		S1_4,
		S1_5,
		S2_1,
		S2_2,
		S2_3,
		S2_4,
		S2_5,
		S3_1,
		S3_2,
		S3_3,
		S3_4,
		S3_5,
		S4_1,
		S4_2,
		S4_3,
		S4_4,
		S4_5
	};


	Stage();
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


	int GetModelId(void) const;
	const Transform* GetTransform(int num);
	VECTOR GetSwinPoint(void);
	void Load(void);
	std::map<int, int> col2_;

private:	
	//ステージの当たり判定生成
	void MakeStageCol(void);

	std::map<STAGE_NUM, StagePiece*> stagePiece_;

	//モデルの読み込み
	ResourceManager& resourceManager_;

	SwingPoint swingpoint_;

	// モデル制御の基本情報
	Transform transform_;

	std::vector<int> frameIndex_;

	VECTOR test;
	//ポリゴン情報格納先
	MV1_REF_POLYGONLIST polygonList_[2];

	int modelH_[2];
};

