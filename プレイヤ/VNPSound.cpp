#include <windows.h>
#include <mmsystem.h>
#include "VNPlayer.h"


void VNPlayer::PlayPcm( const char *file ) {
	if( strcmp( file, "" ) != 0 ) {
		VNString fn = (VNString(PCM_DIR)+file).c_str();
		sndPlaySound (fn.c_str(),
					  SND_ASYNC |
					  SND_NODEFAULT |
					  SND_NOWAIT |
					  SND_FILENAME);
	} else {
		sndPlaySound (NULL, SND_ASYNC);
	}
}

void VNPlayer::PlayCdda( int track, int fade ) {
	if( track == 0 )
		cdplayer->Stop();
	else
		cdplayer->Play( track, track+1, true );
}

void VNPlayer::PlayMidi( const char *filename, int fade ) {
	if( strcmp( filename, "" ) == 0 )
		midiplayer->Stop();
	else {
		char cur_dir[1024];
		if( ::GetCurrentDirectory (1023, cur_dir) == 0 )
			return;	// Ž¸”s
		midiplayer->Play( (VNString(cur_dir)+MIDI_DIR+filename).c_str() );
	}
}
