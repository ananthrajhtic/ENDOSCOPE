#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>


int main(void)
{

  struct libusb_device_handle *devh = NULL;
  int device_ready = 0;
  int result;
  int recv;

  result = libusb_init(NULL);
  fprintf(stderr, "usb init %d\n",result );
  if (result >= 0)
  {
    devh = libusb_open_device_with_vid_pid(NULL, 1204, 249);

    if (devh != NULL)
    {
      libusb_detach_kernel_driver(devh, 0);
        result = libusb_claim_interface(devh, 0);
        if (result >= 0)
        {
          fprintf(stderr, "Device claimed \n");
          device_ready = 1;

        }
        else
        {
          fprintf(stderr, "libusb_claim_interface error %d\n", result);
        }
    }
    else
    {
      fprintf(stderr, "Unable to find the device.\n");
    }
  }
  else
  {
    fprintf(stderr, "Unable to initialize libusb.\n");
  }

  char data_in[4];
  while(1){
  result = libusb_bulk_transfer(devh, 0x82, data_in, 4, &recv, 1000);
  printf("%d",result);
  for(int i = 0; i < recv; i++)
        {
          printf("%02x ",data_in[i]);
        }
        printf("\n");
  
  }

  libusb_release_interface(devh, 0);
  libusb_close(devh);
  libusb_exit(NULL);
  return 0;
}

