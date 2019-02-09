#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
        if(argc < 3){
                printf("error.");
                return 0;
        }
        FILE *fptrain = fopen(argv[1], "r");
        if(fptrain == NULL)
        {
                printf("error.");
                return 0;
        }
        int row, col, i, j;
        fscanf(fptrain, "%d", &col);
        col = col+1;
        fscanf(fptrain, "%d", &row);
        char ch;

        //creates the original X and Y matrix
        double trainX[row][col];
        double trainY[row][1];
        for(i=0; i<row; i++)
        {
                trainX[i][0] = 1.000000;
                for(j=1; j<col; j++)
                {
                        fscanf(fptrain, "%lf%c", &trainX[i][j], &ch);
                }
                        fscanf(fptrain, "%lf%c", &trainY[i][0], &ch);
        }

	 //creates the X transposed matrix
        double trainXtrans[col][row];
        for(i=0; i<row; i++)
        {
                for(j=0; j<col; j++)
                {
                        trainXtrans[j][i] = trainX[i][j];
                }
        }

        //multiplies X and X transposed
        double trainXtemp[row][row];
        int s;
        double num=0;
        for(i=0; i<col; i++)
        {
                for(j=0; j<col; j++)
                {
                        for(s=0; s<row; s++)
                        {
                                num = num +  trainX[s][j]*trainXtrans[i][s];
                        }
			trainXtemp[i][j] = num;
			num = 0;
                }
        }
        //finds the identity matrix of X times X transposed
	double trainXinden[col][col*2];
	for(i=0; i<col; i++)
	{
		for(j=0; j<col; j++)
		{
			trainXinden[i][j] = trainXtemp[i][j];
		}
		for(j=col; j<col*2; j++)
		{
			if(j==i+col)
			{
				trainXinden[i][j] = 1.000000;
			}
			else{
				trainXinden[i][j] = 0.000000;
			}
		}
	}
        //finds the inverse of X times X transposed through Gauss Jordan Elimination
	int k;
	double divscalar;
	for(i=0; i<col; i++)
	{
		divscalar = trainXinden[i][i];
		for(j=0; j<col*2; j++)
		{
			if(trainXinden[i][j] != 0)
			{
				trainXinden[i][j] = trainXinden[i][j]/divscalar;
			}
		}
		for(k=0; k<col; k++)
		{
			if(i!=k)
			{
				double subscalar = trainXinden[k][i];
				for(j=0; j<col*2; j++)
				{
					trainXinden[k][j] = trainXinden[k][j] - subscalar*trainXinden[i][j];
				}
			}
		}
	}

	double trainXinverse[row][row];
        for(i=0; i<row; i++)
        {
                for(j=0; j<col; j++)    
                {
                        trainXinverse[i][j] = trainXinden[i][j+col];
                }        
        }


        //multiplies (X times X transpose) inverse by (X transposed)
	double trainXinvXt[col][row];
	for(i=0; i<col; i++)
	{
		for(j=0; j<row; j++)
		{
			for(s=0; s<col; s++)
			{
				num = num + trainXinverse[i][s]*trainXtrans[s][j];
			}
				trainXinvXt[i][j] = num;
				num = 0;
		}
	} 
		
	//multiples (trainXinvXt) by Y
        double weight[row][1];
        for(i=0; i<col; i++)
        {
                for(s=0; s<row; s++)
                {
                        weight[i][0] +=  trainXinvXt[i][s]*trainY[s][0];
                }
        }

	FILE *fptest = fopen(argv[2], "r");
        if(fptest == NULL)
        {
                printf("error.");
                return 0;
        }
        int testrows;
        fscanf(fptest, "%d", &testrows);

        //creates the test file matrix
        double testM[testrows][col];
        for(i=0; i<testrows; i++)
        {
                testM[i][0] = 1.000000;
                for(j=1; j<col; j++)
                {
                        fscanf(fptest, "%lf%c", &testM[i][j], &ch);
                }
        }
        //multiplies the test file matrix by weights
        double prices[testrows][1];
        for(i=0; i<col; i++)
        {
                for(s=0; s<col; s++)
                {
                        prices[i][0] += testM[i][s]*weight[s][0];
                }
        }

	 printf("Original Matrix:\n");
        for(i=0; i<row; i++)           
        {
                for(j=0; j<col; j++)
                {
                        printf("%lf\t", trainX[i][j]);
                }
                printf("\n");
        }

        printf("Y MATRIX:\n");
        for(i=0; i<row; i++)
        {
                printf("%lf\t", trainY[i][0]);
                printf("\n");
        }



        printf("TRANSPOSED X MATRIX:\n");
        for(i=0; i<col; i++)
        {
                for(j=0; j<row; j++)
                {
                        printf("%lf\t", trainXtrans[i][j]);      
                }
                printf("\n");
        }        

 printf("MULTIPLIED MATRIX:\n");
        for(i=0; i<row; i++)
        {
                for(j=0; j<row; j++)
                {
                        printf("%lf\t", trainXtemp[i][j]);
                }
                printf("\n");
        }

        printf("IDENTITY MATRIX:\n");
        for(i=0; i<row; i++)
        {
                for(j=0; j<row; j++)
                {
                        printf("%lf\t", trainXinden[i][j]);
                }
                printf("\n");
        }
  printf("INVERSE MATRIX: \n");
        for(i=0; i<row; i++)
        {
                for(j=0; j<row; j++)
                {
                        printf("%lf\t", trainXinverse[i][j]);
                }
                printf("\n");
        }

        printf("INVERSE TIMES X TRANSPOSED: \n");
        for(i=0; i<col; i++)
        {
                for(j=0; j<row; j++)
                {
                        printf("%f\t", trainXinvXt[i][j]);
                }
                printf("\n");
        }
 printf("WEIGHTS: \n");
        for(i=0; i<col; i++)
        {
                for(j=0; j<col; j++){
                printf("%lf\t", weight[i][j]);
                }
                printf("\n");
        }


        printf("TEST MATRIX: \n");
        for(i=0; i<testrows; i++)
        {
                for(j=0; j<col; j++)
                {
                        printf("%lf\t", testM[i][j]);
                }
                printf("\n");
        }

	printf("PRICES: \n");
        for(i=0; i<testrows; i++)
        {
                printf("%0.0lf", prices[i][0]);
                printf("\n");
        }

return 0;
}
