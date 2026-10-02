//Description:
//This program prioritzes emails for a ceo using a list_based Maxheap
//
// input emailnext read and count commands
// output: displays prioritized emails and the number of unread emails
//
//collaborators:chatgpt about the explain some part of the code functio
//and ask for the Cpp vocabulary and conecpt
//source: gemini generated the orignal code
//create date september 30th
/// revision date Oct 1
//debug and command and get a better code strucutre by myself
//author yunjing ma

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

// ---------------------------------------------------------------- Date
class DateKey {//class the datakey
private://get the private part
    int dateValue;//set the value of the date
    std::string rawDate;//set the orignal of the date

    int parseKey(const std::string& d) const {//convert the date form
	// convert mm-dd-yyyy to the numberical value
        if (d.length() < 10) return 0;// check if the form have the correct value
        int mm = std::atoi(d.substr(0, 2).c_str());// the first two string should be month
        int dd = std::atoi(d.substr(3, 2).c_str());// the second and the third should be the day
        int yy = std::atoi(d.substr(6, 4).c_str());// the last four are the year
        return yy * 10000 + mm * 100 + dd;//convert the string to the numberical
    }

public:
    DateKey() : dateValue(0), rawDate("") {}//the function of the referring and create with no date
    explicit DateKey(const std::string& d) : rawDate(d), dateValue(parseKey(d)) {}

    int getValue() const { return dateValue; }//return the value of the numberical
    std::string toString() const { return rawDate; }// return the value of the string type

    bool operator>(const DateKey& other) const {// get the boolean to check who is the newest date
        return dateValue > other.dateValue;
    }
};

// ---------------------------------------------------------------- Priority
// this function is to convert the importance of the sender by setting the number of the importance by each catogory of sender
// the more importance, the higer value return
class PriorityRank {
private:// private the variable
    int rank;// get the int of the importance

    int evaluateRank(const std::string& cat) const {//convert the catogory to the number which can stright compare the importance by number
        if (cat == "Boss")              return 5;//set boss > suboardinate > peer > importantperson
        if (cat == "Subordinate")       return 4;
        if (cat == "Peer")              return 3;
        if (cat == "ImportantPerson")   return 2;
        return 1; // OtherPerson
    }

public:
    PriorityRank() : rank(1) {}// create the empty function of the priority rank
    explicit PriorityRank(const std::string& cat) : rank(evaluateRank(cat)) {}// referring the function

    int getRank() const { return rank; }//get the rank value

    bool operator>(const PriorityRank& other) const {//check if more imortance
        return rank > other.rank;
    }
    bool operator==(const PriorityRank& other) const {//check if the same
        return rank == other.rank;
    }
};

// ---------------------------------------------------------------- Email
// this function are import the former class of date and prioity
// This class represents one email and stores its sender, subject,
// date, priority, and arrival order.
class Email {
private:
    std::string senderCategory;//store the sender category
    std::string subjectLine;//store the email subject
    DateKey date;//use the DateKey class
    PriorityRank priority;//store the priority of the email
    long arrivalSequence;//store the arrival order of the email

public:
    Email() : arrivalSequence(0) {}//create an empty email object

    Email(const std::string& sender, const std::string& subject,
          const std::string& dateStr, long sequence)//create an email object using the provided information
        : senderCategory(sender),
          subjectLine(subject),
          date(dateStr),
          priority(sender),
          arrivalSequence(sequence) {}

    bool hasHigherPriorityThan(const Email& other) const {//determine whether this email has higher priority than another email
        if (!(priority == other.priority)) {//check which priority is higher
            return priority > other.priority;
        }

        if (date.getValue() != other.date.getValue()) {//if the priority is the same, compare the dates
            return date > other.date;//newest first for the same category
        }

        return arrivalSequence < other.arrivalSequence;//use arrival order if priority and date are the same
    }

    void display() const {//display the information of the email
        std::cout << "Sender: "  << senderCategory  << "\n"
                  << "Subject: " << subjectLine     << "\n"
                  << "Date: "    << date.toString() << "\n";
    }
};


