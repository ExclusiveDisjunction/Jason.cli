#ifndef IO_RESULT
#define IO_RESULT

#include <variant>
#include <optional>

template <typename T, typename E>
class Result 
{
private:
    std::variant<T, E> data;

public:
    Result() : data(T()) {}
    Result(const T& value) : data(value) {}
    Result(T&& value) : data(std::move(value)) {}
    Result(const E& error) : data(error) {}
    Result(E&& error) : data(std::move(error)) {}

    bool IsOk() const noexcept 
    {
        return std::holds_alternative<T>(data);
    }

    bool IsErr() const noexcept
    {
        return std::holds_alternative<E>(data);
    }

    std::optional<T> GetOk() const noexcept 
    {
        if (IsOk())
            return std::get<T>(data);

        return std::nullopt;
    }
    const T& GetOkDirect() const
    {
        if (!IsOk())
            throw std::logic_error("Value was err, but expected ok");
        
        return std::get<T>(data);
    }  
    T& GetOkDirect()
    {
        return const_cast<T&>(const_cast<const Result<T, E>*>(this)->GetOkDirect());
    } 

    std::optional<E> GetErr() const noexcept 
    {
        if (IsErr())
            return std::get<E>(data);

        return std::nullopt;
    }
    const E& GetErrDirect() const
    {
        if (!IsErr())
            throw std::logic_error("Value was ok, but expected err");
        
        return std::get<E>(data);
    }
    E& GetErrDirect()
    {
        return const_cast<E&>(const_cast<const Result<T, E>*>(this)->GetErrDirect());
    }

    template<typename NewE>
    Result<T, NewE> TransformOk(NewE eIfError = NewE())
    {
        if (!IsOk())
            return Result<T, NewE>(eIfError);
        else
            return Result<T, NewE>(std::get<T>(data));
    }
    template<typename NewT>
    Result<NewT, E> TransformErr(NewT tIfOk = NewT())
    {
        if (!IsErr())
            return Result<NewT, E>(tIfOk);
        else
            return Result<NewT, E>(std::get<E>(data));
    }
};

template<typename E>
using EmptyResult = Result<std::monostate, E>;

#endif