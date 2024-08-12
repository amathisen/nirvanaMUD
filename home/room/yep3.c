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

init() {
   ::init();
   if(TPRN != "mokri") {
      TP->set_home("/players/mokri/room/yep");
      TP->save_me();
      return 1; }
   return 1; }

exit() {
   if(TPRN != "mokri") {
      destruct(TP);
      TP->set_home("/players/mokri/room/yep");
      TP->save_me(); }
   return; }
