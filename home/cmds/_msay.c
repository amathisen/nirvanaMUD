inherit "obj/user/channel";

main(string str)
{
string mess;

mess = this_player()->query_real_name()+" says " + str;
mess = "/players/mokri/letters/block"->block_string(mess);

say(mess+"\n");
write("You say: " + str +"\n");
return 1;
}
