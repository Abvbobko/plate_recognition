//#include "pch.h"
#include "Plate.h"

Plate::Plate(vector<Point> &plateCoordinates)
{
	minPoint = plateCoordinates.at(0);
	maxPoint = plateCoordinates.at(0);

	for (auto &point : plateCoordinates)
	{
		if (point.x < minPoint.x) {
			minPoint.x = point.x;
		}
		else if (point.x > maxPoint.x) {
			maxPoint.x = point.x;
		}

		if (point.y < minPoint.y) {
			minPoint.y = point.y;
		}
		else if (point.y > maxPoint.y) {
			maxPoint.y = point.y;
		}
	}

	height = maxPoint.y - minPoint.y;
	width = maxPoint.x - minPoint.x;
}

bool Plate::operator < (const Plate &otherPlate)// const
{
	return (this->minPoint.x < otherPlate.minPoint.x);
}

bool Plate::operator == (const Plate &otherPlate) //const
{
	return ((this->minPoint.x >= otherPlate.minPoint.x && this->minPoint.x <= otherPlate.maxPoint.x)
		|| (this->maxPoint.x == otherPlate.maxPoint.x));
}