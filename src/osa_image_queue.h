

#ifndef _OSA_IMAGE_QUEUE_ALG_H_
#define _OSA_IMAGE_QUEUE_ALG_H_

#include <osa.h>
#include <osa_que.h>
#include <osa_mutex.h>
#include <osa_buf_cr.h>
class ImageQueue
{
public :
	enum {
		memtype_default = 0,//memtype_glpbo
		memtype_normal,
		memtype_gst,
		memtype_malloc,
		memtype_null,
		memtype_cuhost,
		memtype_cudev,
		memtype_cumap,
		memtype_glpbo
	};
	int image_queue_create(OSA_buf::OSA_BufHndl *hndl, int nBuffers, size_t buffsize, int memtype);

	int image_queue_delete(OSA_buf::OSA_BufHndl *hndl);

	OSA_buf::OSA_BufInfo* image_queue_getEmpty(OSA_buf::OSA_BufHndl *hndl);

	int image_queue_putFull(OSA_buf::OSA_BufHndl *hndl, OSA_buf::OSA_BufInfo* info);

	OSA_buf::OSA_BufInfo* image_queue_getFull(OSA_buf::OSA_BufHndl *hndl);

	OSA_buf::OSA_BufInfo* image_queue_getFullforever(OSA_buf::OSA_BufHndl *hndl);

	int image_queue_putEmpty(OSA_buf::OSA_BufHndl *hndl, OSA_buf::OSA_BufInfo* info);

	OSA_buf::OSA_BufInfo* image_queue_peekFull(OSA_buf::OSA_BufHndl *hndl);

	OSA_buf::OSA_BufInfo* image_queue_peekEmpty(OSA_buf::OSA_BufHndl *hndl);

	OSA_buf::OSA_BufInfo* image_queue_getFulltime(OSA_buf::OSA_BufHndl *hndl,int time);

	OSA_buf::OSA_BufInfo* image_queue_getEmptytime(OSA_buf::OSA_BufHndl *hndl,int time);

	int image_queue_switchEmpty(OSA_buf::OSA_BufHndl *hndl, OSA_buf::OSA_BufInfo* info);

	int image_queue_switchFull(OSA_buf::OSA_BufHndl *hndl, OSA_buf::OSA_BufInfo* info);

	int image_queue_emptyCount(OSA_buf::OSA_BufHndl *hndl);

	int image_queue_fullCount(OSA_buf::OSA_BufHndl *hndl);


	int cuMap(OSA_buf::OSA_BufInfo* info);

	int cuUnmap(OSA_buf::OSA_BufInfo* info);


public :
	ImageQueue(){};
	~ImageQueue(){};
private:
	OSA_buf osa_buf;
};

#endif /* _OSA_IMAGE_QUEUE_H_ */



