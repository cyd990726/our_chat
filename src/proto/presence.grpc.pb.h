#pragma once
#include "presence.pb.h"
#include <grpcpp/grpcpp.h>

namespace im {

class PresenceService {
public:
    virtual ~PresenceService() = default;

    virtual grpc::Status SetOnline(grpc::ServerContext* context,
                                   const SetOnlineRequest* request,
                                   SetOnlineResponse* response) = 0;

    virtual grpc::Status GetOnlineStatus(grpc::ServerContext* context,
                                         const GetOnlineStatusRequest* request,
                                         GetOnlineStatusResponse* response) = 0;
};

} // namespace im
