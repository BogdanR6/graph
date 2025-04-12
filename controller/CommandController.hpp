#pragma once
#include "../ui/Console.hpp"
#include "../service/GraphService.hpp"

class CommandController {
private:
  GraphService& graphService;
public:
  CommandController(Console& console, GraphService& graphService);
};
