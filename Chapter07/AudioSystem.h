//
//  AudioSystem.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef AudioSystem_hpp
#define AudioSystem_hpp

#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

// Forward declarations to avoid including FMOD header
namespace FMOD
{
    class System;
    namespace Studio
    {
        class Bank;
        class EventDescription;
        class EventInstance;
        class System;
        class Bus;
    };
};

class AudioSystem
{
public:
    AudioSystem(class Game* game);
    ~AudioSystem();
    
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    
    // Load/unload banks
    void LoadBank(const std::string& name);
    void UnloadBank(const std::string& name);
    void UnloadAllBanks();
    
    SoundEvent PlayEvent(const std::string& name);
    
protected:
    friend class SoundEvent;
    FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
    
private:
    // Tracks the next ID to use for event instances
    static unsigned int sNextID;
    
    class Game* mGame;
    
    // Map of loaded banks
    std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
    
    // Map of event name to EventDescription
    std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
    
    // Map of event id to EventInstance
    std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
    
    // FMOD studio system
    FMOD::Studio::System* mSystem;
    
    // FMOD Low-level system (in case needed)
    FMOD::System* mLowLevelSystem;
};

#endif /* AudioSystem_hpp */
