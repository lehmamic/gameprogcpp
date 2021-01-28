using System;
using Veldrid;

namespace Chapter06
{
    public abstract class Component : IDisposable
    {
        protected Component(Actor owner, int updateOrder = 100)
        {
            Owner = owner;
            UpdateOrder = updateOrder;

            // Add to actor's vector of components
            Owner.AddComponent(this);
        }

        public Actor Owner { get; }

        public int UpdateOrder { get; }

        public virtual void Update(float deltaTime)
        {
        }

        public virtual void ProcessInput(InputState input)
        {
        }

        public virtual void OnUpdateWorldTransform()
        {
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                Owner.RemoveComponent(this);
            }
        }
    }
}