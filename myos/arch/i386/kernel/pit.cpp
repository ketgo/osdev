#include <stdint.h>
#include <time.h>

#include <kernel/ioport.hpp>
#include <kernel/isr.hpp>

#include <i386/pit.hpp>

//-----------------------------------------------
// Clock Settings
//-----------------------------------------------

/** Internal clock frequency is 1193180 Hz. */
#define TIMER_HZ 1193180 // Built-in timer max frequency

/**
 * The value we send to the PIT is the value to divide it's input
 * clock (1193180 Hz) to get the required frequency.
 * The divisor must be small enough to fit into 16-bits.
 *
 * TIMER_ARCH_FREQ / freq < 65536 => frequency > 18,20
 */
#define TIMER_DIVISOR (TIMER_HZ / CLOCKS_PER_SEC)

//-----------------------------------------------
//	Controller Registers
//-----------------------------------------------

#define PIT_REG_COUNTER_0 0x40
#define PIT_REG_COUNTER_1 0x41
#define PIT_REG_COUNTER_2 0x42
#define PIT_REG_COMMAND 0x43

//-----------------------------------------------
//	Operational Command Bit masks
//-----------------------------------------------

#define PIT_OCW_MASK_BINCOUNT 1   //00000001
#define PIT_OCW_MASK_MODE 0xE     //00001110
#define PIT_OCW_MASK_RL 0x30      //00110000
#define PIT_OCW_MASK_COUNTER 0xC0 //11000000

//-----------------------------------------------
//	Operational Command control bits
//-----------------------------------------------

//! Use when setting binary count mode
#define PIT_OCW_BINCOUNT_BINARY 0 //0
#define PIT_OCW_BINCOUNT_BCD 1    //1

//! Use when setting counter mode
#define PIT_OCW_MODE_TERMINALCOUNT 0   //0000
#define PIT_OCW_MODE_ONESHOT 0x2       //0010
#define PIT_OCW_MODE_RATEGEN 0x4       //0100
#define PIT_OCW_MODE_SQUAREWAVEGEN 0x6 //0110
#define PIT_OCW_MODE_SOFTWARETRIG 0x8  //1000
#define PIT_OCW_MODE_HARDWARETRIG 0xA  //1010

//! Use when setting data transfer
#define PIT_OCW_RL_LATCH 0      //000000
#define PIT_OCW_RL_LSBONLY 0x10 //010000
#define PIT_OCW_RL_MSBONLY 0x20 //100000
#define PIT_OCW_RL_DATA 0x30    //110000

//! Use when setting the counter we are working with
#define PIT_OCW_COUNTER_0 0    //00000000
#define PIT_OCW_COUNTER_1 0x40 //01000000
#define PIT_OCW_COUNTER_2 0x80 //10000000

/**
 * Tick count used for system timer
 */
static volatile uint32_t ticks = 0;

/**
 * Interrupt handler for PIT 
 * 
 * This interrupt is triggered by the PIT counter 0. On each trigger
 * the tick count is increased indicating change in system clock.
 */
static void pit_isr(kernel::ISRFrame *const frame)
{
    ticks++;
}

/**
 * Send command to PIT
 * 
 * @param cmd command to send
 */
static void send_cmd(uint8_t cmd)
{
    return kernel::outb(cmd, PIT_REG_COMMAND);
}

/**
 * Send data bytes to PIT
 * 
 * @param counter counter four which to send data
 * @param data byte of data to send
 */
static void send_data(uint8_t counter, uint8_t data)
{
    uint8_t port = (counter == PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER_0 : ((counter == PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER_1 : PIT_REG_COUNTER_2);

    kernel::outb(data, port);
}

/**
 * Read data bytes from PIT
 * 
 * @param counter counter for which to read data
 * @returns data read from PIC
 */
static uint8_t read_data(uint8_t counter)
{
    uint8_t port = (counter == PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER_0 : ((counter == PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER_1 : PIT_REG_COUNTER_2);

    return kernel::inb(port);
}

void I386::PIT::setup()
{
    // Send operational command
    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | PIT_OCW_MODE_RATEGEN;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | PIT_OCW_COUNTER_0;
    send_cmd(ocw);

    // Set frequency rate
    send_data(TIMER_DIVISOR & 0xff, PIT_OCW_COUNTER_0);
    send_data((TIMER_DIVISOR >> 8) & 0xff, PIT_OCW_COUNTER_0);

    kernel::IVT::register_isr(ISR_TIMER, pit_isr);
}

uint32_t I386::PIT::get_ticks()
{
    return ticks;
}
