#pragma once

//#include <Box2D\Box2D.h>
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2WorldCallbacks.h>

#include "Gameplay.h"
#include "Sltn.h"
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
		if (*impulse->normalImpulses > 150.0f)
		{
			auto bodyA = contact->GetFixtureA()->GetBody();
			auto bodyB = contact->GetFixtureB()->GetBody();

			// Detroy stuff
			auto udA = (UserData*)bodyA->GetUserData();
			if (udA == nullptr) return;
			auto udB = (UserData*)contact->GetFixtureB()->GetBody()->GetUserData();


			if ((udA->kind == UserData::player || udB->kind == UserData::player)){
				FMOD::Sound* sound;
				if (*impulse->normalImpulses > 100.0f)
					sound = Sltn::getInst().getSound("resources/MetalRicochet1.wav");
				else
					sound = Sltn::getInst().getSound("resources/MetalRicochet2.wav");
				Sltn::getInst().playSound(sound);

				if (udA->kind == UserData::Pickup){
					Gameplay::getInst().MakeCircle(to_Vector2(bodyA->GetPosition()), 6, 3);
					Sltn::getInst().EnqueDestroyPhysicsEntity(bodyA);
					//Gameplay::getInst().EnqueueRemoveFromList(udA->creator);
				}
				else if (udB->kind == UserData::Pickup){
					Gameplay::getInst().MakeCircle(to_Vector2(bodyB->GetPosition()), 6, 3);
					Sltn::getInst().EnqueDestroyPhysicsEntity(bodyB);
					//Gameplay::getInst().EnqueueRemoveFromList(udB->creator);
				}

			}

			if ((udA->kind == UserData::bullet || udB->kind == UserData::bullet)){
				if (udA->kind == UserData::bullet){
					Gameplay::getInst().EnqueueRemoveFromList(udA->creator);
					Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
				}
				if (udB->kind == UserData::bullet){
					Gameplay::getInst().EnqueueRemoveFromList(udB->creator);
					Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
					//Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
				}
				if ((udA->kind == UserData::Ball || udB->kind == UserData::Ball)){

					//Gameplay::getInst().EnqueueRemoveFromList(udB->creator);
					// this can be called multiple times in one frame
					Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
					//Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
					//Gameplay::getInst().Remove( ( ud )->creator );
				}


				if (udA->kind == UserData::Enemy) {
					auto bullet = ((Bullet*)udB->creator);
					((BallBase*)udA->creator)->DoDammage(bullet->GetDammage());
					bullet->StartToDie();
					//Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureB()->GetBody());
				}

				if (udB->kind == UserData::Enemy) {
					auto bullet = ((Bullet*)udA->creator);
					((BallBase*)udB->creator)->DoDammage(bullet->GetDammage());
					bullet->StartToDie();
					//Sltn::getInst().EnqueDestroyPhysicsEntity(contact->GetFixtureA()->GetBody());
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