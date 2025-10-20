//#ifndef PX_TASK_HPP
//#define PX_TASK_HPP
//
//#include "exception.hpp"
//
//#include <thread>
//
//namespace px
//{
//	// N.B Polling rate cannot be less than client idling poll rate (bad things happen! task never destroyed Cordell: check this)
//	class Task
//	{
//	public:
//		typedef std::function<void()> Function;			// This is the function that will be the task/work to do (run in secondary thread)...
//		typedef std::function<void()> StepCallback;			// Callback for step (polled by main thread during task)
//		typedef std::function<void()> FinishCallback;		// Callback for finish (called by main thread when complete)
//		typedef std::function<void(const Exception&)> ErrorCallback;		// Callback for error (called by main thread when complete)
//
//		enum State
//		{
//			TaskNotStarted,
//			TaskPolled,
//			TaskRunning,
//			TaskFinished,
//			TaskErrorer
//		};
//
//		Task();
//		Task(Function func);
//		Task(Function func, FinishCallback finish);
//		Task(Function func, StepCallback step, FinishCallback finish);
//
//		virtual ~Task();
//
//		// The function/workload to perform...
//		void Func(Function function) { function_ = function; }
//		void PollingRate(unsigned int pollingRate) { pollRate_ = pollingRate; }
//
//		// These are called and operate in the main thread (that constructed this task), not the threaded task...
//
//		// Check if this task is still running...
//		bool Running() { return taskState_; }
//
//		// Called each step (based on polling rate)...
//		virtual void Step() { if (stepCallback_) { stepCallback_(); } }
//		void Step(StepCallback stepCallback) { stepCallback_ = stepCallback; }
//
//		// Called when finished (will be destroyed after this)...
//		virtual void Finished() { if (finishCallback_) { finishCallback_(); } }
//		void Finished(FinishCallback finishCallback) { finishCallback_ = finishCallback; }
//
//		// Called when error...
//		virtual void Errored(const Exception& e)
//		{
//			if (errorCallback_)
//				errorCallback_(e);
//			/*else
//				AxW::ReportException({ e });*/
//		}
//		void Errored(ErrorCallback errorCallback) { errorCallback_ = errorCallback; }
//
//		// the thread ID of the task
//		unsigned int ThreadID() const;
//
//		// Not to be called externally (only by invoke task)!!!!
//		void Run();
//
//		// Called by main thread to check if to call update
//		State Poll();
//
//		void SetError(const Exception& e) { exception_ = e; }
//		std::optional<Exception> GetError() const { return exception_; }
//
//	private:
//		// the thread...
//		std::thread thread_;
//
//		// the thread state...
//		std::atomic<State> taskState_;
//
//		// the thread id...
//		std::thread::id id_;
//
//		// timer for polling...
//		Timer timer_;
//		unsigned int pollRate_ = 0;
//
//		Function function_;
//		StepCallback stepCallback_;
//		FinishCallback finishCallback_;
//		ErrorCallback errorCallback_;
//		std::optional<AxW::Exception> exception_;
//
//		static void InvokeTask(Task::Function task, Task* owner);
//	};
//
//	// Spawn a task, this will add it to tasks list and then run it...
//	void Spawn(std::shared_ptr<Task> task, unsigned int pollingRate = 0);
//
//	// The finish callback is called after the Task::Finish()...  maybe deprecate!
//	//void Spawn(std::shared_ptr<Task> task, std::function<void()> finishedCallback);
//
//	// Is main thread...
//	bool IsMainThread();
//
//
//}
//
//#endif // PX_TASK_HPP