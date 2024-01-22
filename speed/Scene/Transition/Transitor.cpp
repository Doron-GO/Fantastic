#include "Transitor.h"

Transitor::~Transitor()
{
	DeleteGraph(oldRT_);
	DeleteGraph(newRT_);
}

void Transitor::Start()
{
	VECTOR size = { 1600,1000 };

	oldRT_ = MakeScreen(size.x, size.y);
	newRT_ = MakeScreen(size.x, size.y);

	int result = GetDrawScreenGraph(0, 0, size.x, size.y, oldRT_);
	frame_ = 0;
}


bool Transitor::IsEnd() const
{
	return frame_ >= interval_;
}
