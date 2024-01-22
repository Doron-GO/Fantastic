#include "StripTransitor.h"

StripTransitor::StripTransitor(int width, int interval)
{
}

void StripTransitor::Update()
{
	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void StripTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	VECTOR wsize = { 1600,1000 };
	auto rate = (float)frame_ / (float)interval_;
	int lp = (wsize.x / width_) + 1;
	DrawRectGraph(0, 0, 0, 0, wsize.x, wsize.y, oldRT_, true);
	for (int i = 0; i < lp; ++i) {
		if (i % 2 == 0) {
			DrawRectGraph(i * width_, wsize.y * (rate - 1.0f), i * width_, 0, width_, wsize.y, newRT_, true);
		}
		else {
			DrawRectGraph(i * width_, wsize.y * (1.0f - rate), i * width_, 0, width_, wsize.y, newRT_, true);
		}
	}
}
