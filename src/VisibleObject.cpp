#include "stdafx.h"


VisibleObject::VisibleObject() :
	_isLoaded(false)
{
}


VisibleObject::~VisibleObject()
{

}

void VisibleObject::Load(std::string filename, int x, int y)
{
	char * cstyleFilename = &filename[0];
	std::unique_ptr<FILE> inputFile(fopen(cstyleFilename, "re"));

	if (inputFile == nullptr)
	{
		return;
	}

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, inputFile.get()); // read the 54-byte header

	// extract image height and width from header
	const int width = *(int*)&info[18];
	const int height = *(int*)&info[22];

	const int row_padded = (width * 3 + 3) & (~3);
	auto* data = new unsigned char[row_padded];

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, inputFile.get());
		ImageData.emplace_back();
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			rgb_matrix::Color pixel(data[j + 2], data[j + 1], data[j]);
			ImageData[i].push_back(pixel);
		}
	}

	fclose(inputFile.get());
	PositionX = x;
	PositionY = y;
	_isLoaded = true;
}

void VisibleObject::Draw(rgb_matrix::Canvas* canvas)
{
	if (_isLoaded)
	{
		int canvasY = PositionY;
		int canvasX = PositionX;
		// draw this onto canvas
		for (auto row : ImageData)
		{
			for(auto pixel : row)
			{
				if (canvasX >= 0 && canvasX <= DisplayMain::SCREEN_WIDTH && canvasY >= 0 && canvasY <= DisplayMain::SCREEN_HEIGHT)
				{
					canvas->SetPixel(canvasX, canvasY, pixel.r, pixel.g, pixel.b);
				}

				canvasX++;
			}
			canvasY++;
		}
	}
}


void VisibleObject::Update(double elapsedTime)
{
}

void VisibleObject::SetPosition(double x, double y)
{
	if (_isLoaded)
	{
		PositionX = x;
		PositionY = y;
	}
}

Point VisibleObject::GetPosition()
{
	Point p{ -1, -1 };

	if (_isLoaded)
	{
		p.X = PositionX;
		p.Y = PositionY;
	}

	return p;
}

float VisibleObject::GetHeight() const
{
	return ImageData.size();
}

float VisibleObject::GetWidth() const
{
	return ImageData[0].size();
}

bool VisibleObject::IsLoaded() const
{
	return _isLoaded;
}