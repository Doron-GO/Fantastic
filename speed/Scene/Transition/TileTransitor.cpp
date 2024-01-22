#include "TileTransitor.h"

TileTransitor::TileTransitor(int cellSize, int interval) :cellSize_(cellSize),
Transitor(interval)
{
	VECTOR wsize = { 1600,1000 };
	int xnum = (wsize.x / cellSize_) + 1;
	int ynum = (wsize.y / cellSize_) + 1;
	tiles_.reserve(xnum * ynum);
	for (int yidx = 0; yidx < ynum; ++yidx) {
		for (int xidx = 0; xidx < xnum; ++xidx) {
			tiles_.push_back({ xidx,yidx });
		}
	}
	std::shuffle(tiles_.begin(), tiles_.end(), mt_);
}
void TileTransitor::Update()
{
	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
	if (IsEnd()) {
		return;
	}

	VECTOR wsize = { 1600,1000 };
	int xnum = (wsize.x / cellSize_) + 1;
	int ynum = (wsize.y / cellSize_) + 1;
	int eraseNum = ((xnum * ynum) / interval_);
	if (tiles_.size() > eraseNum) {
		tiles_.erase(tiles_.end() - eraseNum, tiles_.end());
	}
	else {
		tiles_.clear();
	}
}

void TileTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	VECTOR wsize = { 1600,1000 };
	auto rate = (float)frame_ / (float)interval_;
	DrawRectGraph(0, 0, 0, 0, wsize.x, wsize.y, newRT_, true);
	for (const auto& cell : tiles_) {
		DrawRectGraph(
			cell.xidx * cellSize_,
			cell.yidx * cellSize_,
			cell.xidx * cellSize_,
			cell.yidx * cellSize_,
			cellSize_, cellSize_,
			oldRT_, true);
	}
}
