class Train {
public: 
	Train() = delete;
	Train(const Train & t) : speed(t.speed) {};
	Train(const uint32_t s) : speed(s) {}
	Train(Train && t) : speed(t.speed) { t.speed = 0; }
	~Train() = default;
	Train &operator=(const Train &t) { return speed = t.speed, *this; }
	uint32_t getSpeed() { return speed; } 
	void setSpeed(const uint32_t s) { speed = s; }

private:
	uint32_t speed; 
};