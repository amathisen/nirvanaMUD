#include "/players/mokri/define.h"

main(string str)
{
   string name, mess;
   object who;
   
   if(!str)
      {
      write("Tell who what?\n");
      return;
   }
   
   if(sscanf(str,"%s %s",name,str) != 2)
      {
      write("Tell who what?\n");
      return;
   }
   
   who = find_player(name);
   if(!who)
      {
      write("I don't see that person anywhere...\n");
      return 1;
   }
   
   mess = HIR + CAP(this_player()->query_real_name())+" tells you: " + str + NORM + "\n";
   tell_object(who,mess);
   write(HIR+"You tell " + CAP(name) + ": " + str + NORM+"\n");
   who->add_tellhistory(mess);
   who->Replyer(this_player()->query_real_name());
   return 1;
}
