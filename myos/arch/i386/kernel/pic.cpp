#include <stdint.h>

#include <kernel/ioport.hpp>

#include <i386/pic.hpp>

// Master PIC register port addresses
#define MASTER_PIC_REG1 0x20 // command and status register
#define MASTER_PIC_REG2 0x21 // data and interrupt mask register

// Slave PIC register port addresses
#define SLAVE_PIC_REG1 0xA0 // command and status register
#define SLAVE_PIC_REG2 0xA1 // data and interrupt mask register

// Initialization Control Word 1 bit masks
#define PIC_ICW1_MASK_IC4 0x1   //00000001	// Expect ICW 4 bit
#define PIC_ICW1_MASK_SNGL 0x2  //00000010	// Single or Cascaded
#define PIC_ICW1_MASK_ADI 0x4   //00000100	// Call Address Interval
#define PIC_ICW1_MASK_LTIM 0x8  //00001000	// Operation Mode
#define PIC_ICW1_MASK_INIT 0x10 //00010000	// Initialization Command

// Initialization Control Words 2 and 3 do not require bit masks

// Initialization Control Word 4 bit masks
#define PIC_ICW4_MASK_UPM 0x1   //00000001
#define PIC_ICW4_MASK_AEOI 0x2  //00000010
#define PIC_ICW4_MASK_MS 0x4    //00000100
#define PIC_ICW4_MASK_BUF 0x8   //00001000
#define PIC_ICW4_MASK_SFNM 0x10 //00010000

// Command Word 2 bit masks. Use when sending commands
#define PIC_OCW2_MASK_L1 1        //00000001	//Level 1 interrupt level
#define PIC_OCW2_MASK_L2 2        //00000010	//Level 2 interrupt level
#define PIC_OCW2_MASK_L3 4        //00000100	//Level 3 interrupt level
#define PIC_OCW2_MASK_EOI 0x20    //00100000	//End of Interrupt command
#define PIC_OCW2_MASK_SL 0x40     //01000000	//Select command
#define PIC_OCW2_MASK_ROTATE 0x80 //10000000	//Rotation command

// Command Word 3 bit masks. Use when sending commands
#define PIC_OCW3_MASK_RIS 1     //00000001
#define PIC_OCW3_MASK_RIR 2     //00000010
#define PIC_OCW3_MASK_MODE 4    //00000100
#define PIC_OCW3_MASK_SMM 0x20  //00100000
#define PIC_OCW3_MASK_ESMM 0x40 //01000000
#define PIC_OCW3_MASK_D7 0x80   //10000000

//-----------------------------------------------
//	Initialization Command 1 control bits
//-----------------------------------------------

#define PIC_ICW1_IC4_EXPECT 1          //1
#define PIC_ICW1_IC4_NO 0              //0
#define PIC_ICW1_SNGL_YES 2            //10
#define PIC_ICW1_SNGL_NO 0             //00
#define PIC_ICW1_ADI_CALLINTERVAL4 4   //100
#define PIC_ICW1_ADI_CALLINTERVAL8 0   //000
#define PIC_ICW1_LTIM_LEVELTRIGGERED 8 //1000
#define PIC_ICW1_LTIM_EDGETRIGGERED 0  //0000
#define PIC_ICW1_INIT_YES 0x10         //10000
#define PIC_ICW1_INIT_NO 0             //00000

//-----------------------------------------------
//	Initialization Command 4 control bits
//-----------------------------------------------

#define PIC_ICW4_UPM_86MODE 1         //1
#define PIC_ICW4_UPM_MCSMODE 0        //0
#define PIC_ICW4_AEOI_AUTOEOI 2       //10
#define PIC_ICW4_AEOI_NOAUTOEOI 0     //0
#define PIC_ICW4_MS_BUFFERMASTER 4    //100
#define PIC_ICW4_MS_BUFFERSLAVE 0     //0
#define PIC_ICW4_BUF_MODEYES 8        //1000
#define PIC_ICW4_BUF_MODENO 0         //0
#define PIC_ICW4_SFNM_NESTEDMODE 0x10 //10000
#define PIC_ICW4_SFNM_NOTNESTED 0     //a binary 2

