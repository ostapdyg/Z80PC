#ifndef ZPC_FUNCS_PROTO
#define ZPC_FUNCS_PROTO

uint8_t __reverse(uint8_t reversee);

void ZPC_AddressSetOutput(void);

void ZPC_AddressSetInputPulup(void);

void ZPC_DataSetOutput(void);

void ZPC_DataSetInputPulup(void);

void ZPC_SetData(uint8_t data);

uint8_t ZPC_GetData();

void ZPC_SetAddress(uint16_t address);

uint16_t ZPC_GetAddress();

void ZPC_MemWrite(uint16_t address, uint8_t data);

uint8_t ZPC_MemRead(uint16_t address);

#endif
