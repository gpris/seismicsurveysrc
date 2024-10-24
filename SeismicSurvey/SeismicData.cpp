#include "StdAfx.h"
#include "SeismicData.h"

#include ".\NR\utils\nr.h"
using namespace std;

#define M_PI 3.141592
#define MAX_VALUE -1e-12
#define MIN_VALUE 1e-12

#include "SignalProcessingParameter.h"

CSeismicData::CSeismicData(void)
{
	m_DataNo = 0;
	m_Max = 0;
	m_Min = 0;
	m_Mean = 0;
	m_PeakNo = 0;
	m_SamplingTime = 1e-5;
	m_TxDepth = 0.0;
	m_RxDepth = 0.0;

	m_Data = NULL;
	m_ProcessedData = NULL;

//	m_pSigParameter = NULL;
}

CSeismicData::CSeismicData(int datano)
{
	m_DataNo = datano;
	m_Data = new float[m_DataNo];
	m_ProcessedData = new float[m_DataNo];
	m_Max = 0;
	m_Min = 0;
	m_Mean = 0;
	m_PeakNo = 0;
	m_SamplingTime = 1e-5;
	m_TxDepth = 0.0;
	m_RxDepth = 0.0;

//	m_pSigParameter = NULL;
}

//CSeismicData::CSeismicData(int datano, CSignalProcessingParameter * pSigParameter)
//{
//	m_DataNo = datano;
//	m_Data = new float[m_DataNo];
//	m_ProcessedData = new float[m_DataNo];
//	m_Max = 0;
//	m_Min = 0;
//	m_Mean = 0;
//	m_PeakNo = 0;
//	m_SamplingTime = 1e-5;
//	m_TxDepth = 0.0;
//	m_RxDepth = 0.0;
//
//	m_pSigParameter = pSigParameter;
//}

CSeismicData::~CSeismicData(void)
{
	DeleteData();
}

void CSeismicData::DeleteData()
{
	delete m_Data;
	delete m_ProcessedData;
	m_PeakIndex.RemoveAll();

}

void CSeismicData::Analyze()
{
	float max = -1e10;
	float min = 1e10;
	float total = 0;
	for(int indx = 0; indx<m_DataNo; indx++)
	{
		float data = m_Data[indx];
		if(indx>10)
		{
			if(data>max) max = data;
			if(data<min) min = data;
		}
		total += data;		
	}
	m_Max = max;
	m_Min = min;
	m_Mean = total/m_DataNo;
}


