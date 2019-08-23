/*
 * RtspCapture.hpp
 *
 *  Created on: Mar 1, 2019
 *      Author: wj
 */

#ifndef RTSPCAPTURE_HPP_
#define RTSPCAPTURE_HPP_
#include"Capture.hpp"
#include <vector>
#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/gstinfo.h>
#include <gst/app/gstappsink.h>
#include "osa_thr.h"
#include "osa_sem.h"

typedef struct {
	bool bTrack;
	bool bMtd;
	bool bBlobDetect;
	int chId;
	int iTrackStat;

	//Mat frame;
}MAIN_RtspCaptureThrObj_cxt;

typedef struct {
	MAIN_RtspCaptureThrObj_cxt cxt[2];
	OSA_ThrHndl		thrHandleProc;
	OSA_ThrHndl		thrHandleProcdouble;
	OSA_SemHndl	procNotifySem;
	int pp;
	bool bFirst;
	volatile bool	exitProcThread;
	bool						initFlag;
	void 						*pParent;
}MAIN_RtspCaptureThrObj;
class Queue;

class RtspCapture:public Capture
{
public :
	RtspCapture();
	~RtspCapture();
	virtual void init(std::string devname,int width=1920,int height=1080,CaptureFrameCallback callback=NULL);
	virtual void uninit();
private:
	void inittask();
	void uninittask();
	void initgstreamerrtsp();
	void uninitgstreamerrtsp();
	static void appsink_eos(GstAppSink * appsink, gpointer user_data);
	static GstFlowReturn new_buffer(GstAppSink *appsink, gpointer user_data);
	void main_Recv_funcdata();
	int MAIN_threadRecvCreate(void);
	int MAIN_threadRecvDestroy(void);
	void NV212BGR( unsigned char *imgY, unsigned char *imgDst,int width, int height );
	static void *mainRecvTsk(void *context)
		{
			MAIN_RtspCaptureThrObj  * pObj= (MAIN_RtspCaptureThrObj*) context;
			if(pObj==NULL)
				{
					;
					return NULL;
				}
			RtspCapture *ctxHdl = (RtspCapture *) pObj->pParent;
			ctxHdl->main_Recv_funcdata();

			return NULL;
		}
private:
	GstPipeline *gst_pipeline_ = NULL;
	GMainLoop *main_loop=NULL;
	CaptureFrameCallback Callback_;
	MAIN_RtspCaptureThrObj	MainRtspThrObj;
	static Queue *imagequeue_;
	int width_;
	int height_;
	unsigned char *rgbdata;
	std::string rtspname_;

};



#endif /* RTSPCAPTURE_HPP_ */
