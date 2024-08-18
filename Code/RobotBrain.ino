#include <avr/wdt.h>
#include "Funcs.h"

void setup()
{
  Functions.Init();
}

void loop()
{
  Functions.ParseSerialInput();
}
