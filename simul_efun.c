/*
// This is a mudlib file.
// The functions defined in this file should only be replacements of efuns
// no longer supported. Don't use these functions any longer, use the
// replacement instead.
*/

/* ===========================================================================
 * Modified to work with both drivers (Amylaar and LD) - by using ifdef
 * at the top of this file, the current simul_efun file is used for Amylaar
 * and the code below the else is used on LD.  This should make the switch
 * easer, as we can just remove the top portion of the code when done
 * testing and we are fully migrated. - Rumplemintz
 * ===========================================================================
 */

#ifndef __LDMUD__  /* Not using LD Driver */

inherit "/obj/simul_efun/secure/overrides";

inherit "/obj/simul_efun/resolv_path";
inherit "/obj/simul_efun/loadObject";
#if 0 /* Change this to 1 to put it back in - Rumplemintz */
inherit "/obj/simul_efun/check_file";
#endif
inherit "/obj/simul_efun/exclude_array";
inherit "/obj/simul_efun/emit_channel";
inherit "/obj/simul_efun/destruct";
inherit "/obj/simul_efun/m_efuns";
inherit "/obj/simul_efun/matching_arrays";
inherit "/obj/simul_efun/expand_month";
inherit "/obj/simul_efun/basename";
/*
inherit "/closed/verte/command_efun";
*/
inherit "/obj/simul_efun/valid_player";
inherit "/obj/simul_efun/same_array";
inherit "/obj/simul_efun/format_colour";
inherit "/obj/simul_efun/a_delete";
inherit "/obj/simul_efun/elongate_attrib";
inherit "/obj/simul_efun/ip";
inherit "/obj/simul_efun/wizlist";
inherit "/obj/simul_efun/ascii_glyph";
inherit "/obj/simul_efun/waxfuns";
inherit "/obj/simul_efun/comma_number";
/*
*/

/* Temporary til we switch */
string object_name(object ob) { return file_name(ob); }

#pragma strict_types
#pragma save_types
#pragma verbose_errors

/*
// The old 'slice_array' is no longer needed. Use range argument inside
// a pair of brackets instead.
*/
mixed *slice_array(mixed *arr, int from, int to) {
  return arr[from..to];
}

/*
// filter_objects() has been renamed to filter_array().
*/
mixed *filter_objects(mixed *list, string str, object ob, mixed extra) {
  return filter_array(list, str, ob, extra);
}

/*
// Define functions to return the proper third person pronoun based on
// gender.
*/
string
subjective(object ob) {
  string gender;
  if (!ob) {
    ob = this_player();
    if (!ob)
      return "it";
  }
  gender = (string)ob->query_gender();
  if (gender == "male")
    return "he";
  else if (gender == "female")
    return "she";
  else
    return "it";
}

string
objective(object ob) {
  string gender;

  if (!ob) {
    ob = this_player();
    if (!ob)
      return "it";
  }
  gender = (string)ob->query_gender();
  if (gender == "male")
    return "him";
  else if (gender == "female")
    return "her";
  else
    return "it";
}

string
possessive(object ob) {
  string gender;

  if (!ob) {
    ob = this_player();
    if (!ob)
      return "its";
  }
  gender = (string)ob->query_gender();
  if (gender == "male")
    return "his";
  else if (gender == "female")
    return "her";
  else
    return "its";
}
mixed snoop(mixed snoopee) {
  if(snoopee && this_player()->query_level() < snoopee->query_level()){
    write("Failed.\n");
    return 1;
   }
if(snoopee)
 efun::snoop(snoopee);
else efun::snoop();
  return 1;
}
int tell_object_wblock(object to, string what) {
  object from;
  if(this_player()) from = this_player();
  if(!from) { efun::tell_object(to,what); return 1;}
  if(from->query_level() < 21 && !from->query_npc()) 
  if(to->query_tellblock(from)) { 
    efun::tell_object(from,to->query_name()+" is blocking you.\n");
    return 1;
  }
  efun::tell_object(to,what);
}

/*  mal is a whiner.

     WAH WAH WAH LOOK AT ME IM MAL.. cry :)

     hehe
void wizlist();
*/

void dumb();  /* heh.. */
void break_point(); /* YAY */

