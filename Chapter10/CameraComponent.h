//
//  CameraComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
    CameraComponent(class Actor* owner, int updateOrder = 200);
    
protected:
    void SetViewMatrix(const Matrix4& view);
};

#endif /* CameraComponent_hpp */
