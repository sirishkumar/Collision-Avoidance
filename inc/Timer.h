#include <iostream>
#include <boost/asio.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/asio/error.hpp>


using namespace boost;



class event_timer
{
	   asio::io_service& io_service_;
	   std::size_t tid_;
	   std::size_t interval_;
	   std::size_t tick_count_;
	   asio::deadline_timer timer_;
	   boost::posix_time::ptime before_;
	   boost::posix_time::ptime after_;
	   std::size_t total_diff_;

private:
   void handle_timer_event(const system::error_code & error)
    {
       if (!error && interval_)
       {
          after_ = boost::posix_time::microsec_clock::universal_time();
          boost::posix_time::time_duration duration = after_ - before_;
          total_diff_ += std::abs(interval_ - duration.total_milliseconds());
          ++tick_count_;
          if (tick_count_ < 200)
             initiate_timer();
          else
             std::cout << "Timer["<< tid_ <<"]\tTick["<< tick_count_ <<"] Average Diff: " << total_diff_ / (1.0 * tick_count_) << std::endl;
       }
    }

   void initiate_timer()
   {
      if (interval_)
      {
         timer_.expires_from_now(boost::posix_time::seconds(interval_));
         timer_.async_wait(
            boost::bind(&event_timer::handle_timer_event,this,
               asio::placeholders::error));
         before_ = boost::posix_time::microsec_clock::universal_time();
      }
   }


public:

   event_timer(asio::io_service& io_service,
               const std::size_t& tid,
               const std::size_t& interval = 5)
   :io_service_(io_service),
     timer_(io_service),
     tid_(tid),
     interval_(interval),
     tick_count_(0),
     total_diff_(0)
   {
   }

   void start()
   {
      timer_.cancel();
      initiate_timer();
   }

   void stop()
   {
      timer_.cancel();
   }

   void set_interval(const std::size_t& milliseconds)
   {
      interval_ = milliseconds;
   }




};
