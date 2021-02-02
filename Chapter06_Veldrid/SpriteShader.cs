using Veldrid;

namespace Chapter06
{
    public class SpriteShader : ProcessedShader
    {
        public DeviceBuffer WorldTransformBuffer { get; private set; }

        public ResourceLayout WorldTextureLayout { get; private set; }

        public ResourceLayout ProjectionViewLayout { get; private set; }

        public DeviceBuffer ProjectionViewBuffer { get; private set; }

        public ResourceSet ProjectionViewResourceSet { get; private set; }

        public Pipeline Pipeline { get; private set; }

        public override bool Load(GraphicsDevice graphicsDevice, string vertexShaderFileName, string fragmentShaderFileName)
        {
            if (!base.Load(graphicsDevice, vertexShaderFileName, fragmentShaderFileName))
            {
                return false;
            }

            ProjectionViewLayout = graphicsDevice.ResourceFactory.CreateResourceLayout(
                new ResourceLayoutDescription(
                    new ResourceLayoutElementDescription("ViewProjectionBuffer", ResourceKind.UniformBuffer, ShaderStages.Vertex)));
            ProjectionViewBuffer = graphicsDevice.ResourceFactory.CreateBuffer(
                new BufferDescription(64, BufferUsage.UniformBuffer));
            ProjectionViewResourceSet = graphicsDevice.ResourceFactory.CreateResourceSet(
                new ResourceSetDescription(
                    ProjectionViewLayout, 
                    ProjectionViewBuffer));

            WorldTextureLayout = graphicsDevice.ResourceFactory.CreateResourceLayout(
                new ResourceLayoutDescription(
                    new ResourceLayoutElementDescription("WorldTransformBuffer", ResourceKind.UniformBuffer, ShaderStages.Vertex),
                    new ResourceLayoutElementDescription("SurfaceTexture", ResourceKind.TextureReadOnly, ShaderStages.Fragment),
                    new ResourceLayoutElementDescription("SurfaceSampler", ResourceKind.Sampler, ShaderStages.Fragment)));
            WorldTransformBuffer = graphicsDevice.ResourceFactory.CreateBuffer(new BufferDescription(64, BufferUsage.UniformBuffer));

            var pipelineDescription = new GraphicsPipelineDescription
            {
                BlendState = BlendStateDescription.SingleAlphaBlend,
                DepthStencilState = DepthStencilStateDescription.Disabled,
                RasterizerState = RasterizerStateDescription.CullNone,
                PrimitiveTopology = PrimitiveTopology.TriangleStrip,
                ResourceLayouts = new [] { ProjectionViewLayout, WorldTextureLayout },
                ShaderSet = ShaderSet,
                Outputs = graphicsDevice.SwapchainFramebuffer.OutputDescription
            };

            Pipeline = graphicsDevice.ResourceFactory.CreateGraphicsPipeline(pipelineDescription);

            return true;
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (disposing)
            {
                ProjectionViewLayout?.Dispose();
                ProjectionViewBuffer?.Dispose();
                ProjectionViewResourceSet?.Dispose();
                WorldTextureLayout?.Dispose();
                WorldTransformBuffer?.Dispose();
            }
        }
    }
}