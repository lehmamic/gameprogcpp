//
//  NavComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef NavComponent_hpp
#define NavComponent_hpp

#include "MoveComponent.h"
#include "Math.h"

class NavComponent : public MoveComponent {
public:
    // Lower update order to update first
    NavComponent(Actor* owner, int updateOrder = 10);
    
    void Update(float deltaTime) override;
    void StartPath(const class Tile* start);
    void TurnTo(const Vector2& pos);
    
private:
    const class Tile* mNextNode;
};

#endif /* NavComponent_hpp */
