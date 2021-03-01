#include <boost/utility/base_from_member.hpp>
#include <sstream>

class TaskProcessor
{
    std::ostream& log_;

public:
    explicit TaskProcessor(std::ostream& log)
        : log_(log)
    {
    }

    void process()
    {
        log_ << "Starting data processing.";
    }

protected:
    virtual void do_process() = 0;

};

class FakeTaskProcessorFixed
    : boost::base_from_member<std::ostringstream>,
    public TaskProcessor
{
    typedef boost::base_from_member<std::ostringstream> logger_t;

    virtual void do_process() override
    {
        logger_t::member << "Fake processor processed.";
    }

public: 

    FakeTaskProcessorFixed()
        : logger_t(), TaskProcessor(logger_t::member)
    {
    }
};

int main()
{
    FakeTaskProcessorFixed obj;

    return 0;
}