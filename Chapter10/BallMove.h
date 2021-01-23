//
//  BallMove.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 23.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef BallMove_hpp
#define BallMove_hpp

#include "MoveComponent.h"

class BallMove : public MoveComponent
{
public:
    BallMove(class Actor* owner);

    void SetPlayer(Actor* player) { mPlayer = player; }
    void Update(float deltaTime) override;
    
protected:
    class Actor* mPlayer;
};

#endif /* BallMove_hpp */
