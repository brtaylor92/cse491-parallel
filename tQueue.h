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

    size_t pop(T &val) {
      while(inUse.exchange(true));
      size_t oldSize = q.size();
      if(oldSize) {
        val = q.front();
        q.pop();
      }
      inUse = false;
      return oldSize;
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
