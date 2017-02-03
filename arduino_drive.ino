// Based off code by instructables.com user chr. I take no credit for his work

void setup()
{
 
  TCCR0A = (0x01<<COM0A0) | (0x01<<WGM01); // CTC mode, toglle OC0A on compare match
  TCCR0B = (0x01<<CS00); // cpu clock, no prescaler
  OCR0A = 209; // Toggle/clear at 209 to give 38khz at 16mhz clock speed
  
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("start");
  
}


void loop()
{
  ir_off();
  
  unsigned char i;
 
  unsigned char command = 0x00;
   while (true)
   {
     
     //send_command(0x90); // red
     send_command(command);
     
     Serial.println(command, BIN);
     
     for (i=0;i<200;i++)
     {
       delayMicroseconds(200);
     }
     
     command++;
   }
}
 

void send_command(unsigned char data)
{
 unsigned char i;
  
 command_init();
 
 unsigned char address = 0x00;
 
 send_ir_byte(address);
 send_ir_byte(address ^ 0xff);
 
 send_ir_byte(data);
 send_ir_byte(data ^ 0xff);
 
 send_bit_low(); // stop-bit
}

void send_ir_byte(unsigned char data)
{
  unsigned char i;

  for (i=0;i<8;i++) // MSB first
  {
    if ((data<<i) & 0x80)
    {
      send_bit_high();
    } else
    {
      send_bit_low();
    }
  }  
}

void send_bit_high()
{
  // 1.65ms
  ir_on();
  delayMicroseconds(560);
  ir_off();
  delayMicroseconds(1650);
}

void send_bit_low()
{
  // 0.55ms
  ir_on();
  delayMicroseconds(560);
  ir_off();
  delayMicroseconds(560);
}

void command_init()
{
  ir_on();
  delayMicroseconds(9000);
  ir_off();
  delayMicroseconds(4500);
}

void ir_on()
{
  PORTD |= 0x80; // debug output whithout carrier frequency
  
  TIFR0 = (0x01<<TOV0);
  TCNT0 = 0;
  TCCR0A = (0x01<<COM0A0) | (0x01<<WGM01); 
  TCCR0B = (0x01<<CS00); // start the timer, no prescaler
}

void ir_off()
{
  PORTD &= 0b01111111; // debug output whithout carrier frequency
  
  TCCR0B = 0x00; // stop the timer
  TCCR0A = (0x01<<WGM01);
  PORTD &= 0b10111111;
}

