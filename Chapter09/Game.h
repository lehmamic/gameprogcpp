//
// Created by Michael Lehmann on 10.01.21.
//

#ifndef CHAPTER02_GAME_H
#define CHAPTER02_GAME_H

#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"

class Game {
public:
    Game();

    // Initialize the game
    bool Initialize();

    // Runs the game loop until the game is over
    void RunLoop();

    // Shutdown the game
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    class Renderer* GetRenderer() { return mRenderer; }
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    
    // Game-specific (add/remove asteroid)

private:
    // Helper functions for the game loop
    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();
    
    // All the actors in the game
    std::vector<class Actor*> mActors;
    // Any pending actors
    std::vector<class Actor*> mPendingActors;
    
    class Renderer* mRenderer;
    class AudioSystem* mAudioSystem;

    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;
    
    // Game-specific code
    class FPSActor* mFPSActor;
    class FollowActor* mFollowActor;
    class OrbitActor* mOrbitActor;
    class SplineActor* mSplineActor;
    class Actor* mStartSphere;
    class Actor* mEndSphere;
    class SpriteComponent* mCrosshair;
    SoundEvent mMusicEvent;
    SoundEvent mReverbSnap;
    void ChangeCamera(int mode);
};


#endif //CHAPTER02_GAME_H