string
delete_spaces(string arg)
{
    return (implode(explode(arg, " "), ""));
}

/* Simul_efun: resolve_path()
 * string resolve_path(string arg)
 *
 * See /doc/efun/resolve_path
 *
 * - Earwax 5/20/04
 *
 * Added ~/ support, cleaned up code a wee bit.
 * also changed file_size() check to 
 *  .... file_size()
 * [verte] <5-20-04>
*/

string resolve_path(string arg)
{
  int i, siz;
  string junk, junk2, path, filename, *args;
  object y;

  if (!arg) return 0;
  if (arg == "/") return arg;
    
  y = this_player();

  if (arg == "~")
    return "/players/"+(string)y->query_real_name()+"/";
    
  if (text_contains(arg, "~"))
  {
    if (y && sscanf(arg, "~/%s",junk))
      arg = "/players/" + 
       (string)y->query_real_name() + "/" + junk;
    else if (sscanf(arg, "~%s/%s", junk, junk2) == 2)
      arg = "/players/"+junk+"/"+junk2;
    else if (sscanf(arg, "~%s", junk) == 1)
    {
	    junk2 = "/players/"+(string)y->query_real_name()+"/"+junk;
	    arg = (file_size(junk2)) ? junk2 : "/players/"+junk;
    }
  }

  if (arg[0] != '/')
    arg = (y && (int)y->query_level() > 19) ? 
           (string)y->query_pwd() + arg :
           "/" + arg;

  if (text_contains(arg, ".."))
  {
    string b, c, *t;
    int x;

    t = ({ });
    b = (arg[strlen(arg)-1]=='/') ? "/" : "";
    siz = sizeof(args = explode(arg, "/"));
    
    for (i = 0; i < siz; i++)
    {
	    if (args[i] == ".")
	      continue;
	    else if (args[i] != "..")
	      t += ({ args[i] });
	    else
	    {
		    x = sizeof(t) - 1;
        t = (x) ? t[0..x-1] : ({ });
	    }
    }
    arg = "/"+implode(t, "/")+b;
  }
  i = strlen(arg)-1;
  if (arg[i] != '/' && file_size(arg+"/") == -2)
    arg += "/";
    
  while (text_contains(arg, "//"))
  {
	  if (sscanf(arg, "//%s", arg) == 1)
	    arg = "/"+arg;
	  if (sscanf(arg, "%s//%s", arg, junk) == 2)
	    arg = arg + "/" + junk;
	  if (sscanf(arg, "%s//", arg) == 1)
	    arg += "/";
  }
  
  return arg;
}

status is_vowel(mixed char)
{
    return((
      char=="a"||char=='a'||
      char=="e"||char=='e'||
      char=="i"||char=='i'||
      char=="o"||char=='o'||
      char=="u"||char=='u') ? 1 : 0);
}

string a_or_an(string phrase)
{
  phrase = delete_colour(phrase);
  return (is_vowel(phrase[0])?"an":"a");
}

varargs mixed expand_player(string input, int epType)
{
  object *us, nm;
  mixed  obj, nmr;
  string *heh;
  int    s;
  s = sizeof(us = users());
  heh = ({ });
  while(s--)
    if((nm = us[s]) && (nmr = (string)nm->query_real_name()) &&
       (nmr[0..strlen(input)-1] == input) &&
       (obj = efun::find_player(nmr)))
         heh += ({ nmr });
  switch(s=sizeof(heh))
  {
    case 0: return (!epType ? input : 0);
    case 1: return efun::find_player(heh[0]);
    default:
      write("\
Try again.\n"+s+" \
players with a name beginning with \""+input+"\".\n\
You must specify which player you mean a little more clearly.\n");
      write("Possible targets found: ");
      while(s-- > 1) write(capitalize(heh[s])+", ");
      write(capitalize(heh[s])+".\n");
      return;
  }
  return 0; /* Rumplemintz */
}

/* 11/16/06 Earwax: ghetto'd this to try to hack way around invis 60
 *                  issue that's causing some problems with logging
 */
