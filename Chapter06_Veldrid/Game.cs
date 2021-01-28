using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using Veldrid;

namespace Chapter06
{
    public class Game : IDisposable
    {
        private readonly List<Actor> _actors = new();
        private readonly List<Actor> _pendingActors = new();

        private InputSystem _inputSystem;
        private Stopwatch _stopwatch;
        long _ticksCount;
        private bool _updatingActors;
        private Ship _ship;

        public Renderer Renderer { get; private set; }

        public List<Asteroid> Asteroids { get; } = new();

        public bool Initialize()
        {
            // Create the renderer
            Renderer = new Renderer(this);
            if (!Renderer.Initialize(1024, 768))
            {
                Console.WriteLine("Failed to initialize renderer");
                
                Renderer.Dispose();
                return false;
            }

            _inputSystem = new InputSystem();
            if (!_inputSystem.Initialize())
            {
                Console.WriteLine("Failed to initialize input system");
            }

            LoadData();

            _stopwatch = Stopwatch.StartNew();
            _ticksCount = _stopwatch.ElapsedMilliseconds;

            return true;
        }

        public void RunLoop()
        {
            while (Renderer.Window.Exists)
            {
                ProcessInput();
                UpdateGame();
                GenerateOutput();
            }
        }

        public void AddActor(Actor actor)
        {
            // If updating actors, need to add to pending
            if (_updatingActors)
            {
                _pendingActors.Add(actor);
            }
            else
            {
                _actors.Add(actor);
            }
        }

        public void RemoveActor(Actor actor)
        {
            // Is it in pending actors?
            if (_pendingActors.Contains(actor))
            {
                _pendingActors.Remove(actor);
            }
            
            // Is it in actors?
            if (_actors.Contains(actor))
            {
                _actors.Remove(actor);
            }
        }

        public void AddAsteroid(Asteroid asteroid)
        {
            Asteroids.Add(asteroid);
        }

        public void RemoveAsteroid(Asteroid asteroid)
        {
            Asteroids.Remove(asteroid);
        }

        private void GenerateOutput()
        {
            Renderer.Draw();
        }

        private void ProcessInput()
        {
            var input = Renderer.Window.PumpEvents();
            _inputSystem.Update(input);

            var state = _inputSystem.State;
            if (state.Keyboard.GetKeyState(Key.Escape) == ButtonState.Pressed)
            {
                Renderer.Window.Close();
            }

            _updatingActors = true;
            foreach (var actor in _actors)
            {
                actor.ProcessInput(state);
            }
            _updatingActors = false;
        }

        private void UpdateGame()
        {
            // Compute delta time (as in Chapter 1)
            // Wait until 16ms has elapsed since last frame
            while (_stopwatch.ElapsedMilliseconds < _ticksCount + 16)
            {
            }

            // Delta time is the difference in ticks from last frame
            // (converted to seconds)
            var elapsedTicks = _stopwatch.ElapsedMilliseconds;
            float deltaTime = (elapsedTicks - _ticksCount) / 1000f;
            
            // Clamp maximum delta time value
            if (deltaTime > 0.05f)
            {
                deltaTime = 0.05f;
            }

            // Update tick counts (for next frame)
            _ticksCount = elapsedTicks;

            // Update all actors
            _updatingActors = true;
            foreach (var actor in _actors)
            {
                actor.Update(deltaTime);
            }
            _updatingActors = false;

            // Move any pending actors to mActors
            foreach (var pending in _pendingActors)
            {
                pending.ComputeWorldTransform();
                _actors.Add(pending);
            }

            _pendingActors.Clear();

            // Add any dead actors to a temp vector
            var deadActors = _actors.Where(a => a.State == ActorState.Dead).ToArray();

            // Delete dead actors (which removes them from mActors
            foreach(var actor in deadActors)
            {
                actor.Dispose();
            }
        }

        private void LoadData()
        {
            // Create player's ship
            _ship = new Ship(this)
            {
                Rotation = MathUtils.PiOver2,
            };

            // Create asteroids
            const int numAsteroids = 20;
            for (int i = 0; i < numAsteroids; i++)
            {
                var asteroid = new Asteroid(this);
            }
        }

        private void UnloadData()
        {
            // Delete actors
            // Because ~Actor calls RemoveActor, have to use a different style loop
            while (_actors.Any())
            {
                _actors.Last().Dispose();
            }

            Renderer?.UnloadData();
        }

        public void Dispose()
        {
            UnloadData();
            Renderer?.Dispose();
        }
    }
}