using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    class MenuMgr
    {
        fileManager fileMgr;        //needed so menus can access the game data
               
        public MenuMgr(fileManager fm) { fileMgr = fm; } 
 
        //get a string from the user, mostly for naming players.
        String getStringInput()
        {
            String line = Console.ReadLine();
            return line;
        }

        //get numerical input for menus, with error checking.
        int getInput()
        {   
            String line = Console.ReadLine();
            String[] split = line.Split(' ');
            int result;

            //if parsing as an int32 fails, its not a number - and theefore invalid input.
            try
            {
                result = Int32.Parse(split[0]);
                return result;
            }
            catch(System.FormatException e)
            {
                return -1;
            }
        }

        //the main branching function for menus. send it an integer to the menu you need.
        public void menuController()
        {
            String menuID = "main";
            while(true)
            {
                switch(menuID)
                {
                    case "main": { menuID = mainMenu(); break; }
                    case "playermgr": { menuID = playerManager(); break; }
                    case "playermgr_create": { menuID = playerManager_create() ; break; }
                    //case "updatestats": { menuID = updateStats(); break; }
                    case "stats": { menuID = showStats(); break; }
                    case "quit": { break; }
                    default: { menuID = "main"; break; }
                }
                if (menuID == "quit") break;
            }
        }

        //---------------------------------------------------------------
        //MAIN MENU
        //---------------------------------------------------------------
        private String mainMenu()
        {
            int input = 0;
            while(input < 1 || input > 5)
            {
                Draw.mainMenu();
                Draw.prompt();
                input = getInput();
            }

            switch(input)
            {
                case 1: return "newgame";
                case 2: return "playermgr";
                case 3: return "stats";
                case 4: return "randomBattle";
                case 5: return "quit";
            }
            return "main";
        }

        String playerManager()
        {
            int input = 0;
            while (input < 1 || input > 3)
            {
                Draw.playerManager();
                Draw.prompt();
                input = getInput();
            }

            switch (input)
            {
                case 1: return "playermgr_create";
                case 2: return "playermgr";
                case 3: return "main";
            }
            return "main";
        }

        String playerManager_create()
        {
            int input = -1;
            Player[] savedpl = fileMgr.getAllPlayers();

            while (input < 0 || input > 9)
            {
                Draw.playerManager_create_screen1();
                for (int i = 0; i < savedpl.Length; i++) Draw.playerData(savedpl[i], i);

                Draw.prompt();
                input = getInput();
            }
            
            if (savedpl[input].getName() == "empty slot") { playerManager_name(input); return "main"; }
            else return "main";
        }

        String playerManager_name(int refID)
        {
            String input = "empty slot";
            while (input == "empty slot" || input.Length > 14)
            {
                Draw.playerManager_create_screen2();
                Draw.prompt();
                input = getStringInput();
            }

            Player pl = new Player(input, 0);
            fileMgr.replacePlayer(pl, refID);
            updateStats(refID);
            return "main";
        }

        void updateStats(int refID)
        {
            
            int statPoints = fileMgr.getPlayer(refID).getStat("statPts");
            int input = -1;

            while(statPoints > 0)
            {
                Draw.updateStats(refID);
                Draw.prompt();
                input = getInput();
            }
        }

        String showStats()
        {
            //draw the header
            Util.cls();
            Draw.header("Player Statistics");

            //fetch data
            Player[] data = fileMgr.getAllPlayers();

            //draw the data, battle style
            for (int i = 0; i < data.Length;i++ )
            {
                Draw.playerData(data[i], i);
            }

                getInput();
            return "main";
        }


    }
}

