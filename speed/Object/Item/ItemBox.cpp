#include<DxLib.h>
#include "ItemBox.h"
#include"../Stage/LoadMap.h"
#include"../Manager/ImageMng.h"
#include"../../Player/Player.h"

ItemBox::ItemBox(LoadMap& loadMap,std::vector<std::shared_ptr<Player>> players)
	:loadMap_(loadMap),players_(players),count_(0)
{
	LoadDivGraph("Src/Img/Explosion.png", 11, 11, 1, 32, 31, itemBoxImg_);
}

ItemBox::~ItemBox()
{

}

void ItemBox::Draw(Vector2DFloat offset)
{
	auto& worldArea = loadMap_.GetWorldArea();
	const auto& tileSize = loadMap_.GetTileSize();
	auto& mapData = loadMap_.GetMapData();
	for (const auto& layer : loadMap_.GetMapData()) 
	{
		for (int y = 0; y < worldArea.y; y++)
		{
			for (int x = 0; x < worldArea.x; x++)
			{
				if (x + y * worldArea.x < layer.second.size())
				{
					auto gid = layer.second[x + y * worldArea.x];
					if (gid == 0)
					{
						DrawGraph(x * tileSize.x + offset.x,
							(y * tileSize.y + offset.y),
							itemBoxImg_[(count_/2)%11], true);
					}
				}
			}
		}
	}
	count_++;	
	for (const auto& col : loadMap_.itemBoxGetColList())
	{
		DrawBoxAA(col.first.x + offset.x, col.first.y + offset.y,
			col.second.x + offset.x, col.second.y + offset.y, 0xff0000, false, 5.0f);
	}

}

void ItemBox::Update()
{	
	for (const auto& player :players_)
	{
		for (const auto& col : loadMap_.itemBoxGetColList())
		{
			//if (rayCast_.CheckCollision(col, player->pos_))
			if (rayCast_.RectToRectCollision(player->col_.min_, player->col_.max_,col.first, col.second ))
			{
				if (player->IsItem() == Player::ItemList::NON)
				{
					if (player->TestItem())
					{
						player->SetItemList(1);	
						auto ii = std::make_shared<Missile>();
						ii->grndColList_ = loadMap_.GetColList();
						player->SetItem(ii);
					}
					else
					{
						player->SetItemList(2);
						auto ii = std::make_shared<Laser>();
						player->SetItem(ii);

					}
				}
			}
		}
	}

}

bool ItemBox::IsBox(Vector2DFloat pos)
{
	return false;
}
