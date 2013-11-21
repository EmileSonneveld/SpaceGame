#pragma once

#include <Box2D\Box2D.h>
#include "Gameplay.h"
#include "sltn.h"

//MainClass* g_mainClass;

class MyContactListener : public b2ContactListener

{

public:

    void BeginContact(b2Contact* contact)

    { /* handle begin event */ }



    void EndContact(b2Contact* contact)

    { /* handle end event */ 
    }



    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)

    { /* handle pre-solve event */ }



    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)

    { /* handle post-solve event */ 
        if( *impulse->normalImpulses > 50.0f )
        {
            // Detroy stuff
            auto ud= (UserData*)contact->GetFixtureA()->GetBody()->GetUserData();
            Gameplay::getInst().Remove( ( ud )->creator );
        }
    }

};
