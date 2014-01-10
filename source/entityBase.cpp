#include "entityBase.h"
#include <Box2D/Dynamics/b2Fixture.h>

entityBase::entityBase() : sf::Sprite(), m_b2Body(nullptr)
{
#ifdef _DEBUG
	memcpy(memoryMagic, "entityBase", magicSize);
#endif
}
void entityBase::setFilterGroup(int16 group){
	//bool collide =
	//	(filterA.maskBits & filterB.categoryBits) != 0 &&
	//	(filterA.categoryBits & filterB.maskBits) != 0;

	b2Filter filter;
	filter.groupIndex = group;
	m_b2Body->GetFixtureList()->SetFilterData(filter);
}

void entityBase::setFilter(int16 categoryBits, int16 maskBits)
{
	b2Filter filter;
	filter.categoryBits = categoryBits;
	filter.maskBits = maskBits;
	m_b2Body->GetFixtureList()->SetFilterData(filter);
}

void entityBase::setFilter(b2Filter filter)
{
	m_b2Body->GetFixtureList()->SetFilterData(filter);
}

int16 entityBase::getFilterGroup(){
	return m_b2Body->GetFixtureList()->GetFilterData().groupIndex;
}
