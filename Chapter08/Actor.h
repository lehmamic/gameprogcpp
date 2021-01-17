//
// Created by Michael Lehmann on 10.01.21.
//

#ifndef CHAPTER02_ACTOR_H
#define CHAPTER02_ACTOR_H

#include <vector>
#include "Math.h"
#include <cstdint>

class Actor {
public:
    // Used to track state of actor
    enum State {
        EActive,
        EPaused,
        EDead
    };

    // Constructor/destructor
    Actor(class Game* game);
    virtual ~Actor();

    // Update function called from Game (not overridable)
    void Update(float deltaTime);

    // Updates all the components attached to the actor
    void UpdateComponents(float deltaTime);

    // Any actor-specific update coe (overridable)
    virtual void UpdateActor(float deltaTime);
    
    // ProcessInput function called from Game (not overridable)
    void ProcessInput(const struct InputState& state);
    
    // Any actor-specific input code (overridable)
    virtual void ActorInput(const struct InputState& state);

    // Getters/setters
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
    
    void ComputeWorldTransform();
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }
    
    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }

    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    class Game* GetGame() { return mGame; }

    // Add/remove components
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

private:
    // Actor's state
    State mState;

    // Transform
    Matrix4 mWorldTransform;
    Vector2 mPosition; // Center position of actor
    float mScale; // Uniforms scale of actor (1.0f for 100%)
    float mRotation; // Rotation angle (in radians)
    bool mRecomputeWorldTransform;

    // Components held by this actor
    std::vector<class Component*> mComponents;
    class Game* mGame;
};


#endif //CHAPTER02_ACTOR_H
