inherit "obj/user/channel";

main(string str)
{
   string mess;
   
   mess = "/players/mokri/letters/block"->block_string(str);
   mess = "\n"+mess;
   
   command("wiz " + mess,this_player());
   return 1;
}
