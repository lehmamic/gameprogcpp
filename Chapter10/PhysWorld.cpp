//
//  PhysWorld.cpp
//  Game-mac
//
//  Created by Michael Lehmann on 22.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include <SDL/SDL.h>


PhysWorld::PhysWorld(Game* game)
    :mGame(game)
{
}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl)
{
    bool collided = false;
    
    // Initialize closestT to infinity, so first
    // intersection will always update closestT
    float closestT = Math::Infinity;
    Vector3 norm;
    
    // Test agains all boxes
    for (auto box : mBoxes)
    {
        float t;
        
        // Does the segment intersect with the box?
        if (Intersect(l, box->GetWorldBox(), t, norm))
        {
            // Is this closer than previous intersection
            if (t < closestT)
            {
                outColl.mPoint = l.PointOnSegment(t);
                outColl.mBox = box;
                outColl.mActor = box->GetOwner();
            }
            
            collided = true;
        }
    }
    
    return collided;
}

void PhysWorld::TestPairwise(std::function<void (class Actor*, class Actor*)> f)
{
    // Naive implementation =(n^2)
    for (size_t i = 0; i < mBoxes.size(); i++)
    {
        // Don't need to test vs, itself and any previous i values
        for (size_t j = i + 1; j < mBoxes.size(); j++)
        {
            BoxComponent* a = mBoxes[i];
            BoxComponent* b = mBoxes[j];
            
            if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                // Call supplied function to handle intersection
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
}

void PhysWorld::TestSweepAndPrune(std::function<void (class Actor*, class Actor*)> f)
{
    // Sort by min.x
    std::sort(mBoxes.begin(), mBoxes.end(), [](BoxComponent* a, BoxComponent* b)
    {
        return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
    });
    
    for (size_t i = 0; i < mBoxes.size(); i++)
    {
        // Get max.x for box[i]
        BoxComponent* a = mBoxes[i];
        float max = a->GetWorldBox().mMax.x;
        
        for (size_t j = i + 1; j < mBoxes.size(); j++)
        {
            BoxComponent* b = mBoxes[j];
            
            // If box[j] min.x is past the max.x bounds of box[i]
            // then there aren't any other possbile intersections
            // against box[i]
            if (b->GetWorldBox().mMin.x > max)
            {
                break;
            }
            else if(Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
    
}

void PhysWorld::AddBox(BoxComponent* box)
{
    mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box)
{
    auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
    if (iter != mBoxes.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mBoxes.end() - 1);
        mBoxes.pop_back();
    }
}
