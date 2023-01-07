//sound.h: header class for sound.cpp
//CSoundManager allows you to play multiple copies of each
//sound simultaneously

//Copyright Ian Parberry, 1999
//Last updated November 2, 1999

#ifndef __SOUND__
#define __SOUND__

#include <dsound.h> 
#include "GlobalEnum.h" 


#define MAX_SOUNDS 64 

#define DS_NUMCHANNELS 8 
#define DS_CHANSPERSAMPLE 1 
#define DS_BITSPERSAMPLE 8 
#define DS_SAMPLERATE 22050 

class SoundManager
{ 
  private:
    int m_nCount; 
    LPDIRECTSOUND m_lpDirectSound; 
    LPLPDIRECTSOUNDBUFFER m_lpBuffer[MAX_SOUNDS]; 
    int m_nCopyCount[MAX_SOUNDS]; 
    BOOL m_bOperational; 
    BOOL CreateBuffer(int index,int length,int copies);
    BOOL LoadBuffer(int index,BYTE *buffer,int length);
    int LoadSound(wchar_t *filename,BYTE **sound); 
    void CopyBuffer(int index,int copies); 
  public:
    SoundManager(HWND hwnd); 
    ~SoundManager();
    void clear();
    void load(wchar_t *filename,int copies=1); 
    void play(int index,BOOL looping=FALSE); 
    void stopsound(int index);
    void stop(void);
};

#endif
