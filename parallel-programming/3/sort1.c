#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpi.h"
#include "qsort.h"

#define NUM_SIZE 100
#define MAX_SIZE 64

int myval[2*MX_SIZE][2*MX_SIZE];
int matrix[MX_SIZE*MAX_SIZE][MX_SIZE*MAX_SIZE];

MPI_Comm MPI_COMM_NEW;
MPI_Datatype mini_matrix;

// my coordinates
int coords[2];
		
// n size of matrix
int n;

void row_like(int,int,int);
void col_like(int,int,int);

void send_recv(int);

int recv_send_row(int);
int recv_send_col(int);

int verify(int tab[MX_SIZE*MAX_SIZE][MX_SIZE*MAX_SIZE]) {
	int i,j,tmp;
	for(i=0;i<MX_SIZE*n;i++) {
		tmp = tab[i][0];
		for(j=0;j<MX_SIZE*n;j++) {
			if(tmp < tab[i][j]) {
				printf("%d %d\n",i,j);
				return 0;
			}
			tmp = tab[i][j];
		}
	}

	for(i=0;i<MX_SIZE*n;i++) {
		tmp = tab[0][i];
		for(j=0;j<MX_SIZE*n;j++) {
			if(tmp < tab[j][i]) {
				printf("%d %d\n",i,j);
				return 0;
			}
			tmp = tab[j][i];
		}
	}
	return 1;
}

int main(int argc, char** argv) {
	int size, rank, i,j, buf;

	int dims[2];
	int periods[2] = {0, 0};

	struct timeval tv, tv1, tv2;

	int dest_col, source_col, dest_row, source_row;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Datatype max_matrix;
	
	if ((sqrt(size)-(int)sqrt(size))!=0.0) 
		exit(1);
	
	/* n*n datatype construction */
	MPI_Type_vector(MX_SIZE, MX_SIZE, 2*MX_SIZE, MPI_INT, &mini_matrix);
	MPI_Type_commit(&mini_matrix);

	for(i=0;i<MX_SIZE*2;i++) {
		for(j=0;j<MX_SIZE*2;j++) 
			myval[i][j] = 0;
	}
	
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) {
			gettimeofday(&tv, NULL);
			srand(tv.tv_usec);
			myval[i+MX_SIZE][j+MX_SIZE] = ((float)rand()/RAND_MAX)*NUM_SIZE;
		}
	}	
	
	n = sqrt(size);
	dims[0] = dims[1] = n;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &MPI_COMM_NEW);
	MPI_Comm_rank(MPI_COMM_NEW, &rank);
	
	MPI_Cart_coords(MPI_COMM_NEW, rank, 2, coords);
	
	MPI_Cart_shift(MPI_COMM_NEW, 1, 1, &source_row, &dest_row);
	MPI_Cart_shift(MPI_COMM_NEW, 0, 1, &source_col, &dest_col);
	
	// printf("Rank: %d Size: %d Coordinates: %d %d Source Row: %d Dest Row: %d Source Col: %d Dest Col: %d\n", rank, size, coords[0], coords[1], source_row, dest_row, source_col, dest_col);

	MPI_Barrier(MPI_COMM_NEW);
	gettimeofday(&tv1,NULL);

	// printf("Starting to sort\n");
	/*
	printf("My rank:%d My data:\n", rank);
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++)
			printf("%3d ", myval[MX_SIZE+i][MX_SIZE+j]);
		printf("\n");
	}
	*/
	for(i=0;i<(int)ceil(log(n)/log(2));i++) {
		/* main looop */
		/* row like */
		// quicksortmatrix(myval,MX_SIZE,MX_SIZE,MX_SIZE,MX_SIZE);
		row_like(dest_row, source_row, 0);
		
		/* column like */
		// quicksortmatrix(myval,MX_SIZE,MX_SIZE,MX_SIZE,MX_SIZE);
		col_like(dest_col, source_col, 1);
	}

	MPI_Barrier(MPI_COMM_NEW);
	gettimeofday(&tv2,NULL);
	MPI_Type_vector(MX_SIZE, MX_SIZE, MAX_SIZE*MX_SIZE, MPI_INT, &max_matrix);
	MPI_Type_commit(&max_matrix);

	if (rank==0) {
		for(i=0;i<MX_SIZE;i++)
			for(j=0;j<MX_SIZE;j++)
				matrix[coords[0]*MX_SIZE+i][coords[1]*MX_SIZE+j] = myval[MX_SIZE+i][MX_SIZE+j];	
		for(i=1;i<size;i++) {
			MPI_Cart_coords(MPI_COMM_NEW, i, 2, coords);
			MPI_Recv(&(matrix[coords[0]*MX_SIZE][coords[1]*MX_SIZE]), 1, max_matrix, i, 0, MPI_COMM_NEW, &status);
		}
		for(i=0;i<n*MX_SIZE;i++) {
			for(j=0;j<n*MX_SIZE;j++) 
				printf("%3d ", matrix[i][j]);
			printf("\n");
		}
		// printf("Time: %12.5lf\n", (float)(tv2.tv_usec - tv1.tv_usec + (tv2.tv_sec-tv1.tv_sec)*1000000)/1000000.0);
		printf(" %12.5lf\n", (float)(tv2.tv_usec - tv1.tv_usec + (tv2.tv_sec-tv1.tv_sec)*1000000)/1000000.0);
/*		if (!verify(matrix))
			printf("You've blown it!!\n");
*/
	} else {
		MPI_Send(&myval[MX_SIZE][MX_SIZE], 1, mini_matrix, 0, 0, MPI_COMM_NEW);
	}
