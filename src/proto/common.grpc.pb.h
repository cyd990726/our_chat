#pragma once
#include "common.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class CommonService {
public:
    virtual ~CommonService() = default;
};

} // namespace im
