#include <iostream>
#include <thread>
#include <semaphore.h>

// Define semaphores
sem_t semaphore;

void runMeFirst() {
    // Wait until semaphore is signaled
    sem_wait(&semaphore);
    std::cout << "Run me first" << std::endl;
}

void runMeSecond() {
    // Execute second function
    std::cout << "I run second" << std::endl;
    // Signal semaphore to allow the first function to proceed
    sem_post(&semaphore);
}

int main() {
    // Initialize semaphore
    sem_init(&semaphore, 0, 0);

    // Create threads
    std::thread t1(runMeFirst);
    std::thread t2(runMeSecond);

    // Join threads
    t1.join();
    t2.join();

    // Destroy semaphore
    sem_destroy(&semaphore);

    return 0;
}