varargs object find_player(string str, int fpType)
{
  object *obs;
  object obj;
  int i;
  
  if (!str) return 0;
  if(obj = efun::find_player(str)
  ) /*
  || (obj = find_object(str) && living(obj) && (int)obj->is_player()))
*/
    return obj;
  else if (i = sizeof(obs = users()))
  {
    while(i--)
    {
      if ((string)obs[i]->query_real_name() == str)
        return obs[i];
    }
  }
  else if(fpType)
    return expand_player(str, 1);
  return 0;

  /* Original code - Earwax     
  object obj;
  if(obj = efun::find_player(str))
    return obj;
  else if(fpType)
    return expand_player(str, 1);
  */
}

string creator(object ob) { return (objectp(ob) ? efun::creator(ob) : 0); }

void println(string str) { return write((str ? str : "") + "\n"); }

#else /* Using the LD Driver - use code below - Rumplemintz */

#pragma strong_types
#pragma save_types

#define MAX_LOG_SIZE 50000

#define BACKBONE_WIZINFO_SIZE 5

#define LIVING_NAME 3
#define NAME_LIVING 4

#include "/sys/wizlist.h"
#include "/sys/erq.h"
#include "/sys/files.h"

/* New sefuns for LD - Rumplemintz */
private inherit "kernel/support/name";

inherit "kernel/simul/events";
inherit "kernel/simul/shared";
inherit "kernel/simul/fold";
inherit "kernel/simul/array";
inherit "kernel/simul/list";
inherit "kernel/simul/file";
inherit "kernel/simul/lpc";
inherit "kernel/simul/string";
inherit "kernel/simul/actions";
inherit "kernel/simul/bits";
inherit "kernel/simul/mapping";
inherit "kernel/simul/game";
#if 0
inherit "kernel/simul/debug";
#endif
inherit "kernel/simul/message";

#ifdef NEW_LIGHT
inherit "kernel/simul/light";
#endif

#ifdef SIMUL_INPUT_TO
inherit "kernel/simul/input_to";
#endif


/* Compatibility sefuns - Rumplemintz */
#include "/obj/simul_efun/keys.c"
#include "/obj/simul_efun/copy_file.c"
#include "/obj/simul_efun/atoi.c"
#include "/obj/simul_efun/file_name.c"
#include "/obj/simul_efun/file_exists.c"
#include "/obj/simul_efun/member_array.c"
#include "/obj/simul_efun/format.c"
#include "/obj/simul_efun/pad.c"
#include "/obj/simul_efun/index.c"
#include "/obj/simul_efun/caller.c"
#include "/obj/simul_efun/in_editor.c"
#include "/obj/simul_efun/delete_spaces.c"
#include "/obj/simul_efun/grab_file.c"
#include "/obj/simul_efun/pronoun.c"
#include "/obj/simul_efun/root.c"
#include "/obj/simul_efun/save_restore.c"

/* Nirvana sefuns */
#include "/obj/simul_efun/basename.c"
#include "/obj/simul_efun/emit_channel.c"
// #include "/obj/simul_efun/file_size.c"
#include "/obj/simul_efun/m_efuns.c"
#include "/obj/simul_efun/a_delete.c"
#include "/obj/simul_efun/valid_player.c"
#include "/obj/simul_efun/waxfuns.c"
#include "/obj/simul_efun/resolve_path.c"
#include "/obj/simul_efun/comma_number.c"
#include "/obj/simul_efun/resolv_path.c"  /* This is silly to have two... */

//----------------------------------------------------------------------------
void start_simul_efun()

/* Activate the simul-efun object.
 */

{
    mixed *info;

    if ( !(info = get_extra_wizinfo(0)) )
    set_extra_wizinfo(0, info = allocate(BACKBONE_WIZINFO_SIZE));
    if (!(living_name_m = info[LIVING_NAME]))
    living_name_m = info[LIVING_NAME] = m_allocate(0, 1);
    if (!(name_living_m = info[NAME_LIVING]))
    name_living_m = info[NAME_LIVING] = m_allocate(0, 1);
}

//---------------------------------------------------------------------------
void ls (string path)

/* Print the directory listing of <path>, like the unix command.
 */

