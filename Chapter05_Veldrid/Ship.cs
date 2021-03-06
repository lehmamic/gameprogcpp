using System.Linq;
using Veldrid;

namespace Chapter05
{
    public class Ship : Actor
    {
        private float _laserCooldown = 0.0f;

        public Ship(Game game) : base(game)
        {
            // Create a sprite component
            var sprite = new SpriteComponent(this, 150);
            sprite.SetTexture(game.Renderer.GetTexture("Assets/Ship.png"));

            // Create an input component and set keys/speed
            var input = new InputComponent(this)
            {
                ForwardKey = Key.W,
                BackKey = Key.S,
                ClockwiseKey = Key.A,
                CounterClockwiseKey = Key.D,
                MaxForwardSpeed = 300.0f,
                MaxAngularSpeed = MathUtils.TwoPi,
            };
        }

        protected override void UpdateActor(float deltaTime)
        {
            _laserCooldown -= deltaTime;
        }

        protected override void ActorInput(InputState input)
        {
            if (input.Keyboard.GetKeyState(Key.Space) == ButtonState.Pressed && _laserCooldown <= 0.0f)
            {
                // Create a laser and set its position/rotation to mine
                var laser = new Laser(Game)
                {
                    Position = Position,
                    Rotation = Rotation,
                };

                // Reset laser cooldown (half second)
                _laserCooldown = 0.5f;
            }
        }
    }
}