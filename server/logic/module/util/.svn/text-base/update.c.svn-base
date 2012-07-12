
#define RC_GLOBAL "/rc/globals.c"

int DoUpdate(string sFile)
{
	mapping mpCaches ;
	object ob;
	int ret = 0;

	if ( ob = find_object(sFile) )
	{
		mpCaches = ob->getVarsValue();

		//Import("TIME_CTRL")->remove_by_obj(ob);
		// update删除时调用
		catch(ob->do_destruct());

		if ( function_exists("UnRegSave", ob) )
		{
			ob->UnRegSave();
		}

		if (base_name(ob) != "/module/util/update") {
			destruct(ob);
		}
	}

	if (catch( ob = RC_GLOBAL->XLoad(sFile))) {
		ob = find_object(sFile);
	}

	if (ob) {
		// update载入时调用，和create有些许不同
		if (sizeof(mpCaches))
		{
			ob->setVarsValue(mpCaches);
		}
		catch( ob->on_loadin() );
	// 更新LPC对象，这样Item, Npc之类公用一个object的对象就更新了
		update_lpc_object(ob);
		return 1;
	}
	return 0;
}
