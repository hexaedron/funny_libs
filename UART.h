#pragma once

#define APB_CLOCK FUNCONF_SYSTEM_CORE_CLOCK
#define OVER8DIV 4

#define RX_BUFFER_SIZE 80
uint8_t rxBuffer[RX_BUFFER_SIZE] = {0};
volatile uint8_t rxBufferHead = 0;
volatile uint8_t rxBufferTail = 0;

class UART
{
  public:
    void begin(unsigned long);
    void beginHD(unsigned long);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void fillBuff(uint8_t*, size_t);
    void flush(void);
    int availableForWrite();
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    size_t write(const char * buffer, size_t size)
    {
        return write((uint8_t*) buffer, size);
    }
    size_t write(const char *str)
    {
        return write((uint8_t*) str, strlen(str));
    }
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }
};

void UART::begin(unsigned long baud) 
{
    // Hardware Serial Pins D5 / D6
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1; // Enable UART
    GPIOD->CFGLR &= ~(0xf<<(4*5));
    GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*5);

    GPIOD->CFGLR &= ~(0xf<<(4*6));
    GPIOD->CFGLR |= (GPIO_CNF_IN_FLOATING)<<(4*6);

    USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx;
    USART1->CTLR2 = USART_StopBits_1;
    USART1->CTLR3 = USART_HardwareFlowControl_None;

    // Set Baudrate
    uint32_t integerDivider = ((25 * APB_CLOCK)) / (OVER8DIV * baud);
    uint32_t fractionalDivider = integerDivider % 100;

    USART1->BRR = ((integerDivider / 100) << 4) | (((fractionalDivider * (OVER8DIV * 2) + 50) / 100) & 7);

    // Enable Interrupt
    USART1->CTLR1 |= USART_FLAG_RXNE;
    NVIC_EnableIRQ(USART1_IRQn);

    // Enable UART
    USART1->CTLR1 |= CTLR1_UE_Set;
}

void UART::beginHD(unsigned long baud) 
{
    // Hardware Serial Pins D5 / D6
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1; // Enable UART

    USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx;
    USART1->CTLR2 = USART_StopBits_1;
    USART1->CTLR3 = USART_HardwareFlowControl_None | USART_CTLR3_HDSEL;

    // From RM:
    // After setting to half duplex mode, you need to set the IO port of TX to open-drain output high mode. 
    GPIOD->CFGLR &= ~(0xf<<(4*5));
    GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_OD_AF)<<(4*5);
    GPIOD->BSHR = 1<<(5);

    // Set Baudrate
    uint32_t integerDivider = ((25 * APB_CLOCK)) / (OVER8DIV * baud);
    uint32_t fractionalDivider = integerDivider % 100;

    USART1->BRR = ((integerDivider / 100) << 4) | (((fractionalDivider * (OVER8DIV * 2) + 50) / 100) & 7);

    // Enable Interrupt
    USART1->CTLR1 |= USART_FLAG_RXNE;
    NVIC_EnableIRQ(USART1_IRQn);

    // Enable UART
    USART1->CTLR1 |= CTLR1_UE_Set;
}

void UART::end() 
{
    USART1->CTLR1 &= CTLR1_UE_Reset;
}

int UART::available() 
{
    return rxBufferTail - rxBufferHead;
}

int UART::read() 
{
    if(rxBufferHead == rxBufferTail) return -1;

    uint8_t c = rxBuffer[rxBufferHead];

    rxBufferHead = (rxBufferHead + 1) % RX_BUFFER_SIZE;
    if(rxBufferHead != rxBufferTail) 
    {
        USART1->CTLR1 |= USART_FLAG_RXNE;
    }

    return c;
}

void UART::fillBuff(uint8_t* buf, size_t length)
{
    uint8_t offset = 0;

    while (this->available() && (offset < length))
    {
        buf[offset] = (uint8_t)this->read();
        offset++;
    } 
}

int UART::peek(void)
{
    if(rxBufferHead == rxBufferTail) 
        return -1;

    return rxBuffer[rxBufferHead];
}

int UART::availableForWrite() 
{
    return USART1->CTLR1 & CTLR1_UE_Set;
}

size_t UART::write(uint8_t c) 
{
    while( !(USART1->STATR & USART_FLAG_TC));
    USART1->DATAR = c;
    return 1;
}

size_t UART::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;
    while (n <= size) 
    {
        this->write(buffer[n++]);
    }
    return n;
}

void UART::flush(void)
{
    rxBufferHead = 0;
    rxBufferTail = 0;

    memset(rxBuffer, 0, sizeof(rxBuffer));
    USART1->CTLR1 |= USART_FLAG_RXNE;
}

extern "C" void USART1_IRQHandler( void ) INTERRUPT_HANDLER;
extern "C" void USART1_IRQHandler(void) 
{
    if(USART1->STATR & USART_FLAG_RXNE) 
    {
        // Write into buffer
        rxBuffer[rxBufferTail] = USART1->DATAR & (uint16_t)0x01FF;

        rxBufferTail = (rxBufferTail + 1) % RX_BUFFER_SIZE;

        if(rxBufferTail == rxBufferHead) 
        {
            // Buffer empty, disable USART interrupt
            USART1->CTLR1 &= ~USART_FLAG_RXNE;
            return;
        }
    }
}