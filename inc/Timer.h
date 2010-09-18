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


template<class T>
class event_timer
{
	   asio::io_service& io_service_;
	   std::size_t interval_;
	   std::size_t tick_count_;
	   asio::deadline_timer timer_;
	   T *m_poClient;

private:
   void handle_timer_event(const system::error_code & error)
    {
       if (!error && interval_)
       {
          ++tick_count_;
    	   m_poClient->vOnTimeOut();
          initiate_timer();
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
      }
   }


public:

   event_timer(asio::io_service& io_service,
               const std::size_t& tid,
               const std::size_t& interval = 5,
               T * poClient=0)
   :io_service_(io_service),
     timer_(io_service),
     interval_(interval),
     tick_count_(0),
     m_poClient( poClient )
   {
	   std::cout<<"\n Starting Location Subsystem Simulation";
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
