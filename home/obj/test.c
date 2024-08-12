#include "/players/mokri/define.h"

inherit "/obj/treasure.c";
reset(int arg) {
   if(arg) return;
   set_id("test");
   set_short("a test");
   set_long("blah. 'tell player message'\n");
}

init()
{
   ::init();
   add_action("tell","tell");
}

tell(string str) {
   int x;
   string name;
   object who;
   
   if(!str || sscanf(str,"%s %s",name,str) != 2)
      {
      write("Syntax: tell <player> <message>\n");
      return 1;
   }
   
   who = find_player(name);
   
   if(!who) {
      write("Nope.\n");
      return 1; }
   
   if(who->query_invis()) write("Warning: " + CAP(who->query_real_name()) + " is invisible.\n");
   if(in_editor(who)) write("Warning: " + CAP(who->query_real_name()) + " is editing.\n");
   write(HIM+"You tell "+CAP(who->query_real_name())+", \""+NORM+str+HIM+"\"\n"+NORM);
   name = HIM+TPN+" tells you, \""+NORM+str+HIM+"\""+NORM;
   tell_object(who,name + "\n");
   who->add_tellhistory(name);
   who->Replyer(this_player()->query_real_name());
   return 1; }

drop() { return 1; }
