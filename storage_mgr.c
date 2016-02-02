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

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


void initStorageManager(void){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC createPageFile(char *fileName){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC openPageFile (char *fileName, SM_FileHandle *fHandle){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC closePageFile (SM_FileHandle *fHandle){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC destroyPageFile (char *fileName){
}

/* reading blocks from disc */

/* Xiaoliang Wu, Zhipeng Liu*/

/***************************************************************
 * Function Name: readBlock
 * 
 * Description: read the pageNum block from the file defined by fHandle into address memPage
 *
 * Parameters:int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return:RC
 *
 * Author:Zhipeng Liu
 *
 * History:
 *      Date            Name                        Content
 *      2016/1/30      Zhipeng Liu            first time to implement the function
 *
***************************************************************/


RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(pageNum>fHandle->totalNumPages||pageNum<1)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset;
		offset=(pageNum-1)*PAGE_SIZE;
		fread(memPage,sizeof(char),PAGE_SIZE,fp);
		fclose(fp);
		return RC_OK;
	}
}

/***************************************************************
 * Function Name: getBlockPos
 * 
 * Description:return the current block position in the file
 *
 * Parameters:SM_FileHandle *fHandle
 *
 * Return: RC
 *
 * Author:Zhipeng Liu
 *
 * History:
 *      Date            Name                        Content
 *      2016/1/30      Zhipeng Liu            first time to implement the function
 *
***************************************************************/


int getBlockPos (SM_FileHandle *fHandle)
{
	return fHandle->curPagePos;
}

/***************************************************************
 * Function Name:  readFirstBlock
 * 
 * Description:read the first block of the file described in fHandle
 *
 * Parameters:SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return:RC
 *
 * Author:Zhipeng Liu
 *
 * History:
 *      Date                     Name                        Content
 *      2016/1/30      Zhipeng Liu                    first time to implement the function
 *
***************************************************************/


RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *fp;
	fp=fopen(fHandle->fileName,"r");
	fseek(fp,0,SEEK_SET);
	fread(memPage,sizeof(char),PAGE_SIZE,fp);
	fHandle->curPagePos=1;
	fclose(fp);
	return RC_OK;
}

/***************************************************************
 * Function Name:readPreviousBlock
 * 
 * Description:read the previousblock in this file into the address witch memPage pointed
 *
 * Parameters:SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return:RC
 *
 * Author:Zhipeng Liu
 *
 * History:
 *      Date                     Name                             Content
 *      2016/1/27       Zhipeng Liu                    first time to implement the function
 *
***************************************************************/


RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos<=1||fHandle->curPagePos>fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos-2)*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		read(fp,memPage,PAGE_SIZE);
		fHandle->curPagePos=fHandle->curPagePos-1;
		fclose(fp);
		return RC_OK;
	}
}

/***************************************************************
 * Function Name: readCurrentBlock
 * 
 * Description: read the current block (pointed by the fHandle->curPagePos) into memo address memPage
 *
 * Parameters: SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return: RC
 *
 * Author: Zhipeng Liu
 *
 * History:
 *      Date            Name                        Content
 *   2016/1/27     Zhipeng Liu             first time to implement the function
 *
***************************************************************/


RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos<1||fHandle->curPagePos>fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos-1)*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		fread(memPage,sizeof(char),PAGE_SIZE,fp);
		fclose(fp);
		return RC_OK;
	}
}

/***************************************************************
 * Function Name: readNextBlock
 * 
 * Description: read the next block (the current block defined in the fHandle->curPagePos) in to memo address memPage
 *
 * Parameters: SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return: RC
 *
 * Author: Zhipeng Liu
 *
 * History:
 *      Date            Name                        Content
 *   2016/1/27     Zhipeng Liu             first time to implement the function
 *
***************************************************************/


RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos>(fHandle->totalNumPages-1)||fHandle->curPagePos<1)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=fHandle->curPagePos*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		fread(memPage,sizeof(char),PAGE_SIZE,fp);
		fHandle->curPagePos=fHandle->curPagePos+1;
		fclose(fp);
		return RC_OK;
	}
}

/***************************************************************
 * Function Name: readLastBlock
 * 
 * Description: read the last block in this file into memo address memPage
 *
 * Parameters: SM_FileHandle *fHandle, SM_PageHandle memPage
 *
 * Return: RC
 *
 * Author: Zhipeng Liu
 *
 * History:
 *      Date            Name                        Content
 *   2016/1/27     Zhipeng Liu             first time to implement the function
 *
***************************************************************/


RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	FILE *fp;
	fp=fopen(fHandle->fileName,"r");
	int offset=-PAGE_SIZE;
	fseek(fp,offset,SEEK_END);
	fread(memPage,sizeof(char),PAGE_SIZE,fp);
	fHandle->curPagePos=fHandle->totalNumPages;
	fclose(fp);
	return RC_OK;
}

/* writing blocks to a page file */

/* Xincheng Yang*/

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC appendEmptyBlock (SM_FileHandle *fHandle){
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      --------------  --------------------------  ----------------
 *
***************************************************************/


RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
}

