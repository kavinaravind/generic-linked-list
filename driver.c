#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "DListTestHarness.h"
static void reportScores(FILE* Log);

bool profMode = false;
int  ptsAssigned = 0;
static FILE* Log;

void handler(int sig) {
    
    const char* msg1 = "OOPS!!\nCaught a SIGSEGV!!\n";
    write(1, msg1, strlen(msg1));
    
    fflush(Log);
    fprintf(Log, "Note that subsequent testing was interrupted by a SIGSEGV.\n\n");
    
    reportScores(Log);
    fclose(Log);
    exit(-1);
}

int main(int argc, char** argv) {
    
   signal(SIGSEGV, handler);

   // set up for testing:
   unsigned int Seed;
   Log = fopen("DListSummary.txt", "w");

   if ( argc == 2 && strcmp(argv[1], "-prof") == 0 ) {
      profMode = true;
      Seed = time(NULL);
      FILE* fp;
      fp = fopen("Seed.txt", "w");
      fprintf(fp, "%u\n", Seed);
      fclose(fp);
   }
   else {
      profMode = false;
      FILE* fp;
      fp = fopen("Seed.txt", "r");
      fscanf(fp, "%u", &Seed);
      fclose(fp);
   }
   srand( Seed );

   // test DList implementation:
   bool succeeded = true;
   
   if ( ! (succeeded = testDList(Log)) ) {
      fprintf(Log, "Errors found testing DList implementation.\n");
      fprintf(Log, "See the log files for more information.\n");
   }
   
   fprintf(Log, "\n");
   reportScores(Log);
   fclose(Log);
   
   if ( succeeded )
      return 0;
   else
      return -1;
}

static void reportScores(FILE* Log) {
    
    unsigned int total = 0;
    Scores results = getScores();
    
    fprintf(Log, "Initialization:  %3u", results.Initialization);
    if (results.dInitialization != 0 ) {
        fprintf(Log, "   (deducted:  %3u)", results.dInitialization);
    }
    fprintf(Log, "\n");
    total += results.Initialization;
    
    fprintf(Log, "Insertion:       %3u", results.Insertion);
    if (results.dInsertion != 0 ) {
        fprintf(Log, "   (deducted:  %3u)", results.dInsertion);
    }
    fprintf(Log, "\n");
    total += results.Insertion;
    
    fprintf(Log, "Deletion:        %3u", results.Deletion);
    if (results.dDeletion != 0 ) {
        fprintf(Log, "   (deducted:  %3u)", results.dDeletion);
    }
    fprintf(Log, "\n");
    total += results.Deletion;
    
    fprintf(Log, "Reporters:       %3u", results.Reporters);
    if (results.dReporters != 0 ) {
        fprintf(Log, "   (deducted:  %3u)", results.dReporters);
    }
    fprintf(Log, "\n");
    total += results.Reporters;
    
    fprintf(Log, "Total score:     %3u\n", total);
}