void CSeismicData::MakeFilteredSignal(int Mode, int Freq1, int Freq2 = 0)
{
	const int NN = m_DataNo, NN2 = NN+NN, NN_H = NN/2;
	int i, isign;
	Vec_DP data(NN2), window(NN);

	int indx;
	int GainDiff;
	int DataNoDiff;

	float df = 1/(m_SamplingTime)/NN;

	int filterwindowsize1;
	int filterwindowsize2;

	filterwindowsize1 = (int)(Freq1/df);
	filterwindowsize2 = (int)(Freq2/df);
	data[0] = 0.0;
	data[1] = 0.0;

	for(int indx = 0; indx<NN_H; indx++)
	{
		window[2*(indx)] = 0.0;
		window[2*(indx)+1] = 0.0;
	}

	int windowoffset = filterwindowsize1 - (int)((filterwindowsize2+0.5)/2);
	if(windowoffset<0)
		windowoffset = 0;

	for(int indx = 0; indx<filterwindowsize2;indx++)
	{
		int indx2 = indx + windowoffset +  1;
		if(indx2 >= NN_H)
			break;
		window[indx2] = 0.5*(1-cos(2*M_PI*((float)indx/(filterwindowsize2-1.0))));
		window[NN - indx2] = 0.5*(1-cos(2*M_PI*((float)indx/(filterwindowsize2-1.0))));
	}


	//FFT
	for(int indx = 0; indx<NN; indx++)
	{
		data[2*indx] = m_Data[indx];
		data[2*indx+1] = 0.0;

//		TRACE("%d %f\n", indx, data[2*indx]);
	}

    isign=1;
    NR::four1(data,isign);

	switch(Mode)
	{
	case FILTER_LPF:
		filterwindowsize1 = (int)(Freq1/df);
		data[0] = 0.0;
		data[1] = 0.0;

		for(int indx = filterwindowsize1; indx<(NN_H-1); indx++)
		{
			data[2*(indx+1)] = 0.0;
			data[2*(indx+1)+1] = 0.0;

			data[2*(NN-indx-1)] = 0.0;
			data[2*(NN-indx-1)+1] = 0.0;
		}
		break;
	case FILTER_HPF:
		// HPF filtering
		filterwindowsize1 = (int)(Freq1/df);
		data[0] = 0.0;
		data[1] = 0.0;

		for(int indx = 0; indx<filterwindowsize1; indx++)
		{
			data[2*(indx+1)] = 0.0;
			data[2*(indx+1)+1] = 0.0;

			data[2*(NN-indx-1)] = 0.0;
			data[2*(NN-indx-1)+1] = 0.0;
		}
		break;
	case FILTER_BPF:
		// HPF filtering
		filterwindowsize1 = (int)(Freq1/df);
		filterwindowsize2 = (int)(Freq2/df);
		data[0] = 0.0;
		data[1] = 0.0;

		for(int indx = 0; indx<filterwindowsize1; indx++)
		{
			data[2*(indx+1)] = 0.0;
			data[2*(indx+1)+1] = 0.0;

			data[2*(NN-indx-1)] = 0.0;
			data[2*(NN-indx-1)+1] = 0.0;
		}
		for(int indx = filterwindowsize2; indx<(NN/2-1); indx++)
		{
			data[2*(indx+1)] = 0.0;
			data[2*(indx+1)+1] = 0.0;

			data[2*(NN-indx-1)] = 0.0;
			data[2*(NN-indx-1)+1] = 0.0;
		}
		break;
	case FILTER_BPF_HANN:
		// HPF filtering
		data[0] = 0.0;
		data[1] = 0.0;

		for(int indx = 1; indx<NN_H; indx++)
		{
			data[2*(indx+1)] = data[2*(indx+1)] * window[indx];
			data[2*(indx+1)+1] = data[2*(indx+1)+1] * window[indx] ;

			data[2*(NN-indx-1)] = data[2*(NN-indx-1)]  * window[indx];
			data[2*(NN-indx-1)+1] = data[2*(NN-indx-1)+1]  * window[indx];
		}
		break;
	}

	//IFFT
	isign=-1;
	NR::four1(data,isign);

	TRACE("FFT result\n");

	for(int indx = 0; indx<NN; indx++)
	{
		m_ProcessedData[indx] = (float)data[2*indx];
//		TRACE("%d %f\n", indx, data[2*indx]);
	}
}

void CSeismicData::ApplySignalProcessing(CSignalProcessingParameter * pSigParameter)
{
//	SaveRawData("Rawdata.txt");
	MakeIIRFilter(0, pSigParameter->m_HPFIIRCoeff);
//	SaveProcessedData("HPFdata.txt");
	MakeIIRFilter(1, pSigParameter->m_LPFIIRCoeff);
//	SaveProcessedData("LPFdata.txt");
	MakeCorrelation(1, pSigParameter->m_PRBSCode);
//	SaveProcessedData("Corrdata.txt");
	pSigParameter->SetBPFCorrelatedFlag(true);
}

void CSeismicData::ApplyIIRFilter(int mode, CSignalProcessingParameter * pSigParameter)
{
//	SaveRawData("Rawdata.txt");
	MakeIIRFilter(mode, pSigParameter->m_HPFIIRCoeff);
//	SaveProcessedData("HPFdata.txt");
	MakeIIRFilter(1, pSigParameter->m_LPFIIRCoeff);
//	SaveProcessedData("LPFdata.txt");
	pSigParameter->SetIIRFilteredFlag(true);
}

void CSeismicData::ApplyCorrelation(int mode, CSignalProcessingParameter * pSigParameter)
{
	MakeCorrelation(mode, pSigParameter->m_PRBSCode);
//	SaveProcessedData("Corrdata.txt");
	pSigParameter->SetCorrelatedFlag(true);
}

