#ifndef GROUP_SERVICE_IMPL_H
#define GROUP_SERVICE_IMPL_H

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include "group.grpc.pb.h"

namespace ourchat {

class GroupServiceImpl : public im::GroupService, public grpc::Service {
public:
    grpc::Status CreateGroup(grpc::ServerContext* context,
                              const im::CreateGroupRequest* request,
                              im::CreateGroupResponse* response) override;
    
    grpc::Status GetGroupInfo(grpc::ServerContext* context,
                               const im::GetGroupInfoRequest* request,
                               im::GetGroupInfoResponse* response) override;
};

}

#endif
