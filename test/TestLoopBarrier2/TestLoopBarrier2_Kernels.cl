
__kernel void TestLoopBarrier2(
   __global float* input,
   __global float* output,
   __local  float* shared)
{
	int i = get_global_id(0);
	int l = get_local_id(0);

	//shared[l] = 0.f;

	//barrier(CLK_LOCAL_MEM_FENCE);

	for (unsigned j=0, e=10; j<e; ++j) {
		//shared[l] += input[l]+1.f;
		shared[l] += 1.f;
		barrier(CLK_LOCAL_MEM_FENCE);
		//output[i] += (l == get_local_size(0)-1) ? 0.f : shared[l+1];
		output[i] += shared[l];
		barrier(CLK_LOCAL_MEM_FENCE);
	}
}
