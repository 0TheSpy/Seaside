#ifndef IFILELIST_H
#define IFILELIST_H
#ifdef _WIN32
#endif


class IFileList
{
public:
	virtual bool	IsFileInList(const char* pFilename) = 0;
	virtual void	Release() = 0;
};


#endif  
