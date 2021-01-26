using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using Veldrid;

namespace Chapter05
{
    public class Actor : IDisposable
    {
        private readonly List<Component> _components = new();

        private bool _recomputeWorldTransform = true;
        private Vector2 _position = Vector2.Zero;
        private float _scale = 1.0f;
        private float _rotation = 0.0f;

        public Actor(Game game)
        {
            Game = game;

            Game.AddActor(this);
        }

        public Game Game { get; }

        public ActorState State { get; set; } = ActorState.Active;

        public Vector2 Position
        {
            get => _position;
            set
            {
                _position = value;
                _recomputeWorldTransform = true;
            }
        }

        public float Scale
        {
            get => _scale;
            set
            {
                _scale = value;
                _recomputeWorldTransform = true;
            }
        }

        public float Rotation
        {
            get => _rotation;
            set
            {
                _rotation = value;
                _recomputeWorldTransform = true;
            }
        }

        public Vector2 Forward => new((float)Math.Cos(Rotation), (float)Math.Sin(Rotation));

        public Matrix4x4 WorldTransform { get; private set; }

        public void Update(float deltaTime)
        {
            if (State == ActorState.Active)
            {
                ComputeWorldTransform();

                UpdateComponents(deltaTime);
                UpdateActor(deltaTime);
        
                ComputeWorldTransform();
            }
        }

        public void ProcessInput(InputSnapshot input)
        {
            if (State == ActorState.Active)
            {
                // First process input for components
                foreach(var component in _components)
                {
                    component.ProcessInput(input);
                }

                ActorInput(input);
            }
        }

        public void ComputeWorldTransform()
        {
            if (_recomputeWorldTransform)
            {
                _recomputeWorldTransform = false;

                // Scale, then rotate, then translate
                WorldTransform = Matrix4x4.CreateScale(Scale);
                WorldTransform *= Matrix4x4.CreateRotationZ(Rotation);
                WorldTransform *= Matrix4x4.CreateTranslation(new Vector3(Position.X, Position.Y, 0.0f));

                // Inform components world transform updated
                foreach (var component in _components)
                {
                    component.OnUpdateWorldTransform();
                }
            }
        }

        public void AddComponent(Component component)
        {
            // We add it already ordered to the list
            var index = 0;
            for (; index < _components.Count; index++)
            {
                if (component.UpdateOrder < _components[index].UpdateOrder)
                {
                    break;
                }
            }
            
            _components.Insert(index, component);
        }

        public void RemoveComponent(Component component)
        {
            _components.Remove(component);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void UpdateActor(float deltaTime)
        {
        }
        
        protected virtual void ActorInput(InputSnapshot input)
        {
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                Game.RemoveActor(this);

                while (_components.Any())
                {
                    _components.Last().Dispose();
                }
            }
        }
        
        private void UpdateComponents(float deltaTime)
        {
            foreach(var component in _components)
            {
                component.Update(deltaTime);
            }
        }
    }
}