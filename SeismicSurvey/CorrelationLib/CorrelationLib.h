



#ifndef CORRELATIONLIB_H
#define CORRELATIONLIB_H

#include "Constants.h"

#define ERR_NULL_OBJECT -5


//Interface for Binary - Binary scaled corr ======================================================================================================================================

///Creates an object that computes scaled correlation on a pair of binary - binary signals
// Constructor parameters are as follows:
///@param piScaleWindow			- The scale on which you compute the scaled correlation; use the same units as the sampling unit of your time stamps
///@param piCorrelationWindow	- The size of the cross correlation window (eg. 80 for a cross correlation with lags of -80..+80); use the same units as the sampling of your time stamps
///@param piTrialLength			- The size of the trial in units of your time stamps
///@param piIsError				- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetScaledCorrelogram() function will return NULL
///@return An untyped pointer to the created object
void* CreateScaledCorrelationComputerBB(int piScaleWindow,int piCorrelationWindow,int piTrialLength,int &piIsError);

///Destructor for scaled correlation objects on binary-binary pairs; always call this to cleanup your data space
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
void FreeScaledCorrelationComputerBB(void* pvObject);

///Compute the scaled correlation of two vectors of time stamps
//Parameters for ComputeScaledCorrelation
///@param pvObject				- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piaTimeStampsA		- The vector holding the time stamps of the first binary variable
///@param piNrTimeStampsInA		- Number of time stamps in the first vector
///@param piaTimeStampsB		- The vector holding the time stamps of the second binary variable
///@param piNrTimeStampsInB		- Number of time stamps in the second vector
///@param pbUseFisherZTransform	- Set to 1 to use the Fisher Z transform to average the coefficients of correlation; set to 0 for normal computation;
void ComputeScaledCorrelationBB(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,int *piaTimeStampsB,int piNrTimeStampsInB,int pbUseFisherZTransform);
	

///Compute the scaled correlation of two vectors of time stamps, on a partial window of the trial; ONLY ACCEPTS ONE TRIAL!!!
//
///@param pvObject				- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piaTimeStampsA		- The vector holding the time stamps of the first binary variable
///@param piNrTimeStampsInA		- Number of time stamps in the first vector
///@param piaTimeStampsB		- The vector holding the time stamps of the second binary variable
///@param piNrTimeStampsInB		- Number of time stamps in the second vector
///@param piFromOffsetInTrial	- The start offset in the trial where the desired window starts
///@param piToOffsetInTrial		- The end offset in the trial where the desired window stops
///@param pbUseFisherZTransform	- Set to 1 to use the Fisher Z transform to average the coefficients of correlation; set to 0 for normal computation
void ComputeWindowedScaledCorrelationPerTrialBB(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,int *piaTimeStampsB,int piNrTimeStampsInB,int piFromOffsetInTrial,int piToOffsetInTrial,int pbUseFisherZTransform);

///Returns the buffer with the computed scaled-cross-correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the scaled correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow;
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns a pointer to the internal scaled correlation buffer. Do not write into the buffer! \n
///		   The buffer has a NotANumber (NAN = -100000000.0) value for bins where correlation could not be defined because of lack of variance in the input vectors
float* GetScaledCrossCorrelogramBB(void* pvObject);


///Returns the sum of valid Fi coefficients of correlation for each bin of the correlogram
//
///The size of the buffer is (2*iCorrelationWindow+1), and the sum of coefficients for lag 0 is at position iCorrelationWindow.
///Use this function to get the sum of all valid coefficients that were used to compute the average correlation coefficient for each bin of the ScaledCrossCorrelogram: ScaledCrossCorrelogram[i] = FiCoefficientSums[i] / FiCoefficientCounts[i].
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return A pointer to a buffer holding the sum of correlation coefficients for each bin (without dividing them to the counts; the scaled correlogram = the sum of coefficients / the count of coefficients).
///@warning If no valid coefficient was found for a given bin, the buffer containes a value of NAN at that position. Please check for this in the code!
float* GetFiCoefficientSumsBB(void* pvObject);


///Returns how many Fi coefficients of correlation have been averaged for each bin of the scaled cross correlogram
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return A pointer to the internal buffer containing the counts of "Fi" correlation coefficients averaged for each bin.
///The size of the buffer is (2*iCorrelationWindow+1), and the count of valid Fi coefficients, that were averaged to compute the scaled cross correlogram at lag 0, is at position iCorrelationWindow
int* GetFiCoefficientCountsBB(void* pvObject);


///Returns the distribution of coefficients of correlation
//
///@param  pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piNumberOfBins - The number of bins in the distribution (output parameter)
///@param pfBinSize - The size of one bin of the distribution (output parameter) 
///@return - A pointer to the internal buffer that holds the distribution of correlation coefficients
///@return - The number of bins in the distribution (piNumberOfBins); this is equal to the size of the buffer
///@return - The size of one bin of the distribution (pfBinSize); the size of the bin is in sampling units of the original signals
int* GetDistributionOfCorrelationCoefficientsBB(void* pvObject,int& piNumberOfBins,float& pfBinSize);


///Changes the scale window for the scaled correlation;
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piNewScale - The new size of the scale segment
///@return Returns 0 on success, -1 if the scale window is too small; -2 if the new scale window is larger than the trial size
int ScaledCorrelationModifyScaleWindowBB(void* pvObject,int piNewScale);


