//
//  TargetComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 24.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef TargetComponent_hpp
#define TargetComponent_hpp

#include "Component.h"

class TargetComponent : public Component
{
public:
    TargetComponent(class Actor* owner);
    ~TargetComponent();
};

#endif /* TargetComponent_hpp */
