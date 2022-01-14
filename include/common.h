#ifndef __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__
#define __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__

#if defined(COMPILE_LEFT) && defined(COMPILE_RIGHT)
#error "Only one of COMPILE_LEFT and COMPILE_RIGHT can be defined at once."
#endif


#ifdef COMPILE_LEFT

#define PIPE_NUMBER 0

#define C01 3
#define C02 4
#define C03 5
#define C04 6
#define C05 7
#define C06 9
#define C07 10

#define R01 19
#define R02 18
#define R03 17
#define R04 14
#define R05 13

// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={1, 21, 42, 61};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={4, 24, 45, 64};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={7, 27,48, 67};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={10, 31, 50, 71};



#endif

// #define BASE_ADDRESS0 0x01214261
// #define BASE_ADDRESS0 0x04244564
// #define BASE_ADDRESS0 0x07274867
// #define BASE_ADDRESS0 0x10315071

// #define BASE_ADDRESS1 0x13345374
// #define BASE_ADDRESS1 0x18395879
// #define BASE_ADDRESS1 0x15365775
// #define BASE_ADDRESS1 0x23406380

#define BASE_ADDRESS0 0x01020304
#define BASE_ADDRESS1 0x05060708

#ifdef COMPILE_RIGHT

// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={13, 34,  53, 74};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={18, 39, 58, 79};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={15, 36, 57, 75};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={23,40, 63, 80};
#define PIPE_NUMBER 1

#define C01 30
#define C02 0
#define C03 2
#define C04 3
#define C05 4
#define C06 5
#define C07 6

#define R01 21
#define R02 22
#define R03 23
#define R04 28
#define R05 29

#endif
#define CHANNEL_TABLE_SIZE 4
#define TIMESOLTS_PER_CHANNEL 4

// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {1, 13, 21, 34, 42, 53, 61, 74};
// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {4, 18, 24, 39, 45, 58, 64, 79};
static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {7, 15, 27, 36, 48, 57, 67, 75};
// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {10, 23, 31, 40, 50, 63, 71, 80};


#define COLUMNS 7
#define ROWS 5
#define PAYLOAD_LENGTH ROWS // 5 byte payload length

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif /* __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__ */
