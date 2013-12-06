#pragma once

#include <Box2D\Box2D.h>
#include "Gameplay.h"
#include "sltn.h"


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
            if( (udA->kind== UserData::bullet && udB->kind == UserData::ball)
                ||(udB->kind== UserData::bullet && udA->kind == UserData::ball)){
                    // this can be called multiple times in one frame
                    sltn::getInst().EnqueDestroyBody(contact->GetFixtureA()->GetBody());
                    sltn::getInst().EnqueDestroyBody(contact->GetFixtureB()->GetBody());
                    //Gameplay::getInst().Remove( ( ud )->creator );
            }
        }
    }
};
