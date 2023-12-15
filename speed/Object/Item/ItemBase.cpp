#include "ItemBase.h"

ItemBase::ItemBase()
{
}

 ItemBase::~ItemBase()
{
}

void ItemBase::Update()
{
}

void ItemBase::Draw(Vector2DFloat offset)
{
}

void ItemBase::Use(Vector2DFloat pos)
{
	itemPos_ = pos;
}
