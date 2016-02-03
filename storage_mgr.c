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

/* Chuanwei Tu */

/***************************************************************
 * Function Name: initStorageManager
 * 
 * Description:to let user know enter the StorageManager
 *
 * Parameters:void
 *
 * Return:void
 *
 * Author:Chuanwei Tu
 *
 * History:
 *      Date            Name                        Content
 *      2016/2/1    	 Chuanwei Tu				  initializing the Storage Manager 
 *
***************************************************************/


void initStorageManager(void){
	
   
	printf("--------Initialzing the StorageManager----------");   // to let user know enter the StorageManager
	printf("--------the StorageManager version is 2.00---------- \n");
	//return;
}



/***************************************************************
 * Function Name: createPageFile
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
	FILE *fp_new_open;   //get a file 
	char * str_new;   //use the str_new as a char document

	if(fileName == NULL){  //check the fileName if it exists
		printf("-----The file does not exits.Pleas enter a valid fileName!!\n");
		return RC_FILE_NOT_FOUND; 
	}

	fp_new_open = fopen(fileName,"w+");   //make sure the fp_new_open file readable, and writable
	str_new = (char *)malloc(PAGE_SIZE);   //allocate the space for the string
	memset(str_new,'\0',PAGE_SIZE);	//initialize the string
	fwrite(str_new,PAGE_SIZE,1,fp_new_open); // write the string to the file
	printf("A new page has been written to the file..")
	free(str_new); // free the space, bye!
	if(fp_new_open != NULL){
		fclose(fp_new_open); // close it
	}
	return RC_OK;
	
}

/***************************************************************
 * Function Name: createPageFile
 * 
 * Description:
 *
 * Parameters:char *fileName ,SM_FileHandle *fHandle
 *
 * Return:
 *
 * Author:
 *
 * History:
 *      Date            Name                        Content
 *      2016/2/1		 Chuanwei Tu
 *
***************************************************************/


