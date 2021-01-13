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
    
    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);
    
    SDL_Texture* GetTexture(const std::string& fileName);
    
    // Game-specific
    class Grid* GetGrid() { return mGrid; }
    std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
    class Enemy* GetNearestEnemy(const Vector2& pos);
    

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    
    // Map of textures loaded
    std::unordered_map<std::string, SDL_Texture*> mTextures;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    // Any pending actors
    std::vector<class Actor*> mPendingActors;
    
    // All the sprite components drawn
    std::vector<class SpriteComponent*> mSprites;

    // Track if we're updating actors right now
    bool mUpdatingActors;
    
    // Game-specific
    std::vector<class Enemy*> mEnemies;
    class Grid* mGrid;
    float mNextEnemy;
};


#endif //CHAPTER02_GAME_H
