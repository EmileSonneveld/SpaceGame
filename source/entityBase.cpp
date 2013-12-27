#include "entityBase.h"
#include <Box2D/Dynamics/b2Fixture.h>

void entityBase::setFilterGroup(int16 group){
	b2Filter filter;
	filter.groupIndex = group;
	m_b2Body->GetFixtureList()->SetFilterData(filter);
}

int16 entityBase::getFilterGroup(){
	return m_b2Body->GetFixtureList()->GetFilterData().groupIndex;
}
