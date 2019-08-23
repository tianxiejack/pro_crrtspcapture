/*
 * Capture.cpp
 *
 *  Created on: 2019年8月22日
 *      Author: alex
 */


#include "RtspCapture.hpp"

Capture *RTSPCapture_Create()
{
	Capture	*pCapObj = NULL;
	RtspCapture *pTmpCap = NULL;

	pTmpCap= (RtspCapture*)new RtspCapture;
	pCapObj = (Capture*)pTmpCap;
	CV_Assert(pCapObj != NULL);

	return (Capture*)pCapObj;
}

void RTSPCapture_Create(Capture *obj)
{
	RtspCapture	*pCapObj = (RtspCapture*)obj;
	if(pCapObj != NULL){
		delete pCapObj;
		obj = NULL;
	}
}
