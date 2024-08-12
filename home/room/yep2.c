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
