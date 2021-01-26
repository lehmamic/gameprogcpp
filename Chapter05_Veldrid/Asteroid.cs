using System.Numerics;

namespace Chapter05
{
    public class Asteroid : Actor
    {
        public Asteroid(Game game)
            : base(game)
        {
            // Initialize to random position/orientation
            Vector2 randPos = Random.GetVector(new Vector2(-512.0f, -384.0f), new Vector2(512.0f, 384.0f));
            Position = randPos;

            Rotation = Random.GetFloat(0.0f, (float)MathUtils.TwoPi);

            // Create a sprite component
            SpriteComponent sprite = new SpriteComponent(this);
            sprite.SetTexture(Game.Renderer.GetTexture("Assets/Asteroid.png"));

            // Create a move component, and set a forward speed
            MoveComponent move = new MoveComponent(this)
            {
                ForwardSpeed = 150.0f,
            };

            // Create a circle component (for collision)
            Circle = new CircleComponent(this)
            {
                Radius = 40.0f,
            };

            // Add to mAsteroids in game
            Game.AddAsteroid(this);
        }

        public CircleComponent Circle { get; }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            Game.RemoveAsteroid(this);
        }
    }
}