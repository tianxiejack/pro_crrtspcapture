/*
 * Capture.hpp
 *
 *  Created on: Feb 18, 2019
 *      Author: wj
 */

#ifndef CPATURE_HPP_
#define CPATURE_HPP_

#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define CAPTURE_PURE_VIRTUAL =0



class Capture{
public:
	typedef void (*CaptureFrameCallback)(const cv::Mat frame,const int chId);
public:
	Capture(){};
public:
	virtual ~Capture() {};
	virtual void init(std::string devname,int chId,int width=1920,int height=1080,CaptureFrameCallback callback=NULL)CAPTURE_PURE_VIRTUAL;
	virtual void uninit()CAPTURE_PURE_VIRTUAL;

	//virtual void setparam();
};

Capture *RTSPCapture_Create();
void RTSPCapture_Create(Capture *obj);

#endif /* DETECTOR_HPP_ */
