#include "StdAfx.h"
#include "SignalProcessingParameter.h"

#include "CorrelationLib\CorrelationLib.h"
#include "CorrelationLib\Constants.h"

#include "Filter\IIRFIR\IIRFilterCode.h"
#include "Filter\IIRFIR\QuadRootsCode.h"


#define _USE_MATH_DEFINES 
#include <math.h>

//---------------------------------------------------------------------------
// This shows some examples of how to use the functions in this file.
// These define's and include's are only needed for ExampleIIRCall.
#define NUM_PLOT_PTS 1024
#include <stdio.h>
#include "Filter\IIRFIR\FFTCode.h"

CSignalProcessingParameter::CSignalProcessingParameter(void)
{
	m_PRBSCode = new float[2048];

	ClearPRBSCode();

	TCHAR curPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curPath);

	m_PRBSCodePath.Format("%s\\Config\\PRBS_8bit.txt", curPath);
	m_DutyCycle = 4;
	LoadPRBSCode(m_PRBSCodePath, m_DutyCycle);

	m_SamplingFreq = 20e3;
	m_LPFCutoffFreq = 10.0e3;
	m_HPFCutoffFreq = 1e3;

	m_LPFIIRCoeff = new TIIRCoeff;
	m_HPFIIRCoeff = new TIIRCoeff;

	MakeIIRFilterCoeff(ftBUTTERWORTH, iirHPF, m_HPFIIRCoeff, m_SamplingFreq, m_HPFCutoffFreq);
	MakeIIRFilterCoeff(ftBUTTERWORTH, iirLPF, m_LPFIIRCoeff, m_SamplingFreq, m_LPFCutoffFreq);

	m_bIIRFiltered = false;
	m_bCorrelated = false;
	m_bBPFCorrelated = false;

}


CSignalProcessingParameter::~CSignalProcessingParameter(void)
{
}

void CSignalProcessingParameter::ClearPRBSCode()
{
	for(int indx = 0; indx<2048; indx++)
	{
		m_PRBSCode[indx] = 0.0;
	}
}

void CSignalProcessingParameter::LoadPRBSCode(CString filename, int dutycycle)
{
	FILE * fin;
	if(fin = fopen(filename, "r"))
	{

		int count = 0;
		int tempvalue;
		while(fscanf(fin, "%d", &tempvalue) != EOF)
		{
			for(int indx = 0; indx<dutycycle; indx++)
			{
				m_PRBSCode[count+indx] = tempvalue;
			}
			count += dutycycle;
		}

		fclose(fin);
	}

}
void CSignalProcessingParameter::MakeCorrelation(int mode)
{
 float SignalA[2000];
 float SignalB[2000];
 float* CrossCorrelogram;
 int iTrialLength = 1000;               //the legth of the trial is 1000 so we have signal vectors holding 2 trials (2000 entries)
 int iCorrelationWindow = 100;          //-100..+100 correlation lags
 
 //Here you should fill the SignalA and SignalB arrays with values

 //And then compute the correlogram
 int isError = 0;

 void* ClassicCC_Computer = NULL;
 ClassicCC_Computer = CreateCrossCorrelationComputerCC(iCorrelationWindow,iTrialLength,isError);
 if(isError == 0)
 {
        ComputeCrossCorrelationCC(ClassicCC_Computer,SignalA,2000,SignalB,2000,0);
        CrossCorrelogram = GetCrossCorrelogramCC(ClassicCC_Computer);
        for(int j=0;j<2*iCorrelationWindow+1;j++)
        {
                if(CrossCorrelogram[j] != NAN)
                {
                        //use the CrossCorrelogram only if the value is not NAN (only if the correlation was defined)
                }
        }
 }
 if(ClassicCC_Computer != NULL) FreeCrossCorrelationComputerCC(ClassicCC_Computer);


}



