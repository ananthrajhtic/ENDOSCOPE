#include <iostream>
#include <thread>
#include <libusb-1.0/libusb.h>
  
  
int main() {

	libusb_device_handle *devh;
	
	int device_ready = 0;
	int result;

	result = libusb_init(NULL);

	if (result >= 0){
		fprintf(stderr, "the device is initialized %d\n", result);

		devh = libusb_open_device_with_vid_pid(NULL,1204,249);
		if (devh != NULL)
		{
			fprintf(stderr, "The device is opend up.\n");

			libusb_detach_kernel_driver(devh, 0);
			{
				result = libusb_claim_interface(devh, 0);
				if (result >= 0)
				{
					device_ready = 1;
					int bytes_transferred;
					int i = 0;
					int result = 0;;

				 	unsigned char bulk_data_out[4];
				 	unsigned char bulk_data_in[4];
				 	bulk_data_out[0]='a';bulk_data_out[1]='b';bulk_data_out[2]='c';bulk_data_out[3]='d';
				 	for(i = 0; i < 4; i++)
						  	{
						  		printf("%c ",char(bulk_data_out[i]));
						  	}

					result = libusb_bulk_transfer(
									devh,
									0x04,
									bulk_data_out,
									4,
									&bytes_transferred,
									1000);

					if (result >= 0)
					{
						fprintf(stderr, "the bulk transfer is completed %d\n",result );
						if (bytes_transferred > 0)
						{

							result = libusb_bulk_transfer(
									devh,
									0x84,
									bulk_data_in,
									4,
									&bytes_transferred,
									1000);
						  	printf("Data received via bulk transfer:\n");
						  	for(i = 0; i < bytes_transferred; i++)
						  	{
						  		printf("%c ",char(bulk_data_in[i]));
						  	}
						  	printf("\n");
						}
						else
						{
							fprintf(stderr, "No data received in interrupt transfer (%d)\n", result);
							return -1;
						}
				  	return 0;
				  }

				}
				else
				{
					fprintf(stderr, "libusb_claim_interface error %d\n", result);
				}
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

	// fprintf(stderr, "The device is ready.%d\n", device_ready);
  	
  	libusb_release_interface(devh, 0);
  	libusb_close(devh);
	libusb_exit(NULL);


	 
	return 0;
}



