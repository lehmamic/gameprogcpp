namespace Chapter05
{
    public class InputState
    {
        public InputState(KeyboardState keyboard)
        {
            Keyboard = keyboard;
        }

        public KeyboardState Keyboard { get; }
    }
}