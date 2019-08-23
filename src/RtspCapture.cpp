#include"RtspCapture.hpp"
#include "Queuebuffer.hpp"
#include "osa_buf_cr.h"
using namespace cv;
using namespace std;
Queue *RtspCapture::imagequeue_=NULL;

RtspCapture::RtspCapture():Callback_(NULL)
{

}
RtspCapture::~RtspCapture()
{
	uninit();
}
void RtspCapture::init(std::string devname,int width,int height,CaptureFrameCallback callback)
{
	width_=width;
	height_=height;
	Callback_=callback;
	rtspname_=devname;
	inittask();
	initgstreamerrtsp();
	MAIN_threadRecvCreate();
}
void RtspCapture::uninit()
{
	MAIN_threadRecvDestroy();
	uninittask();
	uninitgstreamerrtsp();
}
void RtspCapture::inittask()
{
	imagequeue_=new Queue(width_,height_);
	rgbdata=(unsigned char *)malloc(width_*height_*3);

}
void RtspCapture::uninittask()
{
	if(imagequeue_!=NULL)
	{
		free(imagequeue_);
		imagequeue_=NULL;
	}
	if(rgbdata!=NULL)
	{
		free(rgbdata);
		rgbdata=NULL;
	}



}
void RtspCapture::uninitgstreamerrtsp()
{
	gst_element_set_state((GstElement*)gst_pipeline_, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(gst_pipeline_));
	g_main_loop_unref(main_loop);
}
void RtspCapture::initgstreamerrtsp()
{

	gst_init (NULL, NULL);
    main_loop = g_main_loop_new (NULL, FALSE);
    ostringstream launch_stream;
    int w = 1280;
    int h = 1024;
    string launch_string;

    string latency_string(" latency=0 !");
    string rtspbegin_string("rtspsrc location=");
    rtspname_=rtspbegin_string+rtspname_+latency_string;
    //rtspdata=(unsigned char *)malloc(w*h*3);
    //rtspdatargb=(unsigned char *)malloc(w*h*3);
    GstAppSinkCallbacks callbacks = {appsink_eos, NULL,new_buffer};
    launch_stream
	//<< "nvcamerasrc ! "
	//<< "video/x-raw(memory:NVMM), width="<< w <<", height="<< h <<", framerate=30/1 ! "
    //<<"rtspsrc location=rtsp://admin:admin$2018@192.168.0.64:554/h264/ch0/main/av_stream latency=0 !"
    <<rtspname_
   // << "rtspsrc location=rtsp://192.168.1.188:8554/test.264 latency=0 ! "
    //<< "filesrc 1.avi ! decodebin ! "
    << "decodebin ! "
    << "nvvidconv ! "
   // <<"video/x-raw, format=(string)RGB !"
    << "video/x-raw, format=NV12, width="<< width_ <<", height="<< height_ <<" ! "
  // <<"video/x-raw, format=NV12 !"
   // <<" videoconvert  ! "

   // <<"video/x-raw, format=(string)NV12 !"
    << "appsink name=mysink ";

    launch_string = launch_stream.str();

    g_print("Using launch string: %s\n", launch_string.c_str());

    GError *error = NULL;
    printf("***************%d*************\n",__LINE__);
    gst_pipeline_  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);
    printf("***************%d*************\n",__LINE__);
    if (gst_pipeline_ == NULL) {
        g_print( "Failed to parse launch: %s\n", error->message);
        return ;
    }
    if(error) g_error_free(error);

    GstElement *appsink_ = gst_bin_get_by_name(GST_BIN(gst_pipeline_), "mysink");
    gst_app_sink_set_callbacks (GST_APP_SINK(appsink_), &callbacks, NULL, NULL);

    gst_element_set_state((GstElement*)gst_pipeline_, GST_STATE_PLAYING);


    //sleep(90);
    //g_main_loop_run (main_loop);
