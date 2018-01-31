
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "libuvc/libuvc.h"
#include "libuvc/libuvc_internal.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <thread>



using namespace std;
using namespace cv;


void cb(uvc_frame_t *frame, void *ptr) {
  
  uvc_frame_t *bgr;
  uvc_error_t ret;
  IplImage* cvImg;


  int wid = frame->width;
  int hei = frame->height;
  
 unsigned short data16[wid*hei];
  unsigned char * bytedata = (unsigned char*) frame->data;

  for(int ii=0; ii < wid*hei; ii++) {
    data16[ii] = (unsigned short) (bytedata[2*ii]) | (unsigned short) (bytedata[2*ii+1] << 8) ;
  }

  cv::Mat Bayer( hei, wid, CV_16UC1, (void*) data16);
  double min, max;
  cv::minMaxLoc(Bayer, &min, &max);
  cv::Mat bgrmat(wid,hei,CV_16UC3);
  cv::cvtColor(Bayer, bgrmat, CV_BayerBG2RGB);
  cv::pyrDown(bgrmat,bgrmat);
  cv::pyrUp(bgrmat,bgrmat);
  bgrmat.convertTo(bgrmat,CV_8UC3);
  Bayer.convertTo(Bayer,CV_8UC3);

  #if 0

  #endif




  cv::namedWindow("Test", CV_WINDOW_AUTOSIZE); 
  cv::imshow("Test", bgrmat);

  
  cv::waitKey(1);

}


void task1() {
   struct libusb_device_handle *usb_devh;
  uvc_device_handle_t *internal_devh;
  struct libusb_device_descriptor desc;
  uvc_device_t *dev;
  int result;
  int recv;

  unsigned char data_in[4];


  result = libusb_open(dev->usb_dev, &usb_devh);
  fprintf(stderr, " result %d\n", result);


}




int main(int argc, char **argv) {
  uvc_context_t *ctx;
  uvc_error_t res;
  uvc_device_t *dev;
  uvc_device_handle_t *devh;
  uvc_stream_ctrl_t ctrl;
  struct libusb_device_handle *usb_devh;
  struct libusb_device_descriptor desc;
  


  res = uvc_init(&ctx, NULL);

  if (res < 0) {
    uvc_perror(res, "uvc_init");
    return res;
  }

  puts("UVC initialized");

  res = uvc_find_device(
      ctx, &dev,
      0, 0, NULL);

  if (res < 0) {
    uvc_perror(res, "uvc_find_device");
  } else {
    puts("Device found");

    res = uvc_open(dev, &devh);

    if (res < 0) {
      uvc_perror(res, "uvc_open");
    } else {
      puts("Device opened");

      uvc_print_diag(devh, stderr);

      res = uvc_get_stream_ctrl_format_size(

           devh, &ctrl, UVC_FRAME_FORMAT_YUYV, 320, 240, 30);

      uvc_print_stream_ctrl(&ctrl, stderr);


      if (res < 0) {
        uvc_perror(res, "get_mode");
      } else {
        int dummy;
        int result;
        res = uvc_start_streaming(devh, &ctrl, cb,&dummy, 0);

        if (res < 0) {
          uvc_perror(res, "start_streaming");
        } else {
          puts("Streaming for 10 seconds...");
          uvc_error_t resAEMODE = uvc_set_ae_mode(devh, 1);
          uvc_perror(resAEMODE, "set_ae_mode");
          int i;

          cout << usb_devh << endl;

          usb_devh = uvc_get_libusb_handle (devh);
          cout << usb_devh << endl;

		  cout << devh->usb_devh << endl;

		  usb_devh = libusb_open_device_with_vid_pid(NULL,1204,249);
        
        while(1){  
			unsigned char bulk_data_out[4];
		 	unsigned char bulk_data_in[4];
		 	int tx,rx;
		 	bulk_data_out[0]='a';bulk_data_out[1]='b';bulk_data_out[2]='c';bulk_data_out[3]='d';
		 	auto start = chrono::steady_clock::now();
		 	for(i = 0; i < 4; i++)
		  	{
		  		printf("%c ",char(bulk_data_out[i]));
		  	}

			result = libusb_bulk_transfer( usb_devh, 0x04, bulk_data_out, 4, &tx, 1000 );
			sleep (2);
			fprintf(stderr, "the bulk transfer is completed %d\n",result );
			auto end = chrono::steady_clock::now();
			auto diff = end - start;
			cout << chrono::duration <double, milli> (diff).count() << "tx ms" << endl;
			if (result >= 0)
			{
				
				if (tx > 0)
				{
					auto start = chrono::steady_clock::now();
					sleep(3);
					result = libusb_bulk_transfer(
							usb_devh,
							0x84,
							bulk_data_in,
							4,
							&rx,
							1000);
				  	printf("Data received via bulk transfer:\n");
				  	for(i = 0; i < rx; i++)
				  	{
				  		printf("%c ",char(bulk_data_in[i]));
				  	}
				  	auto end = chrono::steady_clock::now();
					auto diff = end - start;
					cout << chrono::duration <double, milli> (diff).count() << "rx ms" << endl;
				  	printf("\n");
					
				}

				else
				{
					fprintf(stderr, "No data received in interrupt transfer (%d)\n", result);

				}
		  }
		}
 

      sleep(1);
         // }
          sleep(200);
          uvc_stop_streaming(devh);
    puts("Done streaming.");
        }
      }

      uvc_close(devh);
      puts("Device closed");
    }

    uvc_unref_device(dev);
  }

  uvc_exit(ctx);
  puts("UVC exited");

  return 0;
}











