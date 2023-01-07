//sound.cpp: sound manager

//Copyright Ian Parberry, 2000
//Last updated May 22, 2000

#include <stdio.h>
#include "sound.h"

extern SoundManager *Sounds;

SoundManager::SoundManager(HWND hwnd)
{ 
  m_nCount=0; 
  for(int i=0; i<MAX_SOUNDS; i++)
  {
	  m_lpBuffer[i]=NULL; m_nCopyCount[i]=0;
  }
	DirectSoundCreate(NULL,&m_lpDirectSound,NULL);
	m_lpDirectSound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
}

SoundManager::~SoundManager()
{ 
  
  clear();
  (void)m_lpDirectSound->Release();
}

void SoundManager::clear()
{ 
  
  stop(); 
  
  for(int i=0; i<m_nCount; i++)
  {
	  for(int j=0; j<m_nCopyCount[i]; j++)
	  { 
		m_lpBuffer[i][j]->Release(); 
		m_lpBuffer[i][j]=NULL;
	  }
    
	  delete[]m_lpBuffer[i];
  }
  m_nCount=0; 
}

void SoundManager::load(wchar_t *filename,int copies)
{
	int length; 
	
	BYTE *sound=NULL; 

	length=LoadSound(filename,&sound); 
	
	CreateBuffer(m_nCount,length,copies); 
	LoadBuffer(m_nCount,sound,length);
	CopyBuffer(m_nCount,copies); 

	delete[]sound; 

	m_nCount++; 
}

void SoundManager::play(int index,BOOL looping)
{ 
	
	
	m_lpBuffer[index][0]->Play(0,0,looping?DSBPLAY_LOOPING:0); 


}

void SoundManager::stopsound(int index){ 
  
  
  for(int i=0; i<m_nCopyCount[index]; i++)
  { 
    m_lpBuffer[index][i]->Stop();
    m_lpBuffer[index][i]->SetCurrentPosition(0); 
  }
}

void SoundManager::stop(void)
{ 
  
  for(int index=0; index<m_nCount; index++) 
    stopsound(index);
}

void SoundManager::CopyBuffer(int index,int copies)
{ 
  
  m_nCopyCount[index]=copies; 
  for(int i=1; i<copies; i++) 
    m_lpDirectSound->
        DuplicateSoundBuffer(*m_lpBuffer[index],
        &(m_lpBuffer[index][i]));
 
}

BOOL SoundManager::CreateBuffer(int index,int length,int copies)
{
  
  DSBUFFERDESC dsbdesc;
  
  PCMWAVEFORMAT pcmwf;
  if(length<=0)return FALSE; 
  
  memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
  pcmwf.wf.wFormatTag=WAVE_FORMAT_PCM;
  pcmwf.wf.nChannels=DS_CHANSPERSAMPLE;
  pcmwf.wf.nSamplesPerSec=DS_SAMPLERATE;
  pcmwf.wf.nBlockAlign=
    DS_CHANSPERSAMPLE*DS_BITSPERSAMPLE/8;
  pcmwf.wf.nAvgBytesPerSec=
    pcmwf.wf.nSamplesPerSec*pcmwf.wf.nBlockAlign;
  pcmwf.wBitsPerSample=DS_BITSPERSAMPLE;
  
  memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
  dsbdesc.dwSize=sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags=DSBCAPS_STATIC;
  dsbdesc.dwBufferBytes=length;
  dsbdesc.lpwfxFormat=(LPWAVEFORMATEX)&pcmwf;
  

  m_lpBuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
  for(int i=0; i<copies; i++)m_lpBuffer[index][i]=NULL;
  return SUCCEEDED(m_lpDirectSound->
    CreateSoundBuffer(&dsbdesc,m_lpBuffer[index],NULL));
}

BOOL SoundManager::LoadBuffer(int index,BYTE *sound,int length)
{
	if (sound == NULL) {
		return FALSE;
	}

  LPVOID w1,w2; 
  DWORD l1,l2; 
  
  LPDIRECTSOUNDBUFFER buffer=*m_lpBuffer[index];
  
  if(length<=0)return FALSE;
  if(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)==DSERR_BUFFERLOST)
  {
    buffer->Restore();
    if(FAILED(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)))
      return FALSE; 
  }
  CopyMemory(w1,sound,l1); 
  
  if(w2!=NULL)CopyMemory(w2,sound+l1,l2); 
 
  if(FAILED(buffer->Unlock(w1,l1,w2,l2)))return FALSE;
  
  return TRUE;
}

int SoundManager::LoadSound(wchar_t *filename,BYTE **buffer)
{

  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;
  int size=0;

  delete[]*buffer;

  parent.ckid=(FOURCC)0;
  parent.cksize=0;
  parent.fccType=(FOURCC)0;
  parent.dwDataOffset=0;
  parent.dwFlags=0;
  child=parent;

  hmfr=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF);
  if(hmfr==NULL)return NULL;

  parent.fccType=mmioFOURCC('W','A','V','E');
  
  if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF))
  {
    mmioClose(hmfr,0); return NULL; 
  }
  
  child.ckid=mmioFOURCC('f','m','t',' ');
  if(mmioDescend(hmfr,&child,&parent,0))
  {
    mmioClose(hmfr,0); return NULL; 
  }
  
  if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx))!=sizeof(wfmtx))
  {
    mmioClose(hmfr,0); return NULL;
  }
  
  if(wfmtx.wFormatTag!=WAVE_FORMAT_PCM)
  {
    mmioClose(hmfr,0); return NULL; 
  }
 
  if(mmioAscend(hmfr,&child,0))
  {
    mmioClose(hmfr,0); return NULL; 
  }
 
  child.ckid=mmioFOURCC('d','a','t','a');
  
  if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK))
  {
    mmioClose(hmfr,0); return NULL; 
  }
  
  size=child.cksize;
  *buffer=new BYTE[size];
  if(*buffer==NULL){
    mmioClose(hmfr,0); return NULL; 
  }
  
  if((int)mmioRead(hmfr,(char *)*buffer,size)!=size)
  {
      mmioClose(hmfr,0); delete[]*buffer; return NULL; 
  }
  
  mmioClose(hmfr,0);
  return size;
}


void LoadSounds()
{
	Sounds->load(L"Sounds/Track0.wav");
	Sounds->load(L"Sounds/Track1.wav");
	Sounds->load(L"Sounds/Track2.wav");
	Sounds->load(L"Sounds/Track3.wav");
	Sounds->load(L"Sounds/Track4.wav");
	Sounds->load(L"Sounds/Track5.wav");
	Sounds->load(L"Sounds/Track6.wav");
	
	Sounds->load(L"Sounds/Story.wav");
	Sounds->load(L"Sounds/Intro.wav");
	Sounds->load(L"Sounds/awp1.wav");
	Sounds->load(L"Sounds/jump.wav");
	Sounds->load(L"Sounds/jumpland.wav");
	Sounds->load(L"Sounds/treb3.wav");
}
