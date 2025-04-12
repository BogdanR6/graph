#include <stdexcept>

class InvalidUsageError : public std::runtime_error {
public:
  explicit InvalidUsageError(const std::string& message)
    : std::runtime_error(message) {}
};

class InvalidOperationOnGraphType : public std::runtime_error {
public:
  explicit InvalidOperationOnGraphType(const std::string& message)
    : std::runtime_error(message) {}
};
