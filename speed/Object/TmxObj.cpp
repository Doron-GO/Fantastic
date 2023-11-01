#include "TmxObj.h"
#include<sstream>
#include"GetAtr.h"
#include"ImageMng.h"

bool TmxObj::LoadTSX(std::string fileName)
{
	//�t�@�C�����J��
	rapidxml::file<> xmlFile = fileName.c_str();

	//�p�[�T�[�ɓn���@�@tsxDoc_�ŐG���悤�ɂȂ�
	tsxDoc_.parse<0>(xmlFile.data());

	//tsxDoc_����A�m�[�h���Ƃ�
	rapidxml::xml_node<>* tileset = tsxDoc_.first_node("tileset");
	if (tileset == nullptr)
	{
		return false;
	}

	int tilewidth;
	int tilecount;
	int tileheight;
	int columns;

	auto getAtr = GetAtr();
	if (!getAtr(tileset, "tilewidth", tilewidth))
	{
		return false;
	}
	if (!getAtr(tileset, "tileheight", tileheight))
	{
		return false;
	}
	if (!getAtr(tileset, "tilecount", tilecount))
	{
		return false;
	}
	if (!getAtr(tileset, "columns", columns))
	{
		return false;
	}

	rapidxml::xml_node<>* image = tileset->first_node("image");
	if (image == nullptr)
	{
		return false;
	}
	rapidxml::xml_attribute<>* source = image->first_attribute("source");
	if (source == nullptr) {

		return false;
	}

	std::string sourcePass = source->value();
	mapKey_ = "mapChip";
	lpImageMng.GetID("./tmx" + sourcePass.substr(1), mapKey_, { tilewidth, tileheight }, { columns ,tilecount / columns });
	return true;
}

bool TmxObj::LoadTMX(std::string fileName)
{
	//�t�@�C�����J��
	rapidxml::file<> xmlFile = fileName.c_str();

	//�p�[�T�[�ɓn���@�@tsxDoc_�ŐG���悤�ɂȂ�
	tsxDoc_.parse<0>(xmlFile.data());

	rapidxml::xml_node<>* mapNode = tsxDoc_.first_node("map");
	if (mapNode == nullptr)
	{
		return false;
	}

	auto tileset = mapNode->first_node("tileset");
	if (tileset == nullptr)
	{
		return false;
	}

	auto tsxSource = tileset->first_attribute("source");
	if (tileset == nullptr)
	{
		return false;
	}



	auto getAtr = GetAtr();
	if (!getAtr(mapNode, "nextlayerid", layerSize_))
	{
		return false;

	}
	layerSize_--;

	if (!getAtr(mapNode, "width", WorldArea_.x))
	{
		return false;

	}
	if (!getAtr(mapNode, "height", WorldArea_.y))
	{
		return false;

	}

	if (!getAtr(mapNode, "tilewidth", tileSize_.x))
	{
		return false;

	}
	if (!getAtr(mapNode, "tileheight", tileSize_.y))
	{
		return false;

	}
	auto filePass = fileName.substr(0, fileName.find_last_of("/") + 1);
	LoadTSX(filePass + tsxSource->value());

	if (!SetMap(mapNode))
	{
		return false;
	}
	return true;
}

bool TmxObj::SetMap(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		//TRACE("mapNode�擾�G���[\n");

		return false;

	}

	auto tileset = node->first_node("tileset");

	if (tileset == nullptr)
	{
		//TRACE("mapNode�擾�G���[\n");

		return false;
	}
	auto getAtr = GetAtr();
	int firstgid;
	getAtr(tileset, "firstgid", firstgid);

	//���C���[���ꂼ�ꂪ�s����܂�
	for (auto layerNode = node->first_node("layer");
		layerNode != nullptr;
		layerNode = layerNode->next_sibling()
		)
	{

		if (layerNode == nullptr)
		{
			//TRACE("layerNode�擾�G���[\n");

			return false;

		}
		auto getAtr = GetAtr();
		std::string layerName;
		getAtr(layerNode, "name", layerName);

		if (layerName == "col")
		{
			continue;
		}


		Vector2D layerSize;
		getAtr(layerNode, "width", layerSize.x);
		getAtr(layerNode, "height", layerSize.y);

		//�}�b�v��World�T�C�Y��
		if (WorldArea_ != layerSize)
		{
			return false;
		}

		auto layerData = mapData.try_emplace(layerName);
		if (layerData.second)
		{
			layerData.first->second.resize(WorldArea_.x * WorldArea_.y);
		}
		auto dataNode = layerNode->first_node("data");
		if (dataNode == nullptr)
		{
			//TRACE("dataNode�擾�G���[\n");
			return false;
		}
		//�f�[�^�̕�������΂炵�ăf�[�^����}�b�v�̏ڂ��������i�[����	
		auto csvData = dataNode->first_node(); //csv�̃f�[�^���擾
		std::stringstream dataStream;
		dataStream << csvData->value();					//�f�[�^�𗬂����� : << �C�R�[���݂����Ȃ���
		//TRACE("%s", csvData->value());
		if (csvData == nullptr)
		{
			//TRACE("csvNode�擾�G���[\n");
			return false;
		}
		for (auto& vecData : layerData.first->second)
		{
			std::string numStr;
			std::getline(dataStream, numStr, ',');
			vecData = atoi(numStr.c_str()) - firstgid;
		}
	}
	for (auto colNode = node->first_node("objectgroup");
		colNode != nullptr;
		colNode = colNode->next_sibling())
	{

		std::string grpName;
		getAtr(colNode, "name", grpName);
		if (grpName != "col")
		{
			continue;
		}
		for (auto objNode = colNode->first_node("object");
			objNode != nullptr;
			objNode = objNode->next_sibling())
		{
			//	�R���W�����̃f�[�^���Ƃ�
			float x, y, w, h;
			getAtr(objNode, "x", x);
			getAtr(objNode, "y", y);
			getAtr(objNode, "width", w);
			getAtr(objNode, "height", h);
			colList_.push_back(Collision{ Vector2DFloat{x,y},Vector2DFloat{w,h} });
		}
	}
	return true;
}

const Vector2D& TmxObj::GetWorldArea(void)
{
	return WorldArea_;
}

const Vector2D& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const MapData& TmxObj::GetMapData(void)
{
	return mapData;
}

const std::string& TmxObj::GetMapKey(void)
{
	return mapKey_;
}

const ColList& TmxObj::GetColList(void)
{
	return colList_;
}