/*
	if ((coords[0]==0)&&(coords[1]==0)) {
		printf("%4d", myval);
		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) {
				if ((i==0) && (j==0)) continue;
				coords[0] = i;
				coords[1] = j;
				MPI_Cart_rank(MPI_COMM_NEW, coords, &rank);
				MPI_Recv(&buf, 1, MPI_INT, rank, 0, MPI_COMM_NEW, &status);
				printf("%4d", buf);
			}
			printf("\n");
		}
		printf("Time: %12.5lf\n", (float)(tv2.tv_usec - tv1.tv_usec + (tv2.tv_sec-tv1.tv_sec)*1000000)/1000000);
	} else {
		coords[0] = coords[1] = 0;
		MPI_Cart_rank(MPI_COMM_NEW, coords, &rank);
		MPI_Send(&myval, 1, MPI_INT, rank, 0, MPI_COMM_NEW);
	}
*/

	MPI_Type_free(&max_matrix);
	MPI_Type_free(&mini_matrix);
	MPI_Finalize();
}

void col_like(int dest, int source, int dim) {
	int i;
	for(i=0;i<ceil((float)n/2);i++) {
		if ((coords[dim]%2) == 0) {
			if (dest != -1) 
				send_recv(dest);
			if (source != -1) 
				recv_send_col(source);
		} else {
			if (source!=-1) 
				recv_send_col(source);
			if (dest != -1) {
				send_recv(dest);
			}
		}
	}
}

void row_like(int dest, int source, int dim) {
	int i;
	for(i=0;i<ceil((float)n/2);i++) {
		if ((coords[dim]%2) == 0) {
			if (dest != -1) 
				send_recv(dest);
			if (source != -1) 
				recv_send_row(source);
		} else {
			if (source != -1) 
				recv_send_row(source);
			if (dest != -1) {
				send_recv(dest);
			}
		}
	}
}

void send_recv(int dest) {
	int i,j;
	int *tab;
	MPI_Status status;
/*
	printf("Sending data: %d %d\n",dest,inc);
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d ", myval[MX_SIZE+i][MX_SIZE+j]);
		printf("\n");
	}

	tab = (int*)myval;
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d", *(tab+MX_SIZE*2*(i+MX_SIZE)+j+MX_SIZE));
		printf("\n");
	}
*/	
	MPI_Send(&(myval[MX_SIZE][MX_SIZE]), 1, mini_matrix, dest, 10, MPI_COMM_NEW);
	MPI_Recv(&(myval[MX_SIZE][MX_SIZE]), 1, mini_matrix, dest, 10, MPI_COMM_NEW, &status);
}

int recv_send_col(int source) {
	int ret,buf,i,j;
	MPI_Status status;
/*	
	printf("Before receive: %d %d\n",source,inc);
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d ", myval[i][MX_SIZE+j]);
		printf("\n");
	}
*/
	MPI_Recv(&(myval[0][MX_SIZE]), 1, mini_matrix, source, 10, MPI_COMM_NEW, &status);
/*	
	printf("Received data: %d %d\n",source,inc);
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d ", myval[i][MX_SIZE+j]);
		printf("\n");
	}
	*/
	for (i=0;i<MX_SIZE;i++)
		quicksortcol(myval, 0, 2*MX_SIZE-1, MX_SIZE+i,0,0);

	/*
	printf("After sort\n");
	for(i=0;i<2*MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d ", myval[i][j+MX_SIZE]);
		printf("\n");
	}
	printf("\n");
*/
	MPI_Send(&(myval[0][MX_SIZE]), 1, mini_matrix, source, 10, MPI_COMM_NEW);
	return ret;
}

int recv_send_row(int source) {
	int ret,buf,i,j;
	MPI_Status status;
	MPI_Recv(&(myval[MX_SIZE][0]), 1,mini_matrix, source, 10, MPI_COMM_NEW, &status);
/*
	printf("Received data: %d %d\n",source,inc);
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<MX_SIZE;j++) 
			printf("%3d ", myval[MX_SIZE+i][j]);
		printf("\n");
	}
*/
	for(i=0;i<MX_SIZE;i++)
		quicksortrow(myval, 0, 2*MX_SIZE-1, MX_SIZE+i,0,0);
/*
	printf("After sort\n");
	for(i=0;i<MX_SIZE;i++) {
		for(j=0;j<2*MX_SIZE;j++) 
			printf("%3d ", myval[MX_SIZE+i][j]);
		printf("\n");
	}
	printf("\n");
*/
	MPI_Send(&(myval[MX_SIZE][0]), 1, mini_matrix, source, 10, MPI_COMM_NEW);
	return ret;
}

