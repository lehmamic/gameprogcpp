using System.Numerics;

namespace Chapter06
{
    public static class Random
    {
        public static float GetFloat()
        {
            return GetFloat(0.0f, 1.0f);
        }
        
        public static float GetFloat(float min, float max)
        {
            System.Random random = new();
            return (float)random.NextDouble() * (max - min) + min;
        }
        
        public static int GetInt(int min, int max)
        {
            System.Random random = new();
            return random.Next(min, max);
        }

        public static Vector2 GetVector(Vector2 min, Vector2 max)
        {
            Vector2 r = new Vector2(GetFloat(), GetFloat());
            return min + (max - min) * r;
        }
        
        public static Vector3 GetVector(Vector3 min, Vector3 max)
        {
            Vector3 r = new Vector3(GetFloat(), GetFloat(), GetFloat());
            return min + (max - min) * r;
        }
    }
}