#include <stdio.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <wiringPi.h>
//****************************Global variable**********************
snd_pcm_t *handle1,*handle2;
snd_pcm_hw_params_t *params1,*params2;
int rc,i=0;
snd_pcm_uframes_t frames;
char *buffer;	
unsigned int val;

void InitPlayback(void)
{
	int size;
	int dir;
	/////////open PCM DEVICE
	rc = snd_pcm_open(&handle2, "default",  SND_PCM_STREAM_PLAYBACK, 0);  
	if (rc < 0) 
	{  
		fprintf(stderr,  "unable to open pcm device: %s/n",  
		snd_strerror(rc));  
		exit(1);  
	}  
//////////////set hd params to driver
	snd_pcm_hw_params_alloca(&params2);
//////////////赋予默认值
	snd_pcm_hw_params_any(handle2,params2);
//////////////设置所需硬件参数
//////////////inter--- mode
	snd_pcm_hw_params_set_access(handle2,params2,SND_PCM_ACCESS_RW_INTERLEAVED);
//////////////little16
	snd_pcm_hw_params_set_format(handle2,params2,SND_PCM_FORMAT_S16_LE);
//////////////single channel
	snd_pcm_hw_params_set_channels(handle2,params2,1);
//////////////8000bits/second sampling rate
	val=8000;
	snd_pcm_hw_params_set_rate_near(handle2,params2,&val,&dir);
/* Set period size to 32 frames. */  
	frames = 80;  //could be changed
	snd_pcm_hw_params_set_period_size_near(handle2,  params2, &frames, &dir); // 
	printf("frames is %d\n",(int)frames);
/* Write the parameters to the driver */  
	rc = snd_pcm_hw_params(handle2, params2);  
	if (rc < 0)
 	{  
		fprintf(stderr,  "unable to set hw parameters: %s/n",  snd_strerror(rc));  
		exit(1);  
	}  
/* Use a buffer large enough to hold one period */  
	snd_pcm_hw_params_get_period_size(params2, &frames,  &dir);  
	size = frames * 2; /* 2 bytes/sample, 1 channels */  

	buffer = (char *) malloc(size);  

/* We want to loop for 5 seconds */  
	snd_pcm_hw_params_get_period_time(params2,  &val, &dir);  
}
//****************************Initial Capture  function**********************
void InitCapture(void)
{
	int size;

	int dir;
	rc=snd_pcm_open(&handle1,"default",SND_PCM_STREAM_CAPTURE,0);
	if(rc<0)
	{
		fprintf(stderr,"unable to open pcm device:%s\n",snd_strerror(rc));
		exit(1);
	}
//////////////分配硬件参数对象
	snd_pcm_hw_params_alloca(&params1);
//////////////赋予默认值
	snd_pcm_hw_params_any(handle1,params1);
//////////////设置所需硬件参数
//////////////交叉模式
	snd_pcm_hw_params_set_access(handle1,params1,SND_PCM_ACCESS_RW_INTERLEAVED);
//////////////有符号的16位小端格式
	snd_pcm_hw_params_set_format(handle1,params1,SND_PCM_FORMAT_S16_LE);
//////////////单声道
	snd_pcm_hw_params_set_channels(handle1,params1,1);
//////////////8000bits/second sampling rate
	val=8000;
	snd_pcm_hw_params_set_rate_near(handle1,params1,&val,&dir);
//////////////设置采样周期大小为32帧
	frames=80;
	snd_pcm_hw_params_set_period_size_near(handle1,params1,&frames,&dir);
///////////////编写驱动程序的参数
	rc=snd_pcm_hw_params(handle1,params1);
	if(rc<0)
	{
		fprintf(stderr,"unable to set hw parameters:%s\n",snd_strerror(rc));
		exit(1);
	}
///////////////使用一块足够大的缓冲区来保存一个时期
	snd_pcm_hw_params_get_period_size(params1,&frames,&dir);
	size=frames*2; /////////2bytes/sample,1 channels
	printf("size=%d\n",size);
	buffer=(char*)malloc(size);
///////////////////////////循环持续5s 要修改！
	snd_pcm_hw_params_get_period_time(params1,&val,&dir);
}

//****************************Main  Function**********************
int main()
{
	InitCapture();
	InitPlayback();
//////////define gpio
	wiringPiSetup();
	pinMode(0,OUTPUT);
	digitalWrite(0,LOW);

	while(1)
	{
		rc=read(0,buffer,size);
		if (rc == 0) {
      fprintf(stderr, "end of file on input/n");
      break;
    } else if (rc != size) {
      fprintf(stderr,
              "short read: read %d bytes/n", rc);
    }
		//***************************Write to Buffer**************************
		rc=snd_pcm_readi(handle1,buffer,frames);
		if(rc == -EPIPE)
		{
	///////////////EPIPE 表示溢出
			fprintf(stderr,"overrun occurred\n");
			snd_pcm_prepare(handle1);
		}
		else if(rc<0)
		{
			fprintf(stderr,"error from read:%s\n",snd_strerror(rc));
		}
		else if(rc!=(int)frames)
		{
			fprintf(stderr, "short read, read %d frames/n", rc);  
		}

		//***************************Read from Buffer***************************
		rc = snd_pcm_writei(handle2, buffer, frames); 
		if (rc == -EPIPE) 
		{  
	/* EPIPE means underrun */  
		fprintf(stderr, "underrun occurred\n");  
		snd_pcm_prepare(handle2);  
		} 
		else if (rc < 0) 
		{  
			fprintf(stderr,  "error from writei: %s\n",  snd_strerror(rc));  
		}  
		else if (rc != (int)frames) 
		{  
			fprintf(stderr,  "short write, write %d frames\n", rc);  
		}  
	
	}

/////////////close gpio
	digitalWrite(0,HIGH);

	snd_pcm_drain(handle1);
	snd_pcm_close(handle1);
	snd_pcm_drain(handle2);  
	snd_pcm_close(handle2); 
	free(buffer);
	return 0;
}






	 

