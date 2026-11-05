/* 1x4 MEMS switch and shift register functions */

#include <limits.h>

#define N_SR 1 /* number of shift registers */
#define DELAY_CYCLES 1000000

#define SER 2
#define RCLK 3  /* using these pins causes delay not to work */
#define SRCLK 4
#define SRCLR 6

/* 1x4 switch states */
/* A,B,C,D,S3,S2,S1 */
#define MEMS_A 0b01000000
#define MEMS_B 0b00100101
#define MEMS_C 0b00010001
#define MEMS_D 0b00001010

/* must check for some corruption */
#define CHKPTR(p) if (!p) { return -1; }

void toggle_SRCLK();
void toggle_RCLK();

/* because delay doesn't work for some reason */
/* attempting to run delay causes arduino to hang */
void spin_delay(volatile unsigned long n_cycles);

int8_t set_SR_state(uint8_t *state, size_t n_sr);
int8_t switch_MEMS(uint8_t mask);

void toggle_SRCLK()
{
  /* 'duino nano should be slow enough for 25 MHz */
  digitalWrite(SRCLK, HIGH);
  digitalWrite(SRCLK, LOW);
}

void toggle_RCLK()
{
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}

void spin_delay(volatile unsigned long n_cycles)
{
    while (--n_cycles);
}

int8_t set_SR_state(uint8_t *state, size_t n_sr)
{
  size_t i, b;

  for (i = n_sr; i > 0; --i)
  {
    uint8_t sr_state;

    CHKPTR(state + i - 1)
    sr_state = state[i - 1];

    for (b = 0; b < CHAR_BIT; ++b)
    {
      uint8_t val;

      val = (sr_state >> b) & 1;

      digitalWrite(SER, val);
        
      toggle_SRCLK();
    }
  }

  toggle_RCLK();

  return 0;
}

int8_t switch_MEMS(uint8_t mask)
{
    uint8_t state[N_SR];
    int8_t ret;
    
    CHKPTR(state)
    state[0] = mask;

    ret = set_SR_state(state, N_SR);

    return ret;
}

int main()
{
  /* D13 internal LED is for error */
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);

  while(1)
  {
    if (switch_MEMS(MEMS_A) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);

    if (switch_MEMS(MEMS_B) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);
    if (switch_MEMS(MEMS_C) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);
    if (switch_MEMS(MEMS_D) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);
    if (switch_MEMS(MEMS_C) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);
    if (switch_MEMS(MEMS_B) < 0)
    {
      digitalWrite(13, HIGH);
      return 1;
    }
    spin_delay(DELAY_CYCLES);
  }
    
  return 0;
}
