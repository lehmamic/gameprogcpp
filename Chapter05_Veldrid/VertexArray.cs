using System;
using Veldrid;

namespace Chapter05
{
    public class VertexArray : IDisposable
    {
        public VertexArray(GraphicsDevice graphicsDevice, Vertex[] vertices, ushort[] indices)
        {
            Vertices = vertices;
            Indices = indices;

            var factory = graphicsDevice.ResourceFactory;

            VertexBuffer = factory.CreateBuffer(new BufferDescription((uint)vertices.Length * Vertex.SizeInBytes, BufferUsage.VertexBuffer));
            graphicsDevice.UpdateBuffer(VertexBuffer, 0, vertices);

            IndexBuffer = factory.CreateBuffer(new BufferDescription((uint)indices.Length * sizeof(ushort), BufferUsage.IndexBuffer));
            graphicsDevice.UpdateBuffer(IndexBuffer, 0, indices);
        }

        public Vertex[] Vertices { get; }

        public DeviceBuffer VertexBuffer { get; }

        public ushort[] Indices { get; }

        public DeviceBuffer IndexBuffer { get; }

        public void Dispose()
        {
            VertexBuffer?.Dispose();
            IndexBuffer.Dispose();
        }
    }
}