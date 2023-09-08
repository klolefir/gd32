#include "adf4372.h"
#include "systick.h"

static void adf4372_spi_init();
static void adf4372_cs_init();
static void adf4372_init_addr_struct();
static void adf4372_init_val_struct();

static gpio_t cs_pin;
static spi_t spi;

enum {
	init_regs_num = 54
};

static uint8_t init_addr[init_regs_num];
static uint8_t init_val[init_regs_num];


enum {
	rw_op_offset = 16,
	write_op = 0,
	read_op = 1
};

void adf4372_init()
{
	uint32_t addr;
	uint8_t data;
	uint8_t temp; 
	int i;

	adf4372_cs_init();
	adf4372_spi_init();
	adf4372_init_addr_struct();
	adf4372_init_val_struct();

	adf4372_select();
	
	addr = adf4372_reg0000;
	data = 0x18;
	/*temp = (write_op << 7) | (addr >> 8);*/
	temp = addr >> 8;
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#if 0
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#endif
	

	addr = adf4372_reg0001;
	data = 0x80;
	/*temp = (write_op << 7) | (addr >> 8);*/
	temp = (addr >> 8);
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#if 0
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#endif

	for(i = init_regs_num - 1; i >= 0; i--) {
		temp = init_addr[i] >> 8;
		xspi_sendrecv_byte(&spi, &temp);
		temp = (uint8_t)init_addr[i];
		xspi_sendrecv_byte(&spi, &temp);
		temp = init_val[i];
		xspi_sendrecv_byte(&spi, &temp);
	}


	adf4372_deselect();
}

void adf4372_read_reg(const uint8_t addr, uint8_t *reg)
{
	uint8_t temp;

	adf4372_select();

	*reg = 0x00;
	temp = (read_op << 7);
	xspi_send_byte(&spi, temp);
	temp = addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, reg);

	adf4372_deselect();
}

void adf4372_get_id(uint16_t *id)
{
	adf4372_select();
	
	uint32_t addr;
	uint8_t data;
	uint8_t temp;

	addr = adf4372_reg0004; 
	data = 0x00;
	temp = (read_op << 7) | (addr >> 8);
	xspi_send_byte(&spi, temp);
	temp = (uint8_t)addr;
	xspi_send_byte(&spi, temp);
	xspi_recv_byte(&spi, &data);
#if 0
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#endif
	*id = (data << 8);


	addr = adf4372_reg0005; 
	data = 0x00;
	temp = (read_op << 7) | (addr >> 8);
	xspi_send_byte(&spi, temp);
	temp = (uint8_t)addr;
	xspi_send_byte(&spi, temp);
	xspi_recv_byte(&spi, &data);
#if 0
	xspi_sendrecv_byte(&spi, &temp);
	temp = (uint8_t)addr;
	xspi_sendrecv_byte(&spi, &temp);
	xspi_sendrecv_byte(&spi, &data);
#endif
	*id |= (data);
#if 0
	xspi_sendrecv_byte(&spi, &data);
	*id = (data << 8);
	data = 0x00;
	xspi_sendrecv_byte(&spi, &data);
	*id |= data;
#endif

	adf4372_deselect();
}

void adf4372_select()
{
	xgpio_clr(&cs_pin);
}

void adf4372_deselect()
{
	xgpio_set(&cs_pin);
}

void adf4372_spi_init()
{
	spi.spi				= adf4372_spi_num;
	spi.dev_mode		= spi_dev_mode_master;
	/*spi.trans_mode		= spi_trans_mode_bdtransmit;*/
	spi.trans_mode		= spi_trans_mode_fullduplex;
	spi.frame_size		= spi_frame_size_8bit;
	spi.nss				= spi_nss_soft;
	spi.endian			= spi_endian_msb;
	spi.polarity		= spi_polarity_high;
	spi.phase			= spi_phase_1edge;
	spi.psc				= adf4372_spi_psc;
	spi.trans_irq_state = spi_trans_irq_off;
	spi.recv_irq_state 	= spi_recv_irq_off;
	xspi_init(&spi);
}

void adf4372_cs_init()
{
	cs_pin.port = adf4372_cs_port;
	cs_pin.pin = adf4372_cs_pin;
	cs_pin.mode = gpio_mode_out_pp;
	cs_pin.speed = gpio_ospeed_50mhz;
	xgpio_init(&cs_pin);
	xgpio_set(&cs_pin);
}