void CSignalProcessingParameter::MakeIIRFilterCoeff(int FilterType, int PassType, TIIRCoeff * IIRCoeff, float freq_samp, float freq1, float freq2)
{
 int NumPoles = 6;                // 2 - 10 for all filters except the Elliptics, which are 4 - 10
 double Ripple = 0.1;             // 0.0 - 1.0 dB for Chebyshev in increments of 0.10  0.0 - 0.2 for the Elliptics in increments 0f 0.02
 double StopBanddB = 60;          // 20 - 100 dB for the Inv Cheby  40 - 100 for the Elliptics
 double OmegaC;// = 0.2;             // 0.0 < OmegaC < 1.0
 double BW = 0.1;                 // 0.0 < BandWidth < 1.0
 double Gamma = 0.0;              // -1.0 <= Gamma <= 1.0  Gamma controls the transition BW on the Adjustable Gauss
 double RealHofZ[NUM_PLOT_PTS];   // Real and imag parts of H(z). Used with the function IIRFreqResponse.
 double ImagHofZ[NUM_PLOT_PTS];

 OmegaC = 2.0*freq1/freq_samp;

// TIIRCoeff IIRCoeff;                    // These next 3 are defined in the header file.
// TIIRPassTypes PassType = iirLPF;       // iirLPF, iirHPF, iirBPF, iirNOTCH
// TIIRFilterTypes FiterType = ftELLIPTIC_00; // ftBUTTERWORTH, ftGAUSSIAN, ftBESSEL, ftADJGAUSS, ftCHEBYSHEV, ftINVERSE_CHEBY, ftELLIPTIC_00

 // If using an Elliptic, we must select the ELLIPTIC according to the Ripple value.
 // ftELLIPTIC_00 is the 1st ELLIPTIC in TIIRFilterTypes.
 if(FilterType >= ftELLIPTIC_00)
  {
   if(Ripple >= 0.19)       FilterType = ftELLIPTIC_20; // Ripple = 0.20 dB
   else if(Ripple >= 0.17)  FilterType = ftELLIPTIC_18; // Ripple = 0.18 dB
   else if(Ripple >= 0.15)  FilterType = ftELLIPTIC_16; // Ripple = 0.16 dB
   else if(Ripple >= 0.13)  FilterType = ftELLIPTIC_14; // Ripple = 0.14 dB
   else if(Ripple >= 0.11)  FilterType = ftELLIPTIC_12; // Ripple = 0.12 dB
   else if(Ripple >= 0.09)  FilterType = ftELLIPTIC_10; // Ripple = 0.10 dB
   else if(Ripple >= 0.07)  FilterType = ftELLIPTIC_08; // Ripple = 0.08 dB
   else if(Ripple >= 0.05)  FilterType = ftELLIPTIC_06; // Ripple = 0.06 dB
   else if(Ripple >= 0.03)  FilterType = ftELLIPTIC_04; // Ripple = 0.04 dB
   else if(Ripple >= 0.01)  FilterType = ftELLIPTIC_02; // Ripple = 0.02 dB
   else                     FilterType = ftELLIPTIC_00; // Ripple = 0.00 dB
  }

 // Calling CalcIIRFilterCoeff() fills the IIRCoeff struct.
 // Beta, the 2nd function argument, has a different meaning, depending on the filter type.
 // Beta is also range checked in GetSPlaneCoefficients()

 // Beta is ignored and set to 0
 if(FilterType == ftBUTTERWORTH || FilterType == ftBESSEL || FilterType == ftGAUSSIAN)
  {
   *IIRCoeff = CalcIIRFilterCoeff((TIIRFilterTypes)FilterType, 0, (TIIRPassTypes)PassType, NumPoles, OmegaC, BW);
  }

 // Beta = Ripple in dB  >= 0
 if(FilterType == ftCHEBYSHEV)
  {
   if(Ripple < 0.0)Ripple = -Ripple;
   *IIRCoeff = CalcIIRFilterCoeff((TIIRFilterTypes)FilterType, Ripple, (TIIRPassTypes)PassType, NumPoles, OmegaC, BW);
  }

 // Beta = StopBanddB >= 0
 if(FilterType == ftINVERSE_CHEBY || FilterType >= ftELLIPTIC_00)
  {
   if(StopBanddB < 0.0)StopBanddB = -StopBanddB;
   *IIRCoeff = CalcIIRFilterCoeff((TIIRFilterTypes)FilterType, StopBanddB, (TIIRPassTypes)PassType, NumPoles, OmegaC, BW);
  }

 // Beta = Gamma, which adjusts the width of the transition band.  -1 <= Gamma <= 1
 if(FilterType == ftADJGAUSS)
  {
   if(Gamma < -1.0)Gamma = -1.0;
   if(Gamma > 1.0) Gamma = 1.0;
   *IIRCoeff = CalcIIRFilterCoeff((TIIRFilterTypes)FilterType, Gamma, (TIIRPassTypes)PassType, NumPoles, OmegaC, BW);
  }

/*
 This will print the IIR coefficients to a text file. Be sure to set the path before using.
 FILE *OutPutFile;
 OutPutFile = fopen("C:\\Documents and Settings\\Dan\\Desktop\\IIRCoeff.txt","w");
 for(int j=0; j<IIRCoeff.NumSections; j++)
  {
   fprintf(OutPutFile,"\n Section %d", j);
   fprintf(OutPutFile,"\n a0= %9.9f  a1= %9.9f  a2= %9.9f", IIRCoeff.a0[j], IIRCoeff.a1[j], IIRCoeff.a2[j]);
   fprintf(OutPutFile,"\n b0= %9.9f  b1= %9.9f  b2= %9.9f ",IIRCoeff.b0[j], IIRCoeff.b1[j], IIRCoeff.b2[j]);
   fprintf(OutPutFile,"\n ");
  }
 fclose(OutPutFile);
*/

 // This calculates the frequency response of the filter by doing a DFT of the IIR coefficients.
 IIRFreqResponse(*IIRCoeff, IIRCoeff->NumSections, RealHofZ, ImagHofZ, NUM_PLOT_PTS);

/*
 // This is an alternative way to calculate the filter's frequency response.
 // We send an impulse through the filter, and calc the FFT of the filters output.
 // Since the FFT scales the output of a forward transform by 1/N,
 // we use N = NUM_PLOT_PTS instead of 1 for the impulse.

 for(k=0; k<NUM_PLOT_PTS; k++)Samples[k] = RealHofZ[k] = ImagHofZ[k] = 0.0;
 Samples[0] = NUM_PLOT_PTS;                                 // The impulse.
 FilterWithIIR(IIRCoeff, Samples, RealHofZ, NUM_PLOT_PTS);  // Filter the impulse. RealHofZ is used to store the filtered output.
 FFT(RealHofZ, ImagHofZ, NUM_PLOT_PTS, FORWARD);            // The FFT's results are returned in input arrays, RealHofZ and ImagHofZ.
*/


}

