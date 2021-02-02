using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
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

        public Renderer Renderer { get; private set; }

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
// Create actors
            var a = new Actor(this)
            {
                Position = new Vector3(200.0f, 75.0f, 0.0f),
                Scale = 100.0f,
            };
            var q = new Quaternion(Vector3.UnitY, -MathUtils.PiOver2);
            q = Quaternion.Concatenate(q, new Quaternion(Vector3.UnitZ, (float)Math.PI + (float)Math.PI / 4.0f));
            a.Rotation = q;
            var mc = new MeshComponent(a)
            {
                Mesh = Renderer.GetMesh("Assets/Cube.gpmesh"),
            };

            a = new Actor(this)
            {
                Position = new Vector3(200.0f, -75.0f, 0.0f),
                Scale = 3.0f,
            };
            mc = new MeshComponent(a)
            {
                Mesh = Renderer.GetMesh("Assets/Sphere.gpmesh"),
            };

            // Setup floor
            const float start = -1250.0f;
            const float size = 250.0f;
            for (var i = 0; i < 10; i++)
            {
                for (var j = 0; j < 10; j++)
                {
                    a = new PlaneActor(this) {Position = new Vector3(start + i * size, start + j * size, -100.0f)};
                }
            }

            // Left/right walls
            q = new Quaternion(Vector3.UnitX, MathUtils.PiOver2);
            for (var i = 0; i < 10; i++)
            {
                a = new PlaneActor(this) {Position = new Vector3(start + i * size, start - size, 0.0f), Rotation = q};
                a = new PlaneActor(this) {Position = new Vector3(start + i * size, -start + size, 0.0f), Rotation = q};
            }


            // Forward/back walls
            q = Quaternion.Concatenate(q, new Quaternion(Vector3.UnitZ, MathUtils.PiOver2));
            for (var i = 0; i < 10; i++)
            {
                a = new PlaneActor(this) {Position = new Vector3(start - size, start + i * size, 0.0f), Rotation = q};
                a = new PlaneActor(this) {Position = new Vector3(-start + size, start + i * size, 0.0f), Rotation = q};
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