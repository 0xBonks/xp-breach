#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "statemanager.h"

void InitUIComponents();
PcState UpdatePowerButton(PcState currentState);
void DrawPowerButton(PcState currentState);
void UnloadUIComponents();

#endif
