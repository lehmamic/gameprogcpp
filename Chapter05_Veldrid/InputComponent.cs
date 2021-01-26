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
            // Calculate forward speed for MoveComponent
            float forwardSpeed = 0.0f;
            if (input.KeyEvents.Any(e => e.Key == ForwardKey && e.Down))
            {
                forwardSpeed += MaxForwardSpeed;
            }

            if (input.KeyEvents.Any(e => e.Key == BackKey && e.Down))
            {
                forwardSpeed -= MaxForwardSpeed;
            }
            ForwardSpeed = forwardSpeed;

            // Calculate angular speed for MoveComponent
            float angularSpeed = 0.0f;
            if (input.KeyEvents.Any(e => e.Key == ClockwiseKey && e.Down))
            {
                angularSpeed += MaxAngularSpeed;
            }

            if (input.KeyEvents.Any(e => e.Key == CounterClockwiseKey && e.Down))
            {
                angularSpeed -= MaxAngularSpeed;
            }
            AngularSpeed = angularSpeed;
        }
    }
}