using System;
using System.Numerics;

namespace Chapter05
{
    public class MoveComponent : Component
    {
        public MoveComponent(Actor owner, int updateOrder = 10)
            : base(owner, updateOrder)
        {
        }

        public float AngularSpeed { get; set; }

        public float ForwardSpeed { get; set; }

        public override void Update(float deltaTime)
        {
            if (!AngularSpeed.NearZero())
            {
                float rotation = Owner.Rotation;
                rotation += AngularSpeed * deltaTime;
                Owner.Rotation = rotation;
            }

            if (!ForwardSpeed.NearZero())
            {
                Vector2 position = Owner.Position;
                position += Owner.Forward * ForwardSpeed * deltaTime;

                // Screen wrapping (for asteroids)
                var halfScreenWidth = Owner.Game.Renderer.Window.Width / 2;
                if (position.X < -halfScreenWidth)
                {
                    position.X = halfScreenWidth - 2;
                }
                else if (position.X > halfScreenWidth)
                {
                    position.X = -(halfScreenWidth - 2);
                }

                var halfScreenHeight = Owner.Game.Renderer.Window.Height / 2;
                if (position.Y < -halfScreenHeight)
                {
                    position.Y = halfScreenHeight - 2;
                }
                else if (position.Y > halfScreenHeight)
                {
                    position.Y = -(halfScreenHeight - 2);
                }

                Owner.Position = position;
            }
        }
    }
}