using System;
using System.IO;
using Veldrid;
using Veldrid.SPIRV;

namespace Chapter06
{
    public class ProcessedShader : IDisposable
    {
        private Shader[] _shaders;
        private VertexLayoutDescription _vertexLayout;

        public ShaderSetDescription ShaderSet { get; private set; }

        public virtual bool Load(GraphicsDevice graphicsDevice, string vertexShaderFileName, string fragmentShaderFileName)
        {
            _vertexLayout = new VertexLayoutDescription(
                new VertexElementDescription(nameof(Vertex.Position), VertexElementSemantic.TextureCoordinate, VertexElementFormat.Float3),
                new VertexElementDescription(nameof(Vertex.Normal), VertexElementSemantic.TextureCoordinate, VertexElementFormat.Float3),
                new VertexElementDescription(nameof(Vertex.TextureCoordinate), VertexElementSemantic.TextureCoordinate, VertexElementFormat.Float2));

            try
            {
                byte[] vertexShaderBytes = File.ReadAllBytes(vertexShaderFileName);
                ShaderDescription vertexShaderDesc = new ShaderDescription(ShaderStages.Vertex, vertexShaderBytes, "main");

                byte[] fragmentShaderBytes = File.ReadAllBytes(fragmentShaderFileName);
                ShaderDescription fragmentShaderDesc = new ShaderDescription(ShaderStages.Fragment, fragmentShaderBytes, "main");

                _shaders = graphicsDevice.ResourceFactory.CreateFromSpirv(vertexShaderDesc, fragmentShaderDesc);

                ShaderSet = new ShaderSetDescription(new[] {_vertexLayout}, _shaders);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);

                Dispose();

                return false;
            }

            return true;
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (_shaders is not null)
                {
                    foreach (var shader in _shaders)
                    {
                        shader.Dispose();
                    }
                
                    _shaders = null;
                }
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}