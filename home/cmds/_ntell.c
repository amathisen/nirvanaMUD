#include "/players/mokri/define.h"

main(string str) {
   int x;
   string name;
   object who;
   
   if(!str || sscanf(str,"%s %s",name,str) != 2)
      {
      write("Syntax: ntell <player> <message>\n");
      return 1;
   }
   
   who = find_player(name);
   
   if(!who) {
      write("Nope.\n");
      return 1; }
   
   tell_object(who,"\n"+HIR+TPN+" tells you: "+str+"\n"+NORM);
   write(HIR+"You tell "+CAP(name)+": "+str+"\n"+NORM);
   return 1; }
