#include "ZPC_IO_funcs.h"

// void ZPC_IO_HandleRead(uint8_t address)
// {
//     uint8_t data_in = 0xff;
//     uint8_t port = address & 0xff;
//     switch (port)
//     {
//     case 0x0a:
//         data_in = ZPC_IO_ArduinoROM_ReadByte(address);
//         break;
//     case 0x01:
//         data_in = ZPC_IO_Serial_ReadByte();
//         break;
//     default:
//         data_in = ZPC_IO_Serial_ReadByte();
//     }
//     ZPC_DataSetOutput();
//     ZPC_SetData(data_in);
// }

// void ZPC_IO_HandleWrite(uint16_t address, uint8_t data)
// {
//     uint8_t port = address & 0xff;
//     // ZPC_IO_Serial_WriteByte(data);
//     char str_buf[30];
//     switch (port)
//     {
//     case 0x0a:
//         ZPC_IO_ArduinoROM_WriteByte(address, data);
//         break;
//     case 0x01:
//         ZPC_IO_Serial_WriteByte(data);
//         break;
//     case 0x0f:
//         static uint32_t running_time;
//         sprintf(str_buf, "Running time: %li ms \n", millis() - running_time);
//         Serial.print(str_buf);
//         running_time = millis();
//         break;
//     default:
//         ZPC_IO_Serial_WriteByte(data);
//     }
// };

uint8_t ZPC_IO_Serial_ReadByte()
{
    // while (!Serial.available())
    //   ;
    return Serial.available() ? Serial.read() : 0;
}

uint8_t ZPC_IO_Serial_WriteByte(uint8_t data)
{
    Serial.write(data);
    return 0;
}

// uint8_t ZPC_IO_

uint8_t ZPC_IO_ArduinoROM_ReadByte(uint16_t address)
{
    // return EEPROM.read(address);
    return 0;
}

uint8_t ZPC_IO_ArduinoROM_WriteByte(uint16_t address, uint8_t data)
{
    // EEPROM.write(address, data);
    return 0;
}
