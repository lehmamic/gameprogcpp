using System.Linq;
using Veldrid;

namespace Chapter05
{
    public class InputComponent : MoveComponent
    {
        public InputComponent(Actor owner)
            : base(owner)
        {
        }

        public float MaxForwardSpeed { get; set; }

        public float MaxAngularSpeed { get; set; }

        public Key ForwardKey { get; set; }

        public Key BackKey { get; set; }

        public Key ClockwiseKey { get; set; }

        public Key CounterClockwiseKey { get; set; }

        public override void ProcessInput(InputSnapshot input)
        {
            var keyEvents = input.KeyEvents.ToDictionary(e => e.Key);
            KeyEvent e;

            // Calculate forward speed for MoveComponent
            float forwardSpeed = 0.0f;
            if (keyEvents.TryGetValue(ForwardKey, out e) && e.Down)
            {
                forwardSpeed += MaxForwardSpeed;
            }

            if (keyEvents.TryGetValue(BackKey, out e) && e.Down)
            {
                forwardSpeed -= MaxForwardSpeed;
            }
            ForwardSpeed = forwardSpeed;

            // Calculate angular speed for MoveComponent
            float angularSpeed = 0.0f;
            if (keyEvents.TryGetValue(ClockwiseKey, out e) && e.Down)
            {
                angularSpeed += MaxAngularSpeed;
            }

            if (keyEvents.TryGetValue(CounterClockwiseKey, out e) && e.Down)
            {
                angularSpeed -= MaxAngularSpeed;
            }
            AngularSpeed = angularSpeed;
        }
    }
}