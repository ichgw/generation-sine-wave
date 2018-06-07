#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "wave.h"

int main(int argc, char *argv[])
{
    MONO_PCM pcm;
    int n;
    double a, f0;
    char out_filename[50];
    sprintf(out_filename, "%s%s",argv[1],"Hz_sine.wav");
    pcm.fs = 44100;
    pcm.bits = 16;
    pcm.length = pcm.fs;
    pcm.s = (double *) calloc(pcm.length, sizeof(double));

    //振幅
    a = 0.5;
    //基本周波数
    f0 = atoi(argv[1]);

    for (n = 0; n < pcm.length; n++) {
        pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
    }

    //出力
    wave_write_16bit_mono(&pcm, out_filename);

    //メモリ解放
    free(pcm.s);

    return 0;
}
