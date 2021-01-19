//
//  OrbiterActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 19.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef OrbiterActor_hpp
#define OrbiterActor_hpp

#include "Actor.h"

class OrbitActor : public Actor {
public:
    OrbitActor(class Game* game);

    void ActorInput(const uint8_t* keys) override;

    void SetVisible(bool visible);
    
private:
    class OrbitCamera* mCameraComp;
    class MeshComponent* mMeshComp;
};

#endif /* OrbiterActor_hpp */
