#ifndef __COMMANDDATA_H
#define __COMMANDDATA_H

#define COMMAND_RANGE_MIN   -1000
#define COMMAND_RANGE_MID   0
#define COMMAND_RANGE_MAX   1000

typedef struct __attribute__((packed)) _commands_t
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

}   commands_t;

#endif
