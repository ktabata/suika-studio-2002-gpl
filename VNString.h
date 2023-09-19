#ifndef _VNSTRING_H_
#define _VNSTRING_H_


class VNString {
public:
	VNString();
	VNString( const VNString& );
	VNString( const char * );
	VNString( const char *, int );
	VNString( int );
	~VNString();

	VNString& operator=( const char * );
	VNString& operator=( const VNString& );
	VNString operator+( const char * );
	VNString operator+( const VNString& );
	VNString& operator+=( const char * );
	VNString& operator+=( const VNString& );

	operator const char *();

	const char *c_str() const;
	void CopyTo( char * ) const;

private:
	char	*str;
	int		len;

	void CheckLength() const;
};


#endif