///Set the size of the correlation window; the parameter specifies the correlation window; for example for a window of -100..+100 pass 100 as a parameter
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piNewCorrelationWindow - The new size of the correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial
int ScaledCorrelationModifyCorrelationWindowBB(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the scale window; -2 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyTrialLengthBB(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@param piNewScale - The new size of the scale segment
///@param piNewCorrelationWindow - The new size of the correlation window
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success, -1 if the new scale window is too small; -2 if the new correlation window is too small; -3 if the new scale window is larger than the trial size; -4 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyAllParametersBB(void* pvObject,int piNewScale,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the current scale window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns the size of the scale segment window
int ScaledCorrelationGetScaleWindowBB(void* pvObject);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns the size of the correlation window
int ScaledCorrelationGetCorrelationWindowBB(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns the length of the trial in original sampling units
int ScaledCorrelationGetTrialLengthBB(void* pvObject);


///Get the number of bins of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns the number of bins used to compute the distribution of correlation coefficients; the value equals the size of the internal buffer that stores the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficientsBB)
int GetDistributionOfCorrelationCoefficientsBinNrBB(void* pvObject);


///Get the size of a bin of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBB function
///@return Returns the size of the bins used to compute the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficientsBB)
float GetDistributionOfCorrelationCoefficientsBinSizeBB(void* pvObject);

//====================================================================================================================================================================================





//Interface for Continuous - Continuous scaled corr ==================================================================================================================================
///Creates an object that computes scaled correlation on a pair of continuous - continuous signals
//Constructor parameters are as follows:
///@param piScaleWindow		- The scale on which you compute the scaled correlation; use the same units as the sampling unit of your signals
///@param piCorrelationWindow	- The size of the cross correlation window (eg. 80 for a cross correlation with lags of -80..+80); use again the same units as the sampling of your signals
///@param piTrialLength		- The size of the trial in sampling units of your signals
///@param piIsError			- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetScaledCorrelogram() function will return NULL
///@return An untyped pointer to the created object
void* CreateScaledCorrelationComputerCC(int piScaleWindow,int piCorrelationWindow,int piTrialLength,int &piIsError);


///Destructor for scaled correlation objects on continuous-continuous input pairs; always call this to cleanup your data space
//
///@param pvObject - pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
void FreeScaledCorrelationComputerCC(void* pvObject);


///Compute the scaled correlation of two digitized continuous signals
//Parameters for ComputeScaledCorrelation
///@param pvObject				- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param pfaSamplesA			- The vector holding the first signal's input samples
///@param piNrSamplesInA		- Number of samples in the first vector
///@param pfaSamplesB			- The vector holding the second signal's input samples
///@param piNrSamplesInB		- Number of samples in the second vector
///@param pbUseFisherZTransform	- Set to 1 to use the Fisher Z transform to average the coefficients of correlation; set to 0 for normal computation;
void ComputeScaledCorrelationCC(void* pvObject,float *pfaSamplesA,int piNrSamplesInA,float *pfaSamplesB,int piNrSamplesInB,int pbUseFisherZTransform);


///Compute the scaled correlation of two digitized continuous signals, on a partial window of the trial; ONLY ACCEPTS ONE TRIAL!!!
//Parameters for ComputeWindowedScaledCorrelationPerTrial
///@param pvObject				- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param pfaSamplesA			- The vector holding the first signal's input samples
///@param piNrSamplesInA		- Number of samples in the first vector
///@param pfaSamplesB			- The vector holding the second signal's input samples
///@param piNrSamplesInB		- Number of samples in the second vector
///@param piFromOffsetInTrial	- The start offset in the trial where the desired window starts
///@param piToOffsetInTrial		- The end offset in the trial where the desired window stops
///@param pbUseFisherZTransform	- Set to 1 to use the Fisher Z transform to average the coefficients of correlation; set to 0 for normal computation;
void ComputeWindowedScaledCorrelationPerTrialCC(void* pvObject,float *pfaSamplesA,int piNrSamplesInA,float *pfaSamplesB,int piNrSamplesInB,int piFromOffsetInTrial,int piToOffsetInTrial,int pbUseFisherZTransform);


///Returns the buffer with the computed scaled correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the scaled correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow;
///@param pvObject				- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns a pointer to the internal scaled correlation buffer. Do not write into the buffer! \n
///			The buffer has a NotANumber (NAN = -100000000.0) value for bins where correlation could not be defined because of lack of variance in the input signals
float* GetScaledCrossCorrelogramCC(void* pvObject);


///Returns the sum of valid Pearson coefficients of correlation for each bin of the correlogram
//
///The size of the buffer is (2*iCorrelationWindow+1), and the sum of coefficients for lag 0 is at position iCorrelationWindow.
///Use this function to get the sum of all valid coefficients that were used to compute the average correlation coefficient for each bin of the ScaledCrossCorrelogram: ScaledCrossCorrelogram[i] = PearsonCoefficientSums[i] / PearsonCoefficientCounts[i].
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return A pointer to a buffer holding the sum of correlation coefficients for each bin (without dividing them to the counts; the scaled correlogram = the sum of coefficients / the count of coefficients).
///@warning If no valid coefficient was found for a given bin, the buffer containes a value of NAN at that position. Please check for this in the code!
float* GetPearsonCoefficientSumsCC(void* pvObject);


//Returns how many Pearson coefficients of correlation have been averaged for each bin of the scaled cross correlogram
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return A pointer to the internal buffer containing the counts of correlation coefficients averaged for each bin.
///The size of the buffer is (2*iCorrelationWindow+1), and the count of coefficients that were averaged for lag 0 is at position iCorrelationWindow.
int* GetPearsonCoefficientCountsCC(void* pvObject);


///Returns the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param piNumberOfBins - The number of bins in the distribution (output parameter)
///@param pfBinSize - The size of one bin of the distribution (output parameter) 
///@return A pointer to the internal buffer that holds the distribution of correlation coefficients
///@return The number of bins in the distribution (piNumberOfBins); this is equal to the size of the buffer
///@return The size of one bin of the distribution (pfBinSize); the size of the bin is in sampling units of the original signals
int* GetDistributionOfCorrelationCoefficientsCC(void* pvObject,int& piNumberOfBins,float& pfBinSize);


///Changes the scale segment size for the scaled correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param piNewScale - The new size of the scale segment
///@return Returns 0 on success, -1 if the scale window is too small; -2 if the new scale window is larger than the trial size
int ScaledCorrelationModifyScaleWindowCC(void* pvObject,int piNewScale);


///Set the size of the correlation window; the parameter specifies the correlation window; for example for a window of -100..+100 pass 100 as a parameter
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param piNewCorrelationWindow - The new size of the correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial
int ScaledCorrelationModifyCorrelationWindowCC(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the scale window; -2 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyTrialLengthCC(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@param piNewScale - The new size of the scale segment
///@param piNewCorrelationWindow - The new size of the correlation window
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success, -1 if the new scale window is too small; -2 if the new correlation window is too small; -3 if the new scale window is larger than the trial size; -4 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyAllParametersCC(void* pvObject,int piNewScale,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the current scale window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns the size of the scale segment window
int ScaledCorrelationGetScaleWindowCC(void* pvObject);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns the size of the correlation window
int ScaledCorrelationGetCorrelationWindowCC(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns the length of the trial in original sampling units
int ScaledCorrelationGetTrialLengthCC(void* pvObject);


///Get the number of bins of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns the number of bins used to compute the distribution of correlation coefficients; the value equals the size of the internal buffer that stores the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficientsCC)
int GetDistributionOfCorrelationCoefficientsBinNrCC(void* pvObject);


///Get the size of a bin of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerCC function
///@return Returns the size of the bins used to compute the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficientsCC)
float GetDistributionOfCorrelationCoefficientsBinSizeCC(void* pvObject);

//====================================================================================================================================================================================





//Interface for Binary - Continuous scaled corr ====================================================================================================================================

//Construction interface

//Constructor parameters are as follows:
///Creates an object that computes scaled correlation on a pair of binary - continuous signals
//Constructor parameters are as follows:
///@param piScaleWindow			- The scale on which you compute the scaled correlation; use the same units as the sampling unit of your time stamps
///@param piCorrelationWindow	- The size of the cross-correlation window (eg. 80 for a cross-correlation with lags of -80..+80); use again the same units as the sampling of your time stamps
///@param piTrialLength			- The size of the trial in original sampling units
///@param piIsError				- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetScaledCorrelogram() function will return NULL
///@warning The sampling frequency of the continuous and binary signals has to be the same! Also the trial lengths have to match!
///@return An untyped pointer to the created object!
void* CreateScaledCorrelationComputerBC(int piScaleWindow,int piCorrelationWindow,int piTrialLength,int &piIsError);


///Destructor for scaled correlation objects on binary-continuous input pairs; always call this to cleanup your data space
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
void FreeScaledCorrelationComputerBC(void* pvObject);


//Functional interface

///Compute the scaled correlation of one binary signal with one continuos signal
//Parameters for ComputeScaledCorrelation
///@param pvObject					- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piaTimeStampsA			- The vector holding the time stamps of the binary variable
///@param piNrTimeStampsInA			- Number of time stamps in the first vector
///@param pfaSamplesB				- The vector holding samples of the continuous signal
///@param piNrSamplesInB			- Number of samples in the second vector
///@param pbUseFisherZTransform		- Set to 1 to use the Fisher Z transform to average the coefficients of correlation; set to 0 for normal computation;
///@warning The sampling frequency of the continuous and binary signals has to be the same! Also the trial lengths have to match! \n
///			The time stamps of the binary signal must match the samples of the continuous signal, i.e. the first sample of the continuous signal corresponds to time stamp 0.
void ComputeScaledCorrelationBC(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,float *pfaSamplesB,int piNrSamplesInB,int pbUseFisherZTransform);


///Returns the buffer with the computed scaled correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the scaled correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow;
///@param pvObject					- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns a pointer to the internal scaled correlation buffer. Do not write into the buffer! \n
///		   The buffer has a NotANumber (NAN = -100000000.0) value for bins where correlation could not be defined because of lack of variance in the original signals.
float* GetScaledCrossCorrelogramBC(void* pvObject);


///Returns the sum of valid Point Biserial coefficients of correlation for each bin of the correlogram
//
///The size of the buffer is (2*iCorrelationWindow+1), and the sum of coefficients for lag 0 is at position iCorrelationWindow.
///Use this function to get the sum of all valid coefficients that were used to compute the average correlation coefficient for each bin of the ScaledCrossCorrelogram: ScaledCrossCorrelogram[i] = PBsCoefficientSums[i] / PBsCoefficientCounts[i].
///@param pvObject					- Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return A pointer to a buffer holding the sum of correlation coefficients for each bin (without dividing them to the counts; the scaled correlogram = the sum of coefficients / the count of coefficients).
///@warning If no valid coefficient was found for a given bin, the buffer containes a value of NAN at that position. Please check for this in the code!
float* GetPBsCoefficientSumsBC(void* pvObject);


///Returns how many Point Biserial coefficients of correlation have been averaged for each bin of the scaled cross correlogram
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return A pointer to the internal buffer containing the counts of Point Biserial correlation coefficients averaged for each bin.
///The size of the buffer is (2*iCorrelationWindow+1), and the count of valid Point Biserial coefficients, that were averaged to compute the scaled correlogram at lag 0, is at position iCorrelationWindow
int* GetPBsCoefficientCountsBC(void* pvObject);


///Returns the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piNumberOfBins - The number of bins in the distribution (output parameter)
///@param pfBinSize - The size of one bin of the distribution (output parameter)
///@return A pointer to the internal buffer that holds the distribution of correlation coefficients
///@return The number of bins in the distribution (piNumberOfBins); this is equal to the size of the buffer
///@return The size of one bin of the distribution (pfBinSize); the size of the bin is in sampling units of the original signals
int* GetDistributionOfCorrelationCoefficientsBC(void* pvObject,int& piNumberOfBins,float& pfBinSize);


///Changes the scale window for the scaled correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piNewScale - The new size of the scale segment
///@return Returns 0 on success, -1 if the scale window is too small; -2 if the new scale window is larger than the trial size
int ScaledCorrelationModifyScaleWindowBC(void* pvObject,int piNewScale);


///Set the size of the correlation window; the parameter specifies the correlation window; for example for a window of -100..+100 pass 100 as a parameter; 
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piNewCorrelationWindow - The new size of the correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial
int ScaledCorrelationModifyCorrelationWindowBC(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the scale window; -2 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyTrialLengthBC(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@param piNewScale - The new size of the scale segment
///@param piNewCorrelationWindow - The new size of the correlation window
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success, -1 if the new scale window is too small; -2 if the new correlation window is too small; -3 if the new scale window is larger than the trial size; -4 if the trial length is smaller than the correlation window
int ScaledCorrelationModifyAllParametersBC(void* pvObject,int piNewScale,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the current scale window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns the size of the scale segment window
int ScaledCorrelationGetScaleWindowBC(void* pvObject);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns the size of the correlation window
int ScaledCorrelationGetCorrelationWindowBC(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns the length of the trial in original sampling units
int ScaledCorrelationGetTrialLengthBC(void* pvObject);


///Get the number of bins of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns the number of bins used to compute the distribution of correlation coefficients; the value equals the size of the internal buffer that stores the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficients)
int GetDistributionOfCorrelationCoefficientsBinNrBC(void* pvObject);


///Get the size of a bin of the distribution of coefficients of correlation
//
///@param pvObject - Pointer to the object that computes the scaled correlation; please instantiate first using the CreateScaledCorrelationComputerBC function
///@return Returns the size of the bins used to compute the distribution of correlation coefficients (see also function: GetDistributionOfCorrelationCoefficients)
float GetDistributionOfCorrelationCoefficientsBinSizeBC(void* pvObject);

//====================================================================================================================================================================================





//Interface for Binary - Binary Cross corr ======================================================================================================================================


///Creates an object that computes cross-correlation on a pair of binary - binary signals
//Constructor parameters are as follows:
///@param piCorrelationWindow	- The size of the cross-correlation window (eg. 80 for a cross-correlation with laggs from -80 to +80); use the same units as the sampling of your samples
///@param piTrialLength			- The size of the trial in units of your samples
///@param piIsError				- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetCrossCorrelogramBB() function will return NULL
///@return An untyped pointer to the created object!
void* CreateCrossCorrelationComputerBB(int piCorrelationWindow,int piTrialLength,int &piIsError);


///Destructor for cross-correlation objects on binary-binary input pairs; always call this to cleanup your data space
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
void FreeCrossCorrelationComputerBB(void* pvObject);


///Compute the cross-correlation of two vectors of time stamps
//Parameters for ComputeCrossCorrelation
///@param	pvObject				- Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@param	piaTimeStampsA			- The vector holding the time stamps of the first binary variable
///@param	piNrTimeStampsInA		- Number of time stamps in the first vector
///@param	piaTimeStampsB			- The vector holding the time stamps of the second binary variable
///@param	piNrTimeStampsInB		- Number of time stamps in the second vector
///@param	pbNormalizeCorrelogram	- Set to 1 to normalize the correlogram; normalization is made by dividing the correlogram to the number of time stamps in the first buffer (rate-normalized correlogram)
void ComputeCrossCorrelationBB(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,int *piaTimeStampsB,int piNrTimeStampsInB,int pbNormalizeCorrelogram);

///Compute the cross-correlation of two vectors of time stamps, on a partial window of the trial. ONLY ACCEPTS ONE TRIAL!!!
//Parameters for ComputeWindowedCrossCorrelationPerTrial
///@param pvObject				- Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@param piaTimeStampsA		 - The vector holding the time stamps of the first binary variable
///@param piNrTimeStampsInA		 - Number of time stamps in the first vector
///@param piaTimeStampsB		 - The vector holding the time stamps of the second binary variable
///@param piNrTimeStampsInB		 - Number of time stamps in the second vector		
///@param piFromOffsetInTrial	 - The start offset in the trial where the desired window starts (time stamp)
///@param piToOffsetInTrial		 - The end offset in the trial where the desired window stops (time stamp)
///@param pbNormalizeCorrelogram - Set to 1 to normalize the correlogram
void ComputeWindowedCrossCorrelationPerTrialBB(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,int *piaTimeStampsB,int piNrTimeStampsInB,int piFromOffsetInTrial,int piToOffsetInTrial,int pbNormalizeCorrelogram);


///Returns the buffer with the computed cross-correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the cross correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow.
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@return Returns a pointer to the internal cross-correlation buffer. Do not write into the buffer! \n
///		   The buffer is filled with NAN (Not A Number) = -100000000.0 if one of the input buffers was empty and thus the correlation was not defined; please check in your code for this!!!!
float* GetCrossCorrelogramBB(void* pvObject);


///Set the size of the correlation window; the parameter specifies the correlation window; for example for a window of -100..+100 pass 100 as a parameter
//
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@param piNewCorrelationWindow - The new correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial
int CrossCorrelationModifyCorrelationWindowBB(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the correlation window
int CrossCorrelationModifyTrialLengthBB(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@param piNewCorrelationWindow - The new correlation window
///@param piNewTrialLength - The new length of the trial 
///@return Returns 0 on success, -1 if the new correlation window is too small; -2 if the trial length is smaller than the correlation window
int CrossCorrelationModifyAllParametersBB(void* pvObject,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@return The size of the correlation window
int CrossCorrelationGetCorrelationWindowBB(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross correlation; please instantiate first using the CreateCrossCorrelationComputerBB function
///@return The length of the trial in sampling units
int CrossCorrelationGetTrialLengthBB(void* pvObject);

//====================================================================================================================================================================================



//Interface for Continuous - Continuous cross correlation ============================================================================================================================
///Creates an object that computes cross-correlation on a pair of continuous - continuous signals
//Constructor parameters are as follows:
///	@param piCorrelationWindow	- The size of the cross-correlation window (eg. 80 for a cross-correlation with laggs from -80 to +80); use the same units as the sampling of your samples
///	@param piTrialLength		- The size of the trial in units of your samples
///	@param piIsError			- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetCrossCorrelogram() function will return NULL
/// @return An untyped pointer to the created object.
void* CreateCrossCorrelationComputerCC(int piCorrelationWindow,int piTrialLength,int &piIsError);


///Destructor for cross-correlation objects on continuous-continuous input pairs; always call this to cleanup your data space
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
void FreeCrossCorrelationComputerCC(void* pvObject);


///Compute the cross-correlation of two vectors of samples
//Parameters for ComputeCrossCorrelation
///If the number of samples passed is larger than the size of one trial, then the signals are divided into trials and the results are eventually averaged over trials
/// @param pvObject					- Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
/// @param pfaSamplesA				- The vector holding the samples of the first signal
/// @param piNrSamplesInA			- Number of samples in the first vector
/// @param pfaSamplesB				- The vector holding the samples of the second signal
/// @param piNrSamplesInB			- Number of samples in the second vector
/// @param	piNormalizationFlag		- Flag that decides how to normalize the correlogram: 0 - unnormalized, biased; 1 - normalized, biased; 2 - unnormalized, unbiased; 3 - normalized, unbiased; normalization removes the dependency on the length of the signals and gives values in cross-product units of the continuous variables; unbias removes the dependency on the finite length of signal windows
void ComputeCrossCorrelationCC(void* pvObject,float *pfaSamplesA,int piNrSamplesInA,float *pfaSamplesB,int piNrSamplesInB,int piNormalizationFlag);


///Compute the cross-correlation of two digitized continuous signals, on a partial window of the trial
//Parameters for ComputeWindowedCrossCorrelationPerTrial
/// @warning ONLY ACCEPTS ONE TRIAL so do not pass signals longer than the size of a single trial! Subsequent trials will be discarded.
/// @param pvObject					- Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
/// @param	pfaSamplesA				- The vector holding the samples of the first signal 
/// @param	piNrSamplesInA			- Number of samples in the first vector
/// @param	pfaSamplesB				- The vector holding the samples of the second signal
/// @param	piNrSamplesInB			- Number of samples in the second vector
/// @param	piFromOffsetInTrial		- The start offset in the trial where the desired window starts
/// @param	piToOffsetInTrial		- The end offset in the trial where the desired window stops
/// @param	piNormalizationFlag		- Flag that decides how to normalize the correlogram: 0 - unnormalized, biased; 1 - normalized, biased; 2 - unnormalized, unbiased; 3 - normalized, unbiased; normalization removes the dependency on the length of the signals and gives values in cross-product units of the continuous variables; unbias removes the dependency on the finite length of signal windows
void ComputeWindowedCrossCorrelationPerTrialCC(void* pvObject,float *pfaSamplesA,int piNrSamplesInA,float *pfaSamplesB,int piNrSamplesInB,int piFromOffsetInTrial,int piToOffsetInTrial,int piNormalizationFlag);
		

///Returns the buffer with the computed cross-correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the cross-correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow;
/// @param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
/// @return Returns a pointer to the internal cross-correlation buffer. Do not write into the buffer! \n
///			The buffer is filled with NAN (Not A Number) = -100000000.0 if one of the input buffers was empty and thus the correlation was not defined; please check in your code for this!!!!
float* GetCrossCorrelogramCC(void* pvObject);


///Set the size of the correlation window; the parameter specifies the correlation window; for example for a window of -100..+100 pass 100 as a parameter
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
///@param piNewCorrelationWindow - Specifies the new correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial;
int CrossCorrelationModifyCorrelationWindowCC(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the correlation window
int CrossCorrelationModifyTrialLengthCC(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
///@param piNewCorrelationWindow - The new correlation window
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success, -1 if the new correlation window is too small; -2 if the trial length is smaller than the correlation window
int CrossCorrelationModifyAllParametersCC(void* pvObject,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
///@return The size of the correlation window
int CrossCorrelationGetCorrelationWindowCC(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerCC function
///@return The trial length in sampling units
int CrossCorrelationGetTrialLengthCC(void* pvObject);

//===================================================================================================================================================================================




//Interface for Binary - Continuous cross correlation ======================================================================================================================================

///Creates an object that computes cross-correlation on a pair of binary - continuous signals
//Constructor parameters are as follows:
/// @param piCorrelationWindow	- The size of the cross-correlation window (eg. 80 for a cross-correlation with lags of -80..+80); use the same units as the sampling units
/// @param piTrialLength		- The size of the trial in original sampling units
/// @param piIsError			- A return parameter, set to 0 on success. It is set to -1 by the constructor if there is a mismatch of window sizes and the GetCrossCorrelogram() function will return NULL
/// @warning The sampling frequency of the continuous and binary signals has to be the same! Also the trial lengths have to match!
/// @return An untyped pointer to the created object.
void* CreateCrossCorrelationComputerBC(int piCorrelationWindow,int piTrialLength,int &piIsError);


///Destructor for cross-correlation objects on binary-continuous input pairs; always call this to cleanup your data space
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
void FreeCrossCorrelationComputerBC(void* pvObject);


///Compute the cross-correlation of one binary signal with one continuos signal
//Parameters for ComputeCrossCorrelation
///  @param pvObject				- Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///	 @param	piaTimeStampsA			- The vector holding the time stamps of the binary variable
///  @param	piNrTimeStampsInA		- Number of time stamps in the first vector
///  @param	pfaSamplesB				- The vector holding samples of the continuous signal
///  @param	piNrSamplesInB			- Number of samples in the second vector
///	 @param	pbNormalizeCorrelogram	- Set to 1 to normalize the correlogram; ; normalization is made by dividing to the number of time stamps used to compute the correlogram
///  @warning The sampling frequency of the continuous and binary signals has to be the same! Also the trial lengths have to match! \n
///			  The time stamps of the binary signal must match the samples of the continuous signal, i.e. the first sample of the continuous signal corresponds to time stamp 0.
void ComputeCrossCorrelationBC(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,float *pfaSamplesB,int piNrSamplesInB,int pbNormalizeCorrelogram);


///Returns the buffer with the computed cross-correlogram; returns NULL if there was an error; please check the buffer for NULL before using it!!
//
///The size of the cross correlogram buffer is (2*iCorrelationWindow+1), and element with lag 0 is at position iCorrelationWindow.
/// @param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
/// @return Returns a pointer to the internal cross-correlation buffer. Do not write into the buffer! \n
///			The buffer is filled with NAN (Not A Number) = -100000000.0 if one of the input buffers was empty and thus the correlation was not defined; please check in your code for this!!!!
float* GetCrossCorrelogramBC(void* pvObject);


///Set the size of the correlation window; for example for a window of -100..+100 pass 100 as a parameter 
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///@param piNewCorrelationWindow - Specifies the new correlation window
///@return Returns 0 on success, -1 if the window is too small; -2 if the window is larger than the length of the trial;
int CrossCorrelationModifyCorrelationWindowBC(void* pvObject,int piNewCorrelationWindow);


///Set the length of the trial in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///@param piNewTrialLength - The new length of the trial
///@return Returns 0 on success and -1 if the trial length is smaller than the correlation window
int CrossCorrelationModifyTrialLengthBC(void* pvObject,int piNewTrialLength);


///Sets all parameters at once
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///@param piNewCorrelationWindow - The new correlation window
///@param piNewTrialLength - The new length of the trial 
///@return Returns 0 on success, -1 if the new correlation window is too small; -2 if the trial length is smaller than the correlation window
int CrossCorrelationModifyAllParametersBC(void* pvObject,int piNewCorrelationWindow,int piNewTrialLength);


///Get the size of the correlation window
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///@return The size of the correlation window
int CrossCorrelationGetCorrelationWindowBC(void* pvObject);


///Get the trial length in original sampling units
//
///@param pvObject - Pointer to the object that computes the cross-correlation; please instantiate first using the CreateCrossCorrelationComputerBC function
///@return The length of the trial in sampling units
int CrossCorrelationGetTrialLengthBC(void* pvObject);


#endif

//===================================================================================================================================================================================

//Doxygen help comments
/**
* \mainpage
* \section introduction Introduction
* Correlation Library is a C++ library implemented by Raul C. Muresan. The code is free for non-commercial purposes. \n
* For details on the Scaled Correlation algorithm see paper on "Scaled Correlation Analysis" by Danko Nikolic, Raul C. Muresan, Weijia Feng, and Wolf Singer.
* \n
* \section description Description
* The Correlation Library offers support for computing cross and scaled correlation between a pair of continuous valued, binary valued, or mixed signals. \n
* For each method of computing the correlation and each combination of signals, there is one specialized class that can handle the respective operations.\n
* The library compiles into a Windows DLL that exports a number of functions as wrappers over the original classes.
* \section classe C++ classes
* The C++ classes are not explicitly described here. We only provide a short description for the user to understand the structure of the library.\n
* The class functionalities are exported by the means of wrapper functions that are described in section "Dll functions exported".
* \subsection naming Naming conventions
* Each class is named as: C_descriptor_sufix, where
* - descriptor - stands for CrossCorrelation or ScaledCorrelation\n
* - sufix - denotes the combination type of data that the class accepts: B = Binary valued discrete signal (represented as a vector of time stamps corresponding to events with value "1"), C = Continuous valued discrete signal (represented as a vector of samples) => CC = Continuous-Continuos; BC = Binary-Continuous; BB = Binary-Binary.
* \subsection classlist List of classes
* - CCrossCorrelationCC:	A class that computes the cross-correlation for two continuous valued signals (Continuous - Continuous)
* - CCrossCorrelationBC:	A class that computes the cross-correlation for a vector of time stamps corresponding to events in a binary signal and a continuous valued signal (Binary - Continuous)
* - CCrossCorrelationBB:	A class that computes the cross-correlation for two vectors of time stamps corresponding to events in binary signals (Binary - Binary)
* - CScaledCorrelationCC:	A class that computes the scaled correlation for two continuous valued signals (Continuous - Continuous)
* - CScaledCorrelationBC:	A class that computes the scaled correlation for a vector of time stamps corresponding to events in a binary signal and a continuous valued signal (Binary - Continuous)
* - CScaledCorrelationBB:	A class that computes the scaled correlation for two vectors of time stamps corresponding to events in binary signals (Binary - Binary)
* \subsection input Input data formatting
* All classes respect the same standard for accepting the input data. For a binary input, a vector of time stamps is expected as input while for a continuous valued input, a digitized vector of samples is expected as input. \n\n
* When multiple measurements (trials) are available for the data, the classes can handle multiple trials. The length of the trial, in original sampling units, has to be specified in the constructor.
* Next, when the input signals are provided in the method call to ComputeCrossCorrelation or ComputeScaledCorrelation, the method determines how many trials are available, computes the correlogram on each trial and finally averages the correlograms into one single correlogram.\n\n
* When multiple trials are available, the input should be structured as follows:\n\n
* |.........Trial 1...........||.........Trial 2...........||.........Trial 3...........||.........Trial 4...........||.........Trial 5...........|\n\n
* - For continuous valued signals, there must be as many samples in each trial as the previously specified trial length. In any case, the method only considers as many samples as specified in the call to ComputeCrossCorrelation or ComputeScaledCorrelation.\n\n
* - For binary signals, the number of available trials is decided by taking into account the time stamp with the largest value. The method then searches, for each trial, the presence of valid time stamps. Time stamps for consecutive trials have to have increasing values. The trial to which a time stamp belongs is decided by taking into account the length of a trial in sampling units.\n\n
*
* If the user cannot structure the trials as a contiguous series of datapoints, then the best option is to pass a single trial to the class, get the correlogram for each trial, and then the user should compute the average correlogram.
* \note For the case of scaled correlation, the user can use the sum of coefficients of correlation for each bin of the correlogram and the count of coefficients to efficiently compute the average, smooth, scaled correlogram for all trials (see section Example). Averaging the sums of correlation coefficients gives a better and smoother estimate of the final scaled correlogram than averaging the individual scaled correlograms for each trial (see Example: Scaled correlogram example with only one trial passed at a time).\n
* \subsection methodcall Method calls
* For the two types of correlations the classes expose an interface to compute the CrossCorrelation and the ScaledCorrelation respectively. Depending on the type of the input data, the methods accept different parameter types. However, they have a general format:\n
* - ComputeCrossCorrelation(VectorA, SizeOfVectorA, VectorB, SizeOfVectorB, pbNormalizeCorrelogram)\n
* - ComputeScaledCorrelation(VectorA, SizeOfVectorA, VectorB, SizeOfVectorB, pbUseFisherZTransform)\n
*\n
* The first four parameters specify the input vectors and their sizes (in number of elements). Each vector has a different type depending if the input signal is binary or continuous.\n
* For binary data, the vectors are arrays of integers holding the time stamps. For continuous valued data, the vectors are arrays of floats, holding the samples.\n
* The last parameter of each function model is a boolean. pbNormalizeCorrelogram - determines whether the cross-correlogram should be normalized. pbUseFisherZTransform - determines if the scaled correlogram should use the Fisher Z Transform to average the coefficients of correlation.\n
* 
* \subsection output Output data
* Each class exposes an interface that allows access to the computed cross or scaled correlogram. The correlogram is stored in the object as a vector of floats. The vector has a size of 2*CorrelationWindow+1.\n
* The correlogram vector is stored such that the correlation at lag -CorrelationWindow is at position 0, the correlation at lag 0 is stored at position CorrelationWindow, and the correlation at lag +CorrelationWindow is stored at position 2*CorrelationWindow.\n\n
* To retrieve the correlogram, call:
* - GetCrossCorrelogram() - retrieves a pointer to the vector holding the cross-correlogram \n
* - GetScaledCrossCorrelogram() - retrieves a pointer to the vector holding the scaled correlogram \n
* The user should not write into the vectors! \n
*
* \attention Very important: the cross and scaled correlation buffers might contain values of Not A Number (NAN = -100000000.0f) if there was no variance in the input signals and correlation was, as a consequence, not defined! Always check the output buffers for NAN values before using them! Otherwise results may be compromised.
* \section dllfunctions Dll functions exported
* The correlation library provides flexible means to interact with the classes that compute correlation.
* It exports a set of wrapper functions that can be used by an external caller to access the full class functionality.
* \subsection interfacingconvention Interfacing convention
* To interface a given class method, the library exports a function with a similar name that has an additional parameter, which is an untyped pointer to an already created object of that class.\n \n
* For example, let ClassA be the class of interest, and Method1 be a method of ClassA. To create an interface to ClassA functionality, one must first create an object of that class and then call a wrapper function to access the class method.\n\n
* \code
* void* Wrapper_ObjectConstructor_ClassA()
* {
*	return new ClassA();
* }
*
* void Wrapper_ObjectDestructor_ClassA(void* pObj)
* {
*	delete ((ClassA*) pObj);
* }
*
* void Wrapper_Method1_Of_ClassA(void* pObj,int iSomeParam)
* {
*	((ClassA*) pObj)->Method1(iSomeParam);
* }	
*
* void main()
* {
*	void *pObject;
*	pObject = Wrapper_ObjectConstructor_ClassA();	//A wrapper over the constructor of class A
*	Wrapper_Method1_Of_ClassA(pObject,1);		//A wrapper over Method1 of class A
*	Wrapper_ObjectDestructor_ClassA(pObject);	//A wrapper over the destructor of class A
* }
* \endcode
* \note The wrapper constructors create untyped pointers to objects. The DLL caller can use these pointers as handles for the objects of a given class. All class operations have, as an additional parameter, the pointer to the object being handled. Depending on the wrapper function that is called, a corresponding method will be called from a corresponding class of the object whose handle is passed to the wrapper function.
* \subsection naming Naming conventions
* Each function is named as: prefix_name_sufix, where
* - prefix - is optional and is usually added to avoid confusion, when a function with the same name is present in both Cross-Correlation and Scaled-Correlation classes. The prefix could be either CrossCorrelation or ScaledCorrelation
* - name - is usually the name of the class method that is being interfaced \n
* - sufix - denotes the combination type of data that the class accepts: D = Binary, C = Continuous => CC = Continuous-Continuos; BC = Binary-Continuous; BB = Binary-Binary.
* \subsection wrapperfunctionlist List of wrapper functions
*
* Cross-correlation for Continuous-Continuous data:
* - void* 	CreateCrossCorrelationComputerCC(int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void 	FreeCrossCorrelationComputerCC(void *pvObject)
* - void 	ComputeCrossCorrelationCC(void *pvObject, float *pfaSamplesA, int piNrSamplesInA, float *pfaSamplesB, int piNrSamplesInB, int pbNormalizeCorrelogram)
* - void 	ComputeWindowedCrossCorrelationPerTrialCC(void *pvObject, float *pfaSamplesA, int piNrSamplesInA, float *pfaSamplesB, int piNrSamplesInB, int piFromOffsetInTrial, int piToOffsetInTrial, int pbNormalizeCorrelogram)
* - float* 	GetCrossCorrelogramCC(void *pvObject)
* - int 	CrossCorrelationModifyCorrelationWindowCC(void *pvObject, int piNewCorrelationWindow)
* - int 	CrossCorrelationModifyTrialLengthCC(void *pvObject, int piNewTrialLength)
* - int 	CrossCorrelationModifyAllParametersCC(void *pvObject, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	CrossCorrelationGetCorrelationWindowCC(void *pvObject)
* - int 	CrossCorrelationGetTrialLengthCC(void *pvObject)
*
* Cross-correlation for Binary-Continuous data:
* - void* 	CreateCrossCorrelationComputerBC(int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void 	FreeCrossCorrelationComputerBC(void *pvObject)
* - void 	ComputeCrossCorrelationBC(void *pvObject, int *piaTimeStampsA, int piNrTimeStampsInA, float *pfaSamplesB, int piNrSamplesInB, int pbNormalizeCorrelogram)
* - float* 	GetCrossCorrelogramBC(void *pvObject)
* - int 	CrossCorrelationModifyCorrelationWindowBC(void *pvObject, int piNewCorrelationWindow)
* - int 	CrossCorrelationModifyTrialLengthBC(void *pvObject, int piNewTrialLength)
* - int 	CrossCorrelationModifyAllParametersBC(void *pvObject, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	CrossCorrelationGetCorrelationWindowBC(void *pvObject)
* - int 	CrossCorrelationGetTrialLengthBC(void *pvObject)
*
* Cross-correlation for Binary-Binary data:
* - void* 	CreateCrossCorrelationComputerBB(int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void 	FreeCrossCorrelationComputerBB(void *pvObject)
* - void 	ComputeCrossCorrelationBB(void *pvObject, int *piaTimeStampsA, int piNrTimeStampsInA, int *piaTimeStampsB, int piNrTimeStampsInB, int pbNormalizeCorrelogram)
* - void	ComputeWindowedCrossCorrelationPerTrialBB(void* pvObject,int *piaTimeStampsA,int piNrTimeStampsInA,int *piaTimeStampsB,int piNrTimeStampsInB,int piFromOffsetInTrial,int piToOffsetInTrial,int pbNormalizeCorrelogram);
* - float* 	GetCrossCorrelogramBB(void *pvObject)
* - int 	CrossCorrelationModifyCorrelationWindowBB(void *pvObject, int piNewCorrelationWindow)
* - int 	CrossCorrelationModifyTrialLengthBB(void *pvObject, int piNewTrialLength)
* - int 	CrossCorrelationModifyAllParametersBB(void *pvObject, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	CrossCorrelationGetCorrelationWindowBB(void *pvObject)
* - int 	CrossCorrelationGetTrialLengthBB(void *pvObject)
*
* Scaled correlation for Continuous-Continuous data:
* - void* 	CreateScaledCorrelationComputerCC(int piScaleWindow, int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void 	FreeScaledCorrelationComputerCC(void *pvObject)
* - void 	ComputeScaledCorrelationCC(void *pvObject, float *pfaSamplesA, int piNrSamplesInA, float *pfaSamplesB, int piNrSamplesInB, int pbUseFisherZTransform)
* - void 	ComputeWindowedScaledCorrelationPerTrialCC(void *pvObject, float *pfaSamplesA, int piNrSamplesInA, float *pfaSamplesB, int piNrSamplesInB, int piFromOffsetInTrial, int piToOffsetInTrial, int pbUseFisherZTransform)
* - float* GetScaledCrossCorrelogramCC(void *pvObject)
* - float* GetPearsonCoefficientSumsCC(void *pvObject)
* - int* 	GetPearsonCoefficientCountsCC(void *pvObject)
* - int* 	GetDistributionOfCorrelationCoefficientsCC(void *pvObject, int &piNumberOfBins, float &pfBinSize)
* - int 	ScaledCorrelationModifyScaleWindowCC(void *pvObject, int piNewScale)
* - int 	ScaledCorrelationModifyCorrelationWindowCC(void *pvObject, int piNewCorrelationWindow)
* - int 	ScaledCorrelationModifyTrialLengthCC(void *pvObject, int piNewTrialLength)
* - int 	ScaledCorrelationModifyAllParametersCC(void *pvObject, int piNewScale, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	ScaledCorrelationGetScaleWindowCC(void *pvObject)
* - int 	ScaledCorrelationGetCorrelationWindowCC(void *pvObject)
* - int 	ScaledCorrelationGetTrialLengthCC(void *pvObject)
* - int 	GetDistributionOfCorrelationCoefficientsBinNrCC(void *pvObject)
* - float 	GetDistributionOfCorrelationCoefficientsBinSizeCC(void *pvObject)
*
* Scaled correlation for Binary-Continuous data:
* - void* 	CreateScaledCorrelationComputerBC(int piScaleWindow, int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void 	FreeScaledCorrelationComputerBC(void *pvObject)
* - void 	ComputeScaledCorrelationBC(void *pvObject, int *piaTimeStampsA, int piNrTimeStampsInA, float *pfaSamplesB, int piNrSamplesInB, int pbUseFisherZTransform)
* - float* 	GetScaledCrossCorrelogramBC(void *pvObject)
* - float* 	GetPBsCoefficientSumsBC(void *pvObject)
* - int* 	GetPBsCoefficientCountsBC(void *pvObject)
* - int* 	GetDistributionOfCorrelationCoefficientsBC(void *pvObject, int &piNumberOfBins, float &pfBinSize)
* - int		ScaledCorrelationModifyScaleWindowBC(void *pvObject, int piNewScale)
* - int 	ScaledCorrelationModifyCorrelationWindowBC(void *pvObject, int piNewCorrelationWindow)
* - int 	ScaledCorrelationModifyTrialLengthBC(void *pvObject, int piNewTrialLength)
* - int 	ScaledCorrelationModifyAllParametersBC(void *pvObject, int piNewScale, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	ScaledCorrelationGetScaleWindowBC(void *pvObject)
* - int 	ScaledCorrelationGetCorrelationWindowBC(void *pvObject)
* - int 	ScaledCorrelationGetTrialLengthBC(void *pvObject)
* - int 	GetDistributionOfCorrelationCoefficientsBinNrBC(void *pvObject)
* - float 	GetDistributionOfCorrelationCoefficientsBinSizeBC(void *pvObject)
*
* Scaled correlation for Binary-Binary data:
* - void* CreateScaledCorrelationComputerBB(int piScaleWindow, int piCorrelationWindow, int piTrialLength, int &piIsError)
* - void FreeScaledCorrelationComputerBB(void *pvObject)
* - void ComputeScaledCorrelationBB(void *pvObject, int *piaTimeStampsA, int piNrTimeStampsInA, int *piaTimeStampsB, int piNrTimeStampsInB, int pbUseFisherZTransform)
* - void ComputeWindowedScaledCorrelationPerTrialBB(void *pvObject, int *piaTimeStampsA, int piNrTimeStampsInA, int *piaTimeStampsB, int piNrTimeStampsInB, int piFromOffsetInTrial, int piToOffsetInTrial, int pbUseFisherZTransform)
* - float* GetScaledCrossCorrelogramBB(void *pvObject)
* - float* GetFiCoefficientSumsBB(void *pvObject)
* - int* GetFiCoefficientCountsBB(void *pvObject)
* - int* 	GetDistributionOfCorrelationCoefficientsBB(void *pvObject, int &piNumberOfBins, float &pfBinSize)
* - int 	ScaledCorrelationModifyScaleWindowBB(void *pvObject, int piNewScale)
* - int 	ScaledCorrelationModifyCorrelationWindowBB(void *pvObject, int piNewCorrelationWindow)
* - int 	ScaledCorrelationModifyTrialLengthBB(void *pvObject, int piNewTrialLength)
* - int 	ScaledCorrelationModifyAllParametersBB(void *pvObject, int piNewScale, int piNewCorrelationWindow, int piNewTrialLength)
* - int 	ScaledCorrelationGetScaleWindowBB(void *pvObject)
* - int 	ScaledCorrelationGetCorrelationWindowBB(void *pvObject)
* - int 	ScaledCorrelationGetTrialLengthBB(void *pvObject)
* - int 	GetDistributionOfCorrelationCoefficientsBinNrBB(void *pvObject)
* - float 	GetDistributionOfCorrelationCoefficientsBinSizeBB(void *pvObject)
*
* \subsection example Examples of usage
* Let us consider we have two continuous valued signals that we want to correlate. The signals, we suppose, have been sampled at 1 kHz. Each signal has 2 trials recorded, with a length of 1000 ms each. We want to compute the cross-correlation and the scaled correlation.\n\n
* \code
* //Cross correlogram example:
* #include "CorrelationLib.h"
* #include "Constants.h"
* float SignalA[2000];
* float SignalB[2000];
* float* CrossCorrelogram;
* int iTrialLength = 1000;		//the legth of the trial is 1000 so we have signal vectors holding 2 trials (2000 entries)
* int iCorrelationWindow = 100;		//-100..+100 correlation lags
* 
* //Here you should fill the SignalA and SignalB arrays with values
*
* //And then compute the correlogram
* int isError = 0;
*
* void* ClassicCC_Computer = NULL;
* ClassicCC_Computer = CreateCrossCorrelationComputerCC(iCorrelationWindow,iTrialLength,isError);
* if(isError == 0)
* {
*	ComputeCrossCorrelationCC(ClassicCC_Computer,SignalA,2000,SignalB,2000,0);
*	CrossCorrelogram = GetCrossCorrelogramCC(ClassicCC_Computer);
*	for(int j=0;j<2*iCorrelationWindow+1;j++)
*	{
*		if(CrossCorrelogram[j] != NAN)
*		{
*			//use the CrossCorrelogram only if the value is not NAN (only if the correlation was defined)
*		}
*	}
* }
* if(ClassicCC_Computer != NULL) FreeCrossCorrelationComputerCC(ClassicCC_Computer);
* \endcode
* \n\n
* \code
* //Scaled correlogram example:
* #include "CorrelationLib.h"
* #include "Constants.h"
* float SignalA[2000];
* float SignalB[2000];
* float* ScaledCorrelogram;
* int iTrialLength = 1000;		//the legth of the trial is 1000 so we have signal vectors holding 2 trials (2000 entries)
* int iCorrelationWindow = 100;		//-100..+100 correlation lags
* int iScaleWindow = 25;			//25 ms - if the sampling frequency of the signals is 1 kHz => we are looking to components faster than 40 Hz
* 
* //Here you should fill the SignalA and SignalB arrays with values
*
* //And then compute the correlogram
* int isError = 0;
* void* ScaledCC_Computer = NULL;
* ScaledCC_Computer = CreateScaledCorrelationComputerCC(iScaleWindow,iCorrelationWindow,iTrialLength,isError);
* if(isError == 0)
* {
*	ComputeScaledCorrelationCC(ScaledCC_Computer,SignalA,2000,SignalB,2000,0);
*	ScaledCorrelogram = GetScaledCrossCorrelogramCC(ScaledCC_Computer);
*	for(int j=0;j<2*iCorrelationWindow+1;j++)
*	{
*		if(ScaledCorrelogram[j] != NAN)
*		{
*			//use the ScaledCorrelogram only if the value is not NAN (only if the correlation was defined)
*		}
*	}
* }
* if(ScaledCC_Computer != NULL) FreeScaledCorrelationComputerCC(ScaledCC_Computer);
* \endcode
* \n\n
* Let us now assume we have again two signals each with two trials. This time however, we want to compute the correlogram manually by computing the sum of correlation coefficients for each trial and then computing the average in the end.\n\n
* \code
* //Scaled correlogram example with only one trial passed at a time:
* //This is useful when the user wants to compute the correlogram per trial and in the end average the correlation coefficients himself.
* //Averaging the sums of correlation coefficients gives a better and smoother estimate of the final scaled correlogram than averaging the individual scaled correlograms for each trial!!!
* #include "CorrelationLib.h"
* #include "Constants.h"
* float SignalA[2000];
* float SignalB[2000];
* float ScaledCorrelogram[201];
* int   SummedCoefficientsCount[201];
* float *coeff_sum_buffer;
* int *coeff_count_buffer;
* int iTrialLength = 1000;		//the legth of the trial is 1000 so we have signal vectors holding 2 trials (2000 entries)
* int iCorrelationWindow = 100;		//-100..+100 correlation lags
* int iScaleWindow = 25;			//25 ms - if the sampling frequency of the signals is 1 kHz => we are looking to components faster than 40 Hz
* int iTrialNr = 2;			//Two trials
* 
* //Here you should fill the SignalA and SignalB arrays with values
*
* //And then compute the correlogram
* int isError = 0;
* void* ScaledCC_Computer;
* ScaledCC_Computer = CreateScaledCorrelationComputerCC(iScaleWindow,iCorrelationWindow,iTrialLength,isError);
* 
* if(isError == 0)
* {
*	//Cleanup the final buffer and the counting buffer
*	for(int j=0;j<2*iCorrelationWindow+1;j++)
*	{
*		ScaledCorrelogram[j] = 0;
*		SummedCoefficientsCount[j] = 0;
*	}
*
*	//For each trial, compute the sum of correlation coefficients
*	for(int i=0;i<iTrialNr;i++)
*	{
*		ComputeScaledCorrelationCC(ScaledCC_Computer,&(SignalA[i*iTrialLength]),iTrialLength,&(SignalB[i*iTrialLength]),iTrialLength,0);
*		coeff_sum_buffer = GetPearsonCoefficientSumsCC(ScaledCC_Computer);
*		coeff_count_buffer = GetPearsonCoefficientCountsCC(ScaledCC_Computer);
*		for(j=0;j<2*iCorrelationWindow+1;j++)
*		{
*			if(coeff_sum_buffer[j] != NAN) ScaledCorrelogram[j] += coeff_sum_buffer[j];
*			SummedCoefficientsCount[j] += coeff_count_buffer[j];
*		}		
*	}
*
*	//Now compute the final scaled correlogram by computing the average
*	for(j=0;j<2*iCorrelationWindow+1;j++) if(SummedCoefficientsCount[j]) ScaledCorrelogram[j] /= SummedCoefficientsCount[j];
* }
* if(ScaledCC_Computer != NULL) FreeScaledCorrelationComputerCC(ScaledCC_Computer);
* \endcode
*
*/



