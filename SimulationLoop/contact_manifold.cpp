#include "contact_manifold.h"
std::mutex contact_manifold::_manifoldMutex;
contact_manifold::contact_manifold() : m_numberOfPoints(0)
{
}

contact_manifold::~contact_manifold() = default;

void contact_manifold::add(ManifoldP & point)
{
	std::lock_guard<std::mutex> lock(_manifoldMutex);
	m_points.push_back(point);
	m_numberOfPoints++;
}

void contact_manifold::Clear()
{
	std::lock_guard<std::mutex> lock(_manifoldMutex);
	m_points.clear();
	m_numberOfPoints = 0;
}

unsigned int contact_manifold::GetNumberOfPoints() const
{
	//std::lock_guard<std::mutex> lock(_manifoldMutex);
	return m_numberOfPoints;
}

ManifoldP& contact_manifold::GetPoint(int index)
{
	//std::lock_guard<std::mutex> lock(_manifoldMutex);
	return m_points[index];}
