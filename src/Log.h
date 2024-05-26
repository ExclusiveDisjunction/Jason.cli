#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <climits>
#include <type_traits>
#include <concepts>

#include "DateTime.h"

enum LoggerLevel
{
    LL_None = INT_MAX,
    LL_Debug = 1,
    LL_Info = 2,
    LL_Warning = 3,
    LL_Error = 4,
    LL_Critical = 5
};

class Logger
{
private:
    bool StartLog(LoggerLevel level)
    {
        if (level == LoggerLevel::LL_None)
            return EndLog();

        if (this->State != LoggerLevel::LL_None)
            return false;

        this->State = level;
        std::cout << "State is " << this->State << std::endl;

        if (!CurrentLogIsIgnored())
        {
            //We need to write the begining of the level.

            Out << DateTime::Now().ToString(DateStringFormat::ShortDate, TimeStringFormat::ExtendedTime, false) << ' ';
            switch (level)
            {
                case LoggerLevel::LL_None:
                    return false;
                case LoggerLevel::LL_Debug:
                    Out << "DEBUG ";
                    break;
                case LoggerLevel::LL_Info:
                    Out << "INFO ";
                    break;
                case LoggerLevel::LL_Warning:
                    Out << "WARNING ";
                    break;
                case LoggerLevel::LL_Error:
                    Out << "ERROR ";
                    break;
                case LoggerLevel::LL_Critical:
                    Out << "CRITICAL ";
                    break;
            }
        }

        return true;
    }
    bool EndLog()
    {
        if (!IsInLog())
            return false;

        Out << '\n';
        this->State = LoggerLevel::LL_None;
        return true;
    }

    std::ofstream Out;
    LoggerLevel State;
    LoggerLevel Level;

public:
    Logger(const std::string& Path, LoggerLevel Level = LoggerLevel::LL_Info) : Out(Path, std::ios::trunc), State(LoggerLevel::LL_None), Level(Level)
    {
        if (!Out)
            throw std::logic_error("Cannot open file at that path.");
    }
    ~Logger()
    {
        if (IsInLog())
            EndLog();

        Out.close();
    }

    friend Logger& Debug(Logger&);
    friend Logger& Info(Logger&);
    friend Logger& Warning(Logger&);
    friend Logger& Error(Logger&);
    friend Logger& Critical(Logger&);
    friend Logger& EndLog(Logger&);

    /// @brief Returns true if the logger has started, but not ended a log.
    /// @return A boolean containing the state.
    bool IsInLog() const
    {
        return State != LoggerLevel::LL_None;
    }
    bool CurrentLogIsIgnored() const
    {
        return State < Level;
    }

    template<typename T>
    void Write(const T& obj)
    {
        if (!IsInLog())
            throw std::logic_error("Cannot insert into log when a level has not been specified.");

        if (!CurrentLogIsIgnored())
            this->Out << obj;
    }

    Logger& operator<<(const std::string& obj)
    {
        try
        {
            this->Write(obj);
            return *this;
        }
        catch (std::logic_error& e)
        {
            throw e;
        }
    }
    template<typename T>
    requires (std::is_arithmetic_v<T> && !std::is_pointer_v<T>)
    Logger& operator<<(const T& obj)
    {
        try
        {
            this->Write(obj);
            return *this;
        }
        catch (std::logic_error& e)
        {
            throw e;
        }
    }

    Logger& operator<<(Logger& (*func)(Logger&))
    {
        return func(*this);
    }

};

Logger& Debug(Logger& in)
{
    if (!in.StartLog(LoggerLevel::LL_Debug))
        throw std::logic_error("Cannot start log line because another line has not been completed. Please finish that line first.");

    return in;
}
Logger& Info(Logger& in)
{
    if (!in.StartLog(LoggerLevel::LL_Info))
        throw std::logic_error("Cannot start log line because another line has not been completed. Please finish that line first.");

    return in;
}
Logger& Warning(Logger& in)
{
    if (!in.StartLog(LoggerLevel::LL_Warning))
        throw std::logic_error("Cannot start log line because another line has not been completed. Please finish that line first.");

    return in;
}
Logger& Error(Logger& in)
{
    if (!in.StartLog(LoggerLevel::LL_Error))
        throw std::logic_error("Cannot start log line because another line has not been completed. Please finish that line first.");

    return in;
}
Logger& Critical(Logger& in)
{
    if (!in.StartLog(LoggerLevel::LL_Critical))
        throw std::logic_error("Cannot start log line because another line has not been completed. Please finish that line first.");

    return in;
}

Logger& EndLog(Logger& in)
{
    in.EndLog();
    return in;
}

#endif