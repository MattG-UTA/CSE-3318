// Matthew Gutierrez
// 1002333697
// Lab 3
//Compilation command: gcc lab3.c -o lab3
//Run command ./lab3
#include <stdio.h>
#include <stdlib.h>
//Struct for navigating the state of the subsequences.
typedef struct 
{
    char r, c;
    int a, b;
} State;

//Function for making the subsequences.
static int idx(int i, int a, int b, int t) 
{
    return i * (t+1) * (t+1) + a * (t+1) + b;
}

//Main
int main(int argc, char* argv[]) {
    
//Initializes variables for intial size of array, number of elements, and total.
int cap = 8, n = 0, i, total = 0;
int *raw = malloc(cap * sizeof(int)), *v, *g;
State *st;

if (!raw) return printf("Memory allocation failed.\n"), 1;
    
    //While loop to read in N number of elements into the array.
while (scanf("%d", &i) == 1) 
	{
	        if (n == cap) 
	        {
	            cap *= 2;
	            int *tmp = realloc(raw, cap * sizeof(int));
	            if (!tmp) return printf("Memory allocation failed.\n"), free(raw), 1;
	            raw = tmp;
        }
        raw[n++] = i;
    	}
if (!n) return printf("No input values.\n"), free(raw), 1;
    
//Makes arrays for holding subsequences 
int m = (n == raw[0] + 1) ? raw[0] : n;
v = malloc(m * sizeof(int));
g = malloc(m * sizeof(int));
if (!v || !g) return printf("Memory allocation failed.\n"), free(raw), free(v), free(g), 1;
//Checks for positve
for (i = 0; i < m; i++) 
	{
		v[i] = (n == raw[0] + 1) ? raw[i+1] : raw[i];
        	if (v[i] <= 0) return printf("Error: values must be positive.\n"), free(raw), free(v), free(g), 1;
        	total += v[i];
    	}
    	free(raw);

if (total % 3) return printf("Error: total %d not divisible by 3.\n", total), free(v), free(g), 1;
//Stores subsequence values.
int t = total / 3, W = t + 1, S = (m+1) * W * W;
st = calloc(S, sizeof(State));
if (!st) return printf("Memory allocation failed.\n"), free(v), free(g), 1;

st[idx(0,0,0,t)].r = 1;
//For loop for computing the complete subequence answer.
int pref = 0;
for (i = 0; i < m; i++) 
	{
	        int x = v[i];
	        pref += x;
        for (int a = 0; a <= t; a++)
        	for (int b = 0; b <= t; b++) 
        	    {
        	        int k = idx(i,a,b,t);
        	        if (!st[k].r) continue;
	
        	        //group 2
        	        if (pref - a - b <= t) 
        	        {
        	            int k2 = idx(i+1,a,b,t);
        	            if (!st[k2].r) st[k2] = (State){1,2,a,b};
        	        }
        	        //group 0
        	        if (a + x <= t) 
        	        {
        	            int k2 = idx(i+1,a+x,b,t);
        	            if (!st[k2].r) st[k2] = (State){1,0,a,b};
        	        }
        	        //group 1
        	        if (b + x <= t) 
        	        {
        	            int k2 = idx(i+1,a,b+x,t);
        	            if (!st[k2].r) st[k2] = (State){1,1,a,b};
        	        }
        	    }
    }
//Error code for if the answer is not divisible by 3
if (!st[idx(m,t,t,t)].r) 
	{
	        printf("Error: no equal-sum 3-way partition exists.\n");
	        free(v); 
	        free(g); 
	        free(st);
	        return 1;
	    }
//Constructs the completed subsequences.
int a = t, b = t;
for (i = m; i > 0; i--) 
	{
	        State s = st[idx(i,a,b,t)];
	        g[i-1] = s.c;
	        a = s.a;
	        b = s.b;
        }
//For printing the three subsequences.
printf("  i    0    1    2\n");
for (i = 0; i < m; i++) 
	{
        	printf("%3d", i+1);
        	for (int k = 0; k < 3; k++)
        {
        if(g[i] == k)
        printf("%5d",v[i]);
        else
        printf("%5s", "");
        }
        printf("\n");
    	}

    free(v); 
    free(g); 
    free(st);
    return 0;
}
