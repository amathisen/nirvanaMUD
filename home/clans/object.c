inherit "/obj/treasure";

#include "defs.h"
#include "cvars.h"
#include "cquery.h"
#include "cset.h"
#define CMD_DIR TOP_DIR + "cmds/"

reset(int arg) {
   if(arg) return;
   set_id("clan_object");
   set_short("a clan_object");
   set_long("a generic clan object.\n");
   set_weight(0);
}

int restore_me()
{
  if(!TP || !CNAME) return 0;
  return restore_object(SAVE_DIR + CNAME + "/object");
}
 
void save_me()
{
  if(file_size(TOP_DIR + CNAME) != -2) return;
  save_object(SAVE_DIR + CNAME + "/object");
}

void update_everyone()
{
  object t_obj, *us;
  int x;
  
  us = users();
  for(x = 0; x < sizeof(us); x++)
  {
    t_obj = present("clan_object",us[x]);
    if(t_obj && us[x]->query_clan_name() == CNAME)
      t_obj->restore_me();
  }
  return;
}

int is_default(string str)
{
  string *default_cmds;
  
  default_cmds = ({ });
  default_cmds += ({ "clan_help" });
  default_cmds += ({ "enter_clan_hall" });
  default_cmds += ({ "recruit_type" });
  default_cmds += ({ "set_clan_hall" });
  default_cmds += ({ "clan_recruit" });
  default_cmds += ({ "clan_leave" });
  default_cmds += ({ "clan_short" });
  default_cmds += ({ "clanobj_name" });
  default_cmds += ({ "clan_long" });
  default_cmds += ({ "c_who" });
  default_cmds += ({ "c_chat" });
  default_cmds += ({ "private_coffer" });
  default_cmds += ({ "leadership" });
  default_cmds += ({ "un_clan_recruit" });
  default_cmds += ({ "clan_spell" });
  default_cmds += ({ "set_clan_spell" });
  default_cmds += ({ "clan_stats" });
  default_cmds += ({ "clan_board" });
  default_cmds += ({ "wall" });
  if(member(default_cmds,str) == -1)
    return 0;
  return 1;
}

init()
{
  string *clan_cmds;
  int x;
  /* 07/09/07 Earwax: added following 2 lines to fix a bug */
  if (this_player() && this_player() != environment(this_object()))
    return;
  if(root()) return; /* attempt to stop the autoload problem */
  ::init();
  if(restore_me())
  {
    int siz;
    if(!leaders) leaders = ({ });
    if(!abilities) abilities = ({ });
    set_short(clan_short);
    set_long(clan_long);
    set_alias(object_name);
    clan_cmds = get_dir(CMD_DIR);
    /* 07/09/07: Earwax: added ,siz = sizeof...so array size doesn't
                 get calculated every loop iteration */
    for (x = 0, siz = sizeof(clan_cmds); x < siz; x++)
    {
      if(extract(clan_cmds[x],0,0) == "_" && (member(abilities,extract(clan_cmds[x],1,strlen(clan_cmds[x]) - 3)) != -1 || is_default(extract(clan_cmds[x],1,strlen(clan_cmds[x]) - 3))))
        add_action("clan_command",extract(clan_cmds[x],1,strlen(clan_cmds[x]) - 3));
    }
    if(spell_id == 1 && spell_alias) add_action("dam_spell",spell_alias);
    if(spell_id == 2 && spell_alias) add_action("heal_spell",spell_alias);
    if(public_coffer > 0) { /* verte, boltar removed public coffers */
       private_coffer += public_coffer;
       public_coffer = 0;
       save_me();
     }
  }
  else
  {
    write("Error loading clan or clan has disbanded.\nUnsetting clan status and removing object.\n");
    TP->set_clan_name(0);
    destruct(this_object());
  } 
}

dam_spell(string str) { return call_other(CMD_DIR + "_generic_damage","main",str); }
heal_spell(string str) { return call_other(CMD_DIR + "_generic_heal","main",str); }
clan_command(string str) { return call_other(CMD_DIR + "_" + query_verb(),"main",str); }

drop() { return 1; }
