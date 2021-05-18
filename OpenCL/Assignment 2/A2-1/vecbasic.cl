__kernel void vecbasic(__global int4 *a,
					 __global const int *b,
					 __global int *o) {
	__local int8 v, v1, v2;
	__private int8 results;

	size_t g_id_0 = get_global_id(0);
	size_t g_id_1 = get_global_id(1);

	v = (int8)(a[g_id_0], a[g_id_0 + 1]);
	v1 = vload8(0, &b[g_id_0]);
	v2 = vload8(1, &b[g_id_0]);

	if (any(v>5))
		results = select(v2, v1, v>5);
	else
		results = (int8)(v1.lo, v2.lo);

	vstore8(v, 0, &o[g_id_1]);
	vstore8(v1, 1, &o[g_id_1]);
	vstore8(v2, 2, &o[g_id_1]);
	vstore8(results, 3, &o[g_id_1]);
}
