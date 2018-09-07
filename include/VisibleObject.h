#pragma once
#include "Point.h"

class VisibleObject
{
public:
	VisibleObject();
	virtual ~VisibleObject();

	virtual void Load(std::string filename, int = 0, int = 0);
	virtual void Load(rgb_matrix::Color, int = 0, int = 0);
	virtual void Draw(rgb_matrix::Canvas* canvas, rgb_matrix::Color whiteReplace = rgb_matrix::Color(0, 0, 0));
	virtual void Update(double elapsedTime);

	virtual void SetPosition(double x, double y);
	virtual Point GetPosition();
	virtual float GetWidth() const;
	virtual float GetHeight() const;
	virtual rgb_matrix::Color GetPixel(uint, uint);

	virtual bool IsLoaded() const;

private:
	std::vector<std::vector<rgb_matrix::Color>> ImageData;
	double PositionX;
	double PositionY;
	std::string _filename;
	bool _isLoaded;
};
