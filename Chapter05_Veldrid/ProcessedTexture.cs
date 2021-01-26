using System;
using System.IO;
using Veldrid;
using Veldrid.ImageSharp;

namespace Chapter05
{
    public class ProcessedTexture : IDisposable
    {
        private Texture _texture;

        public TextureView TextureView { get; private set; }

        public uint Height => _texture.Height;

        public uint Width => _texture.Width;

        public bool Load(GraphicsDevice graphicsDevice, string fileName)
        {
            try
            {
                ImageSharpTexture image = new ImageSharpTexture(fileName);
                _texture = image.CreateDeviceTexture(graphicsDevice, graphicsDevice.ResourceFactory);
                TextureView = graphicsDevice.ResourceFactory.CreateTextureView(_texture);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);

                TextureView.Dispose();
                _texture.Dispose();

                return false;
            }

            return true;
        }

        public void Dispose()
        {
        }
    }
}