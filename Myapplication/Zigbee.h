#ifndef __Zigbee_H_
#define __Zigbee_H_

#include "MyApplication.h"

#define Zigbee_Receive_Len 512

typedef struct
{
	void (*Zigbee_Receive_Information)(void);
	void (*DMA_Receive_Set3)(void);
	void (*Zigbee_MQTT_Pub)(void);
}Zigbee_t;


/* extern variables-----------------------------------------------------------*/
extern Zigbee_t Zigbee;
#endif
/********************************************************
  End Of File
********************************************************/
