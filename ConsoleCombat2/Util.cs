using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    class Util
    {
        Random rand = new Random();
        public int randint(int range)
        {
            return rand.Next(1, range);
        }

        public static void cls()
        {
            try
            {
                Console.Clear();
                return;
            }
            catch(System.IO.IOException e)
            {
                return;
            }
        }

    }
}
