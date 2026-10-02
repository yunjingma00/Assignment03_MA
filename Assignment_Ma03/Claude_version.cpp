// Program EECS348 Assignment 3 -ceo email prioritizer
//
//Description:
//This program prioritzes emails for a ceo using a list_based Maxheap
//
// input emailnext read and count commands 
// output: displays prioritized emails and the number of unread emails 
//
//collaborators:chatgpt about the explain the code meaning 
//and ask for the Cpp vocabulary
// CEO Email Prioritizer - object-oriented, hand-built list-based MaxHeap.
// Build: g++ -std=c++11 -o ceo_email ceo_email.cpp
// Run:   ./ceo_email commands.txt      (or pipe commands via stdin)
//collobation Claude not any fixing
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

// ---------------------------------------------------------------- Email
class Email {
private:
    std::string sender;
    std::string subject;
    std::string dateStr;   // MM-DD-YYYY as given
    int categoryRank;      // higher = read sooner
    int dateKey;           // YYYYMMDD for comparison
    long sequence;         // arrival order, breaks exact ties (earlier first)

    static int rankOf(const std::string& cat) {
        if (cat == "Boss")            return 5;
        if (cat == "Subordinate")     return 4;
        if (cat == "Peer")            return 3;
        if (cat == "ImportantPerson") return 2;
        return 1;                     // OtherPerson
    }

    static int keyOf(const std::string& d) {
        int mm = std::atoi(d.substr(0, 2).c_str());
        int dd = std::atoi(d.substr(3, 2).c_str());
        int yy = std::atoi(d.substr(6, 4).c_str());
        return yy * 10000 + mm * 100 + dd;
    }

public:
    Email() : categoryRank(0), dateKey(0), sequence(0) {}
    Email(const std::string& s, const std::string& subj,
          const std::string& d, long seq)
        : sender(s), subject(subj), dateStr(d),
          categoryRank(rankOf(s)), dateKey(keyOf(d)), sequence(seq) {}

    // true if this email should be read BEFORE other
    bool hasHigherPriorityThan(const Email& other) const {
        if (categoryRank != other.categoryRank)
            return categoryRank > other.categoryRank;
        if (dateKey != other.dateKey)
            return dateKey > other.dateKey;       // newest first
        return sequence < other.sequence;         // stable for same date
    }

    void display() const {
        std::cout << "Sender: "  << sender  << "\n"
                  << "Subject: " << subject << "\n"
                  << "Date: "    << dateStr << "\n";
    }
};

// -------------------------------------------------------------- MaxHeap
// List (array) based binary max-heap, written from scratch.
class MaxHeap {
private:
    Email* data;
    int capacity;
    int count;

    static int parent(int i) { return (i - 1) / 2; }
    static int left(int i)   { return 2 * i + 1; }
    static int right(int i)  { return 2 * i + 2; }

    void swapAt(int a, int b) {
        Email t = data[a];
        data[a] = data[b];
        data[b] = t;
    }

    void grow() {
        int newCap = capacity * 2;
        Email* bigger = new Email[newCap];
        for (int i = 0; i < count; i++) bigger[i] = data[i];
        delete[] data;
        data = bigger;
        capacity = newCap;
    }

    void siftUp(int i) {
        while (i > 0 && data[i].hasHigherPriorityThan(data[parent(i)])) {
            swapAt(i, parent(i));
            i = parent(i);
        }
    }

    void siftDown(int i) {
        while (true) {
            int best = i, l = left(i), r = right(i);
            if (l < count && data[l].hasHigherPriorityThan(data[best])) best = l;
            if (r < count && data[r].hasHigherPriorityThan(data[best])) best = r;
            if (best == i) break;
            swapAt(i, best);
            i = best;
        }
    }

public:
    MaxHeap() : data(new Email[16]), capacity(16), count(0) {}
    ~MaxHeap() { delete[] data; }

    // non-copyable (owns raw array)
    MaxHeap(const MaxHeap&) = delete;
    MaxHeap& operator=(const MaxHeap&) = delete;

    void insert(const Email& e) {
        if (count == capacity) grow();
        data[count] = e;
        siftUp(count);
        count++;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }

    const Email& peekMax() const { return data[0]; }

    Email extractMax() {
        Email top = data[0];
        count--;
        if (count > 0) {
            data[0] = data[count];
            siftDown(0);
        }
        return top;
    }
};

// ------------------------------------------------------- CEO Inbox
class CEOInbox {
private:
    MaxHeap heap;
    long nextSeq;

public:
    CEOInbox() : nextSeq(0) {}

    void receive(const std::string& sender, const std::string& subject,
                 const std::string& date) {
        heap.insert(Email(sender, subject, date, nextSeq++));
    }

    void next() const {
        if (heap.isEmpty()) {
            std::cout << "No unread emails.\n";
            return;
        }
        heap.peekMax().display();
    }

    void read() {
        if (heap.isEmpty()) {
            std::cout << "No unread emails.\n";
            return;
        }
        heap.extractMax();   // CEO has read it and dealt with it
    }

    void count() const {
        std::cout << "Unread emails: " << heap.size() << "\n";
    }
};

// ------------------------------------------------------- Command parser
class CommandProcessor {
private:
    CEOInbox& inbox;

    static std::string trim(const std::string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == std::string::npos) return "";
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    }

    static std::vector<std::string> splitCommas(const std::string& s) {
        std::vector<std::string> parts;
        std::string cur;
        std::stringstream ss(s);
        while (std::getline(ss, cur, ',')) parts.push_back(trim(cur));
        return parts;
    }

public:
    explicit CommandProcessor(CEOInbox& in) : inbox(in) {}

    void process(const std::string& rawLine) {
        std::string line = trim(rawLine);
        if (line.empty()) return;

        if (line.compare(0, 6, "EMAIL ") == 0) {
            std::vector<std::string> f = splitCommas(line.substr(6));
            if (f.size() != 3) {
                std::cerr << "Malformed EMAIL command: " << line << "\n";
                return;
            }
            inbox.receive(f[0], f[1], f[2]);
        } else if (line == "NEXT") {
            inbox.next();
        } else if (line == "READ") {
            inbox.read();
        } else if (line == "COUNT") {
            inbox.count();
        } else {
            std::cerr << "Unknown command: " << line << "\n";
        }
    }
};

// ------------------------------------------------------------------ main
int main(int argc, char* argv[]) {
    CEOInbox inbox;
    CommandProcessor processor(inbox);

    std::istream* in = &std::cin;
    std::ifstream file;
    if (argc > 1) {
        file.open(argv[1]);
        if (!file) {
            std::cerr << "Cannot open " << argv[1] << "\n";
            return 1;
        }
        in = &file;
    }

    std::string line;
    while (std::getline(*in, line)) processor.process(line);
    return 0;
}
