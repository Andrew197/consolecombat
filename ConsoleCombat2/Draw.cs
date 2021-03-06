﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    static class Draw
    {
        //abstracted console functions, so all write calls can easily be redirected to a GUI instead later.
        public static void writeLine(String s) { System.Console.WriteLine(s); }
        public static void writeLine() { System.Console.Write("\n"); }
        public static void write(String s) { System.Console.Write(s); }

        //menu building blocks
        public static void line() { writeLine("-------------------------------------------------------------------------------"); }
        public static void thirdLine() { writeLine("-------------------------"); }
        public static void prompt() { write("\n>"); }

        public static void playerManager()
        {
            Util.cls();
            headersmall("Player Manager");
            writeLine();
            writeLine("1: Create New Player");
            writeLine("2: Rename Player");
            writeLine();
            writeLine("3: Go Back");
        }

        public static void playerManager_create_screen1()
        {
            //draw playermanager screen
            Util.cls();
            headersmall("Choose Save Slot");
            writeLine();
        }

        public static void playerManager_create_screen2()
        {
            Util.cls();
            headersmall("Enter a Name");
        }

        public static void header(String title)
        {
            line();
            writeLine(title);
            line();
        }

        public static void headersmall(String title)
        {
            thirdLine();
            writeLine(title);
            thirdLine();
        }

        public static void updateStats(int refID)
        {
            Util.cls();
            header("Update Stats");
            writeLine();
            //drawPlayerData(fileMgr.getPlayer(refID), -1);
            writeLine();
            writeLine("1: HP + 10");
            writeLine("2: SP + 2");
            writeLine("3: STR + 1");
            writeLine("4: DEF + 1");
            writeLine("5: SPD + 1");
            writeLine("6: DEX + 1");
            writeLine();
        }

        public static void playerData(Player pl, int i)
        {
            write("\n");
            if (pl.getName() != "empty slot")
                write(i.ToString() + String.Format("{0,14:D}", pl.getName())
                       + " HP:" + String.Format("{0,4:D}", pl.getStat("hp"))
                       + " SP:" + String.Format("{0,3:D}", pl.getStat("sp"))
                       + " STR:" + String.Format("{0,3:D}", pl.getStat("str"))
                       + " DEF:" + String.Format("{0,3:D}", pl.getStat("def"))
                       + " SPD:" + String.Format("{0,3:D}", pl.getStat("spd"))
                       + " DEX:" + String.Format("{0,3:D}", pl.getStat("dex"))
                       + "  LEVEL:" + String.Format("{0,2:D}", pl.getStat("level"))
                       + " EXP:" + String.Format("{0,2:D}", pl.getStat("statPts")));
            else write(i + " " + String.Format("{0,10:D}", pl.getName()));
        }

        public static void playerData(Player pl)
        {
            write("\n");
            write(String.Format("{0,14:D}", pl.getName())
               + " HP:" + String.Format("{0,4:D}", pl.getStat("hp"))
               + " SP:" + String.Format("{0,3:D}", pl.getStat("sp"))
               + " STR:" + String.Format("{0,3:D}", pl.getStat("str"))
               + " DEF:" + String.Format("{0,3:D}", pl.getStat("def"))
               + " SPD:" + String.Format("{0,3:D}", pl.getStat("spd"))
               + " DEX:" + String.Format("{0,3:D}", pl.getStat("dex"))
               + "  LEVEL:" + String.Format("{0,2:D}", pl.getStat("level"))
               + " EXP:" + String.Format("{0,2:D}", pl.getStat("statPts")));
        }

        public static void mainMenu()
        {
            Util.cls();

            headersmall("Ranked Games");
            writeLine("1: New Ranked Game");
            writeLine("2: Manage Players");
            writeLine("3: Rankings and Statistics");

            writeLine();

            headersmall("Quick Play");
            writeLine("4: Generate Random Game");

            writeLine();

            writeLine("5: Exit Game");
        }

        public static void battleHeader(Player[] pls)
        {
            line();
            for(int i=0;i<pls.Length;i++)
            {
                playerData(pls[i]);
            }

        }
    }
}