{
    int max, i, len, tmp;
    status trunc_flag;
    mixed *dir;
    set_this_object(previous_object());
    dir = get_dir (path, GETDIR_NAMES|GETDIR_SIZES);
    if (path != "/")
	path += "/";
    if (!dir) {
        write("No such directory.\n");
        return;
    }
    if (sizeof(dir) > 999)
    {
        dir = dir[0..998];
        trunc_flag = 1;
    }
    for(i = sizeof(dir); i--; ) {
        if(dir[i--] == -2)
            dir[i]+="/";
        len = strlen(dir[i]);
        if (len > max)
            max = len;
    }
    ++max;
    if (max > 79)
        max = 79;
    for (i=0; i < sizeof(dir); i+=2) {
	string name;
            name = dir[i];
	tmp = strlen(name);
	if (len + tmp > 79) {
	    len = 0;
	    write("\n");
	}
	write(name);
        if (len + max > 79) {
            write("\n");
            len = 0;
        } else {
            write("                                                                                "[80-max+tmp..]);
            len += max;
        }
    }
    write("\n");
    if (trunc_flag) write("***TRUNCATED***\n");
}

//---------------------------------------------------------------------------
string create_wizard(string owner, string domain)
{
    mixed result;

    set_this_object(previous_object());
    result =
      (mixed)__MASTER_OBJECT__->master_create_wizard(owner, domain, previous_object());
    if (stringp(result)) return result;
    return 0;
}

//---------------------------------------------------------------------------
void log_file(string file,string str)
{
    string file_name;
    int *st;

    file_name = "/log/" + file;
#ifdef COMPAT_FLAG
    if (sizeof(regexp(({file}), "/")) || file[0] == '.' || strlen(file) > 30 )
    {
        write("Illegal file name to log_file("+file+")\n");
        return;
    }
#endif
    if ( sizeof(st = get_dir(file_name,2) ) && st[0] > MAX_LOG_SIZE) {
	catch(rename(file_name, file_name + ".old")); /* No panic if failure */
    }
    set_this_object(previous_object());
    write_file(file_name, str);
}

//---------------------------------------------------------------------------
void localcmd()
{
    string *verbs;
    int i,j;

    verbs = query_actions(this_player());
    for (i=0, j = sizeof(verbs); --j >= 0; i++) {
	write(verbs[i]+" ");
    }
    write("\n");
}

//---------------------------------------------------------------------------
mixed *unique_array(mixed *arr,string func,mixed skipnum)
{
    mixed *al, last;
    mapping m;
    int i, j, k, *ordinals;

    if (sizeof(arr) < 32)
        return efun::unique_array(arr, func, skipnum);
    for (ordinals = allocate(i = sizeof(arr)); i--; )
	    ordinals[i] = i;
    m = mkmapping(map_objects(arr, func), ordinals, arr);
    al = m_indices(m);
    ordinals = m_values(m, 0);
    arr = m_values(m, 1);
    if (k = i = sizeof(al)) {
        for (last = al[j = --i]; i--; ) {
            if (al[i] != last) {
                if (last != skipnum) {
                    arr[--k] = arr[i+1..j];
                    ordinals[k] = ordinals[j];
                }
                last = al[j = i];
            }
        }
        if (last != skipnum) {
            arr[--k] = arr[0..j];
            ordinals[k] = ordinals[j];
        }
    }
    return m_values(mkmapping(ordinals[k..], arr[k..]),0);
}

//---------------------------------------------------------------------------
varargs mixed snoop(mixed snoopee)
{
    int result;

    if (snoopee && query_snoop(snoopee)) {
        write("Busy.\n");
        return 0;
    }
    result = snoopee ? efun::snoop(this_player(), snoopee)
                     : efun::snoop(this_player());
    switch (result) {
	case -1:
	    write("Busy.\n");
	    break;
	case  0:
	    write("Failed.\n");
	    break;
	case  1:
	    write("Ok.\n");
	    break;
    }
    if (result > 0) return snoopee;
}

/* creator_file - Rumplemintz */
mixed creator_file(string object_name) {
  string *path;

  if (strstr(object_name,"//") >= 0 || member(object_name,'#') >= 0)
    return 0;

  path = explode(object_name,"/") - ({"","."});
  switch(sizeof(path) && path[0]) {
  case "open":
  case "ftp":
  case "log":
  case "newlib":
  case "oldlib":
    return 0;
  case "players":
    if (sizeof(path) < 3)
      return 0;
    return path[1];
  case "domains":
    if (sizeof(path) < 3)
      return 0;
    return capitalize(path[1]);
  default:
    return 1;
  }
  return 1;
}

