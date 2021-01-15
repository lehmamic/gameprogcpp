//
//  CameraActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef CameraActor_hpp
#define CameraActor_hpp


#include "Actor.h"

class CameraActor : public Actor
{
public:
    CameraActor(class Game* game);
    
    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;
    
private:
    class MoveComponent* mMoveComp;
};

#endif /* CameraActor_hpp */
