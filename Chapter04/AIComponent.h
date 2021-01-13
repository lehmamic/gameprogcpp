//
//  AIComponent.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 13.01.21.
//  Copyright © 2021 Michael Lehmann. All rights reserved.
//

#ifndef AIComponent_hpp
#define AIComponent_hpp

#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent : public Component {
public:
    AIComponent(class Actor* owner);
    
    void Update(float deltaTime) override;
    void ChangeState(const std::string& name);
    
    // Add a new state to the map
    void RegisterState(class AIState* state);
    
private:
    // Maps name of state to AIState instance
    std::unordered_map<std::string, class AIState*> mStateMap;
    
    // Current state we're in
    class AIState* mCurrentState;
};

#endif /* AIComponent_hpp */
