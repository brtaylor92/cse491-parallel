#include <queue>
#include <atomic>

using std::queue;
using std::atomic;

template <class T> class TQueue {
  public:
    explicit TQueue() : inUse(false) {};

    TQueue &operator=(TQueue<T> &rhs) {
      while(inUse.exchange(true));
      while(rhs.inUse.exchange(true));
      q = q.rhs();
      rhs.inUse = inUse = false;
      return *this;
    }

    TQueue(TQueue<T> &rhs) : inUse(false) {
      while(rhs.inUse.exchange(true));
      q = q.rhs();
      rhs.inUse = false;
    }

    ~TQueue() = default;

    bool pop(T &val) {
      while(inUse.exchange(true));
      if(empty()) return false;
      val = q.front();
      q.pop();
      inUse = false;
      return true;
    }
    
    void push(T val) {
      while(inUse.exchange(true));
      q.push(val);
      inUse = false;
    }

    bool empty() { return q.empty();  }

  private:
    atomic<bool> inUse;
    queue<T> q;

};
