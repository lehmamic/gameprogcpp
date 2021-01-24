//
//  FollowActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef FollowActor_hpp
#define FollowActor_hpp
#include "Actor.h"

class FollowActor : public Actor
{
public:
    FollowActor(class Game* game);

    void ActorInput(const uint8_t* keys) override;

    void SetVisible(bool visible);
    
private:
    class MoveComponent* mMoveComp;
    class FollowCamera* mCameraComp;
    class MeshComponent* mMeshComp;
};

#endif /* FollowActor_hpp */
