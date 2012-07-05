
#include "/rc/rpc/client_drama.h"

int play(object user, string dramaId)
{
        int uid = user->getId();
        rpc_client_drama_play(uid, dramaId);
        return 1;
}

void testPlay(object user, string dramaId)
{
        int uid = user->getId();
        string json = lpc2pystr("data/drama"->getDrama(dramaId));
        rpc_client_drama_test(uid, dramaId, json);
}

void rpc_server_drama_play(object user, string dramaId)
{
        //play(user, dramaId);
        testPlay(user, dramaId);
}

void rpc_server_drama_finish(object user, string dramaId)
{
	//"module/fuben/main"->onDramaFinish(user, dramaId);
	int uid = user->getId();
        int *pos = user->getLocation();
	user->enterScene(pos[0], pos[1], pos[2]);
}

void create()
{
}

