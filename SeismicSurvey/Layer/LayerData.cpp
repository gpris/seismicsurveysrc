#include "StdAfx.h"
#include "LayerData.h"


CLayerData::CLayerData(void)
{
	m_TotalTraceNumber = 0;
	for (int i = 0; i<10; i++)
	{
		m_ColumnHeaders[i] = _T("");
	}
}

#include <cmath>

void CLayerData::CalculateStatistic()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	float sum[10];
	float min[10];
	float max[10];
	for (int i=0;i<10;i++){
		sum[i]=0.0;
		min[i]=1e8;
		max[i]=-1e8;
	}
	for (int row = 0; row < m_TotalTraceNumber; row++) // Get Max, Min
	{
		CLayerCellOfTwoLayers * pCell;
		pCell = m_LayerCells.GetAt(row);
		for(int k=0;k<10;k++)
		{
			sum[k]+=pCell->m_Data[k] ;
			if(min[k] >(pCell->m_Data[k])){
				min[k]=pCell->m_Data[k];
			}

			if(max[k]<(pCell->m_Data[k])){
				max[k]=pCell->m_Data[k];
			}
		}			
	}
	for (int k3=0;k3<10;k3++)  // Copy Max, Min
	{
		m_max[k3]=max[k3];
		m_min[k3]=min[k3];
	}

	for (int j2=4;j2 <10;j2++){  // Copy Max, Min
		m_avg[j2]=sum[j2]/(float)m_TotalTraceNumber;
	}
	for (int i2=0;i2<10;i2++){ // initialize sum
				sum[i2]=0.0;
	}
	for (int k=0;k<m_TotalTraceNumber;k++)  // initialize sum
	{
		CLayerCellOfTwoLayers * pCell;
		pCell = m_LayerCells.GetAt(k);
		for (int l2=4; l2<10;l2++)
		{	  			
		sum[l2]+=((pCell->m_Data[l2])-m_avg[l2])*((pCell->m_Data[l2])-m_avg[l2]);
		}			
	}
	for (int k=4;k<10;k++) 
	{
		m_std[k]=sqrt(sum[k]/m_TotalTraceNumber);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//#include "libxl.h"
//using namespace libxl;
//
//void CLayerData::LoadfromXlsxLibXL(CString filename)
//{
//	Book* book = xlCreateXMLBook();
//	book->setKey("Sang-Wook Kim", "windows-2d2b2e0502cce7096bbe6069a3hfedl2");
//
//    if(book->load(filename))
//    {
//        Sheet* sheet = book->getSheet(0);
//        if(sheet)
//        {
//			//To access data use this:
//			for (int col = 0; col<10; col++)
//			{
//				m_ColumnHeaders[col].Format("%s",sheet->readStr(0, col));
//			}
//
//			CString tempstring = _T("");
//			BOOL onecellcompleted = false;
//
//			for(;;)
//			{
//				CLayerCellOfTwoLayers * tempcell;
//				tempcell = new CLayerCellOfTwoLayers;
//
//				for (int col = 0; col<10; col++)
//				{
//					CellType currentcelltype = sheet->cellType(m_TotalTraceNumber + 1, col);
//					if(currentcelltype ==  CELLTYPE_EMPTY || currentcelltype == CELLTYPE_BLANK)
//					{
//						onecellcompleted = false;
//						break;
//					}
//					else
//					{
//						tempcell->m_Data[col] = sheet->readNum(m_TotalTraceNumber + 1, col);
//						onecellcompleted = true;
//					}
//				}
//				if(onecellcompleted)
//				{
//					m_TotalTraceNumber ++;
//					m_LayerCells.Add(tempcell);
//					TRACE("m_TotalTraceNumber = %d\n", m_TotalTraceNumber);
//				}
//				else
//				{
//					break;
//				}
//			}
//        }
//        book->release();		
//		CalculateStatistic();
//    } 
//}

//void CLayerData::SavetoXlsxLibXL(CString filename)
//{
//	Book* book = xlCreateXMLBook();
//	book->setKey("Sang-Wook Kim", "windows-2d2b2e0502cce7096bbe6069a3hfedl2");
//
//    if(book)
//    {
//        Sheet* sheet = book->addSheet(_T("Layer"));
//        if(sheet)
//        {
//			//To access data use this:
//			for (int col = 0; col<10; col++)
//			{
//				sheet->writeStr(0, col, m_ColumnHeaders[col]);
//			}
//
//
//			for(int row = 0;row<m_TotalTraceNumber;row++)
//			{
//				CLayerCellOfTwoLayers * tempcell;
//				tempcell = m_LayerCells[row];
//
//				for (int col = 0; col<10; col++)
//				{
//						sheet->writeNum(row + 1, col, tempcell->m_Data[col]);
//				}
//			}
//        }
//		book->save(_T(filename));
//    } 
//}