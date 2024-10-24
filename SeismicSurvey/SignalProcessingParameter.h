#pragma once

struct TIIRCoeff;

class CSignalProcessingParameter
{
public:
	CSignalProcessingParameter(void);
	~CSignalProcessingParameter(void);

	CString m_PRBSCodePath;
	int m_DutyCycle;

	float * m_PRBSCode;
	TIIRCoeff * m_LPFIIRCoeff;                    // These next 3 are defined in the header file.
	TIIRCoeff * m_HPFIIRCoeff;                    // These next 3 are defined in the header file.	

	bool m_bIIRFiltered;
	bool m_bCorrelated;
	bool m_bBPFCorrelated;

	float m_SamplingFreq;
	float m_LPFCutoffFreq;
	float m_HPFCutoffFreq;

	void MakeCorrelation(int mode);
	void MakeIIRFilterCoeff(int FilterType, int PassType, TIIRCoeff * IIRCoeff, float freq_samp, float freq1, float freq2=0);
	void LoadPRBSCode(CString filename, int dutycycle);
	void ClearPRBSCode();

	void SetIIRFilteredFlag(bool value);
	void SetCorrelatedFlag(bool value);
	void SetBPFCorrelatedFlag(bool value);

	void ChangeIIRFilterCoeff(float samplingfreq, float hpffreq, float lpffreq);
	void ChangeCorrelationParameter(CString prbspath, int dutycycle);

	void Copy(CSignalProcessingParameter * pSrc);
};