void CSignalProcessingParameter::SetIIRFilteredFlag(bool value)
{
	m_bIIRFiltered = value;
	m_bCorrelated = false;
	m_bBPFCorrelated = false;
}
void CSignalProcessingParameter::SetCorrelatedFlag(bool value)
{
	m_bIIRFiltered = false;
	m_bCorrelated = value;
	m_bBPFCorrelated = false;
}
void CSignalProcessingParameter::SetBPFCorrelatedFlag(bool value)
{
	m_bIIRFiltered = false;
	m_bCorrelated = false;
	m_bBPFCorrelated = value;
}

void CSignalProcessingParameter::ChangeIIRFilterCoeff(float samplingfreq, float hpffreq, float lpffreq)
{
	m_SamplingFreq = samplingfreq;
	m_HPFCutoffFreq = hpffreq;
	m_LPFCutoffFreq = lpffreq;

	MakeIIRFilterCoeff(ftBUTTERWORTH, iirHPF, m_HPFIIRCoeff, m_SamplingFreq, m_HPFCutoffFreq);
	MakeIIRFilterCoeff(ftBUTTERWORTH, iirLPF, m_LPFIIRCoeff, m_SamplingFreq, m_LPFCutoffFreq);
}
void CSignalProcessingParameter::ChangeCorrelationParameter(CString prbspath, int dutycycle)
{
	m_PRBSCodePath = prbspath;
	m_DutyCycle = dutycycle;
	LoadPRBSCode(m_PRBSCodePath, m_DutyCycle);
}

void CSignalProcessingParameter::Copy(CSignalProcessingParameter * pSrc)
{
	m_PRBSCodePath = pSrc->m_PRBSCodePath;
	m_DutyCycle = pSrc->m_DutyCycle;

	memcpy(m_PRBSCode, pSrc->m_PRBSCode, sizeof(float)*2048);

	memcpy(m_LPFIIRCoeff, pSrc->m_LPFIIRCoeff, sizeof(TIIRCoeff));
	memcpy(m_HPFIIRCoeff, pSrc->m_HPFIIRCoeff, sizeof(TIIRCoeff));

	m_bIIRFiltered = pSrc->m_bIIRFiltered;
	m_bCorrelated = pSrc->m_bIIRFiltered;
	m_bBPFCorrelated  = pSrc->m_bIIRFiltered;

	m_SamplingFreq = pSrc->m_SamplingFreq;
	m_LPFCutoffFreq = pSrc->m_LPFCutoffFreq;
	m_HPFCutoffFreq = pSrc->m_LPFCutoffFreq;
}