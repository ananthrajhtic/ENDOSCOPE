#include <iostream>
#include <thread>
#include <libusb-1.0/libusb.h>
  
 
static const int INTERFACE_NUMBER = 0;
static const int TIMEOUT_MS = 5000;
struct libusb_device_handle *devh = NULL;



	int bytes_transferred;
	int i = 0;
	int result = 0;;

 	//unsigned char bulk_data_out[MAX_INTERRUPT_IN_TRANSFER_SIZE];
 	unsigned char bulk_data_in[512];


int exchange_input_and_output_reports_via_interrupt_transfers(libusb_device_handle *devh)
{
	printf("i am here! \n");
	static const int INTERRUPT_IN_ENDPOINT = 0x82;
	// static const int INTERRUPT_OUT_ENDPOINT = 0x01;


	static const int MAX_INTERRUPT_IN_TRANSFER_SIZE = 2;
	static const int MAX_INTERRUPT_OUT_TRANSFER_SIZE = 2;

	int bytes_transferred;
	int i = 0;
	int result = 0;;

 	unsigned char data_in[MAX_INTERRUPT_IN_TRANSFER_SIZE];

	result = libusb_interrupt_transfer(
					devh,
					INTERRUPT_IN_ENDPOINT,
					data_in,
					MAX_INTERRUPT_OUT_TRANSFER_SIZE,
					&bytes_transferred,
					0);

	if (result >= 0)
	{
		if (bytes_transferred > 0)
		{
		  	printf("Data received via interrupt transfer:\n");
		  	for(i = 0; i < bytes_transferred; i++)
		  	{
		  		printf("%02x ",data_in[i]);
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


int exchange_input_and_output_reports_via_bulk_transfers(libusb_device_handle *devh)
{

	static const int INTERRUPT_IN_ENDPOINT = 0x83;
	static const int INTERRUPT_OUT_ENDPOINT = 0x01;


	static const int MAX_INTERRUPT_IN_TRANSFER_SIZE = 512;
	static const int MAX_INTERRUPT_OUT_TRANSFER_SIZE = 512;

	int bytes_transferred;
	int i = 0;
	int result = 0;;

 	unsigned char bulk_data_out[512];
 	unsigned char bulk_data_in[MAX_INTERRUPT_IN_TRANSFER_SIZE];
 	bulk_data_out[0]='a';bulk_data_out[1]='b';bulk_data_out[2]='c';bulk_data_out[3]='d';
 	for(i = 0; i < 4; i++)
		  	{
		  		printf("%c ",char(bulk_data_out[i]));
		  	}

	result = libusb_bulk_transfer(
					devh,
					INTERRUPT_OUT_ENDPOINT,
					bulk_data_out,
					MAX_INTERRUPT_OUT_TRANSFER_SIZE,
					&bytes_transferred,
					TIMEOUT_MS);

	if (result >= 0)
	{
		fprintf(stderr, "the bulk transfer is completed %d\n",result );
		if (bytes_transferred > 0)
		{

			result = libusb_bulk_transfer(
					devh,
					INTERRUPT_IN_ENDPOINT,
					bulk_data_in,
					512,
					&bytes_transferred,
					TIMEOUT_MS);
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


  
void task1() {

	exchange_input_and_output_reports_via_interrupt_transfers(devh);
	std::cout << "Hello, World task1" << std::endl;
}



void task2() {
    std::cout << "Hello, World 1" << std::endl;
	exchange_input_and_output_reports_via_bulk_transfers(devh);

   }
  
int main() {

	
	int device_ready = 0;
	int result;

	result = libusb_init(NULL);

	if (result >= 0){
		fprintf(stderr, "the device is initialized %d\n", result);

		devh = libusb_open_device_with_vid_pid(NULL,1204,249);
		if (devh != NULL)
		{
			fprintf(stderr, "The device is opend up.\n");

			libusb_detach_kernel_driver(devh, INTERFACE_NUMBER);
			{
				result = libusb_claim_interface(devh, INTERFACE_NUMBER);
				if (result >= 0)
				{
					device_ready = 1;

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

	fprintf(stderr, "The device is ready.%d\n", device_ready);
  	
  	while(1){
		    std::thread x(task1);
		    std::thread y(task2);
			 
			x.join();
			y.join();

  	}

  	libusb_release_interface(devh, 0);
  	libusb_close(devh);
	libusb_exit(NULL);


	 
	return 0;
}



