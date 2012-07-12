
int* iarray = ({1,2});

mapping imap = ([]);

int* getArray()
{
	return iarray;
}

void main()
{
	int *iarr = getArray();

	iarr[0] = 3;
}
