#include "Music.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Empezar-Detener el sonido

void Music::Error(const char *text) 
{
	printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
	BASS_Free();
}

int Music::initSound(void) {
	
	this->scaler=12;
	this->accumSeconds=0;
	
	if (!BASS_Init(-1,44100,0,0,NULL))
	{
		Error("Music: Patada! Can't initialize device");
		return -1;
	}
	else
	{	
		this->musicStarted=0;
		return(0);
	}
}
int Music::freeSound(void) {
	if(this->mp3)
	{
		BASS_StreamFree(this->mp3);
	}
	else if(this->mod)
	{
		BASS_MusicFree(this->mod);
	}
	
	BASS_Free();
	return(0);
}
void Music::playSound(void)
{
	// Ponemos la musica en marcha!
	// TODO no se si este metodo se usa?
	// la canci�n est� en marcha 
	this->musicStarted=1;
}

void Music::stopSound(void)
{
	// TODO este metodo se usa?
	// Obvio no?
	BASS_Stop();
}
int Music::playMod()
{
	BASS_ChannelPlay(this->mod, FALSE);
	return 0;
}

int Music::playMP3() {
	return BASS_ChannelPlay(this->mp3, FALSE);
}

int Music::isPlaying(void) {
	return this->musicStarted;
}

void Music::setPos(unsigned int pattern, unsigned int row) {
	BASS_ChannelSetPosition(this->mod, MAKELONG(pattern, row), BASS_POS_MUSIC_ORDER
		// unsure | BASS_MUSIC_POSRESET
	);
}

void Music::setMP3Pos(float position)
{
	BASS_ChannelSetPosition(this->mp3, BASS_ChannelSeconds2Bytes(this->mp3, position), BASS_POS_BYTE);
}

HMUSIC Music::loadMod(const char *file)
{
	if (!(this->mod = BASS_MusicLoad(FALSE, file, 0, 0, BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN, 0)))
	{
		Error("Can't load mod file");
		return NULL;
	}
	else
	{
		return this->mod;
	}
}

HSTREAM Music::loadMP3(const char *file)
{
	if(!(mp3 = BASS_StreamCreateFile(FALSE, file, 0, 0, BASS_STREAM_PRESCAN)))
	{
		Error("Can't load mp3 file");
		return NULL;
	}
	else
	{
		return mp3;
	}
	
	
}
// Sincronizacion
unsigned int Music::getRow(void) {
	// Devuelve la fila actual
	return this->row;
}

unsigned int Music::getTick(void) {
	// Devuelve el tick actual
	return this->tick;
}

unsigned int Music::getPattern(void) {
	// Devuelve el patron actual
	return this->pattern;
}

float Music::getSeconds(void) {
	// devuelve los segundos transcurridos desde que se inicio la cancion
	// mu util para esos efectillos continuos... ;)
	return this->getMilliseconds() * 0.001f;
}

float Music::getMilliseconds()
{
	DWORD channel;
	
	if(this->mod)
	{
		channel = this->mod;
	}
	else
	{
		channel = this->mp3;
	}
	
	QWORD pos=BASS_ChannelGetPosition(channel,BASS_POS_BYTE);
	float time=BASS_ChannelBytes2Seconds(channel,pos);
	return time * 1000;
}

float Music::getMP3Seconds(void) {
	// TODO KILL THIS METHOd
	return this->getSeconds();
}

int Music::syncro() {
	// fuerza a reconsultar el patron y fila 
	// actualizamos la fila y patron actuales
	
	// Si hay algo nuevo , se devuelve 
	// TICKCHANGED 1
	// ROWCHANGED 2
	// PATTERNCHANGED 3
	// 0 en otro caso
	//QWORD ahora;
	static float sec=0;
	float sec2;
	
	if(this->mod)
	{
		QWORD pos=BASS_ChannelGetPosition(this->mod, BASS_POS_MUSIC_ORDER);
		this->pattern = LOWORD(pos);
		this->row = HIWORD(pos);
	}
	
	
	if(this->pattern2!=this->pattern) {	
		this->pattern2=this->pattern;
		sec=0;
		return(PATTERNCHANGED);
	} else {
		if(this->row2!=this->row)
		{
			this->row2=this->row;
			// Ha cambiado
			return(ROWCHANGED);
		} /*else {
			if(this->tick2!=this->tick) {
				this->tick2=this->tick;
				return(TICKCHANGED);
			} else {
				// No ha cambiado nada
				return(0);
			}
		}*/
		else {
			return(0);
		}
	}
	
}

int Music::getFFT(float *buffer, unsigned int length) {
	int ret;
	
	DWORD channel;
	if(this->mod)
	{
		channel = this->mod;
	}
	else
	{
		channel = this->mp3;
	}
	BASS_ChannelGetData(channel, buffer, BASS_DATA_FFT512);
	
	return 0;
}

void Music::getFFTNatural(float *buffer) {
	float tmpBuf[512];
	this->getFFT(tmpBuf,512);
	
	float scale=0.0010;
	float bias=1.0;
	int i;
	
	for(i=1;i<32;i++) {
		buffer[i] = tmpBuf[i]*scale;
		scale=scale+bias;
		bias=bias*1.6f;
		
	}
}

float Music::getFFTBass(void) {
	float fft[512];
	//float *fftpointer;
	float daBass;
	int i;
	this->getFFT(fft, 512);

	// hago una media para que est� un poco m�s representado el espectro de bajos.
	daBass=0;

	for(i=1; i<20; i++)
	{
		daBass += fft[i];
	}

	daBass *= 0.05;
	
	
	return daBass;
}

bool Music::hasFinished(void) {
	DWORD channel;
	
	if(this->mod)
	{
		channel = this->mod;
	}
	else
	{
		channel = this->mp3;
	}
	
	QWORD pos = BASS_ChannelGetPosition(channel,BASS_POS_BYTE);
	QWORD len = BASS_ChannelGetLength(channel, BASS_POS_BYTE);
	
	return(pos >= len);
	
	return false;
}
