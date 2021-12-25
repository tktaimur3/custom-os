// Code from http://www.brokenthorn.com/Resources/OSDev16.html

// The following devices use PIC 1 to generate interrupts
#define		PIC_IRQ_TIMER		    0
#define		PIC_IRQ_KEYBOARD		1
#define		PIC_IRQ_SERIAL2		    3
#define		PIC_IRQ_SERIAL1		    4
#define		PIC_IRQ_PARALLEL2		5
#define		PIC_IRQ_DISKETTE		6
#define		PIC_IRQ_PARALLEL1		7
 
// The following devices use PIC 2 to generate interrupts
#define		PIC_IRQ_CMOSTIMER		0
#define		PIC_IRQ_CGARETRACE		1
#define		PIC_IRQ_AUXILIARY		4
#define		PIC_IRQ_FPU			    5
#define		PIC_IRQ_HDC			    6

//! Command Word 2 bit masks. Use when sending commands
#define		PIC_OCW2_MASK_L1		1		    //00000001	//Level 1 interrupt level
#define		PIC_OCW2_MASK_L2		2		    //00000010	//Level 2 interrupt level
#define		PIC_OCW2_MASK_L3		4		    //00000100	//Level 3 interrupt level
#define		PIC_OCW2_MASK_EOI		0x20		//00100000	//End of Interrupt command
#define		PIC_OCW2_MASK_SL		0x40		//01000000	//Select command
#define		PIC_OCW2_MASK_ROTATE	0x80		//10000000	//Rotation command

// Command word 3 bit masks
#define		PIC_OCW3_MASK_RIS		1		//00000001
#define		PIC_OCW3_MASK_RIR		2		//00000010
#define		PIC_OCW3_MASK_MODE		4		//00000100
#define		PIC_OCW3_MASK_SMM		0x20		//00100000
#define		PIC_OCW3_MASK_ESMM		0x40		//01000000
#define		PIC_OCW3_MASK_D7		0x80		//10000000

// Port addresses for PIC 1 and PIC 2
#define     PIC1_REG_COMMAND		0x20			// command register
#define     PIC1_REG_STATUS			0x20			// status register
#define     PIC1_REG_DATA			0x21			// data register
#define     PIC1_REG_IMR			0x21			// interrupt mask register (imr)
 
#define     PIC2_REG_COMMAND		0xA0			// ^ see above register names
#define     PIC2_REG_STATUS			0xA0
#define     PIC2_REG_DATA			0xA1
#define     PIC2_REG_IMR			0xA1

// Initialization Control Word 1 bit masks
#define PIC_ICW1_MASK_IC4			0x1	//00000001	// Expect ICW 4 bit
#define PIC_ICW1_MASK_SNGL			0x2	//00000010	// Single or Cascaded
#define PIC_ICW1_MASK_ADI			0x4	//00000100	// Call Address Interval
#define PIC_ICW1_MASK_LTIM			0x8	//00001000	// Operation Mode
#define PIC_ICW1_MASK_INIT			0x10	//00010000	// Initialization Command

#define PIC_ICW1_IC4_EXPECT			    1	//1		//Use when setting PIC_ICW1_MASK_IC4
#define PIC_ICW1_IC4_NO			        0	//0
#define PIC_ICW1_SNGL_YES			    2	//10		//Use when setting PIC_ICW1_MASK_SNGL
#define PIC_ICW1_SNGL_NO			    0	//00
#define PIC_ICW1_ADI_CALLINTERVAL4		4	//100		//Use when setting PIC_ICW1_MASK_ADI
#define PIC_ICW1_ADI_CALLINTERVAL8		0	//000
#define PIC_ICW1_LTIM_LEVELTRIGGERED	8	//1000		//Use when setting PIC_ICW1_MASK_LTIM
#define PIC_ICW1_LTIM_EDGETRIGGERED		0	//0000
#define PIC_ICW1_INIT_YES			    0x10	//10000		//Use when setting PIC_ICW1_MASK_INIT
#define PIC_ICW1_INIT_NO			    0	//00000

#define PIC_ICW4_MASK_UPM		        0x1	//00000001	// Mode
#define PIC_ICW4_MASK_AEOI		        0x2	//00000010	// Automatic EOI
#define PIC_ICW4_MASK_MS		        0x4	//00000100	// Selects buffer type
#define PIC_ICW4_MASK_BUF		        0x8	//00001000	// Buffered mode
#define PIC_ICW4_MASK_SFNM		        0x10	//00010000	// Special fully-nested mode

#define PIC_ICW4_UPM_86MODE		        1	//1		//Use when setting PIC_ICW4_MASK_UPM
#define PIC_ICW4_UPM_MCSMODE	        0	//0
#define PIC_ICW4_AEOI_AUTOEOI	        2	//10		//Use when setting PIC_ICW4_MASK_AEOI
#define PIC_ICW4_AEOI_NOAUTOEOI	        0	//00
#define PIC_ICW4_MS_BUFFERMASTER	    4	//100		//Use when setting PIC_ICW4_MASK_MS
#define PIC_ICW4_MS_BUFFERSLAVE	        0	//000
#define PIC_ICW4_BUF_MODEYES	        8	//1000		//Use when setting PIC_ICW4_MASK_BUF
#define PIC_ICW4_BUF_MODENO		        0	//0000
#define PIC_ICW4_SFNM_NESTEDMODE	    0x10	//10000		//Use when setting PIC_ICW4_MASK_SFNM
#define PIC_ICW4_SFNM_NOTNESTED	        0	//00000

inline void pic_send_cmd (char cmd, char pic);
inline void pic_send_data (char data, char pic);
inline void pic_read_data (char data, char pic);