#ifndef __COMMANDDATA_H
#define __COMMANDDATA_H

#define COMMAND_RANGE_MIN       -1000
#define COMMAND_RANGE_MID       0
#define COMMAND_RANGE_MAX       1000

#define COMMAND_PAYLOAD_LENGTH  64
#define COMMAND_BUFFER_COUNT    8

typedef struct __attribute__((packed)) _command_state_t
{
    uint16_t    driveSpeed;
    uint16_t    turnSpeed;

    union
    {
        uint8_t map;
        struct
        {
            uint8_t armed  : 1;
            uint8_t unused : 7;
        };
    } flags;

}   command_state_t;

typedef struct _command_t
{
    char commandCode[2];
    char payload[COMMAND_PAYLOAD_LENGTH];
    uint8_t payloadLength;

} command_t;

#endif
