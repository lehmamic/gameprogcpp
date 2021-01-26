using System;
using System.ComponentModel.DataAnnotations;
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

        public override void ProcessInput(InputState input)
        {
            // Calculate forward speed for MoveComponent
            float forwardSpeed = 0.0f;
            if (input.Keyboard.GetKeyValue(ForwardKey))
            {
                forwardSpeed += MaxForwardSpeed;
            }

            if (input.Keyboard.GetKeyValue(BackKey))
            {
                forwardSpeed -= MaxForwardSpeed;
            }
            ForwardSpeed = forwardSpeed;

            // Calculate angular speed for MoveComponent
            float angularSpeed = 0.0f;
            if (input.Keyboard.GetKeyValue(ClockwiseKey))
            {
                angularSpeed += MaxAngularSpeed;
            }

            if (input.Keyboard.GetKeyValue(CounterClockwiseKey))
            {
                angularSpeed -= MaxAngularSpeed;
            }
            AngularSpeed = angularSpeed;
        }
    }
}