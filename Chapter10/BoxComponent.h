//
//  BoxComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 22.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef BoxComponent_hpp
#define BoxComponent_hpp

#include "Component.h"
#include "Collision.h"

class BoxComponent : public Component
{
public:
    BoxComponent(class Actor* owner, int updateOrder = 100);
    ~BoxComponent();
    
    void OnUpdateWorldTransform() override;
    
    void SetObjectBox(const AABB& model) { mObjectBox = model; }
    const AABB& GetWorldBox() const { return mWorldBox; }
    
    void SetShouldRotate(bool value) { mShouldRotate = value; }
    
private:
    AABB mObjectBox;
    AABB mWorldBox;
    bool mShouldRotate;
};

#endif /* BoxComponent_hpp */
