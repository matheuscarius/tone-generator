#include<cstdint>
#include<cstdio>
#include<cmath>
#include<cstdlib>

int main () {

  int duration=1;                                           //Tone duration
  const int amplitude=UINT8_MAX;                            //Tone amplitude
  int frequency=200;                                        //Tone frequency
  const double pi = acos(-1);                               //Pi
  const uint32_t ChunkID = 0x46464952;                      //Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).
  const uint32_t Format = 0x45564157;                       //Contains the letters "WAVE" (0x57415645 big-endian form).
  const uint32_t Subchunk1ID = 0x20746d66;                  //Contains the letters "fmt " (0x666d7420 big-endian form)
  const uint16_t AudioFormat = 1;                           //PCM
  const uint16_t NumChannels = 2;                           //Number of channels
  uint32_t SampleRate=44100;                                //SampleRate
  const uint32_t BitsPerSample = 8;                         //Bits per sample (8bits, 16bits);
  const uint16_t BlockAlign = NumChannels*BitsPerSample/8;  //SampleRate * NumChannels * BitsPerSample/8
  const uint32_t Subchunk2ID = 0x61746164;                  //NumChannels * BitsPerSample/8

  printf("Digite a frequência do som em Hz: ");
  scanf("%d", &frequency);
  //printf("Digite a duração do som em segundos: ");
  //scanf("%d", &duration);
  //printf("Digite o sampling rate em Hz: ");
  //scanf("%d", &SampleRate);                                 //SampleRate

  const uint32_t NumSamples = SampleRate*duration;
  const uint32_t ByteRate = SampleRate*NumChannels*BitsPerSample/8;
  const uint32_t Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8;
  const uint32_t Subchunk1Size = 16;
  const uint32_t ChunkSize = 36 + Subchunk2Size;

  //open audio file
  FILE *f = fopen("audio.wav", "wb");
  //write RIFF header
  fwrite(&ChunkID,4,1,f);
  fwrite(&ChunkSize,4,1,f);   //ChunkSize
  fwrite(&Format,4,1,f);

  //write fmt subchunk
  fwrite(&Subchunk1ID,4,1,f);
  fwrite(&Subchunk1Size,4,1,f);
  fwrite(&AudioFormat,2,1,f);
  fwrite(&NumChannels,2,1,f);
  fwrite(&SampleRate,4,1,f);
  fwrite(&ByteRate,4,1,f);
  fwrite(&BlockAlign,2,1,f);
  fwrite(&BitsPerSample,2,1,f);

  //write data subchunk
  fwrite(&Subchunk2ID,4,1,f);  //NumChannels * BitsPerSample/8
  fwrite(&Subchunk2Size,4,1,f);  //NumChannels * BitsPerSample/8
  for(int i = 0; i < duration; i++) {
    for(int j = 0; j < SampleRate; j++) {
      double t = i+j/(double)SampleRate;
      uint8_t x = (uint8_t) amplitude*(1+cos(2*pi*frequency*t))/2;
      fwrite(&x,1,1,f);
    }
  }
  fclose(f);
  char cmd[255];
  sprintf(cmd,"aplay -r %d audio.wav",SampleRate);
  system(cmd);
}
