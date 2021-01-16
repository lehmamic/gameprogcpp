//
//  AudioSystem.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 15.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "AudioSystem.h"
#include <SDL/SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
    :mGame(game)
    ,mSystem(nullptr)
    ,mLowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
    // Initialize debug logging
    FMOD::Debug_Initialize(
       FMOD_DEBUG_LEVEL_ERROR,  // Log only errors
       FMOD_DEBUG_MODE_TTY);    // Output to stdout
    
    // Create FMOD studio system object
    FMOD_RESULT result;
    result = FMOD::Studio::System::create(&mSystem);
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
    }
    
    // Initialize FMOD studio system
    result = mSystem->initialize(
        512,                        // Max number of concurrent sounds
        FMOD_STUDIO_INIT_NORMAL,    // Use default settings
        FMOD_INIT_NORMAL,           // Use default settings
        nullptr);                   // Usually null
    if (result != FMOD_OK)
    {
        SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
        return false;
    }
    
    // Save the low-level system pointer
    mSystem->getLowLevelSystem(&mLowLevelSystem);
    
    // Load the master banks (strings first)
    LoadBank("Assets/Master Bank.strings.bank");
    LoadBank("Assets/Master Bank.bank");
    
    return true;
}

void AudioSystem::Shutdown()
{
    mSystem->release();
}

void AudioSystem::LoadBank(const std::string &name)
{
    // Prevent double-loading
    if (mBanks.find(name) != mBanks.end())
    {
        return;
    }
    
    // Try to load bank
    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT result = mSystem->loadBankFile(
        name.c_str(),                   // File name of bank
        FMOD_STUDIO_LOAD_BANK_NORMAL,   // Normal loading
        &bank);                         // Save pointer to bank
    
    const int maxPathLength = 512;
    if (result == FMOD_OK)
    {
        // Add bank to map
        mBanks.emplace(name, bank);
        
        // Load all non-streaming sample data
        bank->loadSampleData();
        
        // Get the number of events in the this bank
        int numEvents = 0;
        bank->getEventCount(&numEvents);
        if (numEvents > 0)
        {
            // Get list of event descriptions in this bank
            std::vector<FMOD::Studio::EventDescription*> events(numEvents);
            bank->getEventList(events.data(), numEvents, &numEvents);

            char eventName[maxPathLength];
            for (int i = 0; i< numEvents; i++)
            {
                FMOD::Studio::EventDescription* e = events[i];
                
                // Get the path of this event (like event:/Explosion2D)
                e->getPath(eventName, maxPathLength, nullptr);
                
                // Add to event map
                mEvents.emplace(eventName, e);
            }
        }
    }
}

void AudioSystem::UnloadBank(const std::string &name)
{
    // Ignore if not loaded
    auto iter = mBanks.find(name);
    if (iter == mBanks.end())
    {
        return;
    }

    // First we need to remove all events from this bank
    FMOD::Studio::Bank* bank = iter->second;
    int numEvents = 0;
    bank->getEventCount(&numEvents);
    if (numEvents > 0)
    {
        // Get event descriptions for this bank
        std::vector<FMOD::Studio::EventDescription*> events(numEvents);
        
        // Get list of events
        bank->getEventList(events.data(), numEvents, &numEvents);
        char eventName[512];
        for (int i = 0; i < numEvents; i++)
        {
            FMOD::Studio::EventDescription* e = events[i];
            
            // Get the path of this event
            e->getPath(eventName, 512, nullptr);
            
            // Remove this event
            auto eventi = mEvents.find(eventName);
            if (eventi != mEvents.end())
            {
                mEvents.erase(eventi);
            }
        }
    }
    
    // Unload sample data and bank
    bank->unloadSampleData();
    bank->unload();
    
    // Remove from banks map
    mBanks.erase(iter);
}

void::AudioSystem::UnloadAllBanks()
{
    for (auto& iter : mBanks)
    {
        // Unload the sample data, then the bank itself
        iter.second->unloadSampleData();
        iter.second->unload();
    }
    
    mBanks.clear();
    
    // No banks means no events
    mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string &name)
{
    unsigned int retID = 0;
    
    // Make sure event exists
    auto iter = mEvents.find(name);
    if (iter != mEvents.end())
    {
        // Create insatnce of event
        FMOD::Studio::EventInstance* event = nullptr;
        iter->second->createInstance(&event);
        if (event)
        {
            // Start the event instance
            event->start();
            
            // Get th enext id, and ad to map
            sNextID++;
            retID = sNextID;
            mEventInstances.emplace(retID, event);
        }
    }
    
    return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
    // Find any stopped event instances
    std::vector<unsigned int> done;
    for (auto& iter : mEventInstances)
    {
        FMOD::Studio::EventInstance* e = iter.second;
        
        // Get the state of this PlayEvent
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);
        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
            // Release the event and add id to done
            e->release();
            done.emplace_back(iter.first);
        }
    }
    
    // Remove done event instances from map
    for (auto id : done)
    {
        mEventInstances.erase(id);
    }
    
    // Update FMOD
    mSystem->update();
}

namespace
{
    FMOD_VECTOR VecToFMOD(const Vector3& in)
    {
        // Convert from our coordinates (+x forward, +y right, +z up)
        // to FMOD (+z forward, +x right, +y up)
        FMOD_VECTOR v;
        v.x = in.y;
        v.y = in.z;
        v.z = in.x;
        
        return v;
    }
}

void AudioSystem::SetListener(const Matrix4 &viewMatrix)
{
    // Invert the view matrix to get the correct vectors
    Matrix4 invView = viewMatrix;
    invView.Invert();
    
    FMOD_3D_ATTRIBUTES listener;
    
    // Set position, forward, up
    listener.position = VecToFMOD(invView.GetTranslation());
    listener.forward = VecToFMOD(invView.GetZAxis());
    listener.up = VecToFMOD(invView.GetYAxis());
    
    // Set velocity to zero (fix if using Doppler effect)
    listener.velocity = { 0.0f, 0.0f, 0.0f };
    
    // Send to FMOD (0 = only one listener)
    mSystem->setListenerAttributes(0, &listener);
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
    FMOD::Studio::EventInstance* event = nullptr;
    auto iter = mEventInstances.find(id);
    if (iter != mEventInstances.end())
    {
        event = iter->second;
    }
    return event;
}
