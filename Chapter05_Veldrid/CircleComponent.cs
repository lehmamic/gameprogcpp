using System.Numerics;

namespace Chapter05
{
    public class CircleComponent : Component
    {
        public CircleComponent(Actor owner)
            : base(owner)
        {
        }
        
        public float Radius { get; set; }

        public Vector2 Center => Owner.Position;

        public static bool Intersect(CircleComponent a, CircleComponent b)
        {
            // Calculate distance squared
            Vector2 diff = a.Center - b.Center;
            float distSq = diff.LengthSquared();

            // Calculate sum of radii squared
            float radiiSq = a.Radius + b.Radius;
            radiiSq *= radiiSq;

            return distSq <= radiiSq;
        }
    }
}