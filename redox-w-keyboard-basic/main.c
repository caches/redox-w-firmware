/*
 * Either COMPILE_RIGHT or COMPILE_LEFT has to be defined from the make call to allow proper functionality
 */

#include "nrf_drv_config.h"
#include "nrf_gzll.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_drv_rtc.h"
#include "nrf51_bitfields.h"
#include "nrf51.h"
#include "common.h"


/*****************************************************************************/
/** Configuration */
/*****************************************************************************/

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(1); /**< Declaring an instance of nrf_drv_rtc for RTC1. */


// Data and acknowledgement payloads
static uint8_t ack_payload[NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH]; ///< Placeholder for received ACK payloads from Host.

static const uint32_t COL_PINS[] = { C01, C02, C03, C04, C05, C06, C07 };
static uint32_t inactivity_ticks = 0;
static uint8_t keys_snapshot[ROWS] = {0, 0, 0, 0, 0};
static uint32_t debounce_ticks = 0;
static uint32_t input = 0;

// Debounce time (dependent on tick frequency)
#define DEBOUNCE 8
// Mark as inactive after a number of ticks:
#define INACTIVITY_THRESHOLD 500 // 0.5sec

// #define 	NRF_GZLL_CONST_MAX_CHANNEL_TABLE_SIZE   16;Maximum channel table size allowed by Gazell. 
// The valid channels are in the range 0 <= channel <= 125, 
// where the actual centre frequency is (2400 + channel) MHz. 
// The maximum channel table size is defined by NRF_GZLL_CONST_MAX_CHANNEL_TABLE_SIZE.

