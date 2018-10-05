#ifndef TETRIS_EFFECT_H
#define TETRIS_EFFECT_H

#include "Main.h"

extern int FieldEffectBuff[FIELD_HEIGHT][FIELD_WIDTH];

void EffectControl(int field[FIELD_HEIGHT][FIELD_WIDTH]);
void ChainEffect(int field[FIELD_HEIGHT][FIELD_WIDTH] /*int chainHeight*/, int BuffChainCheck[10]);

#endif // !TUTORIAL_H
