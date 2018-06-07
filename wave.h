#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int fs;
    int bits;
    int length;
    double *s;
} MONO_PCM;

typedef struct
{
    int fs;
    int bits;
    int length;
    double *sL;
    double *sR;
} STEREO_PCM;

void wave_read_8bit_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    unsigned char data;
    int n;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = bits_per_sample;
    pcm->length = data_chunk_size;
    pcm->s = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&data, 1, 1, fp);
        pcm->s[n] = ((double)data - 128.0) / 128.0;
    }

    fclose(fp);
}

void wave_write_8bit_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    double s;
    unsigned char data;
    int n;

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + pcm->length;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    wave_format_type = 1;
    channel = 1;
    samples_per_sec = pcm->fs;
    bytes_per_sec = pcm->fs * pcm->bits / 8;
    block_size = pcm->bits / 8;
    bits_per_sample = pcm->bits;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        s = (pcm->s[n] + 1.0) / 2.0 * 256.0;

        if (s > 255.0) {
            s = 255.0;
        } else if (s < 0.0) {
            s = 0.0;
        }

        data = (unsigned char)((int)(s + 0.5));
        fwrite(&data, 1, 1, fp);
    }


    if ((pcm->length % 2) == 1) {
        data = 0;
        fwrite(&data, 1, 1, fp);
    }

    fclose(fp);
}

void wave_read_8bit_stereo(STEREO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    unsigned char data;
    int n;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = bits_per_sample;
    pcm->length = data_chunk_size / 2;
    pcm->sL = (double *) calloc(pcm->length, sizeof(double));
    pcm->sR = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&data, 1, 1, fp);
        pcm->sL[n] = ((double)data - 128.0) / 128.0;

        fread(&data, 1, 1, fp);
        pcm->sR[n] = ((double)data - 128.0) / 128.0;
    }

    fclose(fp);
}

void wave_write_8bit_stereo(STEREO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    double sL;
    double sR;
    unsigned char data;
    int n;

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + pcm->length * 2;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    wave_format_type = 1;
    channel = 2;
    samples_per_sec = pcm->fs;
    bytes_per_sec = pcm->fs * pcm->bits / 8 * 2;
    block_size = pcm->bits / 8 * 2;
    bits_per_sample = pcm->bits;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length * 2;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        sL = (pcm->sL[n] + 1.0) / 2.0 * 256.0;

        if (sL > 255.0) {
            sL = 255.0;
        }
        else if (sL < 0.0) {
            sL = 0.0;
        }

        data = (unsigned char)((int)(sL + 0.5));
        fwrite(&data, 1, 1, fp);

        sR = (pcm->sR[n] + 1.0) / 2.0 * 256.0;

        if (sR > 255.0) {
            sR = 255.0;
        }
        else if (sR < 0.0) {
            sR = 0.0;
        }

        data = (unsigned char)((int)(sR + 0.5));
        fwrite(&data, 1, 1, fp);
    }

    fclose(fp);
}

void wave_read_16bit_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    short data;
    int n;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = bits_per_sample;
    pcm->length = data_chunk_size / 2;
    pcm->s = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&data, 2, 1, fp);
        pcm->s[n] = (double)data / 32768.0;
    }

    fclose(fp);
}

void wave_write_16bit_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    double s;
    short data;
    int n;

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + pcm->length * 2;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    wave_format_type = 1;
    channel = 1;
    samples_per_sec = pcm->fs;
    bytes_per_sec = pcm->fs * pcm->bits / 8;
    block_size = pcm->bits / 8;
    bits_per_sample = pcm->bits;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length * 2;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        s = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

        if (s > 65535.0) {
            s = 65535.0;
        }
        else if (s < 0.0) {
            s = 0.0;
        }

        data = (short)((int)(s + 0.5) - 32768);
        fwrite(&data, 2, 1, fp);
    }

    fclose(fp);
}

void wave_read_16bit_stereo(STEREO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    short data;
    int n;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = bits_per_sample;
    pcm->length = data_chunk_size / 4;
    pcm->sL = (double *) calloc(pcm->length, sizeof(double));
    pcm->sR = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&data, 2, 1, fp);
        pcm->sL[n] = (double)data / 32768.0;

        fread(&data, 2, 1, fp);
        pcm->sR[n] = (double)data / 32768.0;
    }

    fclose(fp);
}

