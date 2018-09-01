#pragma once
#include "stdafx.h"


class VisibleObjectManager
{
public:
	VisibleObjectManager();
	~VisibleObjectManager();

	void Add(std::string name, VisibleObject* visibleObject);
	void Remove(std::string name);
	int GetObjectCount() const;
	VisibleObject* Get(std::string name) const;

	void DrawAll(rgb_matrix::Canvas* canvas);
	void UpdateAll(std::chrono::system_clock::time_point now);

private:
	std::map<std::string, VisibleObject*> visibleObjects;
};
