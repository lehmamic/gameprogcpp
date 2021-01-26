using System.Numerics;

namespace Chapter05
{
    public struct Vertex
    {
        // (position x/y/z) + (normal x/y/z) + (ub x/y) = 8 float values => 8 * 4 (bytes per float) = 32;
        public const uint SizeInBytes = 32;
        
        public Vertex(Vector3 position, Vector3 normal, Vector2 textureCoordinate)
        {
            Position = position;
            Normal = normal;
            TextureCoordinate = textureCoordinate;
        }

        public Vector3 Position { get; }

        public Vector3 Normal { get; }

        public Vector2 TextureCoordinate { get; }
    }
}