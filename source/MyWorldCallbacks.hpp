#pragma once

//#include <Box2D\Box2D.h>
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2WorldCallbacks.h>

#include "Gameplay.h"
#include "sltn.h"
#include "Enemy.h"
#include "Bullet.h"

class MyContactListener : public b2ContactListener

{

public:

	void BeginContact(b2Contact* contact)
	{ /* handle begin event */
	}



	void EndContact(b2Contact* contact)
	{ /* handle end event */
	}



	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{ /* handle pre-solve event */
	}



	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{ /* handle post-solve event */
		//return;
		if (*impulse->normalImpulses > 120.0f)
		{
			// Detroy stuff
			auto udA = (UserData*)contact->GetFixtureA()->GetBody()->GetUserData();
			if (udA == nullptr) return;
			auto udB = (UserData*)contact->GetFixtureB()->GetBody()->GetUserData();


			if ((udA->kind == UserData::bullet || udB->kind == UserData::bullet)){
				if (udA->kind == UserData::bullet){
					Gameplay::getInst().EnqueueRemoveFromList(udA->creator);
					sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
				}
				if (udB->kind == UserData::bullet){
					Gameplay::getInst().EnqueueRemoveFromList(udB->creator);
					sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
					//sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
				}
				if ((udA->kind == UserData::ball || udB->kind == UserData::ball)){

					//Gameplay::getInst().EnqueueRemoveFromList(udB->creator);
					// this can be called multiple times in one frame
					sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
					//sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
					//Gameplay::getInst().Remove( ( ud )->creator );
				}


				if (udA->kind == UserData::Enemy) {
					auto bullet = ((Bullet*)udB->creator);
					((Ball*)udA->creator)->DoDammage(bullet->GetDammage());
					bullet->StartToDie();
					//sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
				}

				if (udB->kind == UserData::Enemy) {
					auto bullet = ((Bullet*)udA->creator);
					((Ball*)udB->creator)->DoDammage(bullet->GetDammage());
					bullet->StartToDie();
					//sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
				}

			}

		}

	}

};

// Handle implicit destroyed objects
class MyDestructionListener : public b2DestructionListener
{
	void 	SayGoodbye(b2Joint *joint)
	{
		auto ud = (UserData*)joint->GetUserData();
	}

	void 	SayGoodbye(b2Fixture *fixture)
	{
		auto ud = (UserData*)fixture->GetUserData();
	}

};