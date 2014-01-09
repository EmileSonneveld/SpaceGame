#pragma once

class entityBase;

#include <memory>
#include <Box2D/Common/b2Settings.h>

struct UserData
{
	// ID an unique bitfield
	enum  Kind : short{
		unspecified = 0,
		player = 0x0004,
		bullet = 0x0008,
		ball = 0x0001,
		Enemy = 0x0010,
		SpriteAnimation = -0x0040,
		Static= 0x1000,
		Pickup= 0x2000
	};
#ifdef _DEBUG
	static const unsigned int magicSize = 11;
	char memoryMagic[magicSize]; // view object in memory
#endif
	Kind kind;
	bool isCore;
	bool isConectedToCluster;
	//unsigned int clusterNumber;
	entityBase *const creator;
	unsigned int cluster;

	UserData(entityBase* cr, Kind knd = UserData::unspecified) : creator(cr), 
		isConectedToCluster(false), kind(knd), isCore(false)
	{
		//typeid(isConectedToCluster).raw_name();
#ifdef _DEBUG
		memcpy(memoryMagic, "UserData ;)", magicSize);
#endif
	}

private:
	// Disabling default copy constructor and default assignment operator.
	UserData(const UserData& yRef);
	UserData& operator=(const UserData& yRef);
};
