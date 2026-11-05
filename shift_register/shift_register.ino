/* shift register functions 
 * Douglas McCulloch 2025
 * X-Band Labs, ORC, UoS
 */

#include <limits.h>

#define N_SR 1 /* number of shift registers */

/* pin definitions (4 and 5 render delay() unusable) */
#define SER 2
#define OE 3
#define RCLK 4
#define SRCLK 5

/* figure out another way to ensure the intented SR state is 
 * applied. Must find a way to check this as far as possible.
 */
#define CHKPTR(p) if (!p) { return -1; }

void toggle_SRCLK();
void toggle_RCLK();
#ifdef USE_SRCLR
#define SRCLR 6
void clear_SR();
#endif
int8_t set_SR_state(uint8_t *state, size_t n_sr);
int8_t switch_MEMS(uint8_t mask);

void toggle_SRCLK()
{
  /* 'duino nano should be slow enough for 25 MHz
   *  SN74HC595 has this rise time
   */
  digitalWrite(SRCLK, HIGH);
  digitalWrite(SRCLK, LOW);
}

void toggle_RCLK()
{
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}

#ifdef USE_SRCLR
void clear_SR()
{
  digitalWrite(SRCLR, LOW);
  toggle_RCLK();
  digitalWrite(SRCLR, HIGH);
}
#endif

int8_t set_SR_state(uint8_t *state, size_t n_sr)
{
  size_t i, b;

  for (i = n_sr; i > 0; --i)
  {
    uint8_t sr_state;

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

    state[0] = mask;

    ret = set_SR_state(state, N_SR);

    return ret;
}
