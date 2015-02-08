#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 20

#include "qsort.h"
/*
int main() {
	int** tab,i,j;
	struct timeval tv;
	tab = (int**)malloc(sizeof(int*)*SIZE);
	for(i=0;i<SIZE;i++)
		tab[i] = (int*)malloc(sizeof(int)*SIZE);

	for(i=0;i<SIZE;i++) {
		for(j=0;j<SIZE;j++) {
			gettimeofday(&tv,NULL);
			srand(tv.tv_usec);
			tab[i][j] = (float)rand()/RAND_MAX*40;
			printf("%3d ", tab[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");

	quicksortmatrix(tab,SIZE,SIZE);
	
	for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++) 
			printf("%3d ", tab[i][j]);
		printf("\n");
	}
	printf("\n");
}
*/
void quicksortmatrix(int t[2*MX_SIZE][2*MX_SIZE], int m, int n, int xstride, int ystride) {
	int x,i,k;
	x =(int)ceil(log(m*n)/log(2));
	for(k=0;k<x;k++) {
		for(i=0;i<n;i++)
			quicksortrow(t,0,m,i,xstride,ystride);
		for(i=0;i<m;i++) 
			quicksortcol(t,0,n,i,xstride,ystride);
	}
	for(i=0;i<n;i++)
		quicksortrow(t,0,m,i,xstride,ystride);
}

void quicksortrow(int t[2*MX_SIZE][2*MX_SIZE], int l, int len, int row,int xstride, int ystride) {
	int i,j,x,y;

	i = l;
	j = len;
	x = t[row+xstride][(l+len)/2+ystride];

	do {
		while(t[row+xstride][i+ystride]>x)
			++i;

		while(x>t[row+xstride][j+ystride])
			j--;

		if(i<=j) {
			y = t[row+xstride][i+ystride];
			t[row+xstride][i+ystride] = t[row+xstride][j+ystride];
			t[row+xstride][j+ystride] = y;
			i++;
			j--;
		}
	} while (i<j);

	if (l<j) quicksortrow(t,l,j,row,xstride,ystride);
	if (i<len) quicksortrow(t,i,len,row,xstride,ystride);
}

void quicksortcol(int t[2*MX_SIZE][2*MX_SIZE], int l, int len, int col,int xstride, int ystride) {
	int i,j,x,y;

	i = l;
	j = len;
	x = t[(l+len)/2+xstride][col+ystride];

	do {
		while(t[i+xstride][col+ystride]>x)
			i++;

		while(x>t[j+xstride][col+ystride])
			j--;

		if(i<=j) {
			y = t[i+xstride][col+ystride];
			t[i+xstride][col+ystride] = t[j+xstride][col+ystride];
			t[j+xstride][col+ystride] = y;
			i++;
			j--;
		}
	} while (i<j);

	if (l<j) quicksortcol(t,l,j,col,xstride,ystride);
	if (i<len) quicksortcol(t,i,len,col,xstride,ystride);
}