RC openPageFile (char *fileName, SM_FileHandle *fHandle){
	 FILE *fh = NULL;
    long fileSize;
    long last_check = -1;
    
    fh = fopen(fileName, "rb+");
   // int returnV;
    if(fh!=NULL)
     {
		int result;
        result=fseek(fh,0L,SEEK_END);
		if (result==0){
			last_check = ftell(fh);                //offset of the tail of the file.
			if(last_check != -1){
				fileSize=last_check+1;                  //fileSizegth from the tail to the head of the page file = bytes of the page file.
				fHandle ->totalNumPages = ((int) (fileSize/(PAGE_SIZE))) +1;
				fHandle ->fileName = fileName;      //assign the fHandle's attributions.
                        
				fHandle ->curPagePos =0;
			
				fHandle ->mgmtInfo = fh;    //still not sure ...need help~~~~
				return RC_OK;              // if failed to fetch the offset, return failed.
			}else{
				 return RC_GET_NUMBER_OF_BYTES_FAILED;
				  } 
		}
		else if(result == -1){
			printf("Error happened!---file seeking  failed");
			return RC_FILE_NOT_FOUND;
		}  // figure out the tail's position of the file, and let fp q             points to this position.
		  // succeed to initialize the file handler
	}
	else
    {
		return RC_FILE_NOT_FOUND;
    }
	return RC_OK;
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
 * Author:Chuanwei Tu
 *
 * History:
 *      Date            Name                        Content
 *      2016/2/1        Chuanwei Tu
 *
***************************************************************/


RC closePageFile (SM_FileHandle *fHandle){       		// extern RC closePageFile (SM_FileHandle *fHandle)

	if (fHandle == NULL){                         //if the fHandle is null, then return not init
		return RC_FILE_HANDLE_NOT_INIT;
	}

	//set the fhandle structure
	fHandle->fileName = NULL;                //make the file name of fhandle is null;

	fHandle->totalNumPages = 0;              //make the file totalnumPages of fhandle is 0;
	
	fHandle->curPagePos = 0;                 //make the file curpagepos of fhandle is 0;
	
	fHandle->mgmtInfo = NULL;                ////make the file mgmtInfo of fhandle is null;

	return RC_OK;
}

/***************************************************************
 * Function Name: 
 * 
 * Description:
 *
 * Parameters:SM_FileHandle *fHandle
 *
 * Return:
 *
 * Author:Chuanwei Tu
 *
 * History:
 *      Date            Name                        Content
 *      2016/2/1        Chuanwei Tu				   destroyPageFile
 *
***************************************************************/


RC destroyPageFile (char *fileName){
	if(fileName == NULL){   //if file is null, then filename is null
		return RC_FILE_NOT_FOUND;
	}

	int temp;
	printf("----- Destroy the file.------")
	temp = remove(fileName);   //use temp to store the result after removing fileName

	if (temp == -1){                //if the result is -1, means destory fail failed

		printf("destory file failed %s\n", fileName);
		return RC_FILE_NOT_FOUND;

	}

	//perroe("remove"); the error exist in the errno 	
	return RC_OK;

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
	if(pageNum>fHandle->totalNumPages||pageNum<0)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset;
		offset=fHandle->curPagePos*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
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
	fHandle->curPagePos=0;
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
	if(fHandle->curPagePos<=0||fHandle->curPagePos>fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos-1)*PAGE_SIZE;
		fseek(fp,offset,SEEK_SET);
		fread(memPage,sizeof(char),PAGE_SIZE,fp);
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
	if(fHandle->curPagePos<0||fHandle->curPagePos>fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=fHandle->curPagePos*PAGE_SIZE;
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
	if(fHandle->curPagePos>(fHandle->totalNumPages-1)||fHandle->curPagePos<0)
		return RC_READ_NON_EXISTING_PAGE;
	else
	{
		FILE *fp;
		fp=fopen(fHandle->fileName,"r");
		int offset=(fHandle->curPagePos+1)*PAGE_SIZE;
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

/***************************************************************
 * Function Name: writeBlock 
 * 
 * Description: Write a page to disk using either the current position or an absolute position.
 *
 * Parameters: int numberOfPages, SM_FileHandle *fHandle
 *
 * Return: RC
 *
 * Author: Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *   2016/2/2		Xincheng Yang             first time to implement the function
 *   2016/2/2		Xincheng Yang			  modified some codes
 *
***************************************************************/
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
	ensureCapacity (pageNum, fHandle);		//Make sure the program have enough capacity to write block.
	
	FILE *fp;
	RC rv;

	fp=fopen(fHandle->fileName,"rb+");
	if(fseek(fp,pageNum * PAGE_SIZE, SEEK_SET) != 0){
		rv = RC_READ_NON_EXISTING_PAGE;	
	} else if (fwrite(memPage, PAGE_SIZE, 1, fp) != 1){
		rv = RC_WRITE_FAILED; 
	} else {
		fHandle->curPagePos=pageNum;		//Success write block, then curPagePos should be changed.
		rv = RC_OK;
	}

	fclose(fp);
	return rv;
}


/***************************************************************
 * Function Name: writeCurrentBlock 
 * 
 * Description: Write a page to disk using either the current position or an absolute position.
 *
 * Parameters: int numberOfPages, SM_FileHandle *fHandle
 *
 * Return: RC
 *
 * Author: Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *   2016/2/2		Xincheng Yang             first time to implement the function
 *
***************************************************************/
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	if(fHandle == NULL){
		return RC_FILE_HANDLE_NOT_INIT;
	} 
	if(fHandle->curPagePos < 0){
		return RC_WRITE_FAILED;
	}

	return writeBlock(fHandle->curPagePos, fHandle, memPage);
}

/***************************************************************
 * Function Name: appendEmptyBlock 
 * 
 * Description: Increase the number of pages in the file by one. The new last page should be filled with zero bytes.
 *
 * Parameters: int numberOfPages, SM_FileHandle *fHandle
 *
 * Return: RC
 *
 * Author: Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *   2016/2/1		Xincheng Yang             first time to implement the function
 *   2016/2/2		Xincheng Yang			  modified some codes
 *
***************************************************************/
RC appendEmptyBlock (SM_FileHandle *fHandle){
	if(fHandle == NULL){
		return RC_FILE_HANDLE_NOT_INIT;
	} 

	FILE *fp;
	char *allocData;
	RC rv;

	allocData = (char *)calloc(1, PAGE_SIZE);
	fp=fopen(fHandle->fileName,"ab+");

	if(fwrite(allocData, PAGE_SIZE, 1, fp) != 1)   
	{
		rv = RC_WRITE_FAILED;
	} else {
		fHandle -> totalNumPages += 1;
		rv = RC_OK;		
	}

	free(allocData);
	fclose(fp);

	return  rv;
}

/***************************************************************
 * Function Name: ensureCapacity 
 * 
 * Description: If the file has less than numberOfPages pages then increase the size to numberOfPages.
 *
 * Parameters: int numberOfPages, SM_FileHandle *fHandle
 *
 * Return: RC
 *
 * Author: Xincheng Yang
 *
 * History:
 *      Date            Name                        Content
 *   2016/2/1		Xincheng Yang             first time to implement the function
 *
***************************************************************/
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
	if(fHandle == NULL){
		return RC_FILE_HANDLE_NOT_INIT;
	} 
	if(fHandle -> totalNumPages >= numberOfPages){
		return RC_OK;
	}
	
	FILE *fp;
	long allocCapacity;
	char *allocData;
	RC rv;

	allocCapacity= (numberOfPages - fHandle -> totalNumPages) * PAGE_SIZE;
	allocData = (char *)calloc(1,allocCapacity);
	
	fp=fopen(fHandle->fileName,"ab+");
   
	if(fwrite(allocData, allocCapacity, 1, fp) == 0)   
	{
		rv = RC_WRITE_FAILED;
	} else {
		fHandle -> totalNumPages = numberOfPages;		//When write success, totalNumPages should be changed to numberOfPages.	
		rv = RC_OK;
	}

	free(allocData);
	fclose(fp);

	return rv;
}
