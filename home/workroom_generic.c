#include <ansi.h>
inherit "/players/vertebraker/closed/std/room.c";
int shield;
string owner_name = "oldkrow";
string *exemptPlayers;

reset(arg) {
   if(arg) return;
   set_light(0);
   set_short("A room (open)");
   set_long("\n\t\theh\n\t\tShield OFF\n\n");
   add_exit("/room/vill_green","green");
}

init()
{
   object me;
   ::init();

   if(!exemptPlayers)
      exemptPlayers = ({ });

   if(this_player()->is_player() && shield > 0 && this_player()->query_real_name() != owner_name && member(exemptPlayers,(string)this_player()->query_real_name()) == -1)
      {
      me = find_player(owner_name);
      if(me)
         tell_object(me,HIG+"["+NORM+"SHIELD"+HIG+"]"+NORM+" " +capitalize(this_player()->query_real_name())+" tried to enter your workroom.\n");

      tell_object(this_player(),"You bounce off of " + capitalize(owner_name) + "'s workroom shield.\n");

      switch(shield) {
         case 1:
         move_object(this_player(),"/room/vill_green");
         break;
         case 2:
         destruct(this_player());
         break;
         case 3:
         move_object(this_player(),"/players/mokri/room/yep");
         break;
      }

      if(this_player() && present(this_player(),this_object()) && me)
         tell_object(me,HIG+"["+NORM+"SHIELD"+HIG+"]"+NORM+" " +capitalize(this_player()->query_real_name())+" is still here.\n");
      return 1;
   }

   add_action("shield","shield");
   add_action("exempt","exempt");
   add_action("make","make");
}

int exempt(string str) {

   int x;

   if((string)this_player()->query_real_name() != owner_name) return 0;

   if(!str) {
      write("Exempt players\n==============\n");
      if(!exemptPlayers || sizeof(exemptPlayers) == 0)
         write("None.\n");
      else {
         for(x = 0; x < sizeof(exemptPlayers); x++) {
            write(capitalize(exemptPlayers[x])+"\n");
         }
      }

      return 1;
   }

   if(str == "clear_all") {
      exemptPlayers = ({ });
      write("Exempt list cleared.\n");
      return 1;
   }

   if(!exemptPlayers)
      exemptPlayers = ({ });

   str = lower_case(str);

   if(member(exemptPlayers,str) > -1) {
      exemptPlayers -= ({ str });
      write("Removed player " + capitalize(str) + " from exempt list.\n");
      return 1;
   }

   exemptPlayers += ({ lower_case(str) });
   write("Added player " + capitalize(str) + " to exempt list.\n");

   return 1;
}

int make(string str) {
   int x;
   string *tmp,*names,*paths,*makeItems;

   if((string)this_player()->query_real_name() != owner_name)
      return 0;

   makeItems = ({"greenboard^/players/snow/ITEMS/notice","wizboard^/obj/wiz_bull_board","wizboard2^/obj/wiz_bull_board2","obit^/players/llew/Other/misc/obituary"});

   for(x = 0; x < sizeof(makeItems); x++) {
      tmp = explode(makeItems[x],"^");
      names += ({ tmp[0] });
      paths += ({ tmp[1] });
   }

   names -= ({ names[0] });
   paths -= ({ paths[0] });

   if(!str || (member(names,str) == -1 && str != "all")) {

      write("You may 'make' the following items:\n");
      for(x = 0; x < sizeof(names); x++) {
         write("    " + pad(names[x],15) + paths[x] + "\n");
      }
      write("    " + pad("all",15) + "all of the above\n");
      return 1;
   }

   if(str == "all") {
      for(x = 0; x < sizeof(names);x++) {
         move_object(clone_object(paths[x]),this_object());
         write("You make '" + names[x] + "'\n");
      }

      return 1;
   }

   x = member(names,str);
   move_object(clone_object(paths[x]),this_object());
   write("You make '" + names[x] + "'\n");
   return 1;

}

shield(string str)
{
   if((string)this_player()->query_real_name() != owner_name) return 0;

   if(!str || (str != "0" && str != "1" && str != "2" && str != "3")) {
      
      write("Shield Levels\n==========================\n"+
         "0\tShield Off\n"+
         "1\tMove to Green\n"+
         "2\tDest\n"+
         "3\tMove to 'yep' room\n\n"+
         "Current shield level: " + shield + "\n");
      return 1;
   }

   if(sscanf(str,"%d",shield));

   write("Shield set to level " + str + ".\n");
   if(str == "0") {
      set_short("a room (open)");
      set_long("\n\t\theh\n\t\tShield OFF\n\n");
   } else {
      set_short("A room (closed)");
      set_long("\n\t\theh\n\t\tShield ON (level " + str + ")\n\n");
   }

   return 1;
}