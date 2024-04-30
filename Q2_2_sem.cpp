#include <iostream>
#include <thread>
#include <semaphore.h>

// Define semaphores
sem_t semaphore1, semaphore2;

void runMeFirst() {
    std::cout << "Run me first" << std::endl;
    // Signal semaphore1 to allow the second function to proceed
    sem_post(&semaphore1);
}

void runMeSecond() {
    // Wait until semaphore1 is signaled
    sem_wait(&semaphore1);
    std::cout << "I run second" << std::endl;
    // Signal semaphore2 to allow the third function to proceed
    sem_post(&semaphore2);
}

void runMeThird() {
    // Wait until semaphore2 is signaled
    sem_wait(&semaphore2);
    std::cout << "I run third" << std::endl;
}

int main() {
    // Initialize semaphores
    sem_init(&semaphore1, 0, 0);
    sem_init(&semaphore2, 0, 0);

    // Create threads
    std::thread t1(runMeFirst);
    std::thread t2(runMeSecond);
    std::thread t3(runMeThird);

    // Join threads
    t1.join();
    t2.join();
    t3.join();

    // Destroy semaphores
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);

    return 0;
}