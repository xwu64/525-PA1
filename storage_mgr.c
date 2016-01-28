#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <errno.h> 
#include <string.h> 
#include <limits.h> 
#include "storage_mgr.h" 

/************************************************************
 *                    handle data structures                *
 ************************************************************/
/*
typedef struct SM_FileHandle {
  char *fileName;
  int totalNumPages;
  int curPagePos;
  void *mgmtInfo;
} SM_FileHandle;

typedef char* SM_PageHandle;
*/

/************************************************************
 *                    interface                             *
 ************************************************************/
/* manipulating page files */
/*
extern void initStorageManager (void);
extern RC createPageFile (char *fileName);
extern RC openPageFile (char *fileName, SM_FileHandle *fHandle);
extern RC closePageFile (SM_FileHandle *fHandle);
extern RC destroyPageFile (char *fileName);
*/

/* reading blocks from disc */
/*
extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
extern int getBlockPos (SM_FileHandle *fHandle);
extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
*/

/* writing blocks to a page file */
/*
extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
extern RC appendEmptyBlock (SM_FileHandle *fHandle);
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);
*/

/* manipulating page files */

/* Chuanwei Xu */

void initStorageManager(void){
}

RC createPageFile(char *fileName){
}

RC openPageFile (char *fileName, SM_FileHandle *fHandle){
}

RC closePageFile (SM_FileHandle *fHandle){
}

RC destroyPageFile (char *fileName){
}

/* reading blocks from disc */

/* Xiaoliang Wu, Zhipeng Liu*/

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
}

int getBlockPos (SM_FileHandle *fHandle){
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *fp;
	fp=fopen(fHandle->fileName,"r");
	fseek(fp,0,SEEK_SET);
	read(fp,memPage,PAGE_SIZE);
	fclose(fp);
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos<=1||fHandle->curPagePos>fHandle->totalNumPages)
		printf("this is the first page or current position invalid in readpreviousblock function\n");
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos-2)*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		read(fp,memPage,PAGE_SIZE);
		fclose(fp);
	}
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos<1||fHandle->curPagePos>fHandle->totalNumPages)
		printf("value of curPagePos is invalid in readcurrentblock function\n");
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos-1)*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		read(fp,memPage,PAGE_SIZE);
		fclose(fp);
	}
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos>(fHandle->totalNumPages-1)||fHandle->curPagePos<1)
		printf("the curPagePos is invalid in readnextblock function\n ");
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=fHandle->curPagePos*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		read(fp,memPage,PAGE_SIZE);
		fclose(fp);
	}
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *fp;
	fp=fopen(fHandle->fileName,"r");
	int offset=-PAGE_SIZE;
	fseek(fp,offset,SEEK_END);
	read(fp,memPage,PAGE_SIZE);
	fclose(fp);
}

/* writing blocks to a page file */

/* Xincheng Yang*/

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
}

RC appendEmptyBlock (SM_FileHandle *fHandle){
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
}

