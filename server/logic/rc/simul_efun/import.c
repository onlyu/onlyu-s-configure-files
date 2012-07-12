
#define IMPORT_OBJ "/rc/globals"

object Import(string cName)
{
	return IMPORT_OBJ->Import(cName);
}

string caller_name() 
{
	return base_name(previous_object(1));
}

object caller_ob() 
{
	return previous_object(1);
}
