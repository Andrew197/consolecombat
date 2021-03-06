﻿using System;
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
        int activeUser;
        Util util;

        //normal battle
        //need to add reference ID's for this.
        public Battle(Player [] pls)
        {
            util = new Util();
            players = pls;
            winner = null;

            foreach (Player pl in players) pl.battlePrep();
            activeUser = 0;
        }

        //this is a random battle. I will implement random battles first.
        public Battle(String[] names, int statPnts, String mode)
        {
            util = new Util();
            //create the players
            Player[] pls = new Player[names.Length];
            for (int i = 0; i < names.Length;i++ )
            {
                pls[i] = new Player(names[i], statPnts);
                pls[i].battlePrep();
            }
            if (mode != "SIMULATE") pls[0].setHuman(true);

            activeUser = 0;
            //run the battle. not ready yet.
            mainloop();
        }

        public void start()
        {

        }

        public void mainloop()
        {
            //draw battle header
            Draw.battleHeader(players);

            //either the player or the cpu must choose an action
            Action act;

            //array of message ID's to display after a turn's action has happened.
            int[] messages;

            do
            {
                //if this player is alive, take its turn. if not, skip this block.
                if (players[activeUser].isAlive())
                {
                    //status effects reset when it is your turn.
                    if (players[activeUser].isDefending()) players[activeUser].undefend();
                    if (players[activeUser].isDodging()) players[activeUser].undodge();

                    //decide if a selection menu is needed or if AI is needed.
                    if (players[activeUser].isHuman()) act = playerTurn();
                    else act = cpuTurn();

                    doBattleAction(act);

                    
                    //check for KO or win conditions
                    //devaluation of stats should happen inside the action handlers, NOT here.

                    //redraw screen
                    //display messages
                }
                changeTurn();
            }
            while (1 == 1);
            
            //end of battle code
        }

        //action enumerations
        //1: attack
        //2: item
        //3: Defend
        //4: dodge
        //5: super attack OR rest (cannot do a SUPER if no SP now)

        void doBattleAction(Action act)
        {
            int damage;
            switch(act.getID())
            {
                case 1:
                    damage = attack(act.getUser(), act.getTarget());
                    break;
                case 2:
                    item();
                    players[act.getUser()].useItem();
                    break;
                case 3:
                    players[act.getUser()].defend();
                    break;
                case 4:
                    players[act.getUser()].dodge();
                    break;
                case 5:
                    superAttack(act.getUser(), act.getTarget());
                    break;

            }
        }

        //FULLY IMPLEMENTED + NEW FEATURES
        int item()
        {
            //the "communism" item
            int statToChange = util.randint(6);
            int newStat = 0;
            int numAlivePlayers = 0;

            for(int i=0;i<players.Length;i++)
            {
                if (players[i].isAlive())
                {
                    numAlivePlayers++;

                    //figure out how much of the picked stat is out there.
                    switch (statToChange)
                    {
                        case 1:
                            newStat += players[i].getStat("hp");
                            break;
                        case 2:
                            newStat += players[i].getStat("sp");
                            break;
                        case 3:
                            newStat += players[i].getStat("str");
                            break;
                        case 4:
                            newStat += players[i].getStat("def");
                            break;
                        case 5:
                            newStat += players[i].getStat("spd");
                            break;
                        case 6:
                            newStat += players[i].getStat("dex");
                            break;
                    }

                    //divide the pool of stats by how many living players we've got
                    newStat /= numAlivePlayers;

                    //set the new stat value to everyone.
                    switch (statToChange)
                    {
                        case 1:
                            players[i].setStat("hp", newStat);
                            break;
                        case 2:
                            players[i].setStat("sp", newStat);
                            break;
                        case 3:
                            players[i].setStat("str", newStat);
                            break;
                        case 4:
                            players[i].setStat("def", newStat);
                            break;
                        case 5:
                            players[i].setStat("spd", newStat);
                            break;
                        case 6:
                            players[i].setStat("dex", newStat);
                            break;
                    }
                }
            }
            return statToChange;
        }

        //PARTIALLY IMPLEMENTED - VERIFY RETURN VALUE
        int superAttack(int usr, int tgt)
        {
            //super attack damage calculation
            int damage = attack(usr, tgt);
            if (damage < 8) damage = 8;
            damage *= 6;

            //damage the player
            players[tgt].damage(damage);
            players[usr].degradeStat("str", 1);

            return damage;
        }

        //FULLY IMPLEMENTED
        int attack(int usr, int tgt)
        {
            if (players[usr].getStat("sp") >= 2)
            {
                //stat variance
                int damage = 0;
                int randDamage = util.randint(20);
                int randSpeed = util.randint(players[tgt].getStat("spd"));
                int randDex = util.randint(players[usr].getStat("dex"));

                /*random speed and dex are "capped" such that they will never
                fall below 75% of the user's stats.*/

                //dodge calculation - speed component
                if (randSpeed < players[tgt].getStat("spd") * 0.75) randSpeed = (int)(players[tgt].getStat("spd") * 0.75);
                randSpeed = randSpeed + util.randint(players[tgt].getStat("spd") / 4);

                //dodge calculation - dex component
                if (randDex < players[usr].getStat("dex") * 0.75) randDex = (int)(players[usr].getStat("dex") * 0.75);
                randDex = randDex + util.randint(players[usr].getStat("dex") / 4);

                if (randDex > randSpeed)
                {
                    //attack hit the target
                    damage = randDamage + (players[usr].getStat("str") - players[tgt].getStat("def"));
                    if (damage <= 0) damage = 0;
                    return damage;
                }
                else
                {
                    //-3 is a special code for an attack miss
                    return -3;
                }
            }
            else
            {
                //-2 is a special code for insufficient SP
                return -2;
            }
        }

        private Action playerTurn()
        {
            //MENU CONTROL SHOULD BE IMPLEMENTED INSIDE THIS FUNCTION
            //passing data values on the state of the battle to Draw might work.

            //create a null action, for now.
            //parameter notes. first is the action type/id, second is user, third is target
            //need to implement the select a target menu before this area will work.
            Action act = new Action(-1,-1,-1);
            return act;
        }

        //action enumerations
        //1: attack
        //2: item
        //3: Defend
        //4: dodge
        //5: super attack OR rest (cannot do a SUPER if no SP now)

        //this function outlines the "main ai"
        //in the future, a dumber classic ai may be included.
        private Action cpuTurn()
        {
            //AI LOGIC

            //super attack
            //always super if you have 5 super points.
            if (players[activeUser].getStat("super") == 5)
            {
                //super attack
                //need choose target logic for this.
            }

            //dodge only if the user's speed is greater than def.
            if (players[activeUser].getStat("def") < players[activeUser].getStat("spd"))
            {
                //if any player has full super, don't consider dodging.
                Boolean shouldDodge = true;
                for (int i = 0; i < players.Length; i++)
                {
                    if (players[i].getStat("super") == 5)
                    {
                        //super attacks cannot be dodged, so don't bother trying.
                        shouldDodge = false;
                    }  
                }
                if (shouldDodge)
                {
                    //do a dodge: id 4, user = activeUser, target = null
                    return new Action(4, activeUser, -1);
                }
            }

            //defend
            //if any player has full super, consider defending.
            Boolean shouldDefend = false;
            for (int i = 0; i < players.Length; i++)
            {
                if (players[i].getStat("super") == 5)
                {
                    //super attacks cannot be dodged, so don't bother trying.
                    shouldDefend = true;
                }
            }
            if (shouldDefend)
            {
                //75% chance of dodging if someone can super
                if (util.randint(4) > 1) return new Action(3, activeUser, -1);
            }

            //use item
            //maybe only do this on a higher difficulty.

            //rest
            //if SP is low, rest.

            //standard attack
            //should be the last option considered.
            return null;
        }

        private void changeTurn()
        {
            if (activeUser != players.Length) activeUser++;
            else activeUser = 0;
        }
    }
}
