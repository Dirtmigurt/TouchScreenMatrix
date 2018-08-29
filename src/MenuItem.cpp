#include "stdafx.h"

void MenuItem::Update(double elapsedTime)
{
	// There is no action to take on menu items, they don't move or change colors
	// but they can in the future!
}

bool MenuItem::IsClicked(int x, int y)
{
	Point p = GetPosition();
	const int width = GetWidth();
	const int height = GetHeight();

	return p.X <= x && p.X + width >= x && p.Y <= y && p.Y + height >= y;
}
