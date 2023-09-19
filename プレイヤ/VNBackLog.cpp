#include "VNBackLog.h"
#include <stddef.h>
#include <assert.h>


VNBackLog::VNBackLog() {
	buffer = new VNString * [BACKLOG_MAX_ITEMS];
	assert( buffer != 0 );
	for( int i = 0; i < BACKLOG_MAX_ITEMS; i ++ )
		buffer[i] = NULL;
	count = 0;
	top_index = 0;
}

VNBackLog::~VNBackLog() {
	for( int i = 0; i < BACKLOG_MAX_ITEMS; i ++ )
		if( buffer[i] != NULL )
			delete buffer[i];
	delete[] buffer;
}

int VNBackLog::GetStockCount() const {
	return count;
}

void VNBackLog::Clean() {
}

void VNBackLog::AddString( const VNString& str ) {
	int index = count;
	if( count == BACKLOG_MAX_ITEMS ) {
		top_index++;
		if( top_index == BACKLOG_MAX_ITEMS ) {
			top_index = 0;
			index = BACKLOG_MAX_ITEMS - 1;
		} else
			index = top_index-1;
	} else
		count++;

	buffer[index] = new VNString( str );
	*buffer[index] += "\r\n";	// ‰üs‚ð•t‰Á
}

const VNString& VNBackLog::GetString( int index ) const {
	int n = top_index + index;
	if( n >= BACKLOG_MAX_ITEMS )
		n -= BACKLOG_MAX_ITEMS;
	return *buffer[n];
}
