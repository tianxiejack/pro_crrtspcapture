#include"Queuebuffer.hpp"

#define IMAGEQUEUE 1
#define ZEROQUEUE 0
Queue::Queue(int width,int height,int channel,int numqueue,int memtype):numqueue_(numqueue),width_(width),height_(height),channel_(channel),
		memtype_(memtype)
{
	for(int chId=0; chId<IMAGEQUEUE; chId++)
		imagequeue.image_queue_create(&bufQue[ZEROQUEUE][ZEROQUEUE], numqueue,
				width_*height_*channel,
				memtype);
}

void *Queue::getfull(int forever)
{
	return imagequeue.image_queue_getFulltime(&bufQue[ZEROQUEUE][ZEROQUEUE],forever);

}
void  Queue::putempty(void *info)
{
	imagequeue.image_queue_putEmpty(&bufQue[ZEROQUEUE][ZEROQUEUE],(OSA_buf::OSA_BufInfo* )info);
}

int Queue::getfullcount()
{
	return imagequeue.image_queue_fullCount(&bufQue[ZEROQUEUE][ZEROQUEUE]);
}

void *Queue::getprefull(void *curentif)
{
	OSA_buf::OSA_BufInfo *current=(OSA_buf::OSA_BufInfo *)curentif;
	return &bufQue[ZEROQUEUE][ZEROQUEUE].bufInfo[(current->currentbufid-1+numqueue_)%numqueue_];
}

void *Queue::getempty(int forever)
{
	return imagequeue.image_queue_getEmptytime(&bufQue[ZEROQUEUE][ZEROQUEUE],forever);
}
void  Queue::putfull(void *info)
{
	imagequeue.image_queue_putFull(&bufQue[ZEROQUEUE][ZEROQUEUE],(OSA_buf::OSA_BufInfo* )info);
}

