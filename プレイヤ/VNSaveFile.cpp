#include "VNSaveFile.h"
#include "VNBinFileWriter.h"
#include "VNBinFileReader.h"
#include <time.h>
#include <stdio.h>
#include <assert.h>

#define SIGNATURE	"savedata"


VNSaveFile::VNSaveFile() {
	LoadDate();
}

VNSaveFile::VNSaveFile(
	const VNString& event_name,
	int offset,
	const VNString& title,
	const VNString rgb_image_name[4],
	const VNString mask_image_name[3],
	int v_align[3] )
{
	LoadDate();
	this->event_name = event_name;
	this->offset = offset;
	this->title = title;
	for( int i = 0; i < 4; i++ )
		this->rgb_image_name[i] = rgb_image_name[i];
	for( i = 0; i < 3; i++ )
		this->mask_image_name[i] = mask_image_name[i];
	for( i = 0; i < 3; i++ )
		this->v_align[i] = v_align[i];
}

bool VNSaveFile::Save( int index ) {
	try {
		VNBinFileWriter		fout( (VNString)"save"+(VNString)index+".sav" );
		fout.PutStringEx( date );
		fout.PutStringEx( event_name );
		fout.PutIntEx( offset );
		fout.PutStringEx( title );
		for( int i = 0; i < 4; i++ )
			fout.PutStringEx( rgb_image_name[i] );
		for( i = 0; i < 3; i++ )
			fout.PutStringEx( mask_image_name[i] );
		for( i = 0; i < 3; i++ )
			fout.PutIntEx( v_align[i] );
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}
	return true;
}

bool VNSaveFile::Load( int index ) {
	try {
		VNBinFileReader		fin( (VNString)"save"+(VNString)index+".sav" );
		date = fin.GetStringEx();
		event_name = fin.GetStringEx();
		offset = fin.GetIntEx();
		title = fin.GetStringEx();
		for( int i = 0; i < 4; i++ )
			rgb_image_name[i] = fin.GetStringEx();
		for( i = 0; i < 3; i++ )
			mask_image_name[i] = fin.GetStringEx();
		for( i = 0; i < 3; i++ )
			v_align[i] = fin.GetIntEx();
	} catch( VNIOException *e ) {
		delete e;
		return false;
	}
	return true;
}

const VNString& VNSaveFile::GetDate() {
	return date;
}

const VNString& VNSaveFile::GetEeventName() {
	return event_name;
}

int VNSaveFile::GetOffset() {
	return offset;
}

const VNString& VNSaveFile::GetTitle() {
	return title;
}

const VNString& VNSaveFile::GetRGBImageName( int n ) {
	assert( n >=0 && n <= 3 );
	return rgb_image_name[n];
}

const VNString& VNSaveFile::GetMaskImageName( int n ) {
	assert( n >=0 && n <= 2 );
	return mask_image_name[n];
}

int VNSaveFile::GetVerticalAlign( int n ) {
	assert( n >=0 && n <= 2 );
	return v_align[n];
}

void VNSaveFile::LoadDate() {
	char *buf = new char [1024];
	struct tm *now;

	time_t ltime;
	time( &ltime );
    now = localtime( &ltime );
    strftime( buf, 128, "%mŒŽ%d“ú%HŽž%M•ª", now );

	date = buf;
	delete buf;
}
