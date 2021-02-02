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

        // Map of textures loaded
        private readonly Dictionary<string, ProcessedTexture> _textures = new();

        // Map of meshes loaded
        private readonly Dictionary<string, Mesh> _meshes = new();

        // All the sprite components drawn
        private readonly List<SpriteComponent> _spriteComponents = new();
        
        // All mesh components drawn
        private readonly List<MeshComponent> _meshComponents = new();

        // View/projection for 3D shaders
        private Matrix4x4 _view;
        private Matrix4x4 _projection;

        public Renderer(Game game)
        {
            _game = game;
        }

        public Sdl2Window Window { get; private set; }

        public GraphicsDevice GraphicsDevice { get; private set; }

        public CommandList CommandList { get; private set; }

        public VertexArray SpriteVertices { get; private set; }

        public SpriteShader SpriteShader { get; private set; }

        public MeshShader MeshShader { get; private set; }

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
            var options = new GraphicsDeviceOptions(
                debug: true,
                swapchainDepthFormat: PixelFormat.R16_UNorm,
                syncToVerticalBlank: true,
                resourceBindingModel: ResourceBindingModel.Improved,
                preferDepthRangeZeroToOne: true,
                preferStandardClipSpaceYDirection: true);
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
            CommandList.ClearColorTarget(0, new RgbaFloat(0.0f, 0.0f, 0.0f, 1.0f));

            // Draw mesh components
            CommandList.SetPipeline(MeshShader.Pipeline);
            CommandList.SetGraphicsResourceSet(0, MeshShader.ProjectionViewResourceSet);
            CommandList.UpdateBuffer(MeshShader.ProjectionViewBuffer, 0, _projection * _view);

            foreach (var mesh in _meshComponents)
            {
                mesh.Draw(MeshShader);
            }

            // Draw all sprite components
            CommandList.SetPipeline(SpriteShader.Pipeline);
            CommandList.SetGraphicsResourceSet(0, SpriteShader.ProjectionViewResourceSet);
            foreach (var sprite in _spriteComponents)
            {
                sprite.Draw(SpriteShader);
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
            for (; index < _spriteComponents.Count; index++)
            {
                if (sprite.DrawOrder < _spriteComponents[index].DrawOrder)
                {
                    break;
                }
            }
            
            _spriteComponents.Insert(index, sprite);
        }

        public void RemoveSprite(SpriteComponent sprite)
        {
            _spriteComponents.Remove(sprite);
        }
        
        public void AddMeshComponent(MeshComponent mesh)
        {
            _meshComponents.Add(mesh);
        }

        public void RemoveMeshComponent(MeshComponent mesh)
        {
            _meshComponents.Remove(mesh);
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
        
        public Mesh GetMesh(string fileName)
        {
            if (_meshes.TryGetValue(fileName, out var mesh))
            {
                return mesh;
            }

            mesh = new Mesh();
            if (mesh.Load(fileName, this))
            {
                _meshes.Add(fileName, mesh);
            }
            else
            {
                mesh.Dispose();
                mesh = null;
            }

            return mesh;
        }

        public void UnloadData()
        {
            // Destroy textures
            foreach (var texture in _textures.Values)
            {
                texture.Dispose();
            }
            _textures.Clear();

            // Destroy meshes
            foreach (var mesh in _meshes.Values)
            {
                mesh.Dispose();
            }
            _meshes.Clear();
        }

        public void Dispose()
        {
            _textures.Clear();

            CommandList?.Dispose();
            SpriteVertices?.Dispose();
            SpriteShader?.Dispose();
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
            // Create Sprite Shader and Pipeline
            SpriteShader = new SpriteShader();
            if (!SpriteShader.Load(GraphicsDevice, "Shaders/Sprite.vert", "Shaders/Sprite.frag"))
            {
                return false;
            }

            // Set the view-projection matrix
            CommandList.Begin();
            Matrix4x4 viewProj = MathUtils.CreateSimpleViewProj(Window.Width, Window.Height);
            // Matrix4x4 viewProj = Matrix4x4.CreateOrthographic(Window.Width, Window.Height, -1.0f, 2.0f);
            CommandList.UpdateBuffer(SpriteShader.ProjectionViewBuffer, 0, viewProj);
            CommandList.End();

            // Create Mesh Shader and Pipeline
            MeshShader = new MeshShader();
            if (!MeshShader.Load(GraphicsDevice, "Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
            {
                return false;
            }

            // Set the view-projection matrix
            CommandList.Begin();
            _view = Matrix4x4.CreateLookAt(Vector3.Zero, Vector3.UnitX, Vector3.UnitZ);
            _projection = Matrix4x4.CreatePerspectiveFieldOfView(MathUtils.ToRadians(70.0f), (float)Window.Width / Window.Height, 25.0f, 10000.0f);
            CommandList.UpdateBuffer(MeshShader.ProjectionViewBuffer, 0, _projection * _view);
            CommandList.End();

            GraphicsDevice.SubmitCommands(CommandList);

            return true;
        }
    }
}