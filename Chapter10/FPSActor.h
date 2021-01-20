//
//  FPSActor.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 18.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef FPSActor_hpp
#define FPSActor_hpp

#include "Actor.h"
#include "SoundEvent.h"

// First Person Shooter Actor
class FPSActor : public Actor
{
public:
    FPSActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t* keys) override;

    void SetFootstepSurface(float value);

    void SetVisible(bool visible);
private:
    class MoveComponent* mMoveComp;
    class AudioComponent* mAudioComp;
    class MeshComponent* mMeshComp;
    class FPSCamera* mCameraComp;
    class Actor* mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};

#endif /* FPSActor_hpp */
