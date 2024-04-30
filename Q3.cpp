#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>

using namespace std;

// Define a structure to represent a request from a client
struct RequestStructure {
    int request_id;         // Unique identifier for the request
    string ip_address;      // IP address of the client
    string page_requested;  // Web page requested by the client
};

// Global variables for message queue and synchronization
queue<RequestStructure> msg_queue;     // Queue to store incoming requests
mutex mtx;                             // Mutex for protecting access to shared resources
condition_variable cv;                 // Condition variable for synchronization
int request_counter = 0;               // Counter to generate unique request IDs

// List of web pages that can be requested
string webPages[10] = {"google.com", "yahoo.com", "bing.com", "amazon.com", "facebook.com", "twitter.com", "linkedin.com", "instagram.com", "reddit.com", "stackoverflow.com"};

// Function to generate a random web page from the list
string getRandomWebPage() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);
    return webPages[dis(gen)]; // Return a random web page from the list
}

// Function to simulate receiving requests from clients
void listen() {
    while (true) {
        // Sleep for a random duration to simulate the arrival of a request
        int sleep_duration = rand() % 3 + 1;
        this_thread::sleep_for(chrono::seconds(sleep_duration));

        // Create a new request with a unique ID and a random web page
        RequestStructure request;
        request.request_id = ++request_counter;
        request.ip_address = ""; // For simplicity, we leave the IP address empty
        request.page_requested = getRandomWebPage();

        // Lock the mutex before accessing the shared message queue
        unique_lock<mutex> lock(mtx);

        // Add the request to the message queue
        msg_queue.push(request);

        // Print a message indicating that a request has been received
        cout << "Request " << request.request_id << " received for webpage: " << request.page_requested << endl;

        // Notify one of the worker threads to process the request
        cv.notify_one();
    }
}

// Function to process requests from the message queue
void do_request(int thread_id) {
    while (true) {
        // Lock the mutex before accessing the shared message queue
        unique_lock<mutex> lock(mtx);

        // Wait until there is a request in the queue
        cv.wait(lock, [] { return !msg_queue.empty(); });

        // Retrieve the request from the front of the queue
        RequestStructure request = msg_queue.front();
        msg_queue.pop();

        // Print a message indicating that the request has been processed
        cout << "Thread " << thread_id << " completed request " << request.request_id << " requesting webpage " << request.page_requested << endl;

        // Unlock the mutex
        lock.unlock();
    }
}

int main() {
    // Create a thread to simulate receiving requests
    thread t1(listen);

    // Create multiple threads to process requests
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
        // Start threads with IDs starting from 2
        threads.push_back(thread(do_request, i + 2));
    }

    // Wait for the listening thread to finish
    t1.join();

    // Wait for all worker threads to finish
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
