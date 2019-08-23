/*
 * Queuebuffer.hpp
 *
 *  Created on: 2018年11月9日
 *      Author: wj
 */

#ifndef QUEUEBUFFER_HPP_
#define QUEUEBUFFER_HPP_
#include "osa_image_queue.h"
#include"osa.h"
#include"stdio.h"
class Queue 
{
	public:
		#define MAXCHANNEL (1)
		#define PROCESSQUEUENUM (4)
		#define MAXQUEUE 1
	public :
		Queue(int width,int height,int channel=3,int numqueue=4,int memtype=ImageQueue::memtype_normal);
		~Queue(){};
		void *getfull(int forever);
		void  putempty(void *info);
		int getfullcount();
		void *getempty(int forever);
		void  putfull(void *info);
		void *getprefull(void *curentif);
		void *getprefull();
		
	private:
		ImageQueue imagequeue;
		OSA_buf::OSA_BufHndl bufQue[MAXQUEUE][MAXCHANNEL];
		int width_;
		int height_;
		int channel_;
		int numqueue_;
		int memtype_;
};



#endif /* QUEUEBUFFER_HPP_ */
