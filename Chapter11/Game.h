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
    class PhysWorld* GetPhysWorld() { return mPhysWorld; }
    class HUD* GetHUD() { return mHUD; }
    
    // Manage UI stack
    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    void PushUI(class UIScreen* screen);
    
    class FPSActor* GetPlayer() { return mFPSActor; }
    
    enum GameState
    {
        EGameplay,
        EPaused,
        EQuit
    };
    
    GameState GetState() const { return mGameState; }
    void SetState(GameState state) { mGameState = state; }
    
    class Font* GetFont(const std::string& fileName);
    
    void LoadText(const std::string& fileName);
    const std::string& GetText(const std::string& key);
    
    // Game-specific (add/remove asteroid)
    void AddPlane(class PlaneActor* plane);
    void RemovePlane(class PlaneActor* plane);
    std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
    
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
    std::vector<class UIScreen*> mUIStack;
    std::unordered_map<std::string, class Font*> mFonts;
    
    // Map for text localization
    std::unordered_map<std::string, std::string> mText;
    
    // Any pending actors
    std::vector<class Actor*> mPendingActors;
    
    class Renderer* mRenderer;
    class AudioSystem* mAudioSystem;
    class PhysWorld* mPhysWorld;
    class HUD* mHUD;

    GameState mGameState;
    
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    // Track if we're updating actors right now
    bool mUpdatingActors;
    
    // Game-specific code
    std::vector<class PlaneActor*> mPlanes;
    class FPSActor* mFPSActor;
    class SpriteComponent* mCrosshair;
    SoundEvent mMusicEvent;
};


#endif //CHAPTER02_GAME_H