/*
    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);
*/
    //g_print("going to exit, decode %d frames in %d seconds \n", frame_count, sleep_count);


}
void RtspCapture::appsink_eos(GstAppSink * appsink, gpointer user_data)
{
    printf("app sink receive eos\n");
   // eos = 1;
//    g_main_loop_quit (hpipe->loop);
}
GstFlowReturn RtspCapture::new_buffer(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = NULL;
    cv::Mat show;
	cv::Mat showrgb;
    g_signal_emit_by_name (appsink, "pull-sample", &sample,NULL);
	//VideoLoadData loaddata;
    OSA_buf::OSA_BufInfo *bufinfo=NULL;
    if (sample)
    {
        GstBuffer *buffer = NULL;
        GstCaps   *caps   = NULL;
        GstMapInfo map    = {0};

        caps = gst_sample_get_caps (sample);
        if (!caps)
        {
            printf("could not get snapshot format\n");
        }
        gst_caps_get_structure (caps, 0);
        buffer = gst_sample_get_buffer (sample);
        gst_buffer_map (buffer, &map, GST_MAP_READ);

	    bufinfo=(OSA_buf::OSA_BufInfo*)(imagequeue_->getempty(OSA_TIMEOUT_NONE));
	    if(bufinfo!=NULL)
	  	{

				memcpy(bufinfo->virtAddr,map.data,map.size);

				imagequeue_->putfull(bufinfo);
	  	}


      //  printf("map.size = %lu\n", map.size);
        //cv::Mat frame_in(1920, 1080, CV_8UC3);
       // static unsigned int  pretime1=0;
       // unsigned int currenttime=OSA_getCurTimeInMsec();
	//if(currenttime-pretime>50||currenttime-pretime<30)
		{
			;
			//OSA_printf("********lost %d ms %s timeoffset=%d ms**********\n", OSA_getCurTimeInMsec(), __func__,currenttime-pretime1);
		}

	//pretime1=currenttime;

       // show = Mat(1080, 1920, CV_8UC2, instance->rtspdata);
	// showrgb = Mat(1080, 1920, CV_8UC3, instance->rtspdatargb);
      //  cvtColor(show,showrgb,cv::COLOR_YUV2BGR_NV12);


        gst_buffer_unmap(buffer, &map);

        gst_sample_unref (sample);
    }
    else
    {
        g_print ("could not make snapshot\n");
    }

    return GST_FLOW_OK;
}
int RtspCapture::MAIN_threadRecvCreate(void)
{
	int iRet = OSA_SOK;
	iRet = OSA_semCreate(&MainRtspThrObj.procNotifySem ,1,0) ;
	OSA_assert(iRet == OSA_SOK);


	MainRtspThrObj.exitProcThread = false;

	MainRtspThrObj.initFlag = true;

	MainRtspThrObj.pParent = (void*)this;

	iRet = OSA_thrCreate(&MainRtspThrObj.thrHandleProc, mainRecvTsk, 0, 0, &MainRtspThrObj);


	return iRet;
}
int RtspCapture::MAIN_threadRecvDestroy(void)
{
	int iRet = OSA_SOK;

	MainRtspThrObj.exitProcThread = true;
	OSA_semSignal(&MainRtspThrObj.procNotifySem);

	iRet = OSA_thrDelete(&MainRtspThrObj.thrHandleProc);

	MainRtspThrObj.initFlag = false;
	OSA_semDelete(&MainRtspThrObj.procNotifySem);

	return iRet;
}
void RtspCapture::main_Recv_funcdata()
{
	OSA_printf("+++++++++++++%s: Main Proc Tsk Is Entering...++++++++++++++++++\n",__func__);
	unsigned char *data=NULL;
	int angle=0;
	int status;
	//memset(&loaddata,0,sizeof(VideoLoadData));
	static double gyrodata=0;
	OSA_buf::OSA_BufInfo *bufinfo;

	while(MainRtspThrObj.exitProcThread ==  false)
	{
		//OSA_semWait(&loadsem,OSA_TIMEOUT_FOREVER);

		bufinfo=(OSA_buf::OSA_BufInfo*)imagequeue_->getfull(OSA_TIMEOUT_FOREVER);
		unsigned char *data=(unsigned char *)bufinfo->virtAddr;
		 NV212BGR(data,rgbdata,width_,height_);
		 imagequeue_->putempty(bufinfo);
		//rtspdatelen=map.size;
		Mat rgb=Mat(height_,width_,CV_8UC3,rgbdata);
		if(Callback_!=NULL)
		{
			//if(RTSPURL)
			Callback_(rgb);
		}
	}
}

//#define MAX(a ,b) ((a > b) ? a : b)
//#define MIN(a ,b) ((a < b) ? a : b)
#define CLAP(a) (MAX((MIN(a, 0xff)), 0x00))
void RtspCapture::NV212BGR( unsigned char *imgY, unsigned char *imgDst,int width, int height )
{
	int w, h;
	int shift = 14, offset = 8192;
	int C0 = 22987, C1 = -11698, C2 = -5636, C3 = 29049;

	int y1,y2,u1,v1;

	unsigned char * pY1 = imgY;
	unsigned char * pY2 = imgY+width;
	unsigned char * pUV = imgY+width*height;

	unsigned char * pD1 = imgDst;
	unsigned char * pD2 = imgDst+width*3;

	for ( h = 0; h < height; h +=2 )
	{
		for ( w = 0; w < width; w +=2 )
		{
			u1 = *pUV-128;
			pUV++;
			v1 = *pUV-128;
			pUV++;


			y1 = *pY1;
			y2 = *pY2;

			*pD1++ = CLAP(y1+((u1 * C3 + offset) >> shift));
			*pD1++ = CLAP(y1+((u1 * C2 + v1 * C1 + offset) >> shift));
			*pD1++ = CLAP(y1+((v1 * C0 + offset) >> shift));
			*pD2++ = CLAP(y2+((u1 * C3 + offset) >> shift));
			*pD2++ = CLAP(y2+((u1 * C2 + v1 * C1 + offset) >> shift));
			*pD2++ = CLAP(y2+((v1 * C0 + offset) >> shift));

			pY1++;
			pY2++;
			y1 = *pY1;
			y2 = *pY2;

			*pD1++ = CLAP(y1+((u1 * C3 + offset) >> shift));
			*pD1++ = CLAP(y1+((u1 * C2 + v1 * C1 + offset) >> shift));
			*pD1++ = CLAP(y1+((v1 * C0 + offset) >> shift));
			*pD2++ = CLAP(y2+((u1 * C3 + offset) >> shift));
			*pD2++ = CLAP(y2+((u1 * C2 + v1 * C1 + offset) >> shift));
			*pD2++ = CLAP(y2+((v1 * C0 + offset) >> shift));
			pY1++;
			pY2++;

		}
		pY1 += width;
		pY2 += width;
		pD1 += 3*width;
		pD2 += 3*width;

	}
}
