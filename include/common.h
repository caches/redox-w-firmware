#ifndef __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__
#define __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__

#if defined(COMPILE_LEFT) && defined(COMPILE_RIGHT)
#error "Only one of COMPILE_LEFT and COMPILE_RIGHT can be defined at once."
#endif

#define CHANNEL_TABLE_SIZE 3
#define TIMESOLTS_PER_CHANNEL 4
#define TIMESOLT_PERIOD 900

#define TX_POWER_LEVEL_DEF NRF_GZLL_TX_POWER_0_DBM
#define TX_POWER_LEVEL_MAX NRF_GZLL_TX_POWER_4_DBM
#define TX_POWER_LEVEL_MIN NRF_GZLL_TX_POWER_N20_DBM
// NRF_GZLL_TX_POWER_4_DBM 	    4 dBm transmit power
// NRF_GZLL_TX_POWER_0_DBM 	    0 dBm transmit power.[DEFAULT]
// NRF_GZLL_TX_POWER_N4_DBM 	-4 dBm transmit power.
// NRF_GZLL_TX_POWER_N8_DBM 	-8 dBm transmit power.
// NRF_GZLL_TX_POWER_N12_DBM 	-12 dBm transmit power.
// NRF_GZLL_TX_POWER_N16_DBM 	-16 dBm transmit power.
// NRF_GZLL_TX_POWER_N20_DBM 	-20 dBm transmit power. 

//{4, 25, 42, 63, 77, 33}
// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {1, 13, 21, 34, 42, 53, 61, 74};
// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {4, 18, 24, 39, 45, 58, 64, 79};
// static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {7, 15, 27, 36, 48, 57, 67, 75};
static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {2, 25, 33, 42, 63, 77};
//static uint8_t combine_channel_table[CHANNEL_TABLE_SIZE * 2] = {2, 38, 72, 108};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE] = {0};

#ifdef COMPILE_LEFT
#define PIPE_NUMBER 0
static uint8_t channel_table[CHANNEL_TABLE_SIZE] = {2, 33, 63};
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

#define BASE_ADDRESS0 0x01020328
#define BASE_ADDRESS1 0x05060423

#ifdef COMPILE_RIGHT
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={13, 34,  53, 74};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={18, 39, 58, 79};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={15, 36, 57, 75};
// static uint8_t channel_table[CHANNEL_TABLE_SIZE]={23,40, 63, 80};
#define PIPE_NUMBER 1
static uint8_t channel_table[CHANNEL_TABLE_SIZE] = {25, 42, 77};

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


// #if defined(COMPILE_LEFT) || defined(COMPILE_RIGHT)
// void init_channel_table()
// {
//     for (int i=0; i<CHANNEL_TABLE_SIZE; i++)
//     {
//         channel_table[i] = combine_channel_table[i*2+PIPE_NUMBER];
//     }
// }
// init_channel_table();
// #endif


#define COLUMNS 7
#define ROWS 5
#define PAYLOAD_LENGTH ROWS // 5 byte payload length

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#endif /* __REDOX_W_FIRMWARE_CONFIG_REDOX_W_H__ */
