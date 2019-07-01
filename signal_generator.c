#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include "signal_generator.h"

#define PI 3.141592654
/*
typedef struct sirgnal_t{
  float amplitude;
  float frequency;
  float time;
  float phase;
  float output;
} SIGNAL_T;
*/

// For generate a signal
// Y[n] = K*Y[n-1]-Y[n-2]
//

float k;

int senoidal(SIGNAL_T *this)
{
  SIGNAL_T *signal;
  signal = this;

  if(signal == NULL)
  {
    return 1;
  }
  signal->output = signal->amplitude*sin((2.0*(PI/180.0)*signal->frequency*signal->time)+signal->phase);
  return 0;
}

int cosenoidal(SIGNAL_T *this)
{
  SIGNAL_T *signal;
  signal = this;
  if(signal == NULL)
  {
    return 1;
  }
  signal->output = signal->amplitude*sin((2*PI*signal->frequency*signal->time)+signal->phase);
  return 0;
}
