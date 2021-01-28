using System;
using System.Collections.Generic;
using System.Numerics;
using Veldrid;
using Veldrid.Sdl2;
using Veldrid.StartupUtilities;

namespace Chapter06
{
    public class Renderer :IDisposable
    {
        private readonly Game _game;
        private readonly Dictionary<string, ProcessedTexture> _textures = new();
        private readonly List<SpriteComponent> _sprites = new();

        private ProcessedShader _spriteShader;
        private Pipeline _spritePipeline;

        public Renderer(Game game)
        {
            _game = game;
        }

        public Sdl2Window Window { get; private set; }

        public GraphicsDevice GraphicsDevice { get; private set; }

        public CommandList CommandList { get; private set; }

        public VertexArray SpriteVertices { get; private set; }

        public ResourceLayout ProjectionViewLayout { get; private set; }

        public ResourceSet ProjectionViewResourceSet { get; private set; }

        public DeviceBuffer ProjectionViewBuffer { get; private set; }

        public ResourceLayout WorldTextureLayout { get; private set; }

        public DeviceBuffer WorldTransformBuffer { get; private set; }

        public bool Initialize(int screenWidth, int screenHeight)
        {
            // Create an SDL Window
            var windowCi = new WindowCreateInfo
            {
                X = 100,
                Y = 100,
                WindowWidth = screenWidth,
                WindowHeight = screenHeight,
                WindowTitle = "Game Programming in C# (Chapter 5)"
            };

            Window = VeldridStartup.CreateWindow(ref windowCi);
            if (!Window.Exists)
            {
                Console.WriteLine("Failed to create window");
                return false;
            }
            
            // Create the graphic device
            GraphicsDeviceOptions options = new GraphicsDeviceOptions
            {
                PreferStandardClipSpaceYDirection = true,
                PreferDepthRangeZeroToOne = true,
            };
            GraphicsDevice = VeldridStartup.CreateGraphicsDevice(Window, options, GraphicsBackend.OpenGL);
            
            // Create command list
            CommandList = GraphicsDevice.ResourceFactory.CreateCommandList();
            
            // Make sure we can create/compile shaders
            if (!LoadShaders())
            {
                Console.WriteLine("Failed to load shaders.");
                return false;
            }

            // Create quad for drawing sprites
            CreateSpriteVertices();

            return true;
        }

        public void Draw()
        {
            CommandList.Begin();
            CommandList.SetFramebuffer(GraphicsDevice.SwapchainFramebuffer);
            CommandList.SetFullViewports();

            // Clear the color buffer
            CommandList.ClearColorTarget(0, new RgbaFloat(0.86f, 0.86f, 0.86f, 1.0f));

            // Draw all sprite components
            foreach (var sprite in _sprites)
            {
                sprite.Draw(_spritePipeline);
            }

            CommandList.End();
            GraphicsDevice.SubmitCommands(CommandList);

            // Once commands have been submitted, the rendered image can be presented to the application window.
            GraphicsDevice.SwapBuffers();
        }
        
        public void AddSprite(SpriteComponent sprite)
        {
            // We add it already ordered to the list
            var index = 0;
            for (; index < _sprites.Count; index++)
            {
                if (sprite.DrawOrder < _sprites[index].DrawOrder)
                {
                    break;
                }
            }
            
            _sprites.Insert(index, sprite);
        }

        public void RemoveSprite(SpriteComponent sprite)
        {
            _sprites.Remove(sprite);
        }

        public ProcessedTexture GetTexture(string fileName)
        {
            if (_textures.TryGetValue(fileName, out var texture))
            {
                return texture;
            }

            texture = new ProcessedTexture();
            if (texture.Load(GraphicsDevice, fileName))
            {
                _textures.Add(fileName, texture);
            }
            else
            {
                texture.Dispose();
                texture = null;
            }

            return texture;
        }

        public void UnloadData()
        {
            // Destroy textures
            foreach (var texture in _textures.Values)
            {
                texture.Dispose();
            }
        }

