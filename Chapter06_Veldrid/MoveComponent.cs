using System;
using System.Numerics;

namespace Chapter06
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
                Quaternion rotation = Owner.Rotation;
                float angle = AngularSpeed * deltaTime;

                // Create quaternion for incremental rotation
                // (Rotate about up axis)
                Quaternion inc = new Quaternion(Vector3.UnitZ, angle);
                rotation = Quaternion.Concatenate(rotation, inc);

                // Concatenate old and new quaternion
                Owner.Rotation = rotation;
            }

            // Updating position based on forward speed stays the same
            if (!ForwardSpeed.NearZero())
            {
                Vector3 position = Owner.Position;
                position += Owner.Forward * ForwardSpeed * deltaTime;

                Owner.Position = position;
            }
        }
    }
}