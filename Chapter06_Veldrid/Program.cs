namespace Chapter06
{
    public static class Program
    {
        public static void Main(string[] args)
        {
            Game game = new();
            bool success = game.Initialize();

            if (success) {
                game.RunLoop();
            }

            game.Dispose();
        }
    }
}
