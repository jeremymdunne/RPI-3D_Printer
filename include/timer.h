#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000
#define HUNDRED 100
#define TEN 10

namespace timer{

   double getElapsedSeconds(timespec *start, timespec *end);
   double getElapsedSeconds(timespec *start);
   double getElapsedNanoSeconds(timespec *start, timespec *end);
   double getElapsedNanoSeconds(timespec *start);
};
