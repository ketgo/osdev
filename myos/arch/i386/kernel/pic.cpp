#include <stdint.h>

#include <i386/pic.hpp>

// PIC 1 register port addresses
#define PIC1_REG_COMMAND 0x20 // command register
#define PIC1_REG_STATUS 0x20  // status register
#define PIC1_REG_DATA 0x21    // data register
#define PIC1_REG_IMR 0x21     // interrupt mask register (imr)

// PIC 2 register port addresses
#define PIC2_REG_COMMAND 0xA0 // command register
#define PIC2_REG_STATUS 0xA0  // status register
#define PIC2_REG_DATA 0xA1    // data register
#define PIC2_REG_IMR 0xA1     // interrupt mask register (imr)

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
class PIC
{
public:
    /**
         * Initialize PIC
         * 
         * @param base base address of interrupt vector table
         */
    void initialize(uint8_t base);

    /**
         * Send data bytes to PIC
         * 
         * @param data byte of data to send
         */
    void send_data(uint8_t data);

    /**
         * Read data bytes from PIC
         * 
         * @returns data read from PIC
         */
    uint8_t read_data();
};

// Master and slave PIC
static PIC m_pic, s_pic;