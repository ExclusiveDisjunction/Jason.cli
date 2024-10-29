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
    Result() : data(T())
    {
        static_assert(std::is_default_constructible_v<T>, "Default constructor can only be used if T is default-constructible");
    }
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
        return std::get<T>(data);
    }  
    T& GetOkDirect()
    {
        return std::get<T>(data);
    } 

    std::optional<E> GetErr() const noexcept 
    {
        if (IsErr())
            return std::get<E>(data);

        return std::nullopt;
    }
    const E& GetErrDirect() const
    {
        return std::get<E>(data);
    }
    E& GetErrDirect()
    {
        return std::get<E>(data);
    }

    template<typename NewE>
    Result<T, NewE> TransformOk(NewE eIfError = NewE())
    {
        if (!IsOk())
            return Result<T, NewE>(std::move(eIfError));
        else
            return Result<T, NewE>(std::move(std::get<T>(data)));
    }
    template<typename NewT>
    Result<NewT, E> TransformErr(NewT tIfOk = NewT())
    {
        if (!IsErr())
            return Result<NewT, E>(std::move(tIfOk));
        else
            return Result<NewT, E>(std::move(std::get<E>(data)));
    }
};

template<typename E>
using EmptyResult = Result<std::monostate, E>;

#endif