void wave_write_16bit_stereo(STEREO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    char data_chunk_ID[4];
    long data_chunk_size;
    double sL;
    double sR;
    short data;
    int n;

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 36 + pcm->length * 4;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 16;
    wave_format_type = 1;
    channel = 2;
    samples_per_sec = pcm->fs;
    bytes_per_sec = pcm->fs * pcm->bits / 8 * 2;
    block_size = pcm->bits / 8 * 2;
    bits_per_sample = pcm->bits;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length * 4;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        sL = (pcm->sL[n] + 1.0) / 2.0 * 65536.0;

        if (sL > 65535.0) {
            sL = 65535.0;
        }
        else if (sL < 0.0) {
            sL = 0.0;
        }

        data = (short)((int)(sL + 0.5) - 32768);
        fwrite(&data, 2, 1, fp);

        sR = (pcm->sR[n] + 1.0) / 2.0 * 65536.0;

        if (sR > 65535.0) {
            sR = 65535.0;
        }
        else if (sR < 0.0) {
            sR = 0.0;
        }

        data = (short)((int)(sR + 0.5) - 32768);
        fwrite(&data, 2, 1, fp);
    }

    fclose(fp);
}

void wave_read_PCMU_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    short s;
    unsigned char c;
    unsigned char sign, exponent, mantissa;
    int n, magnitude;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(&extra_size, 2, 1, fp);
    fread(fact_chunk_ID, 1, 4, fp);
    fread(&fact_chunk_size, 4, 1, fp);
    fread(&sample_length, 4, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = 16;
    pcm->length = sample_length;
    pcm->s = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&c, 1, 1, fp);

        c = ~c;

        sign = c & 0x80;
        exponent = (c >> 4) & 0x07;
        mantissa = c & 0x0F;

        magnitude = ((((int)mantissa << 3) + 0x84) << exponent) - 0x84;

        if (sign == 0x80) {
            s = -(short)magnitude;
        }
        else {
            s = (short)magnitude;
        }

        pcm->s[n] = (double)s / 32768.0;
    }

    fclose(fp);
}

void wave_write_PCMU_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    double x;
    short s;
    unsigned char c;
    unsigned char sign, exponent, mantissa;
    int n, magnitude;

    static short level[8] = {
        0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
    };

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 50 + pcm->length;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 18;
    wave_format_type = 7;
    channel = 1;
    samples_per_sec = pcm->fs;
    bytes_per_sec = samples_per_sec;
    block_size = 1;
    bits_per_sample = 8;
    extra_size = 0;

    fact_chunk_ID[0] = 'f';
    fact_chunk_ID[1] = 'a';
    fact_chunk_ID[2] = 'c';
    fact_chunk_ID[3] = 't';
    fact_chunk_size = 4;
    sample_length = pcm->length;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(&extra_size, 2, 1, fp);
    fwrite(fact_chunk_ID, 1, 4, fp);
    fwrite(&fact_chunk_size, 4, 1, fp);
    fwrite(&sample_length, 4, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        x = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

        if (x > 65535.0) {
            x = 65535.0;
        }
        else if (x < 0.0) {
            x = 0.0;
        }

        s = (short)((int)(x + 0.5) - 32768);

        if (s < 0) {
            magnitude = -s;
            sign = 0x80;
        }
        else {
            magnitude = s;
            sign = 0x00;
        }

        magnitude += 0x84;
        if (magnitude > 0x7FFF) {
            magnitude = 0x7FFF;
        }

        for (exponent = 0; exponent < 8; exponent++) {
            if (magnitude <= level[exponent]) {
                break;
            }
        }

        mantissa = (magnitude >> (exponent + 3)) & 0x0F;

        c = ~(sign | (exponent << 4) | mantissa);

        fwrite(&c, 1, 1, fp);
    }


    if ((pcm->length % 2) == 1) {
        c = 0;
        fwrite(&c, 1, 1, fp);
    }

    fclose(fp);
}

