using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    class Battle
    {
        Player[] players;
        Player winner;
        int turn;

        public Battle(Player [] pls)
        {
            players = pls;
            winner = null;

            foreach (Player pl in players) pl.battlePrep();
            turn = 0;
        }

        public void start()
        {

        }

        public void mainloop()
        {
            if (players[turn].isHuman()) playerTurn();
            else                         cpuTurn();

            changeTurn();
        }

        private void playerTurn()
        {
            //MENU CONTROL
        }

        private void cpuTurn()
        {
            //AI LOGIC
        }

        private void changeTurn()
        {
            if (turn != players.Length) turn++;
            else turn = 0;
        }
    }
}
