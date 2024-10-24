#pragma once

class CSignalProcessingParameter;
struct TIIRCoeff;

class CSeismicData
{
public:
	CSeismicData(void);
	CSeismicData(int datano);
//	CSeismicData(int datano, CSignalProcessingParameter * pSigParameter);
	~CSeismicData(void);
	float * m_Data;
	float * m_ProcessedData;

	float m_SamplingTime;

	float m_Max;
	float m_Min;
	float m_Mean;
	CArray <int, int> m_PeakIndex;
	int m_DataNo;
	int m_PeakNo;
	void Analyze();
	void DeleteData();
	void MakeFilteredSignal(int Mode, int Freq1, int Freq2);

	void ApplySignalProcessing(CSignalProcessingParameter * pSigParameter);
	void ApplyIIRFilter(int mode, CSignalProcessingParameter * pSigParameter);
	void ApplyCorrelation(int mode, CSignalProcessingParameter * pSigParameter);


	void MakeCorrelation(int mode, float * PRBScode);
	void MakeIIRFilter(int mode, TIIRCoeff * IIRCoeff);

	void SaveProcessedData(CString filename);
	void SaveRawData(CString filename);

	float m_TxDepth;
	float m_RxDepth;

	int m_ChID;
	enum FILTER_TYPE {FILTER_LPF, FILTER_HPF, FILTER_BPF, FILTER_BPF_HANN};
	void CopyFrom(CSeismicData * src);

//	CSignalProcessingParameter * m_pSigParameter;

};