        public void Dispose()
        {
            _textures.Clear();

            CommandList?.Dispose();
            SpriteVertices?.Dispose();
            ProjectionViewLayout?.Dispose();
            ProjectionViewBuffer?.Dispose();
            ProjectionViewResourceSet?.Dispose();
            WorldTextureLayout?.Dispose();
            WorldTransformBuffer?.Dispose();
            _spritePipeline?.Dispose();
            GraphicsDevice?.Dispose();
        }
        
        private void CreateSpriteVertices()
        {
            var vertices = new[]
            {
                new Vertex(new Vector3(-0.5f, 0.5f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector2(0.0f, 0.0f)), // top left
                new Vertex(new Vector3(0.5f, 0.5f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector2(1.0f, 0.0f)), // top right
                new Vertex(new Vector3(0.5f,-0.5f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector2(1.0f, 1.0f)), // bottom right
                new Vertex(new Vector3(-0.5f,-0.5f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector2(0.0f, 1.0f)), // bottom left
            };

            var indices = new ushort[]
            {
                0, 1, 2,
                2, 3, 0
            };

            SpriteVertices = new VertexArray(GraphicsDevice, vertices, indices);
        }

        private bool LoadShaders()
        {
            _spriteShader = new ProcessedShader();
            if (!_spriteShader.Load(GraphicsDevice, "Shaders/Sprite.vert", "Shaders/Sprite.frag"))
            {
                return false;
            }

            ProjectionViewLayout = GraphicsDevice.ResourceFactory.CreateResourceLayout(
                new ResourceLayoutDescription(
                    new ResourceLayoutElementDescription("ViewProjectionBuffer", ResourceKind.UniformBuffer, ShaderStages.Vertex)));
            ProjectionViewBuffer = GraphicsDevice.ResourceFactory.CreateBuffer(
                new BufferDescription(64, BufferUsage.UniformBuffer));
            ProjectionViewResourceSet = GraphicsDevice.ResourceFactory.CreateResourceSet(
                new ResourceSetDescription(
                    ProjectionViewLayout, 
                    ProjectionViewBuffer));

            WorldTextureLayout = GraphicsDevice.ResourceFactory.CreateResourceLayout(
                new ResourceLayoutDescription(
                    new ResourceLayoutElementDescription("WorldTransformBuffer", ResourceKind.UniformBuffer, ShaderStages.Vertex),
                    new ResourceLayoutElementDescription("SurfaceTexture", ResourceKind.TextureReadOnly, ShaderStages.Fragment),
                    new ResourceLayoutElementDescription("SurfaceSampler", ResourceKind.Sampler, ShaderStages.Fragment)));
            WorldTransformBuffer = GraphicsDevice.ResourceFactory.CreateBuffer(new BufferDescription(64, BufferUsage.UniformBuffer));

            GraphicsPipelineDescription pipelineDescription = new GraphicsPipelineDescription
            {
                BlendState = BlendStateDescription.SingleAlphaBlend,
                DepthStencilState = DepthStencilStateDescription.Disabled,
                RasterizerState = RasterizerStateDescription.CullNone,
                PrimitiveTopology = PrimitiveTopology.TriangleStrip,
                ResourceLayouts = new [] { ProjectionViewLayout, WorldTextureLayout },
                ShaderSet = _spriteShader.ShaderSet,
                Outputs = GraphicsDevice.SwapchainFramebuffer.OutputDescription
            };

            _spritePipeline = GraphicsDevice.ResourceFactory.CreateGraphicsPipeline(pipelineDescription);

            // Set the view-projection matrix
            CommandList.Begin();

            Matrix4x4 viewProj = MathUtils.CreateSimpleViewProj(Window.Width, Window.Height);
            // Matrix4x4 viewProj = Matrix4x4.CreateOrthographic(Window.Width, Window.Height, -1.0f, 2.0f);
            CommandList.UpdateBuffer(ProjectionViewBuffer, 0, viewProj);

            CommandList.End();
            GraphicsDevice.SubmitCommands(CommandList);

            return true;
        }
    }
}