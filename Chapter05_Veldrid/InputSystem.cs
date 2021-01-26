using System;
using System.Collections.Generic;
using System.Linq;
using Veldrid;

namespace Chapter05
{
    public class InputSystem
    {
        public InputState State { get; private set; }

        public bool Initialize()
        {
            var keys = Enum.GetValues<Key>().Distinct().ToArray();
            var keyboardState = new KeyboardState(
                keys.ToDictionary(key => key, key => false),
                keys.ToDictionary(key => key, key => false));

            State = new InputState(keyboardState);

            return true;
        }

        public void Update(InputSnapshot input)
        {
            // Update Keyboard State
            var previousKeyStates = State.Keyboard.CurrentKeyStates;
            var currentKeyStates = new Dictionary<Key, bool>(previousKeyStates);
            foreach (var keyEvent in input.KeyEvents.Where(e => !e.Repeat))
            {
                currentKeyStates[keyEvent.Key] = keyEvent.Down;
            }

            var currentBoardState = new KeyboardState(previousKeyStates, currentKeyStates);

            State = new InputState(currentBoardState);
        }
    }
}