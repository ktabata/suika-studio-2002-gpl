#ifndef _VNBACKLOG_H_
#define _VNBACKLOG_H_

#include "VNString.h"

#define BACKLOG_MAX_ITEMS	(50)


class VNBackLog {
public:
	VNBackLog();
	~VNBackLog();

	int		GetStockCount() const;
	void	Clean();
	void	AddString( const VNString& );
	const VNString&	GetString( int ) const;

private:
	int			count;
	VNString	**buffer;
	int			top_index;
};


#endif