void adf4372_init_addr_struct()
{
	init_addr[0] = 0x10;
	init_addr[1] = 0x11;
	init_addr[2] = 0x12;
	init_addr[3] = 0x14;
	init_addr[4] = 0x15;
	init_addr[5] = 0x16;
	init_addr[6] = 0x17;
	init_addr[7] = 0x18;
	init_addr[8] = 0x19;
	init_addr[9] = 0x1a;
	init_addr[10] = 0x1b;
	init_addr[11] = 0x1c;
	init_addr[12] = 0x1d;
	init_addr[13] = 0x1e;
	init_addr[14] = 0x1f;
	init_addr[15] = 0x20;
	init_addr[16] = 0x22;
	init_addr[17] = 0x23;
	init_addr[18] = 0x24;
	init_addr[19] = 0x25;
	init_addr[20] = 0x26;
	init_addr[21] = 0x27;
	init_addr[22] = 0x28;
	init_addr[23] = 0x2a;
	init_addr[24] = 0x2b;
	init_addr[25] = 0x2c;
	init_addr[26] = 0x2d;
	init_addr[27] = 0x2e;
	init_addr[28] = 0x2f;
	init_addr[29] = 0x30;
	init_addr[30] = 0x31;
	init_addr[31] = 0x32;
	init_addr[32] = 0x33;
	init_addr[33] = 0x34;
	init_addr[34] = 0x35;
	init_addr[35] = 0x36;
	init_addr[36] = 0x37;
	init_addr[37] = 0x38;
	init_addr[38] = 0x39;
	init_addr[39] = 0x3a;
	init_addr[40] = 0x3d;
	init_addr[41] = 0x3e;
	init_addr[42] = 0x3f;
	init_addr[43] = 0x40;
	init_addr[44] = 0x41;
	init_addr[45] = 0x47;
	init_addr[46] = 0x52;
	init_addr[47] = 0x6e;
	init_addr[48] = 0x6f;
	init_addr[49] = 0x70;
	init_addr[50] = 0x71;
	init_addr[51] = 0x72;
	init_addr[52] = 0x73;
	init_addr[53] = 0x7c;
}

void adf4372_init_val_struct()
{
	init_addr[0] = 0x00;
	init_addr[1] = 0x00;
	init_addr[2] = 0x00;
	init_addr[3] = 0x00;
	init_addr[4] = 0x00;
	init_addr[5] = 0x00;
	init_addr[6] = 0x00;
	init_addr[7] = 0x00;
	init_addr[8] = 0x00;
	init_addr[9] = 0x00;
	init_addr[10] = 0x00;
	init_addr[11] = 0x00;
	init_addr[12] = 0x00;
	init_addr[13] = 0x00;
	init_addr[14] = 0x00;
	init_addr[15] = 0x04;
	init_addr[16] = 0x00;
	init_addr[17] = 0x00;
	init_addr[18] = 0x00;
	init_addr[19] = 0x00;
	init_addr[20] = 0x00;
	init_addr[21] = 0x00;
	init_addr[22] = 0x00;
	init_addr[23] = 0x00;
	init_addr[24] = 0x00;
	init_addr[25] = 0x00;
	init_addr[26] = 0x00;
	init_addr[27] = 0x00;
	init_addr[28] = 0x00;
	init_addr[29] = 0x00;
	init_addr[30] = 0x00;
	init_addr[31] = 0x00;
	init_addr[32] = 0x00;
	init_addr[33] = 0x00;
	init_addr[34] = 0x00;
	init_addr[35] = 0x00;
	init_addr[36] = 0x00;
	init_addr[37] = 0x00;
	init_addr[38] = 0x00;
	init_addr[39] = 0x00;
	init_addr[40] = 0x00;
	init_addr[41] = 0x00;
	init_addr[42] = 0x00;
	init_addr[43] = 0x00;
	init_addr[44] = 0x00;
	init_addr[45] = 0x00;
	init_addr[46] = 0x00;
	init_addr[47] = 0x00;
	init_addr[48] = 0x00;
	init_addr[49] = 0x00;
	init_addr[50] = 0x00;
	init_addr[51] = 0x00;
	init_addr[52] = 0x00;
	init_addr[53] = 0x00;
}
