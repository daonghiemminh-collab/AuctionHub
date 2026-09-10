#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <random>

using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;       // bid price
    long long timestamp; // Unix ms
    int auctionId;
};

// Comparator Utilities
// Ascending: ascending and ties broken by timestamp ascending
bool compareBidAscending(const Bid& a, const Bid& b) {
    if (a.amount != b.amount)
        return a.amount < b.amount;
    return a.timestamp < b.timestamp;
}

// Descending: descending and ties broken by timestamp ascending
bool compareBidDescending(const Bid& a, const Bid& b) {
    if (a.amount != b.amount)
        return a.amount > b.amount;
    return a.timestamp < b.timestamp;
}

//Utilities
void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << fixed << setprecision(2) << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    out.clear();
    vector<string> candidates = {
        path,
        "C:/Users/daong/AuctionHub/" + path,
        "AuctionHub/" + path,
        "../" + path,
        "../../" + path
    };
    ifstream f;
    for (const auto& p : candidates) {
        f.open(p);
        if (f.is_open()) break;
        f.clear();
    }
    if (!f.is_open()) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return !out.empty();
}

//Task A: Insertion Sort
void insertionSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 1; i < n; ++i) {
        Bid key = v[i];
        int j = i - 1;
        while (j >= 0 && compareBidAscending(key, v[j])) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

//Task B: Selection Sort
void selectionSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    for (int i = 0; i < n - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (compareBidDescending(v[j], v[maxIdx])) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            std::swap(v[i], v[maxIdx]);
        }
    }
}

// Helper to print Top 3 Bidders for Task B
void printTop3(const vector<Bid>& v) {
    cout << "Top 3 Bidders:\n";
    for (size_t i = 0; i < min(size_t(3), v.size()); ++i) {
        cout << "  " << (i + 1) << ". " << v[i].bidderId << " ($" << fixed << setprecision(2) << v[i].amount << ")\n";
    }
}

//Task C: Interchange Sort
void interchangeSort(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    long long swapCount = 0;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (compareBidAscending(v[j], v[i])) {
                std::swap(v[i], v[j]);
                swapCount++;
            }
        }
    }
    cout << "Interchange Sort completed. Total swaps performed: " << swapCount << "\n";
}

//Task D: Bubble Sort with Early Stop
bool bubbleSortEarlyStop(vector<Bid>& v) {
    int n = static_cast<int>(v.size());
    bool stableOnFirstPass = true;

    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (compareBidAscending(v[j + 1], v[j])) {
                std::swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (i == 0 && swapped) {
            stableOnFirstPass = false;
        }
        if (!swapped) break;
    }

    if (stableOnFirstPass) {
        cout << "Status: STABLE (Stream is already sorted, 0 swaps on first pass)\n";
    } else {
        cout << "Status: UNSTABLE (Swaps detected on first pass, stream was unsorted)\n";
    }

    return stableOnFirstPass;
}

//Task E: Quick Sort with Median-of-Three
int medianOfThree(vector<Bid>& v, int lo, int hi) {
    int mid = lo + (hi - lo) / 2;

    if (compareBidAscending(v[mid], v[lo]))
        std::swap(v[lo], v[mid]);
    if (compareBidAscending(v[hi], v[lo]))
        std::swap(v[lo], v[hi]);
    if (compareBidAscending(v[hi], v[mid]))
        std::swap(v[mid], v[hi]);

    std::swap(v[mid], v[hi - 1]);
    return hi - 1;
}

// Partition helper for Quick Sort
int partitionMedianOfThree(vector<Bid>& v, int lo, int hi) {
    int pivotIdx = medianOfThree(v, lo, hi);
    Bid pivot = v[pivotIdx];

    int i = lo;
    int j = hi - 1;

    while (true) {
        while (compareBidAscending(v[++i], pivot));
        while (compareBidAscending(pivot, v[--j]));
        if (i >= j) break;
        std::swap(v[i], v[j]);
    }

    std::swap(v[i], v[hi - 1]);
    return i;
}

