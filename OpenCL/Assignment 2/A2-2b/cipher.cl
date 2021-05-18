__kernel void encrypt(__global char16 *i,
					 __global char16 *o,
					 __global int *n) {
	__local int cn;
	__private char16 encrypted, lctouc, ucShift;

	int id = get_global_id(0);

	cn = n[get_global_id(1)];
	// select false (1), true (2), where (3)
	if (cn > 0)
	{	
		// Make Upper Case
		lctouc = select(i[id], (i[id]-(char)(32)), (i[id] <= (char)(122) && i[id] >= (char)(97)));
		// shift upper case
		ucShift = select(lctouc, (lctouc+(char)(cn)), (lctouc >= (char)(65) && lctouc <= (char)(90-cn)));
		// Cycle upper case
		encrypted = select(ucShift, (ucShift-(char)(26)+(char)(cn)), (lctouc <= (char)(90) && lctouc >= (char)(91-cn)));
	}
	else if (cn < 0)
	{
		// Make Upper Case
		lctouc = select(i[id], (i[id]-(char)(32)), (i[id] <= (char)(122) && i[id] >= (char)(97)));
		// shift upper case
		ucShift = select(lctouc, (lctouc+(char)(cn)), (lctouc <= (char)(90) && lctouc >= (char)(65-cn)));
		// Cycle upper case
		encrypted = select(ucShift, (ucShift+(char)(26)+(char)(cn)), (lctouc >= (char)(65) && lctouc <= (char)(64-cn)));
	}
	else
	{
		// Make Upper Case
		lctouc = select(i[id], (i[id]-(char)(32)), (i[id] <= (char)(122) && i[id] >= (char)(97)));
		encrypted = lctouc;
	}
	o[id] = encrypted;
}

__kernel void decrypt(__global char16 *i,
					 __global char16 *o,
					 __global int *n) {
	__local int cn;
	__private char16 decrypted, ucShift;

	int id = get_global_id(0);

	cn = n[get_global_id(1)];
	// select false (1), true (2), where (3)
	if (cn > 0) //move it back
	{	
		// shift upper case
		ucShift = select(i[id], (i[id]-(char)(cn)), (i[id] >= (char)(65+cn) && i[id] <= (char)(90)));
		// Cycle upper case
		decrypted = select(ucShift, (ucShift+(char)(26)-(char)(cn)), (i[id] >= (char)(65) && i[id] <= (char)(64+cn)));
	}
	else if (cn < 0) // move it forward
	{
		// shift upper case
		ucShift = select(i[id], (i[id]-(char)(cn)), (i[id] >= (char)(65) && i[id] <= (char)(90+cn)));
		// Cycle upper case
		decrypted = select(ucShift, (ucShift-(char)(26)-(char)(cn)), ( i[id] <= (char)(90) && i[id] >= (char)(91+cn)));
	}
	else
	{
		decrypted = i[id];
	}
	o[id] = decrypted;
}
