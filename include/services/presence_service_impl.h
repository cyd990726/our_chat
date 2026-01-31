#ifndef PRESENCE_SERVICE_IMPL_H
#define PRESENCE_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include "presence.grpc.pb.h"

namespace ourchat {

class PresenceServiceImpl : public im::PresenceService, public grpc::Service {
public:
    grpc::Status SetOnline(grpc::ServerContext* context,
                           const im::SetOnlineRequest* request,
                           im::SetOnlineResponse* response) override;
    
    grpc::Status GetOnlineStatus(grpc::ServerContext* context,
                                  const im::GetOnlineStatusRequest* request,
                                  im::GetOnlineStatusResponse* response) override;
};

}

#endif
