//
//  AudioComponent.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 16.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "AudioComponent.h"
#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder)
    :Component(owner, updateOrder)
{
}

AudioComponent::~AudioComponent()
{
    StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
    Component::Update(deltaTime);

    // Remove invalid 2D events
    auto iter = mEvents2D.begin();
    while (iter != mEvents2D.end())
    {
        if (!iter->IsValid())
        {
            iter = mEvents2D.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    // Remove invalid 3D events
    iter = mEvents3D.begin();
    while (iter != mEvents3D.end())
    {
        if (!iter->IsValid())
        {
            iter = mEvents3D.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void AudioComponent::OnUpdateWorldTransform()
{
    Matrix4 world = mOwner->GetWorldTransform();
    for(auto& event : mEvents3D)
    {
        if (event.IsValid())
        {
            event.Set3DAttributes(world);
        }
    }
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
    SoundEvent e = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
    
    // Is this 2D or 3D?
    if (e.Is3D())
    {
        mEvents3D.emplace_back(e);
        
        // Set initial 3D attributes
        e.Set3DAttributes(mOwner->GetWorldTransform());
    }
    else
    {
        mEvents2D.emplace_back(e);
    }
    
    return e;
}

void AudioComponent::StopAllEvents()
{
    // Stop all sounds
    for (auto& e : mEvents2D)
    {
        e.Stop();
    }
    
    for (auto& e : mEvents3D)
    {
        e.Stop();
    }
    
    // Clear events
    mEvents2D.clear();
    mEvents3D.clear();
}