void wave_read_PCMA_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    short s;
    unsigned char c;
    unsigned char sign, exponent, mantissa;
    int n, magnitude;

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(&extra_size, 2, 1, fp);
    fread(fact_chunk_ID, 1, 4, fp);
    fread(&fact_chunk_size, 4, 1, fp);
    fread(&sample_length, 4, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = samples_per_sec;
    pcm->bits = 16;
    pcm->length = sample_length;
    pcm->s = (double *) calloc(pcm->length, sizeof(double));

    for (n = 0; n < pcm->length; n++) {
        fread(&c, 1, 1, fp);

        c ^= 0xD5;

        sign = c & 0x80;
        exponent = (c >> 4) & 0x07;
        mantissa = c & 0x0F;

        if (exponent == 0) {
            magnitude = ((int)mantissa << 4) + 0x0008;
        }
        else {
            magnitude = (((int)mantissa << 4) + 0x0108) << (exponent - 1);
        }

        if (sign == 0x80) {
            s = -(short)magnitude;
        }
        else {
            s = (short)magnitude;
        }

        pcm->s[n] = (double)s / 32768.0;
    }

    fclose(fp);
}

void wave_write_PCMA_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    double x;
    short s;
    unsigned char c;
    unsigned char sign, exponent, mantissa;
    int n, magnitude;

    static int level[8] = {
        0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF
    };

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 50 + pcm->length;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 18;
    wave_format_type = 6;
    channel = 1;
    samples_per_sec = pcm->fs;
    bytes_per_sec = samples_per_sec;
    block_size = 1;
    bits_per_sample = 8;
    extra_size = 0;

    fact_chunk_ID[0] = 'f';
    fact_chunk_ID[1] = 'a';
    fact_chunk_ID[2] = 'c';
    fact_chunk_ID[3] = 't';
    fact_chunk_size = 4;
    sample_length = pcm->length;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = pcm->length;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(&extra_size, 2, 1, fp);
    fwrite(fact_chunk_ID, 1, 4, fp);
    fwrite(&fact_chunk_size, 4, 1, fp);
    fwrite(&sample_length, 4, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (n = 0; n < pcm->length; n++) {
        x = (pcm->s[n] + 1.0) / 2.0 * 65536.0;

        if (x > 65535.0) {
            x = 65535.0;
        }
        else if (x < 0.0) {
            x = 0.0;
        }

        s = (short)((int)(x + 0.5) - 32768);

        if (s < 0) {
            magnitude = -s;
            sign = 0x80;
        }
        else {
            magnitude = s;
            sign = 0x00;
        }

        if (magnitude > 0x7FFF) {
            magnitude = 0x7FFF;
        }

        for (exponent = 0; exponent < 8; exponent++) {
            if (magnitude <= level[exponent])
                {
                    break;
                }
        }

        if (exponent == 0) {
            mantissa = (magnitude >> 4) & 0x0F;
        }
        else {
            mantissa = (magnitude >> (exponent + 3)) & 0x0F;
        }

        c = (sign | (exponent << 4) | mantissa) ^ 0xD5;

        fwrite(&c, 1, 1, fp);
    }


    if ((pcm->length % 2) == 1) {
        c = 0;
        fwrite(&c, 1, 1, fp);
    }

    fclose(fp);
}

