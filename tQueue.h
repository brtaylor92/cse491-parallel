#include <queue>
#include <atomic>

using std::queue;
using std::atomic;

template <class T> class tQueue {
  public:

    explicit tQueue() : inUse(false) {};

    explicit tQueue(const Container& cont) : inUse(false), q(cont) {};

    ~tQueue = default;

    T pop() {
      getRights();
      return q.pop();
    }
    
    void push(T val) {
      getRights();
      q.push(val);
    }

    bool empty() {
      getRights();
      return q.empty();
    }

  private:
    atomic<bool> inUse;
    queue q;

    void getRights() {
      while(!inUse.exchange(true));
    }
}
