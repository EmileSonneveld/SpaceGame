#include "entityBase.h"


void Ball::setFilterGroup(int16 group){
	b2Filter filter;
	filter.groupIndex = group;
	m_b2Body->GetFixtureList()->SetFilterData(filter);
}