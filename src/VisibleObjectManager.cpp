#include "stdafx.h"

VisibleObjectManager::VisibleObjectManager()
{
}

VisibleObjectManager::~VisibleObjectManager()
{
	for(auto obj : visibleObjects)
	{
		delete obj.second;
	}
}

void VisibleObjectManager::Add(std::string name, VisibleObject * visibleObject)
{
	visibleObjects.insert(std::pair<const std::string, VisibleObject*>(name, visibleObject));
}

void VisibleObjectManager::Remove(std::string name)
{
	auto itemIterator = visibleObjects.find(name);
	if (itemIterator != visibleObjects.end())
	{
		delete itemIterator->second;
		visibleObjects.erase(name);
	}
}

int VisibleObjectManager::GetObjectCount() const
{
	return visibleObjects.size();
}

VisibleObject * VisibleObjectManager::Get(std::string name) const
{
	const auto item = visibleObjects.find(name);
	if (item != visibleObjects.end())
	{
		return item->second;
	}

	return nullptr;
}

void VisibleObjectManager::DrawAll(rgb_matrix::Canvas * canvas)
{
	for(auto item : visibleObjects)
	{
		item.second->Draw(canvas);
	}
}

void VisibleObjectManager::UpdateAll(std::chrono::system_clock::time_point now)
{
	std::chrono::duration<double> timespan = std::chrono::duration_cast<std::chrono::duration<double>>(now - DisplayMain::lastFrameTime);
	for(auto item : visibleObjects)
	{
		item.second->Update(timespan.count());
	}
}