void CSeismicData::CopyFrom(CSeismicData * src)
{
	m_SamplingTime = src->m_SamplingTime;
	m_Max = src->m_Max;
	m_Min = src->m_Min;
	m_Mean = src->m_Mean;
	m_DataNo = src->m_DataNo;
	m_PeakNo = src->m_PeakNo;
	m_TxDepth = src->m_TxDepth;
	m_RxDepth = src->m_RxDepth;
	m_ChID = src->m_ChID;

	m_Data = new float[m_DataNo];
	m_ProcessedData = new float[m_DataNo];

	for(int dindx = 0; dindx<m_DataNo; dindx++)
	{
		m_Data[dindx] = src->m_Data[dindx];
		m_ProcessedData[dindx] = src->m_ProcessedData[dindx];
	}
}
void CSeismicData::SaveProcessedData(CString filename)
{
	FILE * fout;

	fout = fopen(filename, "w");
	for(int indx = 0; indx<2048; indx++)
	{
		fprintf(fout, "%f\n", m_ProcessedData[indx]);
	}

	fclose(fout);
}

void CSeismicData::SaveRawData(CString filename)
{
	FILE * fout;

	fout = fopen(filename, "w");
	for(int indx = 0; indx<2048; indx++)
	{
		fprintf(fout, "%f\n", m_Data[indx]);
	}

	fclose(fout);
}

#include "CorrelationLib\CorrelationLib.h"
#include "CorrelationLib\Constants.h"

void CSeismicData::MakeCorrelation(int mode, float * PRBScode)
{
 float* CrossCorrelogram;
 int iTrialLength = 2048;               //the legth of the trial is 1000 so we have signal vectors holding 2 trials (2000 entries)
 int iCorrelationWindow = 2048;          //-100..+100 correlation lags
 
 //Here you should fill the SignalA and SignalB arrays with values

 //And then compute the correlogram
 int isError = 0;

 void* ClassicCC_Computer = NULL;
 ClassicCC_Computer = CreateCrossCorrelationComputerCC(iCorrelationWindow,iTrialLength,isError);
 if(isError == 0)
 {
	 switch(mode)
	 {
	 case 0:
        ComputeCrossCorrelationCC(ClassicCC_Computer,PRBScode,2048,m_Data,2048,0);
		break;
	 case 1:
        ComputeCrossCorrelationCC(ClassicCC_Computer,PRBScode,2048,m_ProcessedData,2048,0);
		break;
	 }
    CrossCorrelogram = GetCrossCorrelogramCC(ClassicCC_Computer);
    for(int j=0;j<iCorrelationWindow;j++)
    {
        if(CrossCorrelogram[j] != NAN)
        {
                m_ProcessedData[j] = CrossCorrelogram[j+iCorrelationWindow]/4.0;//use the CrossCorrelogram only if the value is not NAN (only if the correlation was defined)
        }
    }
 }
 if(ClassicCC_Computer != NULL) FreeCrossCorrelationComputerCC(ClassicCC_Computer);


}

#include "Filter\IIRFIR\IIRFilterCode.h"
#include "Filter\IIRFIR\QuadRootsCode.h"


#define _USE_MATH_DEFINES 
#include <math.h>

//---------------------------------------------------------------------------
// This shows some examples of how to use the functions in this file.
// These define's and include's are only needed for ExampleIIRCall.
#define NUM_PLOT_PTS 2048
#include <stdio.h>
#include "Filter\IIRFIR\FFTCode.h"
void CSeismicData::MakeIIRFilter(int mode, TIIRCoeff * IIRCoeff)
{
	double outdata[NUM_PLOT_PTS];
	double indata[NUM_PLOT_PTS];

	for(int indx = 0; indx<NUM_PLOT_PTS; indx++)
	{
		switch(mode)
		{
		case 0:
			indata[indx] =(double)m_Data[indx];
			break;
		case 1:
			indata[indx] =(double)m_ProcessedData[indx];
			break;
		}
	}

	FilterWithIIR(*IIRCoeff, indata, outdata, NUM_PLOT_PTS);

	for(int indx = 0; indx<NUM_PLOT_PTS; indx++)
	{
		m_ProcessedData[indx] = (float)outdata[indx];
	}
}

