#pragma once
class entityBase;

#include <memory>

struct UserData
{
	enum Kind {
		unspecified = 0,
		player = -100,
		bullet = -101,
		ball = -102,
		Enemy = -103,
		SpriteAnimation = -104,
	};
	static const unsigned int magicSize = 8;
	char memoryMagic[magicSize]; // view object in memory
	Kind kind;
	bool isConectedToCluster;
	//unsigned int clusterNumber;
	entityBase *const creator;
	unsigned int cluster;

	UserData(entityBase* cr, Kind knd = UserData::unspecified) : creator(cr), isConectedToCluster(false), kind(knd)
	{
		//typeid(isConectedToCluster).raw_name();
#ifdef _DEBUG

		memcpy(memoryMagic, "UserData", magicSize);
#endif
	}
};