// Setup switch pins with pullups
static void gpio_config(void)
{
    nrf_gpio_cfg_sense_input(R01, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
    nrf_gpio_cfg_sense_input(R02, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
    nrf_gpio_cfg_sense_input(R03, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
    nrf_gpio_cfg_sense_input(R04, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
    nrf_gpio_cfg_sense_input(R05, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);

    nrf_gpio_cfg_output(C01);
    nrf_gpio_cfg_output(C02);
    nrf_gpio_cfg_output(C03);
    nrf_gpio_cfg_output(C04);
    nrf_gpio_cfg_output(C05);
    nrf_gpio_cfg_output(C06);
    nrf_gpio_cfg_output(C07);

    nrf_gpio_pin_clear(C01);
    nrf_gpio_pin_clear(C02);
    nrf_gpio_pin_clear(C03);
    nrf_gpio_pin_clear(C04);
    nrf_gpio_pin_clear(C05);
    nrf_gpio_pin_clear(C06);
    nrf_gpio_pin_clear(C07);
}

// Return the key states
static void read_keys(uint8_t *row_stat)
{
    // scan matrix by columns
    for (uint8_t c = 0; c < COLUMNS; ++c) {
        // Force the compiler to add one cycle gap between activating
        // the column pin and reading the input to allow some time for
        // it to be come stable. Note that compile optimizations are
        // not allowed for next three statements. Setting the pin
        // write a location which is marked as volatile
        // (NRF_GPIO->OUTSET) and reading the input is also a memory
        // access to a location which is marked as volatile too
        // (NRF_GPIO->IN).
        nrf_gpio_pin_set(COL_PINS[c]);
        asm volatile("nop");
        input = NRF_GPIO->IN;
        row_stat[0] = (row_stat[0] << 1) | ((input >> R01) & 1);
        row_stat[1] = (row_stat[1] << 1) | ((input >> R02) & 1);
        row_stat[2] = (row_stat[2] << 1) | ((input >> R03) & 1);
        row_stat[3] = (row_stat[3] << 1) | ((input >> R04) & 1);
        row_stat[4] = (row_stat[4] << 1) | ((input >> R05) & 1);
        nrf_gpio_pin_clear(COL_PINS[c]);
    }

}

static bool compare_keys(const uint8_t* first, const uint8_t* second)
{
    for(uint8_t i=0; i < ROWS; i++)
    {
        if (first[i] ^ second[i])
        {
          return false;
        }
    }
    return true;
}

static bool empty_keys(const uint8_t* keys_buffer)
{
    for(uint8_t i=0; i < ROWS; i++)
    {
        if (keys_buffer[i])
        {
          return false;
        }
    }
    return true;
}

static void handle_inactivity(const uint8_t *keys_buffer)
{
    // looking for 500 ticks of no keys pressed, to go back to deep sleep
    if (empty_keys(keys_buffer)) {
        if (++inactivity_ticks >= INACTIVITY_THRESHOLD) {
            nrf_drv_rtc_disable(&rtc);
            nrf_gpio_pin_set(C01);
            nrf_gpio_pin_set(C02);
            nrf_gpio_pin_set(C03);
            nrf_gpio_pin_set(C04);
            nrf_gpio_pin_set(C05);
            nrf_gpio_pin_set(C06);
            nrf_gpio_pin_set(C07);

            inactivity_ticks = 0;

            NRF_POWER->SYSTEMOFF = 1;
        }
    } else {
        inactivity_ticks = 0;
    }
}

static void handle_send(const uint8_t* keys_buffer)
{
    if (compare_keys(keys_buffer, keys_snapshot)) {
        // debouncing - send only if the keys state has been stable
        // for DEBOUNCE ticks
        if (++debounce_ticks >= DEBOUNCE) {
            // Assemble packet and send to receiver
            nrf_gzll_add_packet_to_tx_fifo(PIPE_NUMBER, keys_snapshot, PAYLOAD_LENGTH);
            debounce_ticks = 0;
        }
    } else {
        // change detected, start over
        debounce_ticks = 0;
        for (uint8_t k = 0; k < ROWS; k++) {
            keys_snapshot[k] = keys_buffer[k];
        }
    }
}

// 1000Hz debounce sampling
static void tick(nrf_drv_rtc_int_type_t int_type)
{
    uint8_t keys_buffer[ROWS] = {0, 0, 0, 0, 0};
    read_keys(keys_buffer);

    handle_inactivity(keys_buffer);

    handle_send(keys_buffer);
}

// Low frequency clock configuration
static void lfclk_config(void)
{
    nrf_drv_clock_init();

    nrf_drv_clock_lfclk_request(NULL);
}

// RTC peripheral configuration
static void rtc_config(void)
{
    //Initialize RTC instance
    nrf_drv_rtc_init(&rtc, NULL, tick);

    //Enable tick event & interrupt
    nrf_drv_rtc_tick_enable(&rtc, true);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}

int main()
{
    // Initialize Gazell
    nrf_gzll_init(NRF_GZLL_MODE_DEVICE);

    // Attempt sending every packet up to 64 times
    nrf_gzll_set_max_tx_attempts(64);
    nrf_gzll_set_timeslots_per_channel(TIMESOLTS_PER_CHANNEL);
    nrf_gzll_set_channel_table(channel_table, CHANNEL_TABLE_SIZE);
    nrf_gzll_set_timeslots_per_channel_when_device_out_of_sync(CHANNEL_TABLE_SIZE * TIMESOLTS_PER_CHANNEL);
    //nrf_gzll_set_device_channel_selection_policy(NRF_GZLL_DEVICE_CHANNEL_SELECTION_POLICY_USE_SUCCESSFUL);
    nrf_gzll_set_datarate(NRF_GZLL_DATARATE_1MBIT);
    nrf_gzll_set_timeslot_period(TIMESOLT_PERIOD);
    nrf_gzll_set_tx_power(TX_POWER_LEVEL_MIN);

//    For NRF_GZLL_DATARATE_2MBIT the timeslot period must be >= 600 us.
//    For NRF_GZLL_DATARATE_1MBIT the timeslot period must be >= 900 us.
//    For NRF_GZLL_DATARATE_250KBIT the timeslot period must be >= 2700 us.


    // Addressing
    nrf_gzll_set_base_address_0(BASE_ADDRESS0);
    nrf_gzll_set_base_address_1(BASE_ADDRESS1);

    // Enable Gazell to start sending over the air
    nrf_gzll_enable();

    // Configure 32kHz xtal oscillator
    lfclk_config();

    // Configure RTC peripherals with ticks
    rtc_config();

    // Configure all keys as inputs with pullups
    gpio_config();

    // Main loop, constantly sleep, waiting for RTC and gpio IRQs
    while(1)
    {
        __SEV();
        __WFE();
        __WFE();
    }
}


/*****************************************************************************/
/** Gazell callback function definitions  */
/*****************************************************************************/
static uint32_t ack_payload_length = NRF_GZLL_CONST_MAX_PAYLOAD_LENGTH;
void  nrf_gzll_device_tx_success(uint32_t pipe, nrf_gzll_device_tx_info_t tx_info)
{
    if (tx_info.payload_received_in_ack)
    {
        // Pop packet and write first byte of the payload to the GPIO port.
        nrf_gzll_fetch_packet_from_rx_fifo(pipe, ack_payload, &ack_payload_length);
    }
}

// no action is taken when a packet fails to send, this might need to change
void nrf_gzll_device_tx_failed(uint32_t pipe, nrf_gzll_device_tx_info_t tx_info){}

// Callbacks not needed
void nrf_gzll_host_rx_data_ready(uint32_t pipe, nrf_gzll_host_rx_info_t rx_info){}
void nrf_gzll_disabled(){}
