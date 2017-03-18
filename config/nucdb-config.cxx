#include <iostream>
#include "getopt.h"
#include <math.h>
#include "NucDBConfig.h"

#define no_argument 0
#define required_argument 1 
#define optional_argument 2

void print_usage();
void print_libs();
void print_cflags();
void print_ldflags();
void print_inc();
void print_version();
void print_grid();
void print_prefix();

int main(int argc, char * argv[]) {

   if( argc < 2 ){
      print_usage();
      std::cout << std::endl;
      return 0;
   }
   const struct option longopts[] =
   {
      {"version",   no_argument,  nullptr, 'v'},
      {"help",      no_argument,  nullptr, 'h'},
      {"libs",      no_argument,  nullptr, 'l'},
      {"cflags",    no_argument,  nullptr, 'c'},
      {"ldflags",   no_argument,  nullptr, 'd'},
      {"inc",       no_argument,  nullptr, 'i'},
      {"grid",      no_argument,  nullptr, 'g'},
      {"prefix",    no_argument,  nullptr, 'p'},
      {nullptr,0,nullptr,0}
   };

   int index = 0;
   int iarg  = 0;

   //turn off getopt error message
   opterr=1; 

   while(iarg != -1)
   {
      iarg = getopt_long(argc, argv, "vhlcpd", longopts, &index);

      switch (iarg)
      {
         case 'h':
            print_usage();
            break;

         case 'v':
            print_version();
            break;

         case 'l':
            print_libs();
            break;

         case 'c':
            print_cflags();
            break;

         case 'i':
            print_inc();
            break;

         case 'd':
            print_ldflags();
            break;

         case 'p':
            print_prefix();
            break;

         case 'g':
            print_grid();
            break;


      }
   }

   std::cout << std::endl;

   return 0; 
}

void print_version(){
   std::cout << "NucDB Version " << NucDB_MAJOR_VERSION << "." << NucDB_MINOR_VERSION << " ";
}

void print_usage(){
   std::cout << "nucdb-config --libs --cflags --ldflags --inc --grid" << " ";
}

void print_libs(){
   std::cout << NucDB_CXX_LIBS << " ";
}

void print_inc(){
   std::cout << NucDB_CXX_INC_DIR << " ";
}

void print_cflags(){
   std::cout << NucDB_CXX_CFLAGS << " ";
}

void print_ldflags(){
   std::cout << NucDB_CXX_LDFLAGS << " ";
}

void print_grid(){
   std::cout << NucDB_DATA_DIR << " ";
}

void print_prefix(){
   std::cout << NucDB_PREFIX << " ";
}

