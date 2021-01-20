//
//  AudioComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 16.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef AudioComponent_hpp
#define AudioComponent_hpp

#include "Component.h"
#include "SoundEvent.h"
#include <vector>
#include <string>

class AudioComponent : public Component
{
public:
    AudioComponent(class Actor* owner, int updateOrder = 200);
    ~AudioComponent();
    
    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;
    
    SoundEvent PlayEvent(const std::string& name);
    void StopAllEvents();
    
private:
    std::vector<SoundEvent> mEvents2D;
    std::vector<SoundEvent> mEvents3D;
};

#endif /* AudioComponent_hpp */
