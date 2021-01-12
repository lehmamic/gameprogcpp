//
//  CircleComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 12.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef CircleComponent_hpp
#define CircleComponent_hpp

#include "Component.h"
#include "Math.h"

class CircleComponent : public Component {
public:
    CircleComponent(class Actor* owner);
    
    void SetRadius(float radius) { mRadius = radius; }
    float GetRadius() const;
    
    const Vector2& GetCenter() const;
    
private:
    float mRadius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);

#endif /* CircleComponent_hpp */
