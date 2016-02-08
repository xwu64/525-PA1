#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

// test name
char *testName;

/* test output files */
#define TESTPF "test_pagefile.bin"

/* prototypes for test functions */
static void testRestFunc(void);

/* main function running all tests */
int
main (void)
{
  testName = "";
  
  initStorageManager();

  testRestFunc();

  return 0;
}


/* check a return code. If it is not RC_OK then output a message, error description, and exit */

/* Try to create, open, and close a page file */
void
testRestFunc(void)
{
  SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  testName = "test single page content";

  ph = (SM_PageHandle) malloc(PAGE_SIZE);

  // create a new page file
  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));
  printf("created and opened file\n");
  
  // read first page into handle
  TEST_CHECK(readFirstBlock (&fh, ph));
  // the page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in first page of freshly initialized page");
  printf("first block was empty\n");
    
  // change ph to be a string and write that one to disk
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeBlock (0, &fh, ph));
  printf("writing first block\n");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readFirstBlock (&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading first block\n");

  // get page position and check that it is correct
  int pageNum = 9999;
  pageNum = getBlockPos(&fh);
  printf("Get page position %d.\n",pageNum);

  // read previous page and check that it is correct
  ASSERT_ERROR((readPreviousBlock(&fh, ph)), "No previous page. Output error is right");
  printf("readPreviousBlock at first block.");

  // read current page and check that it is correct
  TEST_CHECK(readCurrentBlock(&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");

  // read next page and check that it is correct
  ASSERT_ERROR((readNextBlock(&fh, ph)), "No next page. Output error is right");
  printf("readNextBlock at last block.");

  // append empty page and check that it is correct
  TEST_CHECK(appendEmptyBlock(&fh));
  TEST_CHECK(readLastBlock(&fh, ph));

        // the page should be empty (zero bytes)
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in first page of freshly initialized page");
  printf("Appended block was empty\n");

  // write current page and check that it is correct
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 30) + '0';
  TEST_CHECK(writeCurrentBlock (&fh, ph));
  printf("writing appended block\n");
  
  // read previous page and check that it is correct
  TEST_CHECK(readPreviousBlock(&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading previous block\n");

  // read next page and check that it is correct
  TEST_CHECK(readNextBlock(&fh, ph));
  for (i=0; i < PAGE_SIZE; i++)
  {
    ASSERT_TRUE((ph[i] == (i % 30) + '0'), "character in page read from disk is the one we expected.");
  }
  printf("reading next block\n");

  // use ensureCapacity add 2 page and check that it is correct
  TEST_CHECK(ensureCapacity(3,&fh));
  ASSERT_TRUE((fh.totalNumPages == 3), "ensureCapacity correct.\n");

  // destroy new page file
  TEST_CHECK(destroyPageFile (TESTPF));  
  
  TEST_DONE();
}
