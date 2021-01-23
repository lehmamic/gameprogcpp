//
//  PlaneActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef PlaneActor_hpp
#define PlaneActor_hpp

#include "Actor.h"

class PlaneActor : public Actor
{
public:
    PlaneActor(class Game* game);
    ~PlaneActor();
    
    class BoxComponent* GetBox() { return mBox; }
    
private:
    class BoxComponent* mBox;
};

#endif /* PlaneActor_hpp */
