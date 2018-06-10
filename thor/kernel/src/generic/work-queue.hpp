#ifndef THOR_GENERIC_WORK_QUEUE_HPP
#define THOR_GENERIC_WORK_QUEUE_HPP

#include <frg/list.hpp>

namespace thor {

struct WorkQueue;

struct Worklet {
	friend struct WorkQueue;

	void setup(void (*run)(Worklet *), WorkQueue *wq) {
		_run = run;
		_workQueue = wq;
	}

private:
	WorkQueue *_workQueue;
	void (*_run)(Worklet *);
	frg::default_list_hook<Worklet> _hook;
};

struct WorkQueue {
	static void post(Worklet *worklet);

	bool check();

	void run();

protected:
	virtual void wakeup() = 0;

private:
	frg::intrusive_list<
		Worklet,
		frg::locate_member<
			Worklet,
			frg::default_list_hook<Worklet>,
			&Worklet::_hook
		>
	> _pending;
};

} // namespace thor

#endif // THOR_GENERIC_WORK_QUEUE_HPP