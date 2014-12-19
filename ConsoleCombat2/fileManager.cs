using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace ConsoleCombat2
{
    class fileManager
    {
        Util util;
        String [] cpuNames;
        Player [] savedPlayers;

        public Player[] getAllPlayers() { return savedPlayers; }
        public Player getPlayer(int refID) { return savedPlayers[refID];  }

        public fileManager()
        {
            util = new Util();
            savedPlayers = new Player[10];
            loadCPUNames();
            loadPlayers();
        }

        public void replacePlayer(Player pl, int refID)
        {
            savedPlayers[refID] = pl;
        }

        //this function writes the savedPlayers array to players.dat
        public void savePlayers()
        {
            //open the file

            StreamWriter file = new StreamWriter("players.dat", false);

            for(int i=0;i<savedPlayers.Length;i++)
            {
                //each block has one line for the name, and one line for stat data
                //fetch the name and all stats from the player object, and write it. 
                file.WriteLine(savedPlayers[i].getName());
                file.WriteLine(savedPlayers[i].getStat("hp") + " " +
                               savedPlayers[i].getStat("sp") + " " +
                               savedPlayers[i].getStat("str") + " " +
                               savedPlayers[i].getStat("def") + " " +
                               savedPlayers[i].getStat("spd") + " " +
                               savedPlayers[i].getStat("dex") + " " +
                               savedPlayers[i].getStat("statPts") + " " +
                               savedPlayers[i].getStat("level")
                    );
            }
            //close the file
            file.Close();
        }

        //load the players.dat file (the saved game file), if it exists.
        private void loadPlayers()
        {
            try
            {
                //file related variables
                StreamReader file = new StreamReader("players.dat");
                Boolean nameLine = true;
                String line;

                //player-related temporaries
                int playerID = 0;
                String newName;

                while ((line = file.ReadLine()) != null)
                {
                    if (nameLine)
                    {
                        //start of a new player block - save the name
                        newName = line;

                        //read the player's stats
                        line = file.ReadLine();

                        if (newName != "empty slot")
                        {

                            //convert the file line into an array of integers
                            String[] stats = line.Split(' ');
                            int[] nstats = new int[8];
                            for (int i = 0; i < stats.Length; i++) nstats[i] = Int32.Parse(stats[i]);

                            //create a player with the converted data
                            for (int i = 0; i < stats.Length; i++)
                                savedPlayers[playerID] = new Player(newName, nstats[0], nstats[1], nstats[2], nstats[3], nstats[4], nstats[5], nstats[6], nstats[7]);
                        }
                        else
                        {
                            //this player has no data
                            savedPlayers[playerID] = new Player(newName, 0);
                        }
                        //move on to the next player
                        playerID++;
                    }
                    //no more players detected, close file
                    file.Close();
                }
            }
            catch(Exception e)
            
            {
                //no data file exists. just initialize an empty array
                String newName = "empty slot";
                for (int i = 0; i < 10; i++) savedPlayers[i] = new Player(newName, 0);
            }

        }

        //load all of the CPU player names from file. So easy to do in C sharp.
        private void loadCPUNames() { cpuNames = System.IO.File.ReadAllLines("cpunames.dat"); }

        //returns a random name from the list of CPU player names.
        public String getCpuName()
        {
            int rand = util.randint(cpuNames.Length);
            return cpuNames[rand];
        }


    }
}

