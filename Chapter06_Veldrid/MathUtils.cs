using System;
using System.Numerics;

namespace Chapter06
{
    public static class MathUtils
    {
        public static float TwoPi { get; } = (float)Math.PI * 2;
        public static float PiOver2 { get; } = (float)Math.PI / 2.0f;

        public static bool NearZero(this float value, float epsilon = 0.001f)
        {
            if (Math.Abs(value) <= epsilon)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        public static Matrix4x4 CreateSimpleViewProj(float width, float height)
        {
            return new(
                2.0f/width, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f/height, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);
        }
    }
}