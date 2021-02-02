using System.Numerics;

namespace Chapter06
{
    public class CircleComponent : Component
    {
        public CircleComponent(Actor owner)
            : base(owner)
        {
        }
        
        public float Radius { get; set; }

        public Vector3 Center => Owner.Position;

        public static bool Intersect(CircleComponent a, CircleComponent b)
        {
            // Calculate distance squared
            Vector3 diff = a.Center - b.Center;
            float distSq = diff.LengthSquared();

            // Calculate sum of radii squared
            float radiiSq = a.Radius + b.Radius;
            radiiSq *= radiiSq;

            return distSq <= radiiSq;
        }
    }
}