// -------------------------------------------------------------- MaxHeap
class MaxHeap {
private:
    Email* heapArray;//store the address of the email array
    int capacity;//store the total capacity of the array
    int count;//store the current number of emails

    int getParentIndex(int i) const {//get the parent index of the current node
        return (i - 1) / 2;
    }

    int getLeftIndex(int i) const {//get the left child's index
        return 2 * i + 1;
    }

    int getRightIndex(int i) const {//get the right child's index
        return 2 * i + 2;
    }

    void swapElements(int a, int b) {//swap two email objects in the heap
        Email temp = heapArray[a];//store the first email temporarily
        heapArray[a] = heapArray[b];//move the second email to the first position
        heapArray[b] = temp;//move the first email to the second position
    }

    void resize() {//increase the capacity of the heap
        int newCapacity = capacity * 2;//double the current capacity

        Email* newArray = new Email[newCapacity];//create a new larger email array

        for (int i = 0; i < count; i++) {//copy all existing emails to the new array
            newArray[i] = heapArray[i];
        }

        delete[] heapArray;//delete the old array

        heapArray = newArray;//make heapArray point to the new array

        capacity = newCapacity;//update the capacity
    }

    void siftUp(int i) {//move a new email upward to restore the MaxHeap property
        while (i > 0 && heapArray[i].hasHigherPriorityThan(heapArray[getParentIndex(i)])) {//continue while the current email has higher priority than its parent
            swapElements(i, getParentIndex(i));//swap the current email with its parent
            i = getParentIndex(i);//move to the parent's position
        }
    }

    void siftDown(int i) {//move an email downward to restore the MaxHeap property
        while (true) {//continue checking until the email is in the correct position
            int bestIndex = i;//assume the current index has the highest priority
            int left = getLeftIndex(i);//get the left child's index
            int right = getRightIndex(i);//get the right child's index

            if (left < count && heapArray[left].hasHigherPriorityThan(heapArray[bestIndex])) {//check if the left child has higher priority
                bestIndex = left;//set the left child as the best index
            }

            if (right < count && heapArray[right].hasHigherPriorityThan(heapArray[bestIndex])) {//check if the right child has higher priority
                bestIndex = right;//set the right child as the best index
            }

            if (bestIndex == i) break;//stop if the current index already has the highest priority

            swapElements(i, bestIndex);//swap the current node with the higher-priority child
            i = bestIndex;//continue checking from the new position
        }
    }

public:
    MaxHeap() : heapArray(new Email[16]), capacity(16), count(0) {}//create an empty heap with a capacity of 16

    ~MaxHeap() {//destroy the heap and release the memory
        delete[] heapArray;//delete the dynamically allocated array
    }
    MaxHeap(const MaxHeap&) = delete;//prevent copying the MaxHeap object
    MaxHeap& operator=(const MaxHeap&) = delete;//prevent assigning one MaxHeap object to another
    void insert(const Email& email) {//insert a new email into the heap
        if (count == capacity) {//check if the heap is full
            resize();//increase the capacity of the heap
        }
        heapArray[count] = email;//put the new email at the end of the heap
        siftUp(count);//move the new email upward if necessary
        count++;//increase the number of emails
    }

    bool isEmpty() const {//check whether the heap is empty
        return count == 0;
    }

    int getSize() const {//return the number of emails in the heap
        return count;
    }

    const Email& peekMax() const {//get the highest-priority email without removing it
        return heapArray[0];
    }

    Email extractMax() {//remove and return the highest-priority email
        Email topEmail = heapArray[0];//save the highest-priority email
        count--;//decrease the number of emails
        if (count > 0) {//check if there are still emails in the heap
            heapArray[0] = heapArray[count];//move the last email to the root
            siftDown(0);//move the new root downward if necessary
        }

        return topEmail;//return the removed email
    }
};


// ------------------------------------------------------- CEO Inbox
class CEOInbox {
private:
    MaxHeap priorityQueue;//store emails in priority order
    long sequenceCounter;//store the arrival sequence number
public:
    CEOInbox() : sequenceCounter(0) {}//create an empty inbox
    void receiveEmail(const std::string& sender, const std::string& subject, const std::string& date) {//receive and insert a new email
        Email newEmail(sender, subject, date, sequenceCounter++);//create a new email object
        priorityQueue.insert(newEmail);//insert the email into the priority queue
    }

