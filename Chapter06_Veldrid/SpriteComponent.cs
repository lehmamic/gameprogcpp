using System.Numerics;
using Veldrid;

namespace Chapter06
{
    public class SpriteComponent : Component
    {
        private ResourceSet _worldTextureResourceSet;
        private ProcessedTexture _texture;

        public SpriteComponent(Actor owner, int drawOrder = 100)
        : base(owner)
        {
            DrawOrder = drawOrder;
            Owner.Game.Renderer.AddSprite(this);
        }

        public int DrawOrder { get; }

        public void SetTexture(ProcessedTexture texture)
        {
            _texture = texture;
            _worldTextureResourceSet = Owner.Game.Renderer.GraphicsDevice.ResourceFactory.CreateResourceSet(
                new ResourceSetDescription(
                    Owner.Game.Renderer.SpriteShader.WorldTextureLayout,
                    Owner.Game.Renderer.SpriteShader.WorldTransformBuffer,
                    texture.TextureView,
                    Owner.Game.Renderer.GraphicsDevice.Aniso4xSampler));
        }

        public void Draw(SpriteShader shader)
        {
            var commandList = Owner.Game.Renderer.CommandList;

            // Scale the quad by the width/height of texture
            Matrix4x4 scaleMat = Matrix4x4.CreateScale(_texture.Width, _texture.Height, 1.0f);
            Matrix4x4 world = scaleMat * Owner.WorldTransform;

            // Set vertex and index buffers
            commandList.SetVertexBuffer(0, Owner.Game.Renderer.SpriteVertices.VertexBuffer);
            commandList.SetIndexBuffer(Owner.Game.Renderer.SpriteVertices.IndexBuffer, IndexFormat.UInt16);

            // Set world transform matrix
            commandList.UpdateBuffer(shader.WorldTransformBuffer, 0, world);
            commandList.SetGraphicsResourceSet(1, _worldTextureResourceSet);

            commandList.DrawIndexed(
                indexCount: (uint)Owner.Game.Renderer.SpriteVertices.Indices.Length,
                instanceCount: 1,
                indexStart: 0,
                vertexOffset: 0,
                instanceStart: 0);
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (disposing)
            {
                _worldTextureResourceSet.Dispose();
                Owner.Game.Renderer.RemoveSprite(this);
            }
        }
    }
}