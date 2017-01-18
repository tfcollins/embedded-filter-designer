
#include "mex.h"
#include <liquid/liquid.h>
#include <stdio.h>

void liquidfir(int num_taps, int num_bands, double *bands, double *des,
        double *weights, float *h, int pass_bands, int stop_bands)
{
    /* Convert Doubles to Floats */
    int k;
    float bandsF[num_bands*2];
    float desF[num_bands];
    float weightsF[num_bands];
    
    for (k=0; k<num_bands; k++)
    {
        desF[k] = (float)des[k];
        weightsF[k] = (float)weights[k];
    }
    for (k=0; k<num_bands*2; k++)
        bandsF[k] = (float)bands[k];
    
    /* define filter type and weights */
    liquid_firdespm_btype btype = LIQUID_FIRDESPM_BANDPASS;

    int i;
    liquid_firdespm_wtype wtype[num_bands];
    for(i=0;i<pass_bands;i++)
    {
      wtype[i] = LIQUID_FIRDESPM_FLATWEIGHT; /*LIQUID_FIRDESPM_EXPWEIGHT*/
    }
    for(i=0;i<stop_bands;i++)
    {
      wtype[i+pass_bands] = LIQUID_FIRDESPM_EXPWEIGHT;
    }
    
    /* Run filter designer*/
    firdespm_run(num_taps,num_bands,bandsF,desF,weightsF,wtype,btype,h);
}

/* The gateway function */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

    /* get the value of the scalar inputs  */
    int num_taps = mxGetScalar(prhs[0]);
    int num_bands = mxGetScalar(prhs[1]);
    int pass_bands = mxGetScalar(prhs[5]);
    int stop_bands = mxGetScalar(prhs[6]);

    /* create a pointer to the real data in the input matrix  */
    double* bands = mxGetPr(prhs[2]);
    double* des = mxGetPr(prhs[3]);
    double* weights = mxGetPr(prhs[4]);   

    /* create the output matrix */
    plhs[0] = mxCreateDoubleMatrix(1,(mwSize)num_taps,mxREAL);

    /* get a pointer to the real data in the output matrix */
    double* taps = mxGetPr(plhs[0]);
    int i;
    printf("num taps: %d\n", num_taps);
    printf("num bands: %d\n", num_bands);
/*
    printf("bands: ");
    for (i=0; i<num_bands*2; i++)
       printf(" %f", bands[i]);
    printf("\n");
    printf("des: ");
    for (i=0; i<num_bands; i++)
       printf(" %f", des[i]);
    printf("\n");
    printf("weights: ");
    for (i=0; i<num_bands; i++)
      printf(" %f", weights[i]);
    printf("\n");
*/
    
    /* Call Liquid SDR filter designer */
    float tapsF[num_taps];
    liquidfir(num_taps, num_bands, bands, des, weights, tapsF, pass_bands, stop_bands);
    
    /* Convert float taps to double */
    for (i=0; i<num_taps; i++)
      taps[i] = (double)tapsF[i];
    
}