void wave_read_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    short samples_per_block;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    short s;
    unsigned char c;
    unsigned char header[4];
    unsigned char data;
    int n, sp, dp, offset, block, number_of_block, index, step_size;

    static int index_table[16] = {
        -1, -1, -1, -1, 2, 4, 6, 8,
        -1, -1, -1, -1, 2, 4, 6, 8
    };

    static int step_size_table[89] = {
        7, 8, 9, 10, 11, 12, 13, 14,
        16, 17, 19, 21, 23, 25, 28, 31,
        34, 37, 41, 45, 50, 55, 60, 66,
        73, 80, 88, 97, 107, 118, 130, 143,
        157, 173, 190, 209, 230, 253, 279, 307,
        337, 371, 408, 449, 494, 544, 598, 658,
        724, 796, 876, 963, 1060, 1166, 1282, 1411,
        1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
        3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
        7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
        32767
    };

    fp = fopen(file_name, "rb");

    fread(riff_chunk_ID, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(file_format_type, 1, 4, fp);
    fread(fmt_chunk_ID, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&wave_format_type, 2, 1, fp);
    fread(&channel, 2, 1, fp);
    fread(&samples_per_sec, 4, 1, fp);
    fread(&bytes_per_sec, 4, 1, fp);
    fread(&block_size, 2, 1, fp);
    fread(&bits_per_sample, 2, 1, fp);
    fread(&extra_size, 2, 1, fp);
    fread(&samples_per_block, 2, 1, fp);
    fread(fact_chunk_ID, 1, 4, fp);
    fread(&fact_chunk_size, 4, 1, fp);
    fread(&sample_length, 4, 1, fp);
    fread(data_chunk_ID, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    number_of_block = data_chunk_size / block_size;

    pcm->fs = samples_per_sec;
    pcm->bits = 16;
    pcm->length = sample_length;
    pcm->s = (double *) calloc(pcm->length, sizeof(double));

    for (block = 0; block < number_of_block; block++) {
        offset = samples_per_block * block;

        for (n = 0; n < samples_per_block; n++) {
            if (n == 0) {
                fread(header, 1, 4, fp);

                sp = ((short)(char)header[1] << 8) + header[0];
                index = header[2];

                s = sp;
            }
            else {
                if ((n % 2) == 1) {
                    fread(&data, 1, 1, fp);

                    c = (unsigned char)(data & 0x0F);
                }
                else {
                    c = (unsigned char)((data >> 4) & 0x0F);
                }

                step_size = step_size_table[index];


                dp = step_size >> 3;
                if ((c & 0x1) == 0x1) {
                    dp += (step_size >> 2);
                }
                if ((c & 0x2) == 0x2) {
                    dp += (step_size >> 1);
                }
                if ((c & 0x4) == 0x4) {
                    dp += step_size;
                }

                if ((c & 0x8) == 0x8) {
                    sp -= dp;
                }
                else {
                    sp += dp;
                }

                if (sp > 32767) {
                    sp = 32767;
                }
                else if (sp < -32768) {
                    sp = -32768;
                }

                index += index_table[c];
                if (index < 0) {
                    index = 0;
                }
                else if (index > 88) {
                    index = 88;
                }

                s = sp;
            }

            pcm->s[offset + n] = (double)s / 32768.0;
        }
    }

    fclose(fp);
}

void wave_write_IMA_ADPCM_mono(MONO_PCM *pcm, char *file_name)
{
    FILE *fp;
    char riff_chunk_ID[4];
    long riff_chunk_size;
    char file_format_type[4];
    char fmt_chunk_ID[4];
    long fmt_chunk_size;
    short wave_format_type;
    short channel;
    long samples_per_sec;
    long bytes_per_sec;
    short block_size;
    short bits_per_sample;
    short extra_size;
    short samples_per_block;
    char fact_chunk_ID[4];
    long fact_chunk_size;
    long sample_length;
    char data_chunk_ID[4];
    long data_chunk_size;

    double x;
    short s;
    unsigned char c;
    unsigned char header[4];
    unsigned char data;
    int n, sp, d, dp, offset, block, number_of_block, index, step_size;

    static int index_table[16] = {
        -1, -1, -1, -1, 2, 4, 6, 8,
        -1, -1, -1, -1, 2, 4, 6, 8
    };

    static int step_size_table[89] = {
        7, 8, 9, 10, 11, 12, 13, 14,
        16, 17, 19, 21, 23, 25, 28, 31,
        34, 37, 41, 45, 50, 55, 60, 66,
        73, 80, 88, 97, 107, 118, 130, 143,
        157, 173, 190, 209, 230, 253, 279, 307,
        337, 371, 408, 449, 494, 544, 598, 658,
        724, 796, 876, 963, 1060, 1166, 1282, 1411,
        1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024,
        3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484,
        7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
        15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
        32767
    };

    block_size = 256;
    samples_per_block = (block_size - 4) * 2 + 1;
    number_of_block = (int)(pcm->length / samples_per_block);

    riff_chunk_ID[0] = 'R';
    riff_chunk_ID[1] = 'I';
    riff_chunk_ID[2] = 'F';
    riff_chunk_ID[3] = 'F';
    riff_chunk_size = 52 + block_size * number_of_block;
    file_format_type[0] = 'W';
    file_format_type[1] = 'A';
    file_format_type[2] = 'V';
    file_format_type[3] = 'E';

    fmt_chunk_ID[0] = 'f';
    fmt_chunk_ID[1] = 'm';
    fmt_chunk_ID[2] = 't';
    fmt_chunk_ID[3] = ' ';
    fmt_chunk_size = 20;
    wave_format_type = 17;
    channel = 1;
    samples_per_sec = pcm->fs;
    bytes_per_sec = (long)(block_size * samples_per_sec / samples_per_block);
    bits_per_sample = 4;
    extra_size = 2;

    fact_chunk_ID[0] = 'f';
    fact_chunk_ID[1] = 'a';
    fact_chunk_ID[2] = 'c';
    fact_chunk_ID[3] = 't';
    fact_chunk_size = 4;
    sample_length = samples_per_block * number_of_block + 1;

    data_chunk_ID[0] = 'd';
    data_chunk_ID[1] = 'a';
    data_chunk_ID[2] = 't';
    data_chunk_ID[3] = 'a';
    data_chunk_size = block_size * number_of_block;

    fp = fopen(file_name, "wb");

    fwrite(riff_chunk_ID, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(file_format_type, 1, 4, fp);
    fwrite(fmt_chunk_ID, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&wave_format_type, 2, 1, fp);
    fwrite(&channel, 2, 1, fp);
    fwrite(&samples_per_sec, 4, 1, fp);
    fwrite(&bytes_per_sec, 4, 1, fp);
    fwrite(&block_size, 2, 1, fp);
    fwrite(&bits_per_sample, 2, 1, fp);
    fwrite(&extra_size, 2, 1, fp);
    fwrite(&samples_per_block, 2, 1, fp);
    fwrite(fact_chunk_ID, 1, 4, fp);
    fwrite(&fact_chunk_size, 4, 1, fp);
    fwrite(&sample_length, 4, 1, fp);
    fwrite(data_chunk_ID, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    for (block = 0; block < number_of_block; block++) {
        offset = samples_per_block * block;

        for (n = 0; n < samples_per_block; n++) {
            x = (pcm->s[offset + n] + 1.0) / 2.0 * 65536.0;

            if (x > 65535.0) {
                x = 65535.0;
            }
            else if (x < 0.0) {
                x = 0.0;
            }

            s = (short)((int)(x + 0.5) - 32768);

            if (block == 0 && n == 0) {
                index = 0;
            }

            if (n == 0) {
                header[0] = (unsigned char)(s & 0x00FF);
                header[1] = (unsigned char)((s >> 8) & 0x00FF);
                header[2] = (unsigned char)index;
                header[3] = 0;

                fwrite(header, 1, 4, fp);

                sp = s;
            }
            else {
                d = s - sp;
                if (d < 0) {
                    c = 0x8;
                    d = -d;
                }
                else {
                    c = 0x0;
                }

                step_size = step_size_table[index];


                if (d >= step_size) {
                    c |= 0x4;
                    d -= step_size;
                }
                if (d >= (step_size >> 1)) {
                    c |= 0x2;
                    d -= (step_size >> 1);
                }
                if (d >= (step_size >> 2)) {
                    c |= 0x1;
                }


                dp = step_size >> 3;
                if ((c & 0x1) == 0x1) {
                    dp += (step_size >> 2);
                }
                if ((c & 0x2) == 0x2) {
                    dp += (step_size >> 1);
                }
                if ((c & 0x4) == 0x4) {
                    dp += step_size;
                }

                if ((c & 0x8) == 0x8) {
                    sp -= dp;
                }
                else {
                    sp += dp;
                }

                if (sp > 32767) {
                    sp = 32767;
                }
                else if (sp < -32768) {
                    sp = -32768;
                }

                index += index_table[c];
                if (index < 0) {
                    index = 0;
                }
                else if (index > 88) {
                    index = 88;
                }

                if ((n % 2) == 1) {
                    data = c & 0xF;
                }
                else {
                    data |= (c & 0xF) << 4;

                    fwrite(&data, 1, 1, fp);
                }
            }
        }
    }

    fclose(fp);
}
