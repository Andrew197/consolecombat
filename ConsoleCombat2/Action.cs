using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleCombat2
{
    //this class wraps up data about battle actions in a convenient container.
    class Action
    {
        int id;
        int user;
        int target;

        public Action(int nid, int nuser, int ntarget)
        {
            id = nid;
            user = nuser;
            target = ntarget;
        }

        public int getID()     { return id;     }
        public int getUser()   { return user;   }
        public int getTarget() { return target; }
    }
}
