using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    class Player
    {
        //stats
        String name;
        int hp;
        int sp;
        int str;
        int def;
        int spd;
        int dex;

        int statPts;
        int level;

        //temporaries for in-game
        int     superPts;
        Boolean defending;
        Boolean dodging;
        Boolean alive;
        Boolean humanPlayer;

        public Boolean isHuman() { return humanPlayer; }


        public String getName() { return name; }
        public int getStat(String stname)
        {
            switch(stname)
            {
                case "hp": { return hp; break; }
                case "sp": { return sp; break; }
                case "str": { return str; break; }
                case "def": { return def; break; }
                case "spd": { return spd; break; }
                case "dex": { return dex; break; }
                case "level": { return level; break; }
                case "statPts": { return statPts; break; }
            }
            return -1;
        }

        //this function initializes a new player file.
        void init()
        {
            hp = 100;
            sp = 10;
            str = 5;
            def = 5;
            spd = 5;
            dex = 5;
            level = 1;
            statPts = 5;
        }

        //create a player with given stats.
        public Player(String newName, int nhp, int nsp, int nstr, int ndef, int nspd, int ndex, int nstat, int nlev)
        {
            name = newName;
            hp = nhp;
            sp = nsp;
            str = nstr;
            def = ndef;
            spd = nspd;
            dex = ndex;
            statPts = nstat;
            level = nlev;
        }

        //for random players and empty slot players
        public Player(String newName, int tstatPts)
        {
            name = newName;
            Random rand = new Random();
            int randNum;

            if (tstatPts == 0) init();
            else
            {
                while (tstatPts > 0)
                {
                    //randonmly pick a stat to boost
                    randNum = rand.Next(1, 6);

                    //distribute stats according to random integer
                    if (randNum == 1) hp += 10;
                    else if (randNum == 2) sp += 2;
                    else if (randNum == 3) str += 1;
                    else if (randNum == 4) def += 1;
                    else if (randNum == 5) spd += 1;
                    else if (randNum == 6) dex += 1;
                }
            }
        }

        //Battle-Related Temporary Functions
        public void battlePrep()
        {
            defending = false;
            dodging = false;
            alive = true;
            superPts = 0;
        }
    }
}
