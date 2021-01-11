//
// Created by Michael Lehmann on 10.01.21.
//

#ifndef CHAPTER02_GAME_H
#define CHAPTER02_GAME_H

#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

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

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    // Any pending actors
    std::vector<class Actor*> mPendingActors;

    // Track if we're updating actors right now
    bool mUpdatingActors;
};


#endif //CHAPTER02_GAME_H
