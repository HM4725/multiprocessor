/* Page 4, Figure 1.2
 * Page 5, Figure 1.3
 */

#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <ctime>

#define NTHREADS 8

class Counter {
  private:
    long value;
  public:
    long getAndIncrement() {
      return value++;
    }
    Counter() : value(0) {}
};

bool isPrime(long n) {
  for (long i = 2; i<=(long)sqrt(n); i++) {
      if (n % i == 0) {
      return false;
    }
  }
  return true;
}

long countPrime(long limit) {
  long total_count = 0;
  std::vector<std::thread> threads;
  Counter counter;
  long results[NTHREADS] = {0,};
  threads.reserve(NTHREADS);
  for (int i = 0; i < NTHREADS; i++) {
    threads.emplace_back([&results, &counter, limit](int id){
      long n = 1;
      long count = 0;
      while (n < limit) {
        n = counter.getAndIncrement();
        if (isPrime(n)) {
          count += 1;
        }
      }
      results[id] = count;
    }, i);
  }
  for (auto& thread: threads) {
    thread.join();
  }
  for (int i = 0; i < NTHREADS; i++) {
    total_count += results[i];
  }
  return total_count;
}

int main() {
  struct timespec cbegin, cend;
  double duration;
  long result;

  clock_gettime(CLOCK_MONOTONIC, &cbegin);
  result = countPrime((long)pow(10, 7));
  clock_gettime(CLOCK_MONOTONIC, &cend);
  duration = (cend.tv_sec - cbegin.tv_sec) + (cend.tv_nsec - cbegin.tv_nsec) / 1000000000.0;
  std::cout << "Result: " << result << std::endl;
  std::cout << "Runtime: " << duration << "s" << std::endl;
  return 0;
}