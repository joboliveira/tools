#include <inttypes.h>


typedef struct sirgnal_t{
  float amplitude;
  float frequency;
  float time;
  float phase;
  float samplerate;
  float output;
} SIGNAL_T;

//SIGNAL_T signal;

int senoidal(SIGNAL_T *this);

int cosenoidal(SIGNAL_T *this);
