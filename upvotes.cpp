#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// specifies whether the sequence x to x+1 in a range 
// is increasing, decreasing, or equal
enum RangeType {INC, DEC, EQUAL};

// specifies whether the head of the range length queue
// should be incremented, or a new node w/ value 0 should be 
// introduced at the node
enum AddType {PLUS, ZERO};

// simple node data structure
typedef struct node {
  struct node * next;
  int val;
} Node;

RangeType getType(int a, int b);
Node * init(int val, Node * next);
void KEqOne(int N);

// queue data structure with add() and remove() 
// functions that mimic pop() and remove() with
// modified behavior to suit problem
class Queue {
  public:
    
    Queue() {
      head = NULL;
      tail = NULL;
    }

    ~Queue() {}

    // delete tail node
    void deleteTail() {
      Node * temp = tail;
      tail = tail->next;
      delete(temp);  
      if(head == temp) {
        head = NULL;
      }    
    }

    // reflect the number of new subranges
    // caused by the sequence x to x+1 in the
    // queue
    void add(AddType type) {
      if(type == ZERO) {
        // if no new subranges are caused by x+1,
        // add a node with value 0 to the head to reflect
        // the change
        if(!head) {
          Node * elem = init(0, NULL);
          head = elem;
          tail = elem;
        }
        else {
          Node * elem = init(0, NULL);
          head->next = elem;
          head = elem;          
        }
      }
      else if(type == PLUS) {
        // if new subranges are caused by x+1,
        // increment the head node if it has
        // value > 0; add a new node with value 1
        // to the head otherwise
        if(!head) {
          Node * elem = init(1, NULL);
          head = elem;
          tail = elem;
        }
        else if(head->val == 0) {
          Node * elem = init(1, NULL);
          head->next = elem;
          head = elem;  
        }
        else {
          head->val = head->val + 1;          
        }
      }
    }


    // decrement tail node value otherwise
    void remove() {
      if(tail) {
        if(tail->val == 0) {
          deleteTail();
        }
        else {
          tail->val = tail->val - 1;
          if(tail->val == 0) {
            deleteTail();
          }          
        }        
      }
    }

    // get range length at tail
    int getTail() {
      if(!tail) return 0;
      else return tail->val;
    }

    // get range length at head
    int getHead() {
      if(!head) return 0;
      else return head->val;
    }

    // print queue
    void printList() {
      Node * temp;
      for(temp = tail; temp; temp = temp->next) {
        cout << "val: " << temp->val << endl;
      }
    }

  private:
    Node * head;
    Node * tail;
};

// determine range type in sequence
// x to x+1
RangeType getType(int a, int b) {
  if(a < b) return INC;
  else if(a > b) return DEC;
  else return EQUAL;
}

// initializes a new node
Node * init(int val, Node * next) {
  Node * elem = new Node;
  elem->next = next;
  elem->val = val;
  return elem;
}

// when k=1, the net number of subranges
// within each window (of which there are N)
// is always 0
void KEqOne(int N) {
  for(int i = 0; i < N; i++) {
    cout << 0 << endl;
  }
}

// find and print the net number of sub-ranges
// for all windows 
void findNetRanges(vector<int> votes, int k, int N) {
  
  // handle cases where k=1
  if(k == 1) {
    KEqOne(N);
    return;    
  }

  Queue * qInc = new Queue();
  Queue * qDec = new Queue();

  long ans = 0;
  
  for(int i = 0; i < votes.size() - 1; i++) {
    // when at range [x, x+k], remove all ranges that 
    // include x-1
    if(i >= k-1) {
      ans = ans - qInc->getTail() + qDec->getTail();
      qInc->remove();
      qDec->remove();
    }

    // determine whether there is an increasing,
    // decreasing, or equal range from x to x+1
    int type = getType(votes[i], votes[i+1]);

    // update the range length queue and 
    // the net range count 
    if(type == INC) {
      qInc->add(PLUS);
      ans = ans + qInc->getHead();
      qDec->add(ZERO);
    }
    else if(type == DEC) {
      qDec->add(PLUS);
      ans = ans - qDec->getHead();
      qInc->add(ZERO);
    }
    else if(type == EQUAL) {
      qInc->add(PLUS);
      qDec->add(PLUS);
      ans = ans + qInc->getHead() - qDec->getHead();
    }

    // if [0,x+1] has at least length k, then x+1 is 
    // the rightmost element of some subrange  
    if(i >= k-2) {
      cout << ans << endl;
    }
  }
}

// process input format and extract the value of important
// parameter variables
void getParams(vector<int> * votes, int * k, int * N) {
  
  string line;
  for(int i = 0; getline(cin, line); i++) {
    stringstream stream(line);
    // extract and set values of k and N
    if(i == 0) {
      stream >> *N;
      stream >> *k;
    }
    else {
      // extract each number (representing upvote count data 
      // for a day) and store it in an array
      while(stream) {
        int count;
        stream >> count;
        (*votes).push_back(count);
      }
      (*votes).pop_back();
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  // process input and store upvote count
  // for each day in an array
  int k = 0; 
  int N = 0;
  vector<int> votes;
  getParams(&votes, &k, &N);

  // find and print the net number of sub-ranges
  // for all windows 
  findNetRanges(votes, k, N);    

  return 0;
}