/**
 * Programmable Interrupt Controller class
 */
typedef class PIC
{
private:
    /**
     * 8259 command and status register address
     */
    uint8_t reg1;

    /**
     * 8259 data and IMR register address
     */
    uint8_t reg2;

public:
    /**
     * Constructor
     */
    PIC(uint8_t reg1, uint8_t reg2) : reg1(reg1), reg2(reg2) {}

    /**
     * Send command to PIC
     * 
     * @param cmd command to send
     */
    void send_cmd(uint8_t cmd)
    {
        return kernel::outb(cmd, reg1);
    }

    /**
     * Get PIC status
     * 
     * @returns status byte
     */
    uint8_t get_status()
    {
        return kernel::inb(reg1);
    }

    /**
     * Send data bytes to PIC
     * 
     * @param data byte of data to send
     */
    void send_data(uint8_t data)
    {
        kernel::outb(data, reg2);
    }

    /**
     * Read data bytes from PIC
     * 
     * @returns data read from PIC
     */
    uint8_t read_data()
    {
        return kernel::inb(reg2);
    }
} pic_t;

// Master and slave PIC
static pic_t m_pic(MASTER_PIC_REG1, MASTER_PIC_REG2);
static pic_t s_pic(SLAVE_PIC_REG1, SLAVE_PIC_REG2);

void I386::PIC::setup()
{
    uint8_t icw = 0; // initialization control word

    // Begin initialization of PIC

    icw = (icw & ~PIC_ICW1_MASK_INIT) | PIC_ICW1_INIT_YES;
    icw = (icw & ~PIC_ICW1_MASK_IC4) | PIC_ICW1_IC4_EXPECT;

    m_pic.send_cmd(icw);
    s_pic.send_cmd(icw);

    // Send initialization control word 2. This is the base addresses of the irq's.
    // Since the first 32 interrupt vectors are reserved in x86 processors, the hardware
    // interrupts are set from the 32nd vector with each PIC supporting 8 interrupt lines.
    //
    // NOTE: The interrupt numbers are consitent with that set in IDT

    m_pic.send_data(32);
    s_pic.send_data(40);

    // Send initialization control word 3. This is the connection between master and slave.
    // ICW3 for master PIC is the IR that connects to secondary pic in binary format
    // ICW3 for secondary PIC is the IR that connects to master pic in decimal format

    m_pic.send_data(0x04);
    s_pic.send_data(0x02);

    // Send Initialization control word 4. Enables i86 mode

    icw = (icw & ~PIC_ICW4_MASK_UPM) | PIC_ICW4_UPM_86MODE;

    m_pic.send_data(icw);
    s_pic.send_data(icw);
}

void I386::PIC::mask(uint8_t n)
{
    pic_t *pic;
    uint8_t value;

    // Select the master or slave PIC object depending on the interrupt line to mask.
    if (n < 8)
    {
        pic = &m_pic;
    }
    else
    {
        pic = &s_pic;
        n -= 8;
    }
    // Get the value of interrupt mask register and mask the nth bit
    value = pic->read_data() | (1 << n);
    // Load the new mask value in interrupt mask register
    pic->send_data(value);
}

void I386::PIC::unmask(uint8_t n)
{
    pic_t *pic;
    uint8_t value;

    // Select the master or slave PIC object depending on the interrupt line to mask.
    if (n < 8)
    {
        pic = &m_pic;
    }
    else
    {
        pic = &s_pic;
        n -= 8;
    }
    // Get the value of interrupt mask register and unmask the nth bit
    value = pic->read_data() & ~(1 << n);
    // Load the new mask value in interrupt mask register
    pic->send_data(value);
}
