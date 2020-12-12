#include "usb.h"
#include <stdio.h>

typedef unsigned char uchar;

#pragma once
#define CONF 1
#define IF_NUM 0 //bInterfaceNumber
#define RF_ERROR -1 // general rf module error
#define RF_SEND_ERROR -2 // sending to moway caused problem
#define RF_RECEIVE_ERROR -3 // receiving to moway caused problem
#define RF_RECEIVE_TIMEOUT 1000 //defines how long wait for acknowledgement/data from moway

#define MY_VID 0x04d8
#define MY_PID 0x0014
#define OUT_ENDPOINT 0x01
#define IN_ENDPOINT 0x81

/*
    Class maintaining data received from moway.
*/
class Answer {
	public:
    Answer(uchar* d, int n);
    ~Answer();

	//returns number of bytes received from moway
	int answerSize();
    
    //returns array of bytes received from moway
	uchar* getAnswer();

    protected:
    uchar* data;
    int size;
};

/*
    Class providing communication with moway.
*/
class RFModule {
    public:
    RFModule();
    
    /* return
        RF_ERROR on error
        0 on success
    */
    int connect(uchar channel, uchar send_address, uchar receive_address);

    /* return
        RF_ERROR on error
        0 on success
    */
    int disconnect();

    /* return
        NULL on error
        Answer object on success
    */
    Answer* send(uchar* data, int n);

	void RFModule::read(uchar* data);
    
    /* return
        false on libusb not initialized
        true on libusb initialized
    */
    bool isInitialized();

    /* return
        negative value (RF_SEND_ERROR or RF_RECEIVE_ERROR) if last execution of RFModule::send() caused error
        0 if there was no error in last execution of RFModule::send()
    */
    int getSendError();

    ~RFModule();
    int receive_timeout;
	int send_wait;
    protected: 
    uchar buffer[64]; //buffer used to exchange data with moway
    uchar channel;
    uchar sendAddress; //address on which moway initialize itself in RF_CONFIG
    uchar receiveAddress; //address on which RFModule connects to moway
    bool isInit;
    int sendError;
    usb_dev_handle *dev;
    int receive(uchar* data, int n);
    int rawSend(uchar* data, int n);
    usb_dev_handle* open_dev(void);
    void clean_up();  
};
