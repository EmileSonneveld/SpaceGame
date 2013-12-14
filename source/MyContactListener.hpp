#pragma once

#include <Box2D\Box2D.h>
#include "Gameplay.h"
#include "sltn.h"
#include "Enemy.h"

class MyContactListener : public b2ContactListener

{

public:

	void BeginContact(b2Contact* contact)
	{ /* handle begin event */ }


	void EndContact(b2Contact* contact)
	{ /* handle end event */   }


	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{ /* handle pre-solve event */ }


	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{ /* handle post-solve event */ 
		if( *impulse->normalImpulses > 150.0f )
		{
			// Detroy stuff
			auto udA= (UserData*)contact->GetFixtureA()->GetBody()->GetUserData();
			if(udA == nullptr ) return;
			auto udB= (UserData*)contact->GetFixtureB()->GetBody()->GetUserData();


			if(    (udA->kind== UserData::bullet || udB->kind== UserData::bullet) ){

				if( (udA->kind == UserData::ball || udB->kind == UserData::ball)){
					// this can be called multiple times in one frame
					sltn::getInst().EnqueDestroyBody(contact->GetFixtureA()->GetBody());
					sltn::getInst().EnqueDestroyBody(contact->GetFixtureB()->GetBody());
					//Gameplay::getInst().Remove( ( ud )->creator );
				}

				if( udA->kind == UserData::Enemy) {
					( (Enemy*)udA->creator )->DoDammage(5); 
					sltn::getInst().EnqueDestroyBody(contact->GetFixtureB()->GetBody());
				}
				if( udB->kind == UserData::Enemy) {
					( (Enemy*)udB->creator )->DoDammage(5); 
					sltn::getInst().EnqueDestroyBody(contact->GetFixtureA()->GetBody());
				}
			}
		}
	}
};
