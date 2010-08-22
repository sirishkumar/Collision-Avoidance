#include <iostream>
#include <boost/asio.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

using namespace boost;

class event_timer
{
public:

   event_timer(asio::io_service& io_service,
               const std::size_t& tid,
               const std::size_t& interval = 5)
   : io_service_(io_service),
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

private:

   inline void initiate_timer()
   {
      if (interval_)
      {
         timer_.expires_from_now(boost::posix_time::milliseconds(interval_));
         timer_.async_wait(
            boost::bind(&event_timer::handle_timer_event,this,
               asio::placeholders::error));
         before_ = boost::posix_time::microsec_clock::universal_time();
      }
   }

   inline void handle_timer_event(const asio::error_code& error)
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

   asio::io_service& io_service_;
   std::size_t tid_;
   std::size_t interval_;
   std::size_t tick_count_;
   asio::deadline_timer timer_;
   boost::posix_time::ptime before_;
   boost::posix_time::ptime after_;
   std::size_t total_diff_;
};

int main()
{

   std::cout << "Timer Test" << std::endl;

   asio::io_service io_service;

   try
   {
      const std::size_t et_cnt = 1000;
      std::vector<event_timer*> et_lst;

      for(unsigned int i = 0; i < et_cnt; ++i)
      {
         et_lst.push_back(new event_timer(io_service,i,10));
      }

      for(unsigned int i = 0; i < et_cnt;)
      {
         et_lst[i++]->start();
      }

      std::size_t thread_pool_size = 100;

      //Create a pool of threads to run all of the io_services.
      std::vector<boost::shared_ptr<boost::thread> > threads;
      for (std::size_t i = 0; i < thread_pool_size; ++i)
      {
         boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&asio::io_service::run, &io_service)));
         threads.push_back(thread);
      }

      // Wait for all threads in the pool to exit.
      for (std::size_t i = 0; i < threads.size(); ++i)
         threads[i]->join();
      for(unsigned int i = 0; i < et_cnt; delete et_lst[i++]);

   }
   catch(std::exception& e)
   {
      std::cout << "Exception: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}