    void displayNextEmail() const {//display the highest-priority unread email
        if (priorityQueue.isEmpty()) {//check if there are no unread emails
            std::cout << "No unread emails.\n";//display a message when the inbox is empty
            return;//stop the function
        }
        std::cout << "Next email:\n";//display the next email label
        priorityQueue.peekMax().display();//display the highest-priority email
    }

    void readCurrentEmail() {//remove the highest-priority unread email
        if (priorityQueue.isEmpty()) {//check if there are no unread emails
            std::cout << "No unread emails.\n";//display a message when the inbox is empty
            return;//stop the function
        }
        priorityQueue.extractMax();//remove the highest-priority email
    }

    void displayCount() const {//display the number of unread emails
        std::cout << "There are " << priorityQueue.getSize()//get the number of unread emails
                  << " emails to read.\n";//display the unread email count
    }
};


// ------------------------------------------------------- String Utility Object
class StringTokenizer {
public:
    static std::string trim(const std::string& s) {//remove extra spaces from the string
        size_t start = s.find_first_not_of(" \t\r\n");//find the first non-space character
        if (start == std::string::npos) return "";//return an empty string if there is no content
        size_t end = s.find_last_not_of(" \t\r\n");//find the last non-space character
        return s.substr(start, end - start + 1);//return the string without extra spaces
    }

    static std::vector<std::string> splitByComma(const std::string& s) {//split the string using commas
        std::vector<std::string> parts;//store the separated parts
        std::string current;//store the current part
        std::stringstream ss(s);//create a string stream from the input

        while (std::getline(ss, current, ',')) {//read each part separated by a comma
            parts.push_back(trim(current));//remove spaces and add the part to the vector
        }

        return parts;//return all separated parts
    }
};


// ------------------------------------------------------- Command Processor
class CommandProcessor {
private:
    CEOInbox& inbox;//use a reference to the CEO inbox

public:
    explicit CommandProcessor(CEOInbox& targetInbox) : inbox(targetInbox) {}//connect the command processor to the inbox

    void processCommand(const std::string& rawLine) {//process one input command
        std::string line = StringTokenizer::trim(rawLine);//remove extra spaces from the input

        if (line.empty()) return;//ignore empty lines

        if (line.compare(0, 6, "EMAIL ") == 0) {//check if the command is EMAIL
            std::vector<std::string> fields = StringTokenizer::splitByComma(line.substr(6));//split the email information

            if (fields.size() != 3) {//check if the email command has three parts
                std::cerr << "Malformed EMAIL command: " << line << "\n";//display an error for an invalid email command
                return;//stop processing this command
            }

            inbox.receiveEmail(fields[0], fields[1], fields[2]);//add the email to the inbox

        } else if (line == "NEXT") {//check if the command is NEXT
            inbox.displayNextEmail();//display the next email

        } else if (line == "READ") {//check if the command is READ
            inbox.readCurrentEmail();//remove the current highest-priority email

        } else if (line == "COUNT") {//check if the command is COUNT
            inbox.displayCount();//display the number of unread emails

        } else {//handle an unknown command
            std::cerr << "Unknown command: " << line << "\n";//display an error message
        }
    }
};


// ------------------------------------------------------------------ main
int main(int argc, char* argv[]) {//start the main function
    CEOInbox inbox;//create the CEO inbox
    CommandProcessor processor(inbox);//create the command processor

    std::istream* inputStream = &std::cin;//use standard input by default
    std::ifstream fileStream;//create a file input stream

    if (argc > 1) {//check if an input file was provided
        fileStream.open(argv[1]);//open the input file

        if (!fileStream) {//check if the file was opened successfully
            std::cerr << "Cannot open file: " << argv[1] << "\n";//display an error message
            return 1;//end the program with an error
        }

        inputStream = &fileStream;//use the file as the input stream
    }

    std::string line;//store one input line

    while (std::getline(*inputStream, line)) {//read input lines until the end of the file
        processor.processCommand(line);//process the current command
    }

    return 0;//end the program successfully
}
