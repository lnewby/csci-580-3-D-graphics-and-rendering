#include "stdafx.h"
#include "DDA.h"


DDA::DDA(GzCoord start, GzCoord end)
{
	setStart(start);
	setEnd(end);
	setCurrent(start);
	setSlopeX();
	setSlopeZ();
}


DDA::~DDA() {}

void DDA::setStart(GzCoord start) {
	this->start[X] = start[X];
	this->start[Y] = start[Y];
	this->start[Z] = start[Z];
};
void DDA::setEnd(GzCoord end) {
	this->end[X] = end[X];
	this->end[Y] = end[Y];
	this->end[Z] = end[Z];
};
void DDA::setCurrent(GzCoord start) {
	this->current[X] = start[X];
	this->current[Y] = start[Y];
	this->current[Z] = start[Z];
};
GzCoord* DDA::getCurrent() {
	return (GzCoord *) current;
}
void DDA::setSlopeX() {
	slopeZ = slope(start, end, X, Y);
};
void DDA::setSlopeZ() {
	slopeZ = slope(start, end, Z, Y);
};