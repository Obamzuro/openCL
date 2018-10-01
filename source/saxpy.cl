__kernel void SAXPY(__global float *x, __global float *y,
		float a)
{
	int		iGid;

	iGid = get_global_id(0);
	y[iGid] += a * x[iGid];
}
