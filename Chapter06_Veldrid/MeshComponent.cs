using Veldrid;

namespace Chapter06
{
    public class MeshComponent : Component
    {
        private int _textureIndex;
        private Mesh _mesh;
        private ResourceSet _worldTextureResourceSet;

        public MeshComponent(Actor owner)
            : base(owner)
        {
            Owner.Game.Renderer.AddMeshComponent(this);
        }

        public Mesh Mesh
        {
            get => _mesh;
            set
            {
                _mesh = value;
                SetTexture(_textureIndex);
            }
        }

        public int TextureIndex
        {
            get => _textureIndex;
            set
            {
                _textureIndex = value;
                SetTexture(_textureIndex);
            }
        }

        public void Draw(MeshShader shader)
        {
            var commandList = Owner.Game.Renderer.CommandList;

            // Set vertex and index buffers
            commandList.SetVertexBuffer(0, _mesh.VertexArray.VertexBuffer);
            commandList.SetIndexBuffer(_mesh.VertexArray.IndexBuffer, IndexFormat.UInt16);

            // Set world transform matrix
            commandList.UpdateBuffer(shader.WorldTransformBuffer, 0, Owner.WorldTransform);
            commandList.SetGraphicsResourceSet(1, _worldTextureResourceSet);

            commandList.DrawIndexed(
                indexCount: (uint)_mesh.VertexArray.Indices.Length,
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
                Owner.Game.Renderer.RemoveMeshComponent(this);
            }
        }

        private void SetTexture(int index)
        {
            if (_worldTextureResourceSet is not null)
            {
                _worldTextureResourceSet.Dispose();
                _worldTextureResourceSet = null;
            }

            if (_mesh is not null)
            {
                _worldTextureResourceSet = Owner.Game.Renderer.GraphicsDevice.ResourceFactory.CreateResourceSet(
                    new ResourceSetDescription(
                        Owner.Game.Renderer.MeshShader.WorldTextureLayout,
                        Owner.Game.Renderer.MeshShader.WorldTransformBuffer,
                        _mesh.GetTexture(index).TextureView,
                        Owner.Game.Renderer.GraphicsDevice.Aniso4xSampler));
            }
        }
    }
}