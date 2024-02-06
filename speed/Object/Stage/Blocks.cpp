#include "Blocks.h"
#include"LoadMap.h"
#include<DxLib.h>
#include"../Manager/ImageMng.h"

Blocks::Blocks(LoadMap& loadMap):loadMap_(loadMap)
{
	for (auto &col : loadMap_.BlockGetColList())
	{
		auto block = Block(true, col);
		blocks_.push_back(block);
	}
}

Blocks::~Blocks()
{

}

void Blocks::Update()
{
	for (auto& block : blocks_)
	{
		if (!(block.blockFlag_))
		{
			if (block.count_>=80)
			{
				block.blockFlag_ = true;
				block.count_ = 0;
			}
			else
			{
				block.count_++;
			}
		}
	}
}

void Blocks::Draw(Vector2DFloat cameraPos)
{
	for (auto& block : blocks_)
	{
		if (block.blockFlag_)
		{
			auto min = block.col_.first;
			auto max = block.col_.second;
			DrawGraph(min.x+cameraPos.x, max.y + cameraPos.y,
				ImageMng::GetInstsnce().GetID(loadMap_.GetMapKey())
				[6], true);
		}
	}
}

 std::list< Blocks::Block> &Blocks::GetBlockList()
{
	return blocks_;
}
