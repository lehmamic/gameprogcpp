//
//  SplineActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 19.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef SplineActor_hpp
#define SplineActor_hpp

#include "Actor.h"

class SplineActor : public Actor
{
public:
    SplineActor(class Game* game);

    void ActorInput(const uint8_t* keys) override;

    void RestartSpline();
    
private:
    class SplineCamera* mCameraComp;
};

#endif /* SplineActor_hpp */
