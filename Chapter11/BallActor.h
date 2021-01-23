//
//  BallActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef BallActor_hpp
#define BallActor_hpp

#include "Actor.h"

class BallActor : public Actor
{
public:
    BallActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void SetPlayer(Actor* player);

    void HitTarget();
    
private:
    class AudioComponent* mAudioComp;
    class BallMove* mMyMove;
    float mLifeSpan;
};

#endif /* BallActor_hpp */
