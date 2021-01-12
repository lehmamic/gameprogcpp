//
//  Asteroid.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef Asteroid_hpp
#define Asteroid_hpp

#include "Actor.h"

class Asteroid : public Actor
{
public:
    Asteroid(Game *game);
    ~Asteroid();
};

#endif /* Asteroid_hpp */