void quickSort(vector<Bid>& v, int lo, int hi) {
    if (lo + 10 <= hi) {
        int pivotPos = partitionMedianOfThree(v, lo, hi);
        quickSort(v, lo, pivotPos - 1);
        quickSort(v, pivotPos + 1, hi);
    } else {
        for (int i = lo + 1; i <= hi; ++i) {
            Bid key = v[i];
            int j = i - 1;
            while (j >= lo && compareBidAscending(key, v[j])) {
                v[j + 1] = v[j];
                j--;
            }
            v[j + 1] = key;
        }
    }
}

// Wrapper for Quick Sort
void quickSortWrapper(vector<Bid>& v) {
    if (!v.empty()) {
        quickSort(v, 0, static_cast<int>(v.size()) - 1);
    }
}

// ---------- Driver Program ----------
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "AUCTIONHUB\n";

    cout << ">>> TASK A: Insertion Sort (Live Auction Monitor)\n";
    vector<Bid> testA = {
        {"alice", 100.0, 1000, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave",  103.0, 1010, 1}
    };
    cout << "Input Bids:\n";
    printBids(testA);
    insertionSort(testA);
    cout << "Output Bids (Sorted Ascending):\n";
    printBids(testA);
    cout << "------------------------------------------------------------\n\n";

    cout << ">>> TASK B: Selection Sort (Top-K Bid Finder)\n";
    vector<Bid> testB = {
        {"alice", 100.0, 1000, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave",  103.0, 1010, 1}
    };
    selectionSort(testB);
    printTop3(testB);
    cout << "Full Sorted (Descending):\n";
    printBids(testB);
    cout << "------------------------------------------------------------\n\n";

    cout << ">>> TASK C: Interchange Sort (Price Anomaly Validator)\n";
    vector<Bid> testC = {
        {"alice", 100.0, 1000, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave",  103.0, 1010, 1}
    };
    interchangeSort(testC);
    printBids(testC);
    cout << "------------------------------------------------------------\n\n";

    cout << ">>> TASK D: Bubble Sort Early Stop (Stabilization Detector)\n";
    cout << "Test Case 1 (Already-sorted):\n";
    vector<Bid> testD1 = {
        {"alice", 100.0, 1000, 1},
        {"carol", 101.0, 1003, 1},
        {"bob",   102.5, 1005, 1},
        {"dave",  103.0, 1010, 1}
    };
    bool res1 = bubbleSortEarlyStop(testD1);
    cout << "Result: " << (res1 ? "STABLE" : "UNSTABLE") << "\n\n";

    cout << "Test Case 2 (Reversed input):\n";
    vector<Bid> testD2 = {
        {"dave",  103.0, 1010, 1},
        {"bob",   102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"alice", 100.0, 1000, 1}
    };
    bool res2 = bubbleSortEarlyStop(testD2);
    cout << "Result: " << (res2 ? "STABLE" : "UNSTABLE") << "\n";
    cout << "------------------------------------------------------------\n\n";

    // Task E Demonstration: Quick Sort
    cout << ">>> TASK E: Quick Sort\n";
    string dataPath = "test_data/large.txt";
    if (argc > 1) {
        dataPath = argv[1];
    }

    vector<Bid> largeBids;

    auto t1 = high_resolution_clock::now();
    quickSortWrapper(largeBids);
    auto t2 = high_resolution_clock::now();
    duration<double, milli> elapsed = t2 - t1;

    bool sorted = is_sorted(largeBids.begin(), largeBids.end(), compareBidAscending);

    cout << "------------------------------------------------------------\n";
    cout << "                   TASK E RUNTIME BENCHMARK                 \n";
    cout << "------------------------------------------------------------\n";
    cout << "Total Bids Sorted    : " << largeBids.size() << "\n";
    cout << "Verification Check   : " << (sorted ? "PASSED (Correctly Sorted)" : "FAILED") << "\n";
    cout << "Execution Time       : " << fixed << setprecision(2) << elapsed.count() << " ms (" 
         << (elapsed.count() / 1000.0) << " seconds)\n";
    if (elapsed.count() > 0.0) {
        double throughput = largeBids.size() / (elapsed.count() / 1000.0);
        cout << "Throughput Rate      : " << setprecision(0) << throughput << " bids/sec\n";
    }
    cout << "------------------------------------------------------------\n";
    cout << "First 5 Sorted Bids:\n";
    printBids(largeBids, 5);
    cout << "============================================================\n";

    return 0;
}
