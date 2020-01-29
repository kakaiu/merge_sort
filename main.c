int binary_search(int* A, int left, int right, int value) {
	int mid = left + (right-left)/2;
	if (left>right) {
		return left;
	} 
	if (value==A[mid]) {
		return mid;
	} else if (value<A[mid]) {
		return binary_search(A, left, mid-1, value); 
	} else {
		return binary_search(A, mid+1, right, value); 
	}
} 

void parallel_merge(int *C, int *A, int l_a, int *B, int l_b) { 
	if (l_a < l_b) {
		parallel_merge(C, B, l_b, A, l_a); 
	} else if (l_a==1) {
		for (int i=0; i<l_a; i++) {
			C[i] = A[i];
		}
		for (int i=0; i<l_b; i++) {
			C[i+l_a] = A[i];
		}
	} else {
		int mid_a = l_a/2;
		int mid_b = 1+binary_search(B, 0, l_b-1, A[mid_a]); //it is length, not idx
		C[mid_a+mid_b-1] = A[mid_a];//
		cilk_spawn parallel_merge(C, A, mid_a, B, mid_b); 
		parallel_merge(C+mid_a+mid_b, A+mid_a, l_a-mid_a, B+mid_b, l_b-mid_b); 
		cilk_sync;
	} 
}

int* merge_sort(int* array, int l_array){
	int mid = l_array/2;
	int l_b = mid;
	int* B = (int*)malloc(l_b * sizeof(int));
	int l_c = l_array-mid;
	int* C = (int*)malloc(l_c * sizeof(int));

	cilk_spawn merge_sort(B, l_b);
	cilk_spawn merge_sort(C, l_c);
	cilk_sync;

	parallel_merge(array, B, l_b, C, l_c);
	free(B);
	free(C);
}