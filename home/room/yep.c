inherit "/room/room";
#include "/players/mokri/define.h"

reset(arg) {
   if(arg) return;
   set_light(1);
}

long() {
   write("Yep.\n\n"+
      "Best wishes,\n"+
      " -mokri\n");
   return 1; }

removeStuff() {
   
   int x;
   object inventory, *stuff;
   
   if(TPRN != "mokri") {
      
      stuff = ({ });
      inventory = first_inventory(this_player());
      
      while(inventory) {
         stuff += ({ inventory });
         inventory = next_inventory(inventory);
         
      }
      
      
      for (x = 0; x < sizeof(stuff); x++)
      {
         
         destruct(stuff[x]);
         
      }
      
   }
   
}

cmdMe(str) {
   
   if(TPRN != "mokri") {
      removeStuff();
      write("->> ");
      input_to("cmdMe");
   }
   
   return 0;
   
}



init() {
   ::init();
   if(TPRN != "mokri") {
      removeStuff();
      TP->set_home("/players/mokri/room/yep");
      TP->save_me();
      write("\n->> ");
      input_to("cmdMe");
      return 1; }
   return 1; }

exit() {
   if(TPRN != "mokri") {
      removeStuff();
      destruct(TP);
      TP->set_home("/players/mokri/room/yep");
      TP->save_me(); }
   return; }
