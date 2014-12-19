using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    class Program
    {
        MenuMgr menus;            //this component controls menu flow
        fileManager fileMgr;      //handles storage

        static void Main(string[] args)
        {
            //run the main program
            Program prog = new Program();
            prog.Start();
        }

        public void Start()
        {
            //initialize the game
            init();

            //run the menu controller
            menus.menuController();

            //we're done here. shutdown functions go here
            fileMgr.savePlayers();
        }

        void init()
        {
            fileMgr = new fileManager();
            menus = new MenuMgr(fileMgr);
            
        }
    }
}
