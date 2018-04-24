#include <stdio.h>
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <wiringPi.h>
//****************************Global variable**********************/
snd_pcm_t *handle1,*handle2;
snd_pcm_hw_params_t *params1,*params2;
int rc;
snd_pcm_uframes_t frames;
void *buffer;	
unsigned int val;

void InitPlayback(void)
{
	int size;
	int dir;
	/////////open PCM DEVICE
	rc = snd_pcm_open(&handle2, "default",  SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);  
	if (rc < 0) 
	{  
		fprintf(stderr,  "unable to open pcm device: %s/n",  
		snd_strerror(rc));  
		exit(1);  
	}  
//////////////allocate hd params to driver
	snd_pcm_hw_params_alloca(&params2);
//////////////set default params
	snd_pcm_hw_params_any(handle2,params2);
//////////////inter--- mode
	snd_pcm_hw_params_set_access(handle2,params2,SND_PCM_ACCESS_RW_NONINTERLEAVED);
//////////////little16
	snd_pcm_hw_params_set_format(handle2,params2,SND_PCM_FORMAT_S16_LE);
//////////////single channel
	snd_pcm_hw_params_set_channels(handle2,params2,1);
//////////////8000bits/second sampling rate
	val=8000;
	snd_pcm_hw_params_set_rate_near(handle2,params2,&val,&dir);
printf("playback rate:%d\n", val);
/* Set period size to 32 frames. */  
	frames = 80;  //could be changed
	snd_pcm_hw_params_set_period_size_near(handle2,  params2, &frames, &dir); 
//	printf("frames is %d\n",(int)frames);
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
printf("playback frame = %d\n",frames);
	buffer = (char *) malloc(size);  

/* We want to loop for 5 seconds */  
	snd_pcm_hw_params_get_period_time(params2,  &val, &dir);  
}
//****************************Initial Capture  function**********************/
void InitCapture(void)
{
	int size;

	int dir;
	rc=snd_pcm_open(&handle1,"default",SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK);
	if(rc<0)
	{
		fprintf(stderr,"unable to open pcm device:%s\n",snd_strerror(rc));
		exit(1);
	}
//////////////allocate params for hw
	snd_pcm_hw_params_alloca(&params1);
//////////////set handle1's params
	snd_pcm_hw_params_any(handle1,params1);
//////////////intermode
	snd_pcm_hw_params_set_access(handle1,params1,SND_PCM_ACCESS_RW_NONINTERLEAVED);
//////////////Little16
	snd_pcm_hw_params_set_format(handle1,params1,SND_PCM_FORMAT_S16_LE);
//////////////single channel
	snd_pcm_hw_params_set_channels(handle1,params1,1);
//////////////8000bits/second sampling rate
	val=8000;
	snd_pcm_hw_params_set_rate_near(handle1,params1,&val,&dir);
printf("capture rate:%d\n",val);
//////////////set sample period as 80 frames
	frames=80;
	snd_pcm_hw_params_set_period_size_near(handle1,params1,&frames,&dir);
///////////////set hw parameters
	rc=snd_pcm_hw_params(handle1,params1);
	if(rc<0)
	{
		fprintf(stderr,"unable to set hw parameters:%s\n",snd_strerror(rc));
		exit(1);
	}
///////////////use a buffer which is big enough to save one period
	snd_pcm_hw_params_get_period_size(params1,&frames,&dir);
	size=frames*2; /////////2bytes/sample,1 channels
	printf("capture size=%d\n",frames);
//	buffer=(char*)malloc(size);
///////////////////////////lasting ?s
	snd_pcm_hw_params_get_period_time(params1,&val,&dir);
}

//****************************Main  Function**********************/
int main()
{
	int i=0;
	InitCapture();
	InitPlayback();
//////////define gpio
	wiringPiSetup();
	pinMode(0,OUTPUT);
	digitalWrite(0,LOW);

	while(i++<10)
	{
		//***************************Write to Buffer**************************/
		rc=snd_pcm_readn(handle1,&buffer,frames);
printf("'readn returns %d\n",rc);		
if(rc == -EPIPE)
		{
	/* EPIPE means overrun */  
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
		//delay(39);
		//***************************Read from Buffer***************************/
		rc = snd_pcm_writen(handle2, &buffer, frames); 
		printf("%d\n",rc);
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





	 

