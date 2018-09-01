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

	unsigned char info[1024];
	fread(info, sizeof(unsigned char), 14, inputFile.get()); // read the 14-byte header

	const int headerLength = (*(int*)&info[10]) - 14;

	fread(info, sizeof(unsigned char), headerLength, inputFile.get()); // read the n-byte header
	// extract image height and width from header
	const int width = *(int*)&info[4];
	const int height = *(int*)&info[8];

	printf("Loaded %s with Width=%d, Height=%d\n", cstyleFilename, width, height);
	//const int row_padded = (width * 3 + 3) & (~3);
	const int row_padded = (width * 3 + 3) & (~3);
	auto* data = new unsigned char[row_padded];

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, inputFile.get());
		ImageData.emplace_back();
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			rgb_matrix::Color pixel(data[j+2], data[j + 1], data[j]);
			printf("X = %d, Y = %d, Color(RGB) = %d, %d, %d\n", j/3, i, pixel.r, pixel.g, pixel.b);
			ImageData[i].push_back(pixel);
		}
	}

	// reverse the order of the rows since Bitmaps put the origin 0,0 pixel at bottom left instead of top left.
	std::reverse(ImageData.begin(), ImageData.end());
	PositionX = x;
	PositionY = y;
	_isLoaded = true;
}

void VisibleObject::Draw(rgb_matrix::Canvas* canvas)
{
	if (_isLoaded)
	{
		int canvasY = PositionY;
		// draw this onto canvas
		for (auto row : ImageData)
		{
			int canvasX = PositionX;
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