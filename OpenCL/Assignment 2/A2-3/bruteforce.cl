__constant int8 passcode = (int8)(7,5,8,0,0,5,5,9);

int8 checkPasscode(int8 attempt) {
	if (all(attempt == passcode)) 
		return attempt;
	else
		return (int8)(-1);
}

__kernel void makeCode(__global int8 *correct) {
	__private int8 attempt, checked;
	int gid = get_global_id(0);

	for (int i1 = 0; i1 < 10; i1++)
	{for (int i2 = 0; i2 < 10; i2++)
	{for (int i3 = 0; i3 < 10; i3++)
	{for (int i4 = 0; i4 < 10; i4++)
	{for (int i5 = 0; i5 < 10; i5++)
	{for (int i6 = 0; i6 < 10; i6++)
	{for (int i7 = 0; i7 < 10; i7++)
	{for (int i8 = 0; i8 < 10; i8++)
	{
		attempt = (int8)(i1,i2,i3,i4,i5,i6,i7,i8);
		checked = checkPasscode(attempt);
		if (all(checked != (int8)(-1)))
			correct[gid] = attempt;
	}}}}}}}}
}