string query_version() {
    return __VERSION__;
}

//---------------------------------------------------------------------------
string query_host_name() {
    return __HOST_NAME__;
}

//---------------------------------------------------------------------------
nomask void set_environment() {}

nomask void set_this_player() {}

//---------------------------------------------------------------------------
varargs void add_worth(int value, object ob)
{
    mixed old;
#ifdef __COMPAT_MODE__
    switch (explode(object_name(previous_object()), "/")[0]) {
#else
    switch (explode(object_name(previous_object()), "/")[1]) {
#endif
      default:
	raise_error("Illegal call of add_worth.\n");
      case "obj":
      case "std":
      case "room":
    }
    if (!ob) {
	if ( !(ob = previous_object(1)) )
	    return;
    }
    if (intp(old = get_extra_wizinfo(ob)))
        set_extra_wizinfo(ob, old + value);
}

//---------------------------------------------------------------------------
varargs void wizlist(string name)
{
    int i, pos, total_cmd;
    int *cmds;
    mixed *a;
    mixed *b;

    if (!name) {
        name = (string)this_player()->query_real_name();
        if (!name)
        {
            write("Need to provide a name or 'ALL' to the wizlist function.\n");
            return;
        }
    }
    a = transpose_array(wizlist_info());
    cmds = a[WL_COMMANDS];
    a[WL_COMMANDS] = a[0];
    a[0] = cmds;

    a = unmkmapping(apply(#'mkmapping, a));
    cmds = a[0];
    a[0] = a[WL_COMMANDS];
    a[WL_COMMANDS] = cmds;

    if ((pos = member(a[WL_NAME], name)) < 0 && name != "ALL")
    {
        write("No wizlist info for '"+name+"' found.\n");
        return;
    }
    b = allocate(sizeof(cmds));
    for (i = sizeof(cmds); i;) {
        b[<i] = i;
        total_cmd += cmds[--i];
    }
    a = transpose_array(a + ({b}) );
    if (name != "ALL") {
        if (pos + 18 < sizeof(cmds)) {
            a = a[pos-2..pos+2]+a[<15..];
        } else if (pos < sizeof(cmds) - 13) {
            a = a[pos-2..];
        } else {
            a = a[<15..];
        }
    }
    write("\nWizard top score list\n\n");
    if (total_cmd == 0)
        total_cmd = 1;
    for (i = sizeof(a); i; ) {
        b = a[<i--];
        if (b[WL_GIGACOST] > 1000)
            printf("%-15s %5d %2d%% (%d)\t[%d%4dk,%5d] %6d %d\n",
              b[WL_NAME], b[WL_COMMANDS],
              b[WL_COMMANDS] * 100 / total_cmd, b[<1],
              b[WL_GIGACOST] / 1000,
              b[WL_COST] / 1000 + (b[WL_GIGACOST] % 1000) * 1000000000,
              b[WL_HEART_BEATS], b[WL_EXTRA], b[WL_ARRAY_TOTAL]
            );
        else
            printf("%-15s %5d %2d%% (%d)\t[%4dk,%5d] %6d %d\n",
              b[WL_NAME], b[WL_COMMANDS],
              b[WL_COMMANDS] * 100 / total_cmd, b[<1],
              b[WL_COST] / 1000 + (b[WL_GIGACOST] % 1000) * 1000000000,
              b[WL_HEART_BEATS], b[WL_EXTRA], b[WL_ARRAY_TOTAL]
            );
    }
    printf("\nTotal         %7d     (%d)\n\n", total_cmd, sizeof(cmds));
}

#if 0
//---------------------------------------------------------------------------
void set_living_name(string name)
{
    string old;
    mixed a;
    int i;

    if (old = living_name_m[previous_object()]) {
	if (pointerp(a = name_living_m[old])) {
	    a[member(a, previous_object())] = 0;
	} else {
	    efun::m_delete(name_living_m, old);
	}
    }
    living_name_m[previous_object()] = name;
    if (a = name_living_m[name]) {
	if (!pointerp(a)) {
	    name_living_m[name] = ({a, previous_object()});
	    return;
	}
	/* Try to reallocate entry from destructed object */
	if ((i = member(a, 0)) >= 0) {
	    a[i] = previous_object();
	    return;
	}
	name_living_m[name] = a + ({previous_object()});
	return;
    }
    name_living_m[name] = previous_object();
}

//---------------------------------------------------------------------------
object find_living(string name)
{
    mixed *a, r;
    int i;

    if (pointerp(r = name_living_m[name])) {
	if ( !living(r = (a = r)[0])) {
	    for (i = sizeof(a); --i;) {
		if (living(a[<i])) {
		    r = a[<i];
		    a[<i] = a[0];
		    return a[0] = r;
		}
	    }
	}
	return r;
    }
    return living(r) && r;
}

//---------------------------------------------------------------------------
object find_player(string name)
{
    mixed *a, r;
    int i;

    if (pointerp(r = name_living_m[name])) {
	if ( !(r = (a = r)[0]) || !query_once_interactive(r)) {
	    for (i = sizeof(a); --i;) {
		if (a[<i] && query_once_interactive(a[<i])) {
		    r = a[<i];
		    a[<i] = a[0];
		    return a[0] = r;
		}
	    }
	    return 0;
	}
	return r;
    }
    return r && query_once_interactive(r) && r;
}
#endif

//---------------------------------------------------------------------------
status is_vowel(mixed char)
{
    return((
      char=="a"||char=='a'||
      char=="e"||char=='e'||
      char=="i"||char=='i'||
      char=="o"||char=='o'||
      char=="u"||char=='u') ? 1 : 0);
}

//---------------------------------------------------------------------------
string a_or_an(string phrase)
{
  phrase = delete_colour(phrase);
  return (is_vowel(phrase[0])?"an":"a");
}

/*===========================================================================
 * The following functions provide the necessary compatibility of this
 * compat-mode mudlib with a plain driver.
 * Just the parse_command() efun is not simulated.
 */

#ifndef __COMPAT_MODE__
//---------------------------------------------------------------------------
string function_exists (string str, object ob)
{
    string rc;

    rc = efun::function_exists(str, ob);
    return stringp(rc) ? rc[1..] : 0;
}

//---------------------------------------------------------------------------
string object_name(object ob)
{
    string rc;

    rc = efun::object_name(ob);
    return stringp(rc) ? rc[1..] : 0;
}

//---------------------------------------------------------------------------
string program_name(object ob)
{
    string rc;

    rc = efun::program_name(ob);
    return stringp(rc) ? rc[1..] : 0;
}

//---------------------------------------------------------------------------
string* inherit_list(object ob)
{
    string *rc;
    int i;

    rc = efun::inherit_list(ob);
    for (i = sizeof(rc); i-- > 0; )
        rc[i] = rc[i][1..];
    return rc;
}

//---------------------------------------------------------------------------
string to_string(mixed arg)
{
    string rc;

    rc = efun::to_string(arg);
    return objectp(arg) ? rc[1..] : rc;
}

//---------------------------------------------------------------------------
string creator(object ob)
{
    return getuid(ob);
}

//---------------------------------------------------------------------------
varargs void add_action(string fun, string cmd, int flag)
{
    if (fun == "exit")
        raise_error("Illegal to define a command to the exit() function.\n");

    efun::set_this_object(previous_object());
    if (cmd)
        efun::add_action(fun, cmd, flag);
}

//---------------------------------------------------------------------------
#endif /* __COMPAT_MODE__ */

#if !__EFUN_DEFINED__(present)
//---------------------------------------------------------------------------
varargs object present(mixed ob, object env)
{
    int specific, num, i;
    object found;
    string str;

    if (!env)
    {
        env = previous_object();
        specific = 0;
    }
    else
        specific = 1;

    if (objectp(ob))
    {
        /* Quick check: is ob there or not? */

        if (specific)
            return environment(ob) == env ? ob : 0;
        if (environment(ob) == env
         || (environment(env) && environment(ob) == environment(env))
           )
            return ob;
        return 0;
    }

    /* Search by name. Prepare the search parameters */
    if (2 != sscanf(ob, "%s %d", str, num))
    {
        num = 1;
        str = ob;
    }

    /* First, search in env by name */
    for (found = first_inventory(env), i = 0
        ; found
        ; found = next_inventory(env))
    {
        if (found->id(str) && ++i == num)
            break;
        if (!found)  /* may happen */
            break;
    }

    if (found || specific)
        return found;

    /* If not found, search in environment(env) by name */
    env = environment(env);
    if (!env)
        return 0;
    if (env->id(ob))
        return env;
    if (!env) /* the id() may have destructed env */
        return 0;

    for (found = first_inventory(env), i = 0
        ; found
        ; found = next_inventory(env))
    {
        if (found->id(str) && ++i == num)
            break;
        if (!found)  /* may happen */
            break;
    }

    return found;
}

#endif /* !efun_defined(present) */

#if !__EFUN_DEFINED__(transfer)
//---------------------------------------------------------------------------
/*
 * Transfer an object from an object to an object.
 * Call add_weight(), drop(), get(), prevent_insert(), add_weight(),
 * and can_put_and_get() where needed.
 * Return 0 on success, and special code on failure:
 *
 * 1: To heavy for destination.
 * 2: Can't be dropped.
 * 3: Can't take it out of it's container.
 * 4: The object can't be inserted into bags etc.
 * 5: The destination doesn't allow insertions of objects.
 * 6: The object can't be picked up.
 */
int transfer(object item, object dest)
{
    int weight;
    object from;

    efun::set_this_object(previous_object());

    weight = item->query_weight();
    if (!item)
        return 3;

    from = environment(item);
    if (from)
    {
        /*
         * If the original place of the object is a living object,
         * then we must call drop() to check that the object can be dropped.
         */
        if (living(from))
        {
            if (item->drop() || !item)
                return 2;
        }
        /*
         * If 'from' is not a room and not a player, check that we may
         * remove things out of it.
         */
        else if (environment(from))
        {
            if (!from->can_put_and_get() || !from)
                return 3;
        }
    }

    /*
     * If the destination is not a room, and not a player,
     * Then we must test 'prevent_insert', and 'can_put_and_get'.
     */
    if (environment(dest) && !living(dest))
    {
        if (item->prevent_insert())
            return 4;
        if (!dest->can_put_and_get() || !dest)
            return 5;
    }

    if (living(dest))
    {
        if (!item->get() || !item)
            return 6;
    }

    /*
     * If it is not a room, correct the total weight in the destination.
     */
    if (environment(dest) && weight)
    {
        if (!dest->add_weight(weight) || !dest)
            return 1;
    }

    /*
     * If it is not a room, correct the weight in the 'from' object.
     */
    if (from && environment(from) && weight)
    {
        from->add_weight(-weight);
    }

    move_object(item, dest);

    return 0;
}

#endif /* !efun_define(transfer) */

#if !__EFUN_DEFINED__(extract)
//---------------------------------------------------------------------------
mixed extract (mixed data, varargs mixed*from_to)

{
    int from, to;

    if (!stringp(data) && !pointerp(data))
    {
        raise_error("Illegal type for extract(): must be string or array.\n");
        return 0;
    }

    switch(sizeof(from_to))
    {
    case 0: return data;
    case 1:
        if (!intp(from_to[0]))
        {
            raise_error("Illegal 'from' index for extract(): must be a number.\n");
            return 0;
        }
        from = from_to[0];
        if (from >= 0)
            return data[from..];
        return data[<-from..];
    case 2:
        if (!intp(from_to[0]) || !intp(from_to[1]))
        {
            raise_error("Illegal index for extract(): must be a number.\n");
            return 0;
        }
        from = from_to[0];
        to = from_to[1];
        if (from >= 0)
        {
            if (to >= 0)
                return data[from..to];
            return data[<from..<-to];
        }
        if (to >= 0)
            return data[<-from..to];
        return data[<from..<-to];
    }

    raise_error("Illegal number of arguments for extract().\n");
    return 0;
}

#endif /* !efun_defined(extract) */

/*************************************************************************/

#endif /* LDMud check */
