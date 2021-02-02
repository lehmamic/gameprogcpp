namespace Chapter06
{
    public class PlaneActor : Actor
    {
        public PlaneActor(Game game) : base(game)
        {
            Scale = 10.0f;
            var mc = new MeshComponent(this) {Mesh = Game.Renderer.GetMesh("Assets/Plane.gpmesh")};
        }
    